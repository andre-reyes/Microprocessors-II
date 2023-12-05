// Andre Reyes, Conor Duston and Harrington Idahosa 
// Lab 3 - Use Sensors to Control Motors
// w/arduino of EECE.4520
// 12/5/2023

// Motor_control.ino
// This sketch combines the functions of a DC motor, microphone and 
// a real-time clock to ouput to an LCD. The microphone works together 
// with a button to control the speed and direction of the motor. While  
// this is happening, the RTC is providing the time and update frequency 
// for the LCD to display the current state of the motor. 

// Contribution definitions: 
// Primary - Wrote most of the functionality and base of code snippet
// Secondary - Provided fine tuning, debugging and/or added functionality  

//libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include <arduinoFFT.h>
#include <DS3231.h> 


//Library objects
DS3231 clock;
RTCDateTime time;
arduinoFFT FFT = arduinoFFT();

//Motor state enum to switch states (i.e. rpm = full)
enum motor_speed { zero,
                   half,
                   three_quarter,
                   full };
motor_speed rpm;  

// Motor pins
#define CLOCKWISE_PIN 2
#define COUNTERCLOCKWISE_PIN 3
#define ENABLE_PIN 4
#define MIC_PIN 0

//PWM values - only needed for 1/2 and 3/4, off and full are digital
#define HALF_SPEED 128
#define THREE_FOURTHS_SPEED 192


#define SAMPLES 128 //Must be a 2^x value
#define SAMPLING_FREQUENCY 1024 //Generally twice as much as the highest frequency being sensed

//LCD interface pins
const uint8_t v0 = 5, rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char line0[17];                                               //Top line buffer on LCD
char line1[17];                                               //Bottom line Buffer on LCD
char rpm_display[4][5] = { "   0", " 1/2", " 3/4", "Full" };  //Display string for rpm on LCD
char dir_display[2][3] = { " C", "CC" };                      //Display string for direction on LCD

//Flags
volatile bool updateFlag = false;    //Timer1 flag to update display/motor
volatile bool clockwise = 1;         //Direction flag 1 == "C" 0 == "CC"
volatile byte button_pin = 19;       //button pin assignment

//Sound module variables 
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double peak = 0;

//Function declarations
void updateDisplay();           //outputs direction, rpm, time to lcd display
void updateSpeed(double peak);  //updates rpm based on input from listenForPeak()
void setMotorDir();             //Sets the direction using the pins for the L293D
void changeDir();               //changes value of motor direction based on the ISR button flag
double listenForPeak();         //returns peak sound value


void setup() { //Primary: Andre Secondary: Conor
  pinMode(v0, OUTPUT);
  analogWrite(v0, 100);
  lcd.begin(16, 2);

  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);

  pinMode(button_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(button_pin), changeDir, RISING);

  setMotorDir();

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


void loop() { //Primary: Andre Secondary: Conor

  double peak = listenForPeak();
  
  if (updateFlag) {
    time = clock.getDateTime();
    updateSpeed(peak);
    updateDisplay();
    updateFlag = false;
  }
}

//****************Functions*********************//
void updateDisplay() { //Primary: Andre
  sprintf(line0, "DIR: %-2s RPM:%4s", dir_display[clockwise], rpm_display[rpm]);
  sprintf(line1, "    %02d:%02d:%02d    ", time.hour, time.minute, time.second);  //sprintf: %[flags][width][.precision][length]specifier
  
  lcd.setCursor(0, 0);
  lcd.print(line0);
  
  lcd.setCursor(0, 1);
  lcd.print(line1);
}

void updateSpeed(double peak) { //Primary: Conor Secondary: Andre, Harrington
  if (peak >= 257.0 && peak <= 267.0 && rpm > zero) {
    // C4, decrease speed
    rpm = rpm - 1;
  } else if (peak >= 430.0 && peak <= 450.0 && rpm < full) {
    // A4, increase speed
    rpm = rpm + 1;
  }

  switch (rpm) {
    case full:
      digitalWrite(ENABLE_PIN, HIGH);
      break;
    case half:
    Serial.println(" half");
      //To overcome friction. Put full power to motor for split second to start it up
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

void setMotorDir() { //Primary: Conor 
  if(!clockwise) {
    digitalWrite(CLOCKWISE_PIN, LOW);
    digitalWrite(COUNTERCLOCKWISE_PIN, HIGH);
  } else {
    digitalWrite(CLOCKWISE_PIN, HIGH);
    digitalWrite(COUNTERCLOCKWISE_PIN, LOW);
  }
}

double listenForPeak() {//Primary: Harrington Secondary: Andre
  //listen for raw values
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();
    vReal[i] = analogRead(MIC_PIN);
    vImag[i] = 0;

    // while loop delay for raw data gathering time
    while (micros() < (microSeconds + samplingPeriod)) {
    }
  }

  //calculate peak using raw values
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  return peak;
}

//****************Interrupts*********************//
ISR(TIMER1_COMPA_vect) { //Primary: Andre
  //Update display with time
  updateFlag = true;
}

void changeDir() { //Primary: Andre Secondary: Conor
  //Button ISR to change direction of fan, this occurs async
  clockwise ^= 1;
  setMotorDir();
  updateFlag = true; 
}