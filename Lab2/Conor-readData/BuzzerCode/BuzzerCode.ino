/*
    Addapted from Serial-WR,
    made to turn on and off buzzer
*/

#define BUZZER_PIN 7
#define SCORE_CHAR 's'
#define TICKS_TIL_TIME_OUT 12000

bool buzzer_active = false;
unsigned int incomingByte = 0;
unsigned int buzzer_ticks = 0;

void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize digital pin Buzzer Pin as an output.
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
}

void loop() {
  // put your main code here, to run repeatedly:
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

  if (buzzer_active) {
    buzzer_ticks ++;

    if (buzzer_ticks > TICKS_TIL_TIME_OUT) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzer_ticks = 0;
      buzzer_active = false;
    }
  }
}
