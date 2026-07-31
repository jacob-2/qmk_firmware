# Cheapino V2

![Cheapino V2](https://raw.githubusercontent.com/tompi/cheapino/master/doc/gallery/cheapino-in-case-10.jpg)

A 36 key column-staggered split keyboard with a rotary encoder, designed to be as
cheap as possible to build. The reversible PCB uses a Japanese duplex matrix, so
a single RP2040-Zero drives both halves over an RJ45 cable and there is no
controller or serial link on the right hand side. This is therefore an ordinary
single-controller build rather than a split keyboard one.

* Keyboard Maintainer: [Thomas Haukland](https://github.com/tompi)
* Hardware Supported: Cheapino V2 PCB with an RP2040-Zero
* Hardware Availability: [Cheapino repository](https://github.com/tompi/cheapino)

Make example for this keyboard (after setting up your build environment):

    make cheapino:default

Flashing example for this keyboard:

    make cheapino:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the top left key and plug in the keyboard
* **Physical reset button**: Hold BOOT on the RP2040-Zero while plugging in USB-C
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available

## Board specifics

The duplex matrix shares each of the six column pins between two logical columns
to halve the pin count, so the board uses a custom matrix scan (`matrix.c`)
rather than the stock one.

The rotary encoder is wired into that same matrix instead of to dedicated pins,
so it is decoded from the raw matrix state in `encoder.c` rather than through
QMK's encoder feature. Its actions are fixed in firmware: press plays/pauses,
and turning scrolls a page at a time by default, changes volume on layer 6,
switches tabs on layer 3, and undoes/redoes on layer 5.

The duplex wiring also leaves some sneak paths that the diodes do not fully
block, so a few three-key combinations report a key that was never pressed - on a
QWERTY layout, holding a+s+e also reports q. `ghosting.c` filters out the
observed patterns.
