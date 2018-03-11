/*
  Spieluhr_with_TMRpcm
 
 Dieses Programm macht aus einer Kiste mit drei Schubfaechern (und einem Arduino) eine Spieluhr mit drei Schaltern, 
 die unterschiedliche .wav-Dateien von einer SD Karte abspielen kann. 

 The circuit:
 * SD card is attached to SPI bus as follows:                                                                  */
#define PIN_ChipSelect 10                                                                                      /*
 ** CS - This can be the hardware SS pin - pin 10 (on most Arduino boards) or pin 53 (on the Mega) - 
         or another pin specified in the call to SD.begin().
         Even if you don't use the hardware SS pin, it must be left as an output or the SD library won't work. 
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 * Speaker (PWM) and Buttons for music playback are attached as follows:                                     */
#define PIN_BUTTON1  4
#define PIN_BUTTON2  7
#define PIN_BUTTON3  8
#define PIN_SPEAKER 9                                                                                        /*

 created Mrz 2016
 by EarlAngus
 
*/

#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>

TMRpcm music;   // create an object for use in this sketch
uint8_t tracknumber; // the track to be played

void setup(){

  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
  pinMode(PIN_BUTTON3, INPUT_PULLUP);

  music.speakerPin = PIN_SPEAKER; //11 on Mega, 9 on Uno, Nano, etc

  Serial.begin(9600);
  if (!SD.begin(PIN_ChipSelect)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }

  tracknumber = 255;
  music.loop(0);
  music.setVolume(3);
}

void loop(){
    if (music.isPlaying() == 0) {
      playMusic();
    }
    if (changeMusic()) {
      debugOutput("Change track\nNew Song is track" + String(tracknumber) + ".wav\n");
      music.stopPlayback();
      playMusic();
    }
    delay(50);
}

boolean changeMusic() {
  uint8_t oldTrack = tracknumber;
  tracknumber = 0;
  if (digitalRead(PIN_BUTTON1) == LOW) { tracknumber |= B001; }
  if (digitalRead(PIN_BUTTON2) == LOW) { tracknumber |= B010; }
  if (digitalRead(PIN_BUTTON3) == LOW) { tracknumber |= B100; }
  return (oldTrack != tracknumber);
}

void playMusic() {
  switch(tracknumber) {
    case 0: music.play("track0.wav");
            break;
    case 1: music.play("track1.wav");
            break;
    case 2: music.play("track2.wav"); 
            break;
    case 3: music.play("track3.wav"); 
            break;
    case 4: music.play("track4.wav"); 
            break;
    case 5: music.play("track5.wav"); 
            break;
    case 6: music.play("track6.wav"); 
            break;
    case 7: music.play("track7.wav"); 
            break;
    default:break;
  }
  if (music.isPlaying()) {
    debugOutput("Playback started\n");
  } else {
    debugOutput("Track not found\n");
  }
}

void debugOutput(String text) {
  if (Serial.availableForWrite()) {
    Serial.print(text);
  }
}

