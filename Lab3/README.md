
# Lab 3 - Using Sensors to Control Motors

This lab is to design a controller to control a fan driven by a DC motor. Our design uses and LCD display to show the clock and the fan’s rotation direction and speed. Using a timer interrupt the system uses sound sensor to detect music notes and control the speed of the motor.

## Folder Structure

    changelog.md                    record of changes made with dates
    README.md                       lab description
    motor_control/
        motor_control.ino           Arduino program
    schematics/
        lab3-breadboard.pdf         circuit schematic design using fritzing
        lab3-schematic.pdf          breadboard layout for easier implementation
        lab3-schematic.fzz          .fzz files used with fritzing (ver. 1.0.1)

## Components Used

Component               | Quantity
---------               | :---------:
Arduino ATMEGA 2560     | 1
Power supply module     | 1
DC Motor                | 1
L293D                   | 1
LCD                     | 1
DS1307 RTC              | 1
Mic. sound module       | 1
Button                  | 1
10k resistor            | 1

## Other Resources Used

- [elegoo examples](https://www.elegoo.com/download)
- [pin mapping](https://docs.arduino.cc/hacking/hardware/PinMapping2560)
- [lcd refresh buffer](https://www.baldengineer.com/arduino-lcd-display-tips.html)
- Arduino IDE Example sketches
