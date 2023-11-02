

#include <Wire.h>

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t GyX, GyY;

int minVal = -2000;
int maxVal = 2000;

double gyro_x;
double gyro_y;


int sensitivity = 2;


void setup() {

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

}

void loop() {
  char new_direction = ' '; // Initialize new_direction with a space
  char old_direction = ' '; // Initialize old_direction 

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43); // Start reading from the gyro data registers
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true); // request a total of 6 registers

  GyY = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)

 // Uncomment this and delete or comment the res of the code after this to test 0-1024 scale
  // x = map(GyX, minVal, maxVal, 0, 1024);
  // y = map(GyY, minVal, maxVal, 0, 1024);
  //
  // For tilting left or right to control A or D movement.
  // the 250 is what the joystick uses, but
  // if (x - 250 > 500) {
  //   new_direction = 'w';
  // } 
  // else if (abs(x - 250) > 500) {
  //   new_direction = 's'; 
  // }

  // // For tilting up or down to control W or S movement.
  // else if (y - 250 > 500) {
  //   new_direction = 'd'; 
  // } 
  // else if (abs(y - 250) > 500) {
  //   new_direction = 'a'; 
  // }

  // if (old_direction != new_direction) {
  //   old_direction = new_direction;
  //   Serial.println(new_direction);
  // }
  // Adjust the sensitivity as needed based on your sensor and game requirements
  // You may need to calibrate the sensor and adjust these values.

  gyro_x = map(GyX, minVal, maxVal, -sensitivity, sensitivity);
  gyro_y = map(GyY, minVal, maxVal, -sensitivity, sensitivity);

  // Control WASD movement based on the gyro data.
  
  // For tilting left or right to control A or D movement.
  if (gyro_x > sensitivity) {
    new_direction = 'w'; // Right
  } 
  else if (gyro_x < -sensitivity) {
    new_direction = 's'; // Left
  }

  // For tilting up or down to control W or S movement.
  else if (gyro_y > sensitivity) {
    new_direction = 'd'; // Up
  } 
  else if (gyro_y < -sensitivity) {
    new_direction = 'a'; // Down
  }

  if (old_direction != new_direction) {
    old_direction = new_direction;
    Serial.println(new_direction);
  }