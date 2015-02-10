/*
    Basic Pin setup:
    ------------                                  ---u----
    ARDUINO   13|-> SCLK (pin 25)           OUT1 |1     28| OUT channel 0
              12|                           OUT2 |2     27|-> GND (VPRG)
              11|-> SIN (pin 26)            OUT3 |3     26|-> SIN (pin 11)
              10|-> BLANK (pin 23)          OUT4 |4     25|-> SCLK (pin 13)
               9|-> XLAT (pin 24)             .  |5     24|-> XLAT (pin 9)
               8|                             .  |6     23|-> BLANK (pin 10)
               7|                             .  |7     22|-> GND
               6|                             .  |8     21|-> VCC (+5V)
               5|                             .  |9     20|-> 2K Resistor -> GND
               4|                             .  |10    19|-> +5V (DCPRG)
               3|-> GSCLK (pin 18)            .  |11    18|-> GSCLK (pin 3)
               2|                             .  |12    17|-> SOUT
               1|                             .  |13    16|-> XERR
               0|                           OUT14|14    15| OUT channel 15
    ------------                                  --------

    -  Put the longer leg (anode) of the LEDs in the +5V and the shorter leg
         (cathode) in OUT(0-15).
    -  +5V from Arduino -> TLC pin 21 and 19     (VCC and DCPRG)
    -  GND from Arduino -> TLC pin 22 and 27     (GND and VPRG)
    -  digital 3        -> TLC pin 18            (GSCLK)
    -  digital 9        -> TLC pin 24            (XLAT)
    -  digital 10       -> TLC pin 23            (BLANK)
    -  digital 11       -> TLC pin 26            (SIN)
    -  digital 13       -> TLC pin 25            (SCLK)
    -  The 2K resistor between TLC pin 20 and GND will let ~20mA through each
       LED.  To be precise, it's I = 39.06 / R (in ohms).  This doesn't depend
       on the LED driving voltage.
    - (Optional): put a pull-up resistor (~10k) between +5V and BLANK so that
                  all the LEDs will turn off when the Arduino is reset.

    If you are daisy-chaining more than one TLC, connect the SOUT of the first
    TLC to the SIN of the next.  All the other pins should just be connected
    together:
        BLANK on Arduino -> BLANK of TLC1 -> BLANK of TLC2 -> ...
        XLAT on Arduino  -> XLAT of TLC1  -> XLAT of TLC2  -> ...
    The one exception is that each TLC needs it's own resistor between pin 20
    and GND.

    This library uses the PWM output ability of digital pins 3, 9, 10, and 11.
    Do not use analogWrite(...) on these pins.

    This sketch does the Knight Rider strobe across a line of LEDs.

    Alex Leone <acleone ~AT~ gmail.com>, 2009-02-03 */

#include "Tlc5940.h"

int inputPin = 2;

void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  Tlc.init();
  pinMode(inputPin, INPUT);
}

void loop()
{
  testAll();
//  knightRider();
//  fillingOrChasing();
}

void testAll() {;
  int sensorValue = analogRead(0); // get the sensor value
  int intensity = map(sensorValue, 0, 1023, 0, 4095); // map to TLC range
  Tlc.clear();
  for (int channel = 0; channel < 16*NUM_TLCS; channel += 1) {
    Tlc.set(channel, intensity); // set the intensity for this LED
  }
    Tlc.update();
    delay(75);
}

void knightRider() {
  const int sensorPin = 4;
  int direction = 1;
  int sensorValue = analogRead(0); // get the sensor value
  int intensity = map(sensorValue, 0, 1023, 0, 4095); // map to TLC range
  int dim = intensity / 4; // 1/4 the value dims the LED

  for (int channel = 0; channel < 16*NUM_TLCS; channel += direction) {
    Tlc.clear();
    if (channel == 0) {
      direction = 1;
    } else {
      Tlc.set(channel - 1, dim); // set the intensity for prev LED
    }
    Tlc.set(channel, intensity); // set the intensity for this LED
    if (channel < 15) {
      Tlc.set(channel + 1, dim); // set the intensity for next LED
    } else {
      direction = -1;
    }
    Tlc.update();
    delay(75);
  }
}

void fillingOrChasing() {

  int PHALANXSIZE = 2;
  int POW_HIGH = 4095;
  int POW_NORM = 100;
  int POW_LOW = 0;

  while(true) {
    int val = digitalRead(inputPin);
    if (val == LOW) {
      Tlc.clear();
      for (int channel = 0; channel < 16*NUM_TLCS; channel++) {
        int pow = 0;
        switch (((channel-1)/PHALANXSIZE)%3) {
          case 2:
          case 1:
            pow = POW_LOW;
            break;
          case 0:
            pow = POW_HIGH;
            break;
          default:
            break;
        }
        Tlc.set(channel, pow);
      }
      Tlc.update();
      delay(100);
      
      Tlc.clear();
      for (int channel = 0; channel < 16*NUM_TLCS; channel++) {
        int pow = 0;
        switch (((channel-1)/PHALANXSIZE)%3) {
          case 1:
            pow = POW_HIGH;
            break;
          case 2:
          case 0:
            pow = POW_LOW;
            break;
          default:
            break;
        }
        Tlc.set(channel, pow);
      }
      Tlc.update();
      delay(100);
      
      Tlc.clear();
      for (int channel = 0; channel < 16*NUM_TLCS; channel++) {
        int pow = 0;
        switch (((channel-1)/PHALANXSIZE)%3) {
          case 2:
            pow = POW_HIGH;
            break;
          case 1:
          case 0:
            pow = POW_LOW;
            break;
          default:
            break;
        }
        Tlc.set(channel, pow);
      }
      Tlc.update();
      delay(100);
      
    } else {
      Tlc.clear();
      for (int channel = 0; channel < 16*NUM_TLCS; channel++) {
        Tlc.set(channel, POW_NORM);
        Tlc.update();
        delay(100);
      }
    }
  }
}

