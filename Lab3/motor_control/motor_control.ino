//libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h> // location: Lab3\datasheets\Lesson 33 Real Time Clock Module\DS1307.zip
                    // to install: in arduino ide > sketch > include library > add .ZIP library

DS3231 clock;
RTCDateTime time;

//Motor state enum to switch states (i.e. rpm = full)
enum motor_speed{zero, half, three_quarter, full};
motor_speed rpm = zero; //default: off state

//LCD interface pins
const int v0 = 5, rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char line0[17];                                         //Top line buffer on LCD
char line1[17];                                         //Bottom line Buffer on LCD
char rpm_display[4][5] = {"0", "1/2", "3/4", "Full"};   //Display string for rpm on LCD
char dir_display[2][3] = {"C", "CC"};                   //Display string for direction on LCD

//Flags
volatile bool updateFlag = false; //Timer1 flag to update display/motor
volatile bool dir = false;        // direction flag 0 == "C" 1 == "CC"
volatile byte button_pin = 37;    //button pin assignment

//Declare functions
void updateDisplay();
void updateMotor();
void getRpm();
void changeDir();

// Main code starts here
void setup() {
  // set up LCD
  pinMode(v0, OUTPUT);
  analogWrite(v0, 100);
  lcd.begin(16, 2);

  // Set up clock
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);

  //set up button interrupt
  pinMode(button_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(button_pin), changeDir, RISING); 
  
  //set timer1 interrupt at 1Hz, reused from Lab 1
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624; 
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
  sei();
}


void loop() {
  if(updateFlag){
    time = clock.getDateTime();
    updateMotor();
    updateDisplay();
    updateFlag = false;
  }
}

//****************Functions*********************//
void updateDisplay() {
  sprintf(line0, "DIR: %-2s RPM:%4s" , dir_display[dir], rpm_display[rpm]);
  sprintf(line1, "    %02d:%02d:%02d", time.hour, time.minute, time.second); //sprintf: %[flags][width][.precision][length]specifier
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);
  lcd.print(line1);
}

void updateMotor(){
  // TODO: update actual motor dir and speed, truth table in L293D datasheet 
  getRpm();
  
}
void getRpm(){
  //TODO: insert sound module function here to be used in updateMotor
  //if C4 then increase else if A4 decrease, else keep current rpm
  //set rpm = zero, half, three_quarter or full
  rpm = three_quarter;
  
}


//****************Interrupts*********************//
ISR(TIMER1_COMPA_vect){
  updateFlag = true;
}

void changeDir(){
  dir ^= 1;
}