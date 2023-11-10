//libraries


#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h> // location: Lab3\datasheets\Lesson 33 Real Time Clock Module\DS1307.zip
                    // to install: in arduino ide > sketch > include library > add .ZIP library

DS3231 clock;
RTCDateTime time;

// initialize the library with the numbers of the interface pins
const int v0 = 5, rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char line0[17];
char line1[17];
char spd[5] = "3/4";
char dir[3] = "C";
//initialize functions
void updateDisplay();
// Main code starts here
void setup() {
  // set up the LCD
  pinMode(v0, OUTPUT);
  analogWrite(v0, 100);
  lcd.begin(16, 2);

  // Set up clock
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);
  time = clock.getDateTime();
}


void loop() {
  //TODO: timer interrupt to display clock/rpm/direction every sec
  updateDisplay();
}

void updateDisplay() {
  sprintf(line0, "DIR: %-2s SPD:%4s" , dir, spd);
  sprintf(line1, "    %02d:%02d:%02d", time.hour, time.minute, time.second); //sprintf: %[flags][width][.precision][length]specifier
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);
  lcd.print(line1);
}


