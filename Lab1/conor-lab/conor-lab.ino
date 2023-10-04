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

//Definitions for timer change points
//Time is given in seconds
#define BUZZER_ACTIVATION_TIME 3
#define FLASHING_START_TIME 3

#define red_ledpin 10
#define yellow_ledpin 11
#define green_ledpin 12

#define buzzer_pin 6

#define buttonInterruptPin 3


//State enumeration
enum light_states {
  reset,
  red,
  yellow,
  green
};

//storage variables
boolean toggle1 = 0;
//
volatile bool buttonPressed = false;

volatile unsigned int seconds_left = 0;

volatile light_states this_state = reset;

volatile byte current_light = red_ledpin;
volatile byte light_state = HIGH;

void setup(){
  Serial.begin(9600);
  //set pins as outputs
  pinMode(red_ledpin, OUTPUT);
  pinMode(yellow_ledpin, OUTPUT);
  pinMode(green_ledpin, OUTPUT);

  pinMode(buzzer_pin, OUTPUT);

  pinMode(buttonInterruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonInterruptPin), solidState, RISING); //initiates button interrupt (pin to watch for interrupt, function to execute on interrupt, when interrupt should trigger[i.e. rising, falling, high, low])

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 7812;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts

}//end setup

// Timer1's interrupt service routing (ISR)
// The code in ISR will be executed every time timer1 interrupt occurs
// That is, the code will be called once every second
// TODO
//   you need to set a flag to trigger an evaluation of the conditions
//   in your buttonPressed machine, then potentially transit to next buttonPressed
//
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  
  if (toggle1){
    //Serial.println("Toggle reset low");
    toggle1 = 0;
    
    if (this_state == reset) {
      //Serial.println("State is reset, doing nothing");
      return;
    }

    if(seconds_left <= FLASHING_START_TIME && (this_state == red || this_state == green)) {
      //Serial.println("Flash low");
      digitalWrite(current_light, LOW);
    }
  }
  else{
    toggle1 = 1;
    
    //Serial.println("Toggle reset HIGH");

    if (this_state == reset) {
      //Serial.println("Changing light state...");
      
      if (light_state == LOW) {
        light_state = HIGH;
      } else {
        light_state = LOW;
      }

      digitalWrite(red_ledpin, light_state);
      return;
    } 
    
    seconds_left--;

    if (seconds_left < FLASHING_START_TIME && (this_state == red || this_state == green)) {
      //Serial.println("Flash High");
      digitalWrite(current_light, HIGH);
    }
    
    if (seconds_left < BUZZER_ACTIVATION_TIME) {
      activate_buzzer();
    } 
    if (seconds_left == 0) {
      deactivate_buzzer();
      set_next_state();
    }
  }
}



void loop(){
  //do other things here
  if(buttonPressed){
    //do stuff here when button pressed
    
  }
  else{
    if (toggle1) {
    }
    else {
    }
  }
}

void solidState(){
    if ( buttonPressed == false) {
        Serial.println("Button Pressed once");
        buttonPressed = true;
        set_next_state();
    }
}

//Set function to only output to RED LED
void set_red() {
  digitalWrite(red_ledpin, HIGH);
  digitalWrite(yellow_ledpin, LOW);
  digitalWrite(green_ledpin, LOW);
  current_light = red_ledpin;
}

void set_yellow() {
  digitalWrite(red_ledpin, LOW);
  digitalWrite(yellow_ledpin, HIGH);
  digitalWrite(green_ledpin, LOW);
  current_light = yellow_ledpin;
}

void set_green() {
  digitalWrite(red_ledpin, LOW);
  digitalWrite(yellow_ledpin, LOW);
  digitalWrite(green_ledpin, HIGH);
  current_light = green_ledpin;
}

void turn_all_off() {
  digitalWrite(red_ledpin, LOW);
  digitalWrite(yellow_ledpin, LOW);
  digitalWrite(green_ledpin, LOW);
  deactivate_buzzer();
}

void red_light() {
  set_red();
  seconds_left = 24;
}

void yellow_light() {
 set_yellow();
 seconds_left = 3;
}

void green_light() {
  set_green();
  seconds_left = 20;
}

void activate_buzzer() {
  digitalWrite(buzzer_pin, HIGH);
}

void deactivate_buzzer() {
  digitalWrite(buzzer_pin, LOW);
}

void set_next_state() {
  if (this_state == reset || this_state == yellow) {
    
    red_light();
    this_state = red;
  
  } else if (this_state == green) {

    yellow_light();
    this_state = yellow;

  } else if (this_state == red) {
    
    green_light();
    this_state = green;
  
  } else {
    //ERROR STATE, should never occur
  }
}
