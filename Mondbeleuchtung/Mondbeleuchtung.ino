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

    Alex Leone <acleone ~AT~ gmail.com>, 2009-02-03 */

#include <Tlc5940.h>

/* additional pins */
const int brightnessPoti = A0;
const int buttonPrev = 7;
const int buttonNext = 8;

const int maxChannel = 16 * NUM_TLCS;

int intensity = 0;
int buttonUpDnCounter = 0;  // counter for the number of button presses
//int buttonPrevState = 0;    // current state of the button previous
int buttonNextState = 0;    // current state of the button next
//int lastButtonPrevState = 0;    // current state of the button previous
int lastButtonNextState = 0;    // current state of the button next

int moonState = 0; // Beleuchtung

void setup() {
  Tlc.init();
  pinMode(brightnessPoti, INPUT);
  
  // initialize the button pins as inputs
  pinMode(buttonPrev, INPUT);
  pinMode(buttonNext, INPUT);

//  Serial.begin(9600);
}

void loop() {
  Tlc.clear();
  getBrightness();
  nextState();
  
  illuminateMoon();

  Tlc.update();
  delay(75);
}

void getBrightness() {;
  int sensorValue = analogRead(brightnessPoti); // get the sensor value
  intensity = map(sensorValue, 0, 1023, 0, 511); // map to TLC range
}

void nextState() {
  /*
  // read the button input pin
  buttonPrevState = digitalRead(buttonPrev);
  // compare the buttonState to its last state
  if (buttonPrevState != lastButtonPrevState) {
    // if the state has changed, decrement the counter
    if (buttonPrevState == HIGH) {
      // if the current state is HIGH then the button went from off to on
      buttonUpDnCounter--;
    }
  }
  // save the current state as the last state for next time through the loop
  lastButtonPrevState = buttonPrevState;
  */
  
  // read the button input pin
  buttonNextState = digitalRead(buttonNext);
  // compare the buttonState to its last state
  if (buttonNextState != lastButtonNextState) {
    // if the state has changed, increment the counter
    if (buttonNextState == HIGH) {
      // if the current state is HIGH then the button went from off to on
      buttonUpDnCounter++;
    }
  }
  // save the current state as the last state for next time through the loop
  lastButtonNextState = buttonNextState;
}

void illuminateMoon()
{
  boolean moonIncresing = (buttonUpDnCounter % 20) < 10;
  int linesToActivate = (buttonUpDnCounter % 10) + 1;
  int threshold;
  
  switch (linesToActivate) {
  case 1:
    threshold = 1;
    break;
  case 2:
    threshold = 4;
    break;
  case 3:
    threshold = 8;
    break;
  case 4:
    threshold = 12;
    break;
  case 5:
    threshold = 16;
    break;
  case 6:
    threshold = 20;
    break;
  case 7:
    threshold = 24;
    break;
  case 8:
    threshold = 28;
    break;
  case 9:
    threshold = 31;
    break;
  case 10:
    threshold = 32;
    break;
  default:;
  }
  threshold--;
  
  for (int channel = 0; channel < maxChannel; channel += 1) {  
    if (moonIncresing) { 
      // Newmoon and incresing
      if (channel < threshold) {
        Tlc.set(channel, intensity);
      }
    } else {
      // Fullmoon and decresing
      if (channel >= threshold) {
        Tlc.set(channel, intensity);
      }
    }
  }
}

void twoOn()
{
  // turns on two corresponting LEDs by checking the modulo of the up-down counter
  int activeChannel = buttonUpDnCounter % maxChannel;
  for (int channel = 0; channel < maxChannel; channel += 1) {
    if (channel == activeChannel) {
      Tlc.set(channel, 100);
    }
    else {
      Tlc.set(channel, 0);
    }
  }
}

void allOn() {
  Tlc.setAll(intensity);
}

