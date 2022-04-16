MelodixMod
========
The MelodixMod project adds internal memory to the TOA MELODIX ML-304 melody generator. Hardware and software documentation is also in progress.

## Overview
ML-304 devices use a Yamaha YM2203 OPN FM sound source to play back melodies, chimes and tones stored on memory cards. The MelodixMod system serves as an effective replacement for these cards. Full function of the original unit is retained -- physical cards inserted into the slots will override any data stored on the mod's flash. A bank switching option is available through a hole on the rear panel which allows the user to switch between multiple banks of simulated cards. Impact to the device's appearance is also minimized, with the only change visible from the outside being the removal of a ground screw.

## MELODIX CARD information

The memory cards used in the ML-304 are pre-programmed and follow the Fujisoku B-series physical and electrical standard. Despite a very vague resemblance to PCMCIA and other common memory cards, this is an obsolete and relatively rare format. As the Melodix units are niche in addition to not having been marketed outside of Japan, original TOA cards fetch prices in the hundreds of USD range on auction sites, making it infeasible for most (certainly me!) to collect any more than the less-desirable cards.

## Why making reproduction cards isn't viable

Fujisoku B-series cards use very thin contacts on the card side, which mate with "forks" in the slot. A reproduction card could likely be made out of very thin PCB, but there is not much remaining height for components on the board. Surface mount parts may be viable (assuming a programmer which the cards could slot into existed -- more on that later), but any through-hole parts like socketed memory would not be usable without hanging further than usual out of the front of the device (outside of the normal card form factor).

EEPROM, flash, and SRAM-based B-series cards were produced, but these are also becoming increasingly harder to find. As these cards are writable, they also require hardware mods to the Melodix unit as it does not drive all of the control lines they need.

## Card slot availability

Availability of B-series card slots is essentially nil. An exhaustive personal and crowdsourced search turned up no supplies of slots, aside from ones that could be removed from an existing piece of equipment. I have also not been able to establish an actual part number or manufacturer for the slots.

I ended up getting one as a spare part for an Ensoniq keyboard which happened to use this standard, but I did feel a little bit bad about it. All in all, this is another nail in the coffin for using the programmable variants as reproduction cards.

## I made a reproduction card anyway

Before deciding to proceed with an internal card mod, I ended up purchasing an SRAM card and designing an Arduino Mega-based programmer for it. The programmer can connect to the Mega board with no extra components needed, and the code allows for card memory to be uploaded and downloaded via XMODEM. I also performed the required mods to the Melodix PCB to allow SRAM cards to be used.

## TODO

The rest of this readme!
