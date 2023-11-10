# Microprocessors II - Labs
This is a collection of the Labs required by the course By Professor Yan Luo in the Fall of 2023 at UMass Lowell

- [x] [Lab 1 - Traffic Light Controller](Lab1)
- [x] [Lab 2](Lab2)
  - [x] Joystick (Andre)
  - [x] buzzer when eating apple (Conor)
  - [x] MPU-6060 (gyro only) to move (Harry)
- [ ] [Lab 3](Lab3)
  - [ ] [DC Motor control, truth table located in datasheets](Lab3/datasheets/TIL293D.PDF)
    - [ ] Wire - [view schematic](Lab3/schematics/lab3-schematic.png)
    - [ ] to test: use buttons/serial input before implementing sound sensor
    - [ ] control speed with digital inputs
    - [ ] control direction with digital inputs
  - [ ] [LCD Display](Lab3/datasheets/Lesson%2028%20LCD%20Display/) (Andre)
    - [x] wire - [view schematic](Lab3/schematics/lab3-schematic.png)
    - [x] display time
    - [x] display motor direction (“C” for clockwise, or “CC” ) 
    - [x] display motor speed (“Full”, “3/4”, “1/2”, or “0”)
    - [ ] use timer interrupt to update each second
  - [x] [RTC module](Lab3/datasheets/Lesson%2033%20Real%20Time%20Clock%20Module/) (Andre)
    - [x] wire - [view schematic](Lab3/schematics/lab3-schematic.png)
    - [x] retrieve time
  - [ ] [Sound sensor module](Lab3/datasheets/Lesson%2016%20Large%20Microphone%20Module/) (Harry)
    - [ ] wire - [view schematic](Lab3/schematics/lab3-schematic.png)
    - [ ] sample sound and convert to digital values
    - [ ] use FFT library to detect peak freq.
    - [ ] [match peak freq to notes C4 (262Hz) & A4 (440Hz), max 2% error allowed](https://www.youtube.com/watch?v=rP0a7b9IXwU)
      - [ ] increase fan speed with C4
      - [ ] decrease fan speed with A4
