/* 
  Morselampe
  Ueber die USB Schnittstelle sollen Nachrichten entgegengenommen werden.
  Die onboard LED an Pin 13 soll die Nachrichten als Morsezeichen ausgeben. 
*/

// Die LED auf Pin 13
int led = 13;
// Standardzeit zum Anzeigen eines Blinks der LED (Anzeigedauer eines Dots) in ms.
int defaultTime = 500;
// Puffer fuer Eingabe
int incomingByte = 0;
// Puffer fuer ein Morsezeichen
String morseZeichen = "";
// Flag um Wortende zu erkennen
int betweenTwoWords = false;

void setup() {
  // den Pin fuer die Ausgabe initialisieren
  pinMode(led, OUTPUT);
  // Seriellen Port oeffnen
  Serial.begin(9600);
}

// Ein Durchlauf der loop() liest ein Zeichen ein, wandelt es in Morse Code um und gibt es auf der LED aus.
void loop () {
  // die LED nur blinken lassen, wenn Daten empfangen wurden
  if (Serial.available() > 0) {
    // Seriell Daten einlesen
    incomingByte = Serial.read();
    
    // Nur alphanumerische Zeichen bearbeiten
    if (isAlphanumerisch(incomingByte)) {
      // Datum umwandeln in MorseCode
      charToMorseCode(incomingByte);
      // Zeichen als MorseCode auf LED ausgeben
      morseCodeAusgeben(morseZeichen);
    }
  }   
}

void morseCodeAusgeben(String morseCode) {
  for (int i = 0; i < morseCode.length(); i++) {
    if (morseCode[i] == '.') { //DOT
      digitalWrite(led, HIGH);
      delay(defaultTime);
      digitalWrite(led, LOW);
      delay(defaultTime);
    } else if(morseCode[i] == '-') { //DASH
      digitalWrite(led, HIGH);
      delay(defaultTime * 3);
      digitalWrite(led, LOW);
      delay(defaultTime);
    } 
  }
  // Zeichenende
  delay(defaultTime * 2);
}

int isAlphanumerisch(int character) {
 return ((incomingByte >= 0x30 && incomingByte <= 0x39) ||
         (incomingByte >= 0x41 && incomingByte <= 0x5A) ||
         (incomingByte >= 0x61 && incomingByte <= 0x7A) ||
          incomingByte == 0x20);
}

/* 
  charToMorseCode
  Wandelt ein Zeichen in eine Folge von Dots und Dashes.
*/
void charToMorseCode (char input) {
  // Grossbuchstaben in Kleinbuchstaben umwandeln
  if (input >= 0x41 && input <= 0x5A) {
    input += 0x20;
  }
  
  // Leerzeichen gesondert bearbeiten
  if (input == 0x20) {
    betweenTwoWords = true;
  } else {
    betweenTwoWords = false;
  }
  
  switch(input) {
  case 0x20:
    break;
  case 0x61:
    morseZeichen = ".-";
    break;
  case 0x62:
    break;
  case 0x63:
    break;
  case 0x64:
    break;
  case 0x65:
    break;
  case 0x66:
    break;
  case 0x67:
    break;
  case 0x68:
    break;
  case 0x69:
    break;
  case 0x6A:
    break;
  case 0x6B:
    break;
  case 0x6C:
    break;
  case 0x6D:
    break;
  case 0x6E:
    break;
  case 0x6F:
    break;
  case 0x70:
    break;
  case 0x71:
    break;
  case 0x72:
    break;
  case 0x73:
    break;
  case 0x74:
    break;
  case 0x75:
    break;
  case 0x76:
    break;
  case 0x77:
    break;
  case 0x78:
    break;
  case 0x79:
    break;
  case 0x7A:
    break;
  case 0x30:
    break;
  case 0x31:
    break;
  case 0x32:
    break;
  case 0x33:
    break;
  case 0x34:
    break;
  case 0x35:
    break;
  case 0x36:
    break;
  case 0x37:
    break;
  case 0x38:
    break;
  case 0x39:
    break;
  default:
    break;    
  }
}


