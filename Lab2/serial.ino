//  Andre Reyes, Conor Duston and Harrington Idahosa modified for Lab 2 - joystick/gyro comm. 
//  w/arduino of EECE.4520/5520
//  11/1/2023

//  Using the elegoo example scketch as a base for MPU-6050
//  Modified Example Sketches from www.elegoo.com used as a base for 
//  gyro implementation
//  serial-rw.ino used as a start to communication with snake.py
//  and modified for the joystick commuication for this lab
\
#include <Wire.h> // needed for serial commmunication with gyro

#define joystick_x A0
#define joystick_y A1
#define BUZZER_PIN 7
#define SCORE_CHAR 's'
#define TICKS_TIL_TIME_OUT 500
#define ANALOG_RANGE 1024 // general range of the analog input of arduino

// gyro variables
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t GyX, GyY;
int minVal = -2000;
int maxVal = 2000;
double gyro_x;
double gyro_y;
int sensitivity = 2;

// joystick variables
int js_resting_threshold = ANALOG_RANGE/4; // "deadzone" around joystick at rest, 25% all around
int js_center = ANALOG_RANGE/2; // at rest joystick hovers around 500
int js_x_dir = 0; // x value of joystick (left: -1024 to -750, right: 750 to 1024)
int js_y_dir = 0; // y value of joystick (down: -1024 to -750, up: 750 to 1024)
char curr_dir = "";  // current direction
char prev_dir = ""; // previous direction

// buzzer variables
unsigned int incomingByte = 0; 
unsigned int buzzer_ticks = 0;
bool buzzer_active = false;



void setup() {
  // set up wire comm. with MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  // initialize digital pins
  pinMode(joystick_x, INPUT);
  pinMode(joystick_y, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
  
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {

  /************************ ADDED BY HARRY - BEGIN ************************/
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43); // Start reading from the gyro data registers
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true); // request a total of 6 registers

  GyY = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)

  gyro_x = map(GyX, minVal, maxVal, -sensitivity, sensitivity);
  gyro_y = map(GyY, minVal, maxVal, -sensitivity, sensitivity);
  /************************ ADDED BY HARRY - END ************************/


  /************************ ADDED BY CONOR - BEGIN ************************/
  //Recieve apple eaten flag from snake.py
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.println(incomingByte);
    
    if(incomingByte == SCORE_CHAR) {
      digitalWrite(BUZZER_PIN, HIGH);
      buzzer_ticks = 0;
      buzzer_active = true;
    }
  }

  // Sound buzzer if flagged
  if (buzzer_active) {
      buzzer_ticks ++;

    if (buzzer_ticks > TICKS_TIL_TIME_OUT) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzer_ticks = 0;
      buzzer_active = false;
    }
  }
  /************************ ADDED BY CONOR - END ************************/


  /************************ ADDED BY ANDRE - BEGIN ************************/
  //Read joystick values and prevent repeated output
  js_x_dir = analogRead(joystick_x);
  js_y_dir = analogRead(joystick_y);

  //Andre, process direction output conditions
  if(js_y_dir - js_center > js_resting_threshold || gyro_x > sensitivity){
    curr_dir = 'w';
  }
  else if(abs(js_y_dir - js_center) > js_resting_threshold || gyro_x < -sensitivity){
    curr_dir = 's';
  }
  else if(js_x_dir - js_center > js_resting_threshold || gyro_y < -sensitivity){
    curr_dir = 'a';
  }
  else if(abs(js_x_dir - js_center) > js_resting_threshold || gyro_y > sensitivity){
    curr_dir = 'd';
  }
  if(prev_dir != curr_dir){
    prev_dir = curr_dir;
    Serial.println(curr_dir);
  }
  /************************ ADDED BY ANDRE - END ************************/

}
