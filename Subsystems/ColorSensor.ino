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
Adafruit_TCS34725 ColorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void ColorOpen() {
  Serial.begin(9600);

  while(ColorSensor.begin() == false){
    print("Color Sensor Not Found");
    if (ColorSensor.begin())
        break;

  }

  // Now we're ready to get readings!
}

void ColorGrab() {
  uint16_t r, g, b, c, colorTemp, lux;

  ColorSensor.getRawData(&r, &g, &b, &c);

  colorTemp = ColorSensor.calculateColorTemperature(r, g, b);
  lux = ColorSensor.calculateLux(r, g, b);
  /*
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");

  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");

  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  */
}

int colorReadRed(){
    uint16_t r;
    ColorSensor.get

}
void colorReadGreen()
void colorReadBlue()
void colorReadLux()
void colorReadColorTemp()
