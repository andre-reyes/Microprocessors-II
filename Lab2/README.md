changelog: 
10/16/23
 - setup lab2 files, tested Yan Luo's example and succeeded in getting a serial connection
 - connected joystick and implemented a basic code structure using analogRead() in order to gain an 
     understanding of joystick diresction values. 0-1024 hovering around 500
 - values varied at center so a threshold was created, the idea splitting each direction into 4 
     where only the most extreme changes (at least a 25% change) will register a direction input
 - conditions were then added to an if/else statement to determine the direction and ouputs it
      in the form of a letter to the arduino serial, which is the read by andre-pyserial.py
 - andre-pyserial.py was added as a test bench for just the serial communication between the
     arduino and pc. the base of this code was taken from snake.py movement functions
 * working joystick code added to snake.py
   * similar in function to andre-Serial except with condition to handle excess readings in buffer
   * lots of issues with finding a way to flush buffer, solution I found was to read_all() so that it removes from buffer
   * TO RUN: after ensuring your device is working with class example code
     1 install andre-Serial.ino to arduino
     2 run snake.py in terminal
     3 alternatively before running snake.py, run andre-pyserial.py in terminal to test joystick outputs