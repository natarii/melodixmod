#define FLASH_MBITS 2 //1, 2, or 4
#define HAS_DUAL_FLASH //comment out for single flash ic

#define PIN_BANK 0
#define PIN_A18 1
#define PIN_A17 2
#define PIN_SW 3 //active low
#define PIN_LED 4

#define STATE_IDLE 0
#define STATE_WAITKEYUP 1
#define STATE_WAITKEYTM 2

#include <EEPROM.h>

uint8_t current_state = 0;
uint8_t current_bank = 0;
uint8_t new_bank = 0;
uint32_t state_time = 0;
#ifdef HAS_DUAL_FLASH
uint8_t max_bank = (FLASH_MBITS*2);
#else
uint8_t max_bank = FLASH_MBITS;
#endif

void set_state(uint8_t state) {
  current_state = state;
  state_time = millis();
}

void set_bank(uint8_t bank) {
  digitalWrite(PIN_A17, bank&(1<<0));
  digitalWrite(PIN_A18, bank&(1<<1));
  #ifdef HAS_DUAL_FLASH
  digitalWrite(PIN_BANK, bank&FLASH_MBITS);
  #endif
  current_bank = bank;
  new_bank = bank;
}

bool is_eeprom_inited() {
  if (EEPROM.read(0) != 0x13) return false;
  if (EEPROM.read(1) != 0x37) return false;
  if (EEPROM.read(2) != 0xba) return false;
  if (EEPROM.read(3) != 0xbe) return false;
  if (EEPROM.read(4) >= max_bank) return false;
  return true;
}

void save_bank() {
  EEPROM.write(4, current_bank);
}

void load_set_bank() {
  set_bank(EEPROM.read(4));
}

void init_eeprom() {
  EEPROM.write(0, 0x13);
  EEPROM.write(1, 0x37);
  EEPROM.write(2, 0xba);
  EEPROM.write(3, 0xbe);
  save_bank();
}

void set_led(bool on) {
  digitalWrite(PIN_LED, on);
}

void blink_x(uint8_t times) {
  while (times --> 0) {
    set_led(true);
    delay(250);
    set_led(false);
    delay(250);
  }
}

void setup_io() {
  pinMode(PIN_A17, OUTPUT);
  pinMode(PIN_A18, OUTPUT);
  pinMode(PIN_BANK, OUTPUT);
  pinMode(PIN_SW, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_A17, LOW);
  digitalWrite(PIN_A18, LOW);
  digitalWrite(PIN_BANK, LOW);
  digitalWrite(PIN_LED, LOW);
}

void setup() {
  setup_io();
  if (!is_eeprom_inited()) {
    set_bank(0);
    init_eeprom();
  } else {
    load_set_bank();
  }
  blink_x(current_bank+1);
}

bool is_key_pressed() {
  return (digitalRead(PIN_SW) == LOW);
}

void handle_keypress() {
  if (is_key_pressed()) {
    set_led(true);
    if (current_state != STATE_WAITKEYTM) {
      new_bank = 0;
    } else if (new_bank == max_bank) {
      new_bank = 0;
    } else {
      new_bank++;
    }
    set_state(STATE_WAITKEYUP);
  }
}

void loop() {
  switch (current_state) {
    case STATE_WAITKEYUP:
      if (is_key_pressed()) {
        set_state(STATE_WAITKEYUP); //update timestamp
      } else {
        if (millis() - state_time >= 30) { //debounce
          set_led(false);
          set_state(STATE_WAITKEYTM);
        }
      }
      break;
    
    case STATE_WAITKEYTM:
      if (millis() - state_time >= 2000) {
        set_state(STATE_IDLE);
        set_bank(new_bank);
        save_bank();
        blink_x(current_bank+1);
      }
    case STATE_IDLE:
    default:
      handle_keypress();
      break;
  }
}
