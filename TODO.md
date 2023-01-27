# TODO

## Power Optimization
- Power management menu
  - [ ] Lower clock speed
  - [ ] Change display timeout
  - [ ] Change display brightness

## Networking
- [x] Channel Selection and communication
- [ ] ~~ESP-NOW Encryption~~
- [ ] ~~Node Mode using VoIP~~
- [x] Channel Count

## I2S Audio
- [x] Get data from microphone
- [x] Split it into groups of 250 bytes & transmit
- [x] Store incoming audio in circular buffer & play over speaker
- [x] Talk control - no audio is played when talk button is held
- [ ] Channel sweep for activity with fft
- [x] Specific process core pinning (I2S Core 1, all other functions core 0) 

## Hardware
- [x] Screen navigation buttons
- [x] Talk button
- [x] Hook up battery sens
- [x] BMS + Voltage Reg
- [x] Power Switch
- [x] Audio
- Case
  - [ ] CAD
  - [ ] Printing
  - [ ] Assembly

## Screen Menus
- [ ] Power Management Menu
- [ ] Update connection icons
- [ ] UDP Connection Icon
- [ ] Node Mode Screen
- [ ] Debug Menu  
  - [ ] Test buttons
  - [ ] Reset battery config
  - [ ] Test Display
  - [ ] Test audio passthrough

## Extras
- [ ] Stats (Power on time, Battery cycles, Talk time)

## General
- [ ] Sanitize KiCAD Files
