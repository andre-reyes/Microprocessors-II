// Start of Serial read: reads if snake ate apple from snake.py
// and write: sends joystick new_direction to snake.py

#define joystick_x A0
#define joystick_y A1
#define analog_range 1024
int resting_threshold = analog_range/4;
int joystick_center = analog_range/2;
int x_dir = 0;
int y_dir = 0;
int incomingByte = 0; 
char new_direction = "";
char old_direction = "";
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // initialize digital pins
  pinMode(joystick_x, INPUT);
  pinMode(joystick_y, INPUT);

}

// the loop routine runs over and over again forever:
void loop() {
  // read from the Serial port:
  // if (Serial.available() > 0) {
  //   // read the incoming byte:
  //   incomingByte = Serial.read();
  //   Serial.println(incomingByte);
    
  //   if(incomingByte == 'E') {
  //     // flip LED
  //     led_status = (led_status == HIGH)? LOW : HIGH;
  //     digitalWrite(LED_BUILTIN, led_status);
  //   }
  // }

  x_dir = analogRead(joystick_x);
  y_dir = analogRead(joystick_y);

  // Serial.print("x = ");
  // Serial.print(x_dir);
  // Serial.print(", y = ");
  // Serial.println(y_dir);
  // delay(200);
  
  if(y_dir - joystick_center > resting_threshold){
    new_direction = 'w';
  }
  else if(abs(y_dir - joystick_center) > resting_threshold){
    new_direction = 's';
  }
  else if(x_dir - joystick_center > resting_threshold){
    new_direction = 'a';
  }
  else if(abs(x_dir - joystick_center) > resting_threshold){
    new_direction = 'd';
  }
  if(old_direction != new_direction){
    old_direction = new_direction;
    Serial.println(new_direction);
  }
  delay(100);
}
