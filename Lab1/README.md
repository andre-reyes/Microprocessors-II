# Lab 1
        The purpose of this lab is to design a controller for traffic lights that face one traffic direction. The system will start up in a red flashing, pending, mode. Once a button is pressed, The program will rotate between solid red, green and yellow lights to imitate a traffic controller. 

## Components Used:
Component           | Quantity
---------           | :---------:
Arduino board       | 1
Red LED             | 1
Yellow LED          | 1 
Green LED           | 1
Push Button Switch  | 1
Active Buzzer       | 1
220 ohm resistor    | 3
10k ohm resistor    | 1

## The Color Rotation: 
- solid-red (24s)
- blinking red with beep (3s)
- Solid Green (20s)
- blinking green with beep (3s)
- Yellow (3s)
- Pattern repeats



## Other Resources Used:
- Button interrupt - https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
- Starting point for Timer portion - https://github.com/ACANETS/eece-4520-5520-labs/blob/main/lab1/TimerSample/TimerSample.ino