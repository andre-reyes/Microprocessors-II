// changelog: 
// 10/16/23: 
//  - setup lab2 files, tested Yan Luo's example and succeeded in getting a serial connection
//  - connected joystick and implemented a basic code structure using analogRead() in order to gain an 
//      understanding of joystick diresction values. 0-1024 hovering around 500
//  - values varied at center so a threshold was created, the idea splitting each direction into 4 
//      where only the most extreme changes (at least a 25% change) will register a direction input
//  - conditions were then added to an if/else statement to determine the direction and ouputs it
//       in the form of a letter to the arduino serial, which is the read by andre-pyserial.py
//  - andre-pyserial.py was added as a test bench for just the serial communication between the
//      arduino and pc. the base of this code was taken from snake.py movement functions

#define joystick_x A0
#define joystick_y A1
#define analog_range 1024
int resting_threshold = analog_range/4;
int joystick_center = analog_range/2;
int x_dir = 0;
int y_dir = 0;
int incomingByte = 0; 

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
    Serial.println("w");
  }
  else if(abs(y_dir - joystick_center) > resting_threshold){
    Serial.println("s");
  }
  else if(x_dir - joystick_center > resting_threshold){
    Serial.println("a");
  }
  else if(abs(x_dir - joystick_center) > resting_threshold){
    Serial.println("d");
  }
  else{
    Serial.println("");
  }
}
