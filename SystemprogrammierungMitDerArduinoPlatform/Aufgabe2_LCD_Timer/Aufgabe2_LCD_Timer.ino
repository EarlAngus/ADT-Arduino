/*
 * LCD Timer
 * Shows a stop watch with a 1/100th seconds precision on a 4-line lcd.
 * The running timer is displayed in the first line.
 * The second to fourth line displays three split times taken when a (green) button is pressed.
 * By pressing a (red) button the stop watch will be reset. All split times will be cleared and
 * the timer restarts from zero.
 */

 // Bei 0:1:5:53 passiert ein ungeplanter reset.

// Pin assignment
const int pinResetButton = 2;     // The red button
const int pinSplitTimeButton = 3; // The green button

// Multiple split times in milliseconds
const int numSplitTimes = 3;
volatile unsigned long splitTime[numSplitTimes];

// The instant of time of the last reset (difference bewteen current timer and uptime of the microcontroller)
// in milliseconds
volatile unsigned long resetTime = 0;

void setup() {
  // Initialize timer/splitTimes
  resetSplitTimes();
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for Leonardo only
  }
  // Prepare the interrupt service routines to be called by pressing the red and green button
  pinMode(pinResetButton, INPUT_PULLUP);
  pinMode(pinSplitTimeButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinResetButton), resetTimer, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinSplitTimeButton), takeSplitTime, FALLING);
}

void loop() {
  timer();
  delay(10);
}

/*
 * Reset the timer and clear all split times.
 */
void resetTimer() {
  // reset timer
  resetTime = millis();
  // delete previous times
  resetSplitTimes();
  // clear LCDisplay
//  lcd.clear();
}

/*
 * Delete all previously taken split times.
 */
void resetSplitTimes() {
  for (int i = 0; i < numSplitTimes; i++) {
    splitTime[i] = 0;
  }
}

/*
 * Take the current time and displays it
 */
void takeSplitTime() {
  // Shift taken split times
  for (int i = numSplitTimes; i > 0; i--) {
    splitTime[i] = splitTime[i-1];
  }
  // Take new split time
  splitTime[0] = currentTime();
  // Display split times
  printSplitTimes();
}

/*
 * Return time in milliseconds since last timer reset
 */
 unsigned long currentTime() {
  return millis() - resetTime;
 }

/*
 * Display the timer on the first line of the LCD.
 * The time format is hh:mm:ss.hun
 */
void timer() {
  unsigned long hundredth;
  unsigned long seconds;
  unsigned long minutes;
  unsigned long hours;
  getCurrentTime(&hundredth, &seconds, &minutes, &hours);
  printTime(hundredth, seconds, minutes, hours);
}

/*
 * Get the elapsed time in hours, minutes, seconds and hundredth since the last timer reset.
 */
void getCurrentTime(unsigned long *hundredth, unsigned long *seconds, unsigned long *minutes, unsigned long *hours) {
  unsigned long cTime = currentTime();
  unsigned long totalSeconds = cTime / 1000;
  *hundredth = (cTime / 10) % 100;
  *seconds = totalSeconds % 60;
  *minutes = (totalSeconds / 60) % 60;
  *hours = (totalSeconds / 3600);
}

void printTime(unsigned long hundredth, unsigned long seconds, unsigned long minutes, unsigned long hours) {
  //Serial.println("");
  Serial.print(hours, DEC);
  Serial.print(":");
  Serial.print(minutes, DEC);
  Serial.print(":");
  Serial.print(seconds, DEC);
  Serial.print(".");
  Serial.println(hundredth, DEC);
}

void printSplitTimes() {
  
}

