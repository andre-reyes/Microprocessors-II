//Define
const int redPin = 2;


// Timing
const unsigned long redDuration = 24000;    // 24 seconds


void setup() {
 pinMode(redPin, OUTPUT);

 }


 
 digitalWrite(redPin, HIGH);
 delay(redDuration - 3000);  // Stay red for 24 seconds, minus the last 3 seconds
 for (int i = 0; i < 6; i++) {
   digitalWrite(redPin, HIGH);
   delay(flashDuration);
   digitalWrite(redPin, LOW);
   delay(flashDuration);
 }
 
