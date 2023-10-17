# pyserial-test.py
# test serial communication with Arduino
#
# require pip install pyserial
#
# Usage: python3 pyserial-test.py
#
# yluo
#
import serial 

# serial port dev file name
# need to change based on the particular host machine
# serialDevFile = '/dev/tty.usbmodem14601' - linux port example
serialDevFile = 'COM3' # Andre's windows port
ser=serial.Serial(serialDevFile, 9600, timeout=1)


def go_up():
    print('up')

def go_down():
    print('down')

def go_left():
    print('left')

def go_right():
    print('right')

while True:
        # read joystick direction and set direction
    joystick_position = ser.readline().decode('UTF-8').rstrip()
    #print(joystick_position)

  
    match joystick_position:
        case 'w':
            go_up()
        case "s":
            go_down()
        case "a":
            go_left()
        case "d":
            go_right()
        case _:
            pass
