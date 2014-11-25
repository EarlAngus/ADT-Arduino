
/*
 * Belegung der Pins.
 * Sieben Segment Anzeige steht auf dem Kopf (Beginne unten zu zaehlen).
 * Segment 3 (Mitte, oben) und 4 (Links, oben) sind vertauscht.
 */
const int aa = 4;
const int ab = 5;
const int ac = 6;
const int ad = 1;
const int ae = 2;
const int af = 3;
const int ag = 7;
const int cat = 8;
const int button = 9;

/*
 * Die Funktion erwartet eine Zahl zwischen 0-9 als Parameter und aktiviert die
 * PINs zur Darstellung einer Ziffer auf einer Siebensegmentanzeige.
 */
void siebenSegZiffer(int zahl){
  switch(zahl){
    case 0:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, HIGH);
      digitalWrite(af, HIGH);
      digitalWrite(ag, LOW);
    break;
    case 1:
      digitalWrite(aa, LOW);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, LOW);
      digitalWrite(ae, LOW);
      digitalWrite(af, LOW);
      digitalWrite(ag, LOW);
    break;
    case 2:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, LOW);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, HIGH);
      digitalWrite(af, LOW);
      digitalWrite(ag, HIGH);
    break;
    case 3:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, LOW);
      digitalWrite(af, LOW);
      digitalWrite(ag, HIGH);
    break;
    case 4:
      digitalWrite(aa, LOW);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, LOW);
      digitalWrite(ae, LOW);
      digitalWrite(af, HIGH);
      digitalWrite(ag, HIGH);
    break;
    case 5:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, LOW);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, LOW);
      digitalWrite(af, HIGH);
      digitalWrite(ag, HIGH);
    break;
    case 6:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, LOW);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, HIGH);
      digitalWrite(af, HIGH);
      digitalWrite(ag, HIGH);
    break;
    case 7:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, LOW);
      digitalWrite(ae, LOW);
      digitalWrite(af, LOW);
      digitalWrite(ag, LOW);
    break;
    case 8:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, HIGH);
      digitalWrite(af, HIGH);
      digitalWrite(ag, HIGH);
    break;
    case 9:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, LOW);
      digitalWrite(af, HIGH);
      digitalWrite(ag, HIGH);
    break;
    case 10:
      digitalWrite(aa, HIGH);
      digitalWrite(ab, HIGH);
      digitalWrite(ac, HIGH);
      digitalWrite(ad, HIGH);
      digitalWrite(ae, HIGH);
      digitalWrite(af, HIGH);
      digitalWrite(ag, LOW);
    break;
    default:
      return;
    break;
  }
}

void siebenSegZahl(int zahl){
  int zehnerstelle = zahl / 10;
  int einerstelle = zahl - (zehnerstelle*10);
  
  
  digitalWrite(cat, LOW);
  siebenSegZiffer(zehnerstelle);
  delay(10);
  digitalWrite(cat, HIGH);
  siebenSegZiffer(einerstelle);
  delay(10);
}

void setup(){
  pinMode(aa,OUTPUT);
  pinMode(ab,OUTPUT);
  pinMode(ac,OUTPUT);
  pinMode(ad,OUTPUT);
  pinMode(ae,OUTPUT);
  pinMode(af,OUTPUT);
  pinMode(ag,OUTPUT);
  pinMode(cat,OUTPUT);
  pinMode(button,INPUT);
}

int state;
int suspended = 0;
void loop(){
  int i = 0;
  while (i<1000) {
    state = digitalRead(button);
    if(state == HIGH){
      suspended = (suspended + 1) % 2;
    }
    while (state == HIGH) {
      state = digitalRead(button);
      siebenSegZahl(i/10);
    }
    siebenSegZahl(i/10);
    i += (1-suspended);
  }
}


