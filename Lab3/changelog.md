### 11/5/23
 - created initial circuit diagram/schematic, includes connections for rtc, motor, lcd
### 11/9/23
  - wired lcd, redesigned circuit for better wirting on breadboard
  - used elegoo, lcd and rtc examples
  - displayed time on lcd
  - rtc library doesn't 0 pad and when time (seconds) is 1 digit, "9" still displays after "59"
  - used https://www.baldengineer.com/arduino-lcd-display-tips.html as insperation to use sprintf
  - sprintf allowed for 0 padding
  - created buffer of output to draw display all at once instead of 1 char at a time
      - this prevented ghosting and allowed for customization of padding and lcd config
  - basic strings hardcoded for speed and direction to test display
  displayed as:
          
          //DIR: {C, CC} RPM: {full, 3/4, 1/2, 0}//
          //            00:00:00                 //            
       
  - rerouted L293D EN pin from always HIGH to Digital pin to control power to motor
### 11/10/23
- reused timer1 interrupt code from lab1 using 1Hz signal instead
- added functions to dynamically change rpm/dir
  - issue with rpm/dir char array not updating
    - changed dir to a char array[2][3] an array of 2 strings of buffer 3 each
    - changed dir_disply from function to based off of of dir bool (ie.  dir_display[dir]) where dir_display[0] is "C" and dir_display[1] is "CC", this matches the logic and significantly reduced display code
    - changed rpm_display to follow the same logic as dir_display
        - removed switch case to change array value to accomplish this
        - used rpm enum to determine array string to display on lcd
- LCD Functions tested and complete, waiting for other modules to full test
- updated schematic with button to control motor direction