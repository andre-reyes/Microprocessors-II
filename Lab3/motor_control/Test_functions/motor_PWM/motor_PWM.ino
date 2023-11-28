#define FOWARD 2
#define BACKWARD 3
#define ENABLE 4

#define MAX_ITTER 8192

#define OFF 0
#define HALF_SPEED 128
#define THREE_FOURTHS_SPEED 192
#define FULL_SPEED 255

int itteration = 0;

int8_t motor_state = 0;
int8_t max_state = 3;

void updateMotor();
void getSpeed();
void setSpeed();

void setup() {
  // put your setup code here, to run once:
  pinMode(FOWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(ENABLE, OUTPUT);

  digitalWrite(FOWARD, HIGH);
  digitalWrite(BACKWARD, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  updateMotor();
  
}

void updateMotor(){
  // TODO: update actual motor dir and speed, truth table in L293D datasheet 
  getSpeed();
  
}

void getSpeed(){
  if (itteration == MAX_ITTER) {
    itteration = 0;
    motor_state = (motor_state + 1) % (max_state + 1);
    Serial.println("Max itterations reached");
    setSpeed();

  } else {
    itteration ++;
  }
}

void setSpeed () {
  
  switch (motor_state) {
    case 1:
      //Overcome friction
      digitalWrite(ENABLE, HIGH);
      
      //Then set speed to half
      analogWrite(ENABLE, HALF_SPEED);
      Serial.println("Set to 1/2 Speed");
      delay(20000);
      break;
    case 2:
      analogWrite(ENABLE, THREE_FOURTHS_SPEED);
      Serial.println("Set to 3/4 Speed");
      delay(1000);
      break;
    case 3:
      digitalWrite(ENABLE, HIGH);
      Serial.println("Set to full Speed");
      delay(1000);
      break;
    case 0:
    default:
      digitalWrite(ENABLE, LOW);
      Serial.println("Turned off");
      delay(1000);
  }
}
