#define FOWARD 2
#define BACKWARD 3
#define ENABLE 4

#define MAX_ITTER 8192

#define OFF 0
#define HALF_SPEED 128
#define THREE_FOURTHS_SPEED 192
#define FULL_SPEED 255

int itteration = 0;

volatile int8_t motor_state = 0;
int8_t max_state = 3;

void updateMotor();
void getSpeed();
void setSpeed();

void setup() {
  // put your setup code here, to run once:
  pinMode(FOWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(FOWARD, HIGH);
  digitalWrite(BACKWARD, LOW);
  Serial.begin(9600);

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

void loop() {
  // put your main code here, to run repeatedly:
  
  
}
//**Interupt**//
ISR(TIMER1_COMPA_vect) {
  motor_state = (motor_state + 1) % (max_state + 1);
  setSpeed();
}

void setSpeed () {
  
  switch (motor_state) {
    case 1:
      //Overcome friction
      digitalWrite(ENABLE_PIN, HIGH);
      
      //Then set speed to half
      analogWrite(ENABLE_PIN, HALF_SPEED);
      Serial.println("Set to 1/2 Speed");
      
      break;
    case 2:
      analogWrite(ENABLE_PIN, THREE_FOURTHS_SPEED);
      Serial.println("Set to 3/4 Speed");
      
      break;
    case 3:
      digitalWrite(ENABLE_PIN, HIGH);
      Serial.println("Set to full Speed");
      break;
    case 0:
    default:
      digitalWrite(ENABLE_PIN, LOW);
      Serial.println("Turned off");
      
      
  }
}
