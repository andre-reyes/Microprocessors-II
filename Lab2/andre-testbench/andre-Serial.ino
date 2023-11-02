// Start of Serial read: reads if snake ate apple from snake.py
// and write: sends joystick curr_dir to snake.py

#define joystick_x A0
#define joystick_y A1
#define analog_range 1024 // general range of the analog input of arduino
int js_resting_threshold = analog_range/4; // "deadzone" around joystick at rest, 25% all around
int js_center = analog_range/2; // at rest joystick hovers around 500
int js_x_dir = 0; // x value of joystick (left: -1024 to -750, right: 750 to 1024)
int js_y_dir = 0; // y value of joystick (down: -1024 to -750, up: 750 to 1024)
int incomingByte = 0; 
char curr_dir = "";  // current direction
char prev_dir = ""; // previous direction

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
  // TODO: Uncomment to read from the Serial port:
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


  // read joystick values and prevent repeated output
  js_x_dir = analogRead(joystick_x);
  js_y_dir = analogRead(joystick_y);

  if(js_y_dir - js_center > js_resting_threshold){
    curr_dir = 'w';
  }
  else if(abs(js_y_dir - js_center) > js_resting_threshold){
    curr_dir = 's';
  }
  else if(js_x_dir - js_center > js_resting_threshold){
    curr_dir = 'a';
  }
  else if(abs(js_x_dir - js_center) > js_resting_threshold){
    curr_dir = 'd';
  }
  if(prev_dir != curr_dir){
    prev_dir = curr_dir;
    Serial.println(curr_dir);
  }
}
