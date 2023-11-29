//libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>  // location: Lab3\datasheets\Lesson 33 Real Time Clock Module\DS1307.zip
                     // to install: in arduino ide > sketch > include library > add .ZIP library



DS3231 clock;
RTCDateTime time;

//Motor state enum to switch states (i.e. rpm = full)
enum motor_speed { zero,
                   half,
                   three_quarter,
                   full };
motor_speed rpm = zero;  //default: off state

// Motor pins
#define CLOCKWISE_PIN 2
#define COUNTERCLOCKWISE_PIN 3
#define ENABLE_PIN 4

//PWM values - only needed for 1/2 and 3/4, off and full are digital
#define HALF_SPEED 128
#define THREE_FOURTHS_SPEED 192

//LCD interface pins
const int v0 = 5, rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char line0[17];                                          //Top line buffer on LCD
char line1[17];                                          //Bottom line Buffer on LCD
char rpm_display[4][5] = { "0", "1/2", "3/4", "Full" };  //Display string for rpm on LCD
char dir_display[2][3] = { "C", "CC" };                  //Display string for direction on LCD

//Flags
volatile bool updateFlag = false;  //Timer1 flag to update display/motor
volatile bool clockwise = true;         // direction flag 0 == "C" 1 == "CC"
volatile byte button_pin = 37;     //button pin assignment

//Declare functions
void updateDisplay();
void updateSpeed();
void updateDirection();
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

  //Setup direction of motor
  updateDirection(clockwise);

  //set timer1 interrupt at 1Hz, reused from Lab 1
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
  
}

//Updates
//Speed - called when valid note is read
//Direction - called when direction button changed
//Display - Speed, Direction, or time changed
void loop() {
    
  if (updateFlag) {
    time = clock.getDateTime();
    getSpeed();
    updateSpeed();
    updateDisplay();
    updateFlag = false;
  }
}

//****************Functions*********************//
void updateDisplay() {
  sprintf(line0, "DIR: %-2s RPM:%4s", dir_display[clockwise], rpm_display[rpm]);
  sprintf(line1, "    %02d:%02d:%02d", time.hour, time.minute, time.second);  //sprintf: %[flags][width][.precision][length]specifier
  lcd.setCursor(0, 0);
  lcd.print(line0);
  lcd.setCursor(0, 1);
  lcd.print(line1);
}
//TODO: remove these lines? @conor
// short int motor_state = 0;
// short int max_state = 0;

void updateSpeed() {
  // TODO: update actual motor CCW and speed, truth table in L293D datasheet
  switch (rpm) {
    case full:
      digitalWrite(ENABLE_PIN, HIGH);
      break;
    case half:
      //Overcome friction.
      //This does not always work 100% of time, just tap fan if it does not work
      digitalWrite(ENABLE_PIN, HIGH); //FIXME: motor turned on to 100% and then half, maybe remove this?
      //Set speed to half
      analogWrite(ENABLE_PIN, HALF_SPEED);
      break;
    case three_quarter:
      analogWrite(ENABLE_PIN, THREE_FOURTHS_SPEED);
      break;
    case zero:
      //FIXME: rpm, default to zero at start
      // Fast stop + PWM low
      digitalWrite(COUNTERCLOCKWISE_PIN, LOW);
      digitalWrite(CLOCKWISE_PIN, LOW);

      digitalWrite(ENABLE_PIN, LOW);
      //Reset direction
      updateDirection(); //FIXME: direction should be called only when button pressed otherwise it's gllobal and should stay the same
      break;
  }
}

void updateDirection() {
  //TODO: maybe turn off before changing direction if any issues occur
  if(!clockwise) {
    digitalWrite(CLOCKWISE_PIN, LOW);
    digitalWrite(COUNTERCLOCKWISE_PIN, HIGH);
  } else {
    digitalWrite(CLOCKWISE_PIN, HIGH);
    digitalWrite(COUNTERCLOCKWISE_PIN, LOW);
  }
}

void getSpeed() {

  //TODO: insert sound module function here to be used in updateMotor
  //if C4 then increase else if A4 decrease, else keep current rpm
  //set rpm = zero, half, three_quarter or full
  rpm = half;
}


//****************Interrupts*********************//
ISR(TIMER1_COMPA_vect) {
  updateFlag = true;
}

void changeDir() {
  clockwise ^= 1;
  updateDirection();
  updateFlag = true;//FIXME: will be called with isr timer every second
}