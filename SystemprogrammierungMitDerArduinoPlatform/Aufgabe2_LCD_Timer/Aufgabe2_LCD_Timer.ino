/*
 * LCD Timer
 * Shows a stop watch with a 1/100th seconds precision on a 4-line lcd.
 * The running timer is displayed in the first line.
 * The second to fourth line displays three split times taken when a (green) button is pressed.
 * By pressing a (red) button the stop watch will be reset. All split times will be cleared an 
 * the timer restarts from zero.
 */

// Pin assignment
int pinResetButton = 2;     // The red button
int pinSplitTimeButton = 3; // The green button

// Split times in milliseconds
volatile int splitTime1 = 0;
volatile int splitTime2 = 0;
volatile int splitTime3 = 0;

// The instant of time of the last reset (difference bewteen current timer and uptime of the microcontroller)
// in milliseconds
volatile int resetTime = 0;

void setup() {
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
}

/*
 * Reset the timer and clear all split times.
 */
void resetTimer() {
  // delete previous times
  splitTime1 = 0;
  splitTime2 = 0;
  splitTime3 = 0;
  // clear LCDisplay
  // TODO
  // reset timer
  resetTime = millis();
}

/*
 * Take the current time and displays it
 */
void takeSplitTime() {
  // Shift taken split times
  splitTime3 = splitTime2;
  splitTime2 = splitTime1;
  // Take new split time
  splitTime1 = currentTime();
  // Display split times
  printSplitTimes();
}

/*
 * Return time in milliseconds since last timer reset
 */
 int currentTime() {
  return millis() - resetTime;
 }

/*
 * Display the timer on the first line of the LCD.
 * The time format is hh:mm:ss.hun
 */
void timer() {
  int hundredth;
  int seconds;
  int minutes;
  int hours;
  getCurrentTime(int &hundredth, int &seconds, int &minutes, int &hours);
  printTime(int hundredth, int seconds, int minutes, int hours);
}

/*
 * Get the elapsed time in hours, minutes, seconds and hundredth since the last timer reset.
 */
void getCurrentTime(int *hundredth, int *seconds, int *minutes, int *hours) {
  int currentTime = currentTime();
  int totalSeconds = currentTime / 1000;
  *hundredth = (currentTime / 10) % 100;
  *seconds = totalSeconds % 60;
  *minutes = (totalSeconds / 60) % 60;
  *hours = (totalSeconds / 3600);
}

void printTime(int hundredth, int seconds, int minutes, int hours) {
  Serial.println("");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds);
  Serial.print(".");
  Serial.println(hundredth);
}

void printSplitTimes() {
  
}

