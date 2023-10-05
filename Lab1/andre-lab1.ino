// timer interrupt sample
// yluo modified for lab1 of EECE.4520/5520
// 9/12/2021
//
// original source:-
//timer interrupts
//by Amanda Ghassaei
//June 2012
//https://www.instructables.com/id/Arduino-Timer-Interrupts/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

//timer setup for timer0, timer1, and timer2.
//For arduino uno or any board with ATMEL 328/168.. diecimila, duemilanove, lilypad, nano, mini...

//this code will enable one arduino timer interrupt
//timer1 will interrupt at 1Hz

//storage variables
const byte buttonInterruptPin = 2;
const byte GREEN_LED = 9;
const byte YELLOW_LED = 9;
const byte RED_LED = 9;
volatile byte buttonPressed = 0;
unsigned long blinkInterval = 500; // 0.5 seconds
unsigned long greenInterval = 20000; // 20 seconds
unsigned long yellowInterval = 3000; // 3 seconds
unsigned long redInterval = 24000; // 24 seconds
volatile byte red_flag = 0; //determine if in red light state
volatile byte green_flag = 0; //determine if in green light state
volatile byte blink_state = 0;
unsigned long blink_duration = 3000;
void setup(){
  
  //set pins as outputs
  pinMode(9, OUTPUT);
  pinMode(buttonInterruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonInterruptPin), ISR_button, RISING); //initiates button interrupt (pin to watch for interrupt, function to execute on interrupt, when interrupt should trigger[i.e. rising, falling, high, low])

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);


  sei();//allow interrupts

}//end setup


void loop(){
  if(buttonPressed){
    unsigned long currentMillis = 0;
    OCR1A = 31249; //(16*10^6) / (0.5*1024) - 1, change register to reflect 0.5 second intervals after button pressed

    //turn red on 24 seconds, blink/buzz last 3 seconds
    do{
      currentMillis = millis();
      digitalWrite(RED_LED, HIGH);

      if((millis()- currentMillis) - redInterval <= blink_duration){
        //turn on buzzer here
        digitalWrite(RED_LED, blink_state); 
      }
    }while(millis() - currentMillis < redInterval);
    digitalWrite(RED_LED, LOW);

    //turn green on 20 seconds, blink/buzz last 3 seconds
    do{
      currentMillis = millis();
      digitalWrite(GREEN_LED, HIGH);

      if((millis()- currentMillis) - greenInterval <= blink_duration){
        //turn on buzzer here
        digitalWrite(GREEN_LED, blink_state); 
      }
    }while(millis() - currentMillis < greenInterval);
    digitalWrite(GREEN_LED, LOW);

    //turn yellow on 3 seconds
    do{
      currentMillis = millis();
      digitalWrite(YELLOW_LED, HIGH);
    }while(millis() - currentMillis < yellowInterval);
    digitalWrite(YELLOW_LED, LOW);
  }
  
}

// Timer1's interrupt service routing (ISR)
// The code in ISR will be executed every time timer1 interrupt occurs
// That is, the code will be called once every second
// TODO
//   you need to set a flag to trigger an evaluation of the conditions
//   in your buttonPressed machine, then potentially transit to next buttonPressed
//
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 9 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  blink_state ^ 1;
}

void ISR_button(){
  buttonPressed ^ 1;
}