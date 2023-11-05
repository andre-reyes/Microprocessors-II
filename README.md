# Microprocessors II - Labs
This is a collection of the Labs required by the course By Professor Yan Luo in the Fall of 2023 at UMass Lowell

- [x] [Lab 1 - Traffic Light Controller](Lab1)
- [x] [Lab 2](Lab2)
  - [x] Joystick (Andre)
  - [x] buzzer when eating apple (Conor)
  - [x] MPU-6060 (gyro only) to move (Harry)
- [ ] [Lab 3](Lab3)
  - [ ] DC Motor control, truth table located in datasheets
    - [ ] Wire - view schematic
    - [ ] to test: use buttons/serial input before implementing sound sensor
    - [ ] control speed with digital inputs
    - [ ] control direction with digital inputs
  - [ ] LCD Display, 
    - [ ] wire - view schematic
    - [ ] display time
    - [ ] display motor direction (“C” for clockwise, or “CC” ) 
    - [ ] display motor speed (“Full”, “3/4”, “1/2”, or “0”)
    - [ ] use timer interrupt to update each second
  - [ ] RTC module
    - [ ] wire - view schematic
    - [ ] retrieve time
  - [ ] Sound sensor module
    - [ ] wire - view schematic
    - [ ] sample sound and convert to digital values
    - [ ] use FFT library to detect peak freq.
    - [ ] match peak freq to notes C4 (262Hz) & A4 (440Hz), max 2% error allowed
      - [ ] increase fan speed with C4
      - [ ] decrease fan speed with A4
