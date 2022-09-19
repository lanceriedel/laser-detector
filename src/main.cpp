#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_1X);
int led = 13;
int avg_values_countdown = 20;
uint32_t total_k = 0;
uint32_t total_r = 0;
uint32_t avg_k = 0;
uint32_t avg_r=0;


void setup(void) {
  Serial.begin(9600);
    pinMode(led, OUTPUT);     


  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Now we're ready to get readings!
}

void loop(void) {

  uint16_t r, g, b, c, colorTemp, lux;
  

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");

  Serial.println(" ");
  if (avg_values_countdown) {
    total_k+=colorTemp;
    total_r+=r;
    delay(500);
    Serial.print("Calibrating... "); Serial.print(" ");
    avg_values_countdown--;
    return;
  }

  //Calculate the Average
  if (avg_k==0) {
    avg_k = total_k/20;
    avg_r = total_r/20;
    Serial.print("AVG K: "); Serial.print(avg_k, DEC); Serial.println(" ");
    Serial.print("AVG R: "); Serial.print(avg_r, DEC); Serial.println(" ");
  }




  int32_t dff_temp = (abs(avg_k-colorTemp));
  if (dff_temp>100000) dff_temp = 0;
  //if ((abs(avg_r-r))>100) {
    Serial.print("DIFF K: "); Serial.print(dff_temp); Serial.println(" ");

  if (dff_temp>500) {
    Serial.print("FLASH LED"); Serial.println(" ");

    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(led, LOW); 
  }

 }