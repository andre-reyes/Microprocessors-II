//libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include <arduinoFFT.h>
#include <DS3231.h>  // location: Lab3\datasheets\Lesson 33 Real Time Clock Module\DS1307.zip
                     // to install: in arduino ide > sketch > include library > add .ZIP library



DS3231 clock;
RTCDateTime time;
arduinoFFT FFT = arduinoFFT();
//Motor state enum to switch states (i.e. rpm = full)
enum motor_speed { zero,
                   half,
                   three_quarter,
                   full };
motor_speed rpm;  //default: off state

// Motor pins
#define CLOCKWISE_PIN 2
#define COUNTERCLOCKWISE_PIN 3
#define ENABLE_PIN 4

//PWM values - only needed for 1/2 and 3/4, off and full are digital
#define HALF_SPEED 128
#define THREE_FOURTHS_SPEED 192

//sound module
#define SAMPLES 128
#define SAMPLING_FREQUENCY 1024

//LCD interface pins
const uint8_t v0 = 5, rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char line0[17];                                          //Top line buffer on LCD
char line1[17];                                          //Bottom line Buffer on LCD
char rpm_display[4][5] = { "   0", " 1/2", " 3/4", "Full" };  //Display string for rpm on LCD
char dir_display[2][3] = { " C", "CC" };                  //Display string for direction on LCD

//Flags
volatile bool updateFlag = false;  //Timer1 flag to update display/motor
volatile bool clockwise = 1;         // direction flag 1 == "C" 0 == "CC"
volatile byte button_pin = 19;     //button pin assignment

//sound variables 
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double peak = 0;

//Declare functions
void updateDisplay();
void updateSpeed();
void setMotorDir();
void changeDir();
void getSpeed();
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
  setMotorDir();

  //setup sound module
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

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

  getSpeed();
  
  if (updateFlag) {
    time = clock.getDateTime();
    //TODO: insert something here or in getSpeed() to prevent stage skipping
    updateSpeed();
    updateDisplay();
    updateFlag = false;
  }
}

//****************Functions*********************//
void updateDisplay() {
  sprintf(line0, "DIR: %-2s RPM:%4s", dir_display[clockwise], rpm_display[rpm]);
  sprintf(line1, "    %02d:%02d:%02d    ", time.hour, time.minute, time.second);  //sprintf: %[flags][width][.precision][length]specifier
  
  lcd.setCursor(0, 0);
  lcd.print(line0);
  //Second row
  lcd.setCursor(0, 1);
  lcd.print(line1);
}

void updateSpeed() {

  switch (rpm) {
    case full:
      analogWrite(ENABLE_PIN, 255);
      break;
    case half:
      //Overcome friction. Put full power to motor for split second to start it up
      //This does not always work 100% of time, just tap fan if it does not work
      digitalWrite(ENABLE_PIN, HIGH);
      //Set speed to half
      analogWrite(ENABLE_PIN, HALF_SPEED);
      break;
    case three_quarter:
      analogWrite(ENABLE_PIN, THREE_FOURTHS_SPEED);
      break;
    case zero:
      // Fast stop + PWM low
      digitalWrite(COUNTERCLOCKWISE_PIN, LOW);
      digitalWrite(CLOCKWISE_PIN, LOW);

      digitalWrite(ENABLE_PIN, LOW);
      //Reset direction pins so that motor will spin next update
      setMotorDir();
      break;
  }
}

void setMotorDir() {
  //Turning off motor not required, can just switch direction by changing output polarity
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
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();
    vReal[i] = analogRead(0);
    vImag[i] = 0;

      //TODO: while loop is blocking, what's it for?
    while (micros() < (microSeconds + samplingPeriod)) {
    }
  }


  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  // Adjust the speed based on the detected note
  if (peak >= 257.0 && peak <= 267.0 && rpm > zero) {
    // C4, decrease speed
    rpm = rpm - 1;
  } else if (peak >= 430.0 && peak <= 450.0 && rpm < full) {
    // A4, increase speed
    rpm = rpm + 1;
  }
  
}

//****************Interrupts*********************//
ISR(TIMER1_COMPA_vect) {
  //Update display with time
  updateFlag = true;
}

//This occurs async
void changeDir() {
  clockwise ^= 1;
  updateFlag = true; // update display IMMEDIATLY with change in direction this tick - done through button press
  setMotorDir();
}