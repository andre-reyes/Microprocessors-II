#include "arduinoFFT.h"


#define SAMPLES 128
#define SAMPLING_FREQUENCY 1024


arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double peak = 0;
int curr_speed=0;
int prev_speed=0;

void setup() {
 Serial.begin(9600);
 samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}


void loop() {
  
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();
    vReal[i] = analogRead(0);
    vImag[i] = 0;

      //TODO: while loop is blocking, what's it for?
    while (micros() < (microSeconds + samplingPeriod)) {
    }
  }


  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

// Adjust the speed based on the detected note
  if (peak >= 257.0 && peak <= 267.0 && curr_speed > 0) {
  // C4, decrease speed
  curr_speed -= 1;
  } else if (peak >= 430.0 && peak <= 450.0 && curr_speed < 3) {
  // A4, increase speed
  curr_speed += 1;
  }
  if(prev_speed != curr_speed){
    prev_speed = curr_speed;
    Serial.println(curr_speed);
  }
}
