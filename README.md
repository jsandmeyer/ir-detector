## IR Reflection Detector

This provides resources to help you build an IR detector for use with O-scale model railroad track in order to detect passing trains for operating block signals.

It is designed to be placed beneath the tracks, to allow for the use of more realistic trackside accessories and buildings.

It emits and detects IR at 38KHz (though the frequency can be changed based on part choice).
This prevents background light from affecting function.

The original analog designs utilized a TLC555 CMOS timer.
The timer was unstable through stray capacitance caused by rail operation, and also likely by temperature fluctuations from the power supply.
Small microcontrollers like the ATTINY family are inexpensive, simple to program, and more stable even without an external clock.

The current versions utilize a switching power supply to avoid heat, so the device can be safely placed under track.
The input voltage is 18V RMS AC, which rectifies to 26V DC.
Since only 3-5 volts are needed for the circuit, power dissipation through a linear supply is excessive.

### Design Requirements

1. Must be able to handle 18V RMS AC (or a little more)
2. Must be able to handle rapid shutoff of power
3. Must avoid producing noticeable heat
4. Must fit hidden under track
   1. Shape limits (avoid track supports, power connectors, etc)
   2. Size limits (must not extend past track base, must have a way to feed wires)
   3. Some components must be aligned under a single oblong hole in track base
5. Emitter LED must be shrouded to avoid directly triggering detector
6. Must handle stray capacitance, excessive heat, and some signal interference
7. Must be small to reduce cost, especially when a 4-layer design is used

### Circuit Parts

1. MCU: Responsible for PWM to control the IR emitter, and controlling the relay when a reflection is detected.
2. Switching Power Supply: reduce up to 35VDC input to 3V output for the MCU, IR LED, and relay.
3. IR LED: an emitter running at ~70mA, and 50% duty cycle at 36-56KHz depending on detector part selection.
4. IR Detector: an IC containing both a detector and frequency detection logic, to ensure only correct-frequency light is detected.  36, 38, 40, or 56 KHz.
5. Relay: Controls AC (or DC) current from inputs to a normally-open and normally-closed output.
6. Trimmer: Controls duration of relay after IR is no longer detected.

### Design Versions

Some versions are skipped.
Those were only built on breadboard, or designed and considered non-viable.
Generally, each version is smaller or similar to the previous.

* Version 7: Used TLC556.  Had heat issues from power supply.
* Version 11: Used a switching power supply.  Still had stability issues.
* Version 13: Switched to use an ATTINY84.  Switched to 4-layer board.  Bridge rectifier could not handle rapid shutoff of some transformers.
* Version 14: Larger bridge rectifier.  Production model.  [Image](./blob/master/resources/v14_production_model.jpg)
* Version 17: Updated layout, components (ATTINY20), and code.  Reduced to 2-layer board.  Manufacturing in process.

