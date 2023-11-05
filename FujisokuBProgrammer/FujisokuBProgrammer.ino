#include <digitalWriteFast.h>

//ports
//A data
//F addr lo
//K addr hi
#define PIN_N_CE1 21
#define PIN_CE2 20
#define PIN_N_WE 19
#define PIN_N_OE 18
#define PIN_LED 13

uint8_t xmbuf[128];

void setup() {
  pinMode(PIN_N_CE1, OUTPUT);
  digitalWriteFast(PIN_N_CE1, HIGH);
  pinMode(PIN_N_OE, OUTPUT);
  digitalWriteFast(PIN_N_OE, HIGH);
  pinMode(PIN_N_WE, OUTPUT);
  digitalWriteFast(PIN_N_WE, HIGH);
  pinMode(PIN_CE2, OUTPUT);
  digitalWriteFast(PIN_CE2, HIGH); //always assert this one
  DDRA = 0;
  DDRF = 0xff;
  DDRK = 0xff;
  Serial.begin(115200);
  while (!Serial);
  pinMode(PIN_LED, OUTPUT);
  digitalWriteFast(PIN_LED, LOW);
  Serial.println("started");
}

void card_write(uint16_t addr, uint8_t data) {
  digitalWriteFast(PIN_LED, HIGH);
  PORTF = addr & 0xff; //addr lo
  PORTK = addr >> 8; //addr hi
  DDRA = 0xff; //bus out
  PORTA = data; //put data on bus
  digitalWriteFast(PIN_N_CE1, LOW);
  digitalWriteFast(PIN_N_WE, LOW);
  delayMicroseconds(5);
  digitalWriteFast(PIN_N_WE, HIGH);
  delayMicroseconds(5);
  digitalWriteFast(PIN_N_CE1, HIGH);
  DDRA = 0; //bus in
  digitalWriteFast(PIN_LED, LOW);
}

uint8_t card_read(uint16_t addr) {
  digitalWriteFast(PIN_LED, HIGH);
  uint8_t val = 0;
  PORTF = addr & 0xff; //addr lo
  PORTK = addr >> 8; //addr hi
  DDRA = 0; //bus in, just in case
  digitalWriteFast(PIN_N_CE1, LOW);
  digitalWriteFast(PIN_N_OE, LOW);
  delayMicroseconds(5);
  val = PINA; //get bus
  digitalWriteFast(PIN_N_OE, HIGH);
  digitalWriteFast(PIN_N_CE1, HIGH);
  digitalWriteFast(PIN_LED, LOW);
  return val;
}

void do_read() {
  uint16_t readaddr = 0;
  uint8_t readcycle = 1;
  bool eotacked = false;
  bool started = false;
  while (1) {
    if (!Serial.available()) continue;
    char r = Serial.read();
    if (r == 0x06 || (!started && r == 0x15)) { //ack, nak if not started
      started = true;
      if (readaddr == 32768) {
        if (!eotacked) {
          Serial.write(0x04);
          eotacked = true;
          continue;
        } else {
          Serial.write(0x17);
          return;
        }
      }
      uint8_t ck = 0;
      Serial.write(0x01);
      Serial.write(readcycle);
      Serial.write(255-readcycle);
      for (uint8_t i=0;i<128;i++) {
        uint8_t d = card_read(readaddr++);
        ck += d;
        Serial.write(d);
      }
      Serial.write(ck);
      readcycle++;
    } else if (r == 'C') {
      Serial.write(0x15); //nak 16bit mode
    }
  }
}

void do_write() {
  uint8_t recvmode = 0;
  uint8_t cyc = 1;
  uint8_t recvcyc1 = 0;
  uint8_t recvcyc2 = 0;
  uint8_t gotdata = 0;
  uint8_t ck = 0;
  uint32_t lastdata = millis();
  uint8_t myck = 0;
  uint16_t cardpos = 0;
  //0 waiting for first block
  //1 waiting for cyc 1
  //2 waiting for cyc 2
  //3 getting data
  //4 waiting for checksum
  //5 waiting for next block
  while (1) {
    if (recvmode == 0) {
      Serial.write(0x15);
      delay(500);
    } else {
      if (millis() - lastdata >= 1000) {
        Serial.write(0x15);
        recvmode = 5;
        lastdata = millis();
      }
    }
    if (Serial.available()) {
      uint8_t r = Serial.read();
      lastdata = millis();
      if (recvmode == 0 || recvmode == 5) {
        if (r == 0x01) { //soh
          recvmode = 1;
          myck = 0;
          gotdata = 0;
        } else if (r == 0x04) { //eot
          Serial.write(0x06);
          return; //hack ??????
        } else if (r == 0x17) { //etb
          Serial.write(0x06);
          return;
        }
      } else if (recvmode == 1) {
        recvcyc1 = r;
        recvmode = 2;
      } else if (recvmode == 2) {
        recvcyc2 = r;
        recvmode = 3;
      } else if (recvmode == 3) {
        xmbuf[gotdata++] = r;
        myck += r;
        if (gotdata == 128) {
          recvmode = 4;
        }
      } else if (recvmode == 4) {
        ck = r;
        if (ck != myck) {
          Serial.write(0x15);
        } else if (recvcyc1 != cyc) {
          Serial.write(0x15);
        } else if (recvcyc2 != 255-cyc) {
          Serial.write(0x15);
        } else {
          for (uint8_t i=0;i<128;i++) {
            card_write(cardpos++, xmbuf[i]);
          }
          Serial.write(0x06);
          cyc++;
        }
        recvmode = 5;
      }
    }
  }
}

void loop() {
  if (Serial.available()) {
    char r = Serial.read();
    if (r == '?') {
      Serial.println("r - enter read mode");
      Serial.println("w - enter write mode");
      Serial.println("z - erase card with all 00");
      Serial.println("v - verify card is fully readable and writable");
    } else if (r == 'r') {
      Serial.println("going into read mode, begin xmodem transfer now");
      do_read();
    } else if (r == 'w') {
      Serial.println("going into write mode, begin xmodem transfer now");
      do_write();
    } else if (r == 0x0d || r == 0x0a) {
    } else if (r == 'z') {
      Serial.println("zeroing");
      for (uint16_t i=0;i<32768;i++) {
        card_write(i, 0);
      }
      Serial.println("done");
    } else if (r == 'v') {
      Serial.println("writing 1/2...");
      for (uint16_t i=0;i<32768;i++) {
        card_write(i, 0x55);
      }
      Serial.println("remove card, then press any key");
      while (!Serial.available());
      Serial.read();
      card_write(0, 0x00);
      Serial.println("reinsert card, then press any key");
      while (!Serial.available());
      Serial.read();
      Serial.println("verifying 1/2...");
      for (uint16_t i=0;i<32768;i++) {
        if (card_read(i) != 0x55) {
          Serial.println("fail!!!");
          return;
        }
      }
      Serial.println("writing 2/2...");
      for (uint16_t i=0;i<32768;i++) {
        card_write(i, 0xaa);
      }
      Serial.println("remove card, then press any key");
      while (!Serial.available());
      Serial.read();
      card_write(0, 0x00);
      Serial.println("reinsert card, then press any key");
      while (!Serial.available());
      Serial.read();
      Serial.println("verifying 2/2...");
      for (uint16_t i=0;i<32768;i++) {
        if (card_read(i) != 0xaa) {
          Serial.println("fail!!!");
          return;
        }
      }
      Serial.println("verify ok");
    }
  }
}
