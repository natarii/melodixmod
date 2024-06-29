MelodixMod
========
The MelodixMod project adds internal memory to the TOA MELODIX ML-304 melody generator. This repository also contains WIP hardware and software documentation for the original device.

## Overview
ML-304 devices use a Yamaha YM2203 OPN FM sound source to play back melodies, chimes and tones stored on memory cards. The MelodixMod system serves as an effective replacement for these cards. Full function of the original unit is retained -- physical cards inserted into the slots will override any data stored on MelodixMod's flash. A bank switching option is available through a hole on the rear panel which allows the user to switch between multiple banks of simulated cards. Impact to the device's appearance is also minimized, with the only change visible from the outside being the removal of a grounding screw.

### Mod Files
- `Logic` - Data to program the mod's decode logic chip
- `MCU` - Program for the mod's ATTiny85 MCU
- `PCB` - EAGLE schematic and PCB design files
- `PCBSpacer` - OpenSCAD design file and STL for a 3D-printable spacer to support the mod PCB

### Research Files
- `Disassembly` - Original ML-304 program disassembly
- `FujisokuBProgrammer` Arduino Mega-based programmer for Melodix cards
- `MelodixCard` - Information about the data format on Melodix cards

![](<Pictures/modv1-installed.jpg>)
![](<Pictures/modv1-detail.jpg>)

## MELODIX CARD information

The memory cards used in the ML-304 are pre-programmed and follow the Fujisoku B-series physical and electrical standard. Despite a very vague resemblance to PCMCIA and other common memory cards, this is an obsolete and relatively rare format. As the Melodix units are niche in addition to not having been marketed outside of Japan, original TOA cards fetch prices in the hundreds of USD range on auction sites, making it infeasible for most (certainly me!) to collect any more than the less-desirable cards.

## Why making reproduction cards isn't viable

Fujisoku B-series cards use very thin contacts on the card side, which mate with "forks" in the slot. A reproduction card could likely be made out of very thin PCB, but there is not much remaining height for components on the board. Surface mount parts may be viable (assuming a programmer which the cards could slot into existed -- more on that later), but any through-hole parts like socketed memory would not be usable without hanging further than usual out of the front of the device (outside of the normal card form factor).

UV EPROM, EEPROM, flash, and SRAM-based B-series cards were produced, but these are also becoming increasingly difficult to find. As these cards are writable, they also require hardware modifications to the Melodix unit as it does not drive all of the control lines they need.

## Card slot availability

In 2021, the availability of B-series card slots was essentially nil. An exhaustive personal and crowdsourced search turned up no supplies of slots, aside from ones that could be removed from an existing piece of equipment. I was also not able to establish an actual part number or manufacturer for the slots.

In 2023, some slots have turned up on US eBay. I also found further documentation which reveals some card slot part numbers, which are listed below. Note that not all of these are usable as replacements in the original device, but could be used in an external programmer or dumping tool.

- ITT Cannon 990-2000-053 (right angle pins)
- ITT Cannon DICMI-38S-RPC-01 (right angle pins)
- Hakuto BC1R (right angle pins)
- Hakuto BC1S (straight pins)

Regardless of any current availability of card slots, they are no longer being produced and supplies will eventually run out. This is another nail in the coffin for using the programmable variants as reproduction cards.

## I made a reproduction card anyway

Before deciding to proceed with an internal card modification, I ended up purchasing an SRAM card and designing an Arduino Mega-based programmer for it. The programmer can connect to the Mega board with no extra components needed, and the code allows for card memory to be uploaded and downloaded via XMODEM. I also performed the required modifications to the Melodix PCB to allow SRAM cards to be used.

It is also possible to adapt these cards to be used in a standard EPROM programmer.

## Related WIP projects / TODO

  * Verify v1.1 mod PCB
  * Disassembly of ML-304/ML-301 MCU program (nearly complete, needs some updates now that I have the hardware to reference, also needs to be done in proper f9dasm format rather than simple text find-and-replace)
  * MAME driver for Melodix series (working, needs better integration with MAME cartridge system, needs UI)
  * Melodix card to VGM conversion utility (working, needs cleanup but otherwise complete)
  * MIDI to Melodix card conversion utility (partially working)
  * Replacement main MCU (not started)
  * Document the process to prepare data for MelodixMod's flash (really just concatenate 32KByte binaries)

## Acknowledgements
  * Thanks to Hind for providing the initial MCU ROM dump.
