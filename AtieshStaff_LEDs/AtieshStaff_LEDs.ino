/*
Atiesh Staff LED Code

Created by Nic Neidenbach based on sample code from Adafruit

Modes:
   1 All LEDS on Full
   2 All LEDs fade in and out in sync
   3 Blue Orb LEDs fade in and out at random intervals, Red LED eyes "blink" occasionally
   4 All LEDs on at about half brightness
   5 Off
*/

// Variables for LEDs
int ledPins[] = { 3, 9, 10, 11};              // an array of PWM pin numbers to which LEDs are attached
int pinCount = 4;                             // the number of pins (i.e. the length of the array)
int brightness[] = { 250, 250, 250, 250};     // Starting Brightness values for all pins
int fader[] = { 5, 5, 5, 5, 5};               // Initial Fade step value
int pinStatus [] = { 0, 0, 0, 0};             // Initial Pin status (ie, what is the pin doing?)


// Variables for Status Button Control
const int buttonPin = 2;
const int totalModes = 5; // Number of different display modes, not including default/off
unsigned int timeDebounce = 350;
unsigned long clickTime = 0;
volatile int statusMode = 1;
volatile int priorMode = 0;
volatile unsigned long lastClickTime = 0;
int justChanged = 1;
long randNumber;
long lastTime;
long newTime;

// the setup routine runs once when you press reset:
void setup() {
  // declare pins for output:
Serial.begin(9600); // used for debugging
  for (int pIndex = 0; pIndex < pinCount; pIndex++) {

    pinMode(ledPins[pIndex], OUTPUT);

  }

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), Click, FALLING);

  randomSeed(analogRead(0));
  lastTime = millis();
}

// the loop routine runs over and over again forever:
void loop() {

  switch (statusMode) {

     case 1:
       staticOn();
       break;
     case 2:
       fadePulse();
       break;
     case 3:
       Complex();
       break;
     case 4:
       staticHalf();
       break;
     case 5:
       staticOff();
       break;
     default:
       staticOff();
  }
  // wait for 30 milliseconds to see the dimming effect
  //Serial.println(justChanged); // Used for Debugging
  delay(30);
  justChanged = 0; // Set to 0 after first pass through loop
}

void fadePulse(){

  for (int pIndex = 0; pIndex < pinCount; pIndex++) {

  // Set Brightness to 250 at first
  if (statusMode != priorMode){
      brightness[pIndex] = 250;
      priorMode = statusMode;  
  }

  // set the brightness of pin pIndex:
  analogWrite(ledPins[pIndex], brightness[pIndex]);
  
  // change the brightness for next time through the loop:
  brightness[pIndex] = brightness[pIndex] + fader[pIndex];

  // reverse the direction of the fading at the ends of the fade:
  if (brightness[pIndex] <= 10 || brightness[pIndex] >= 255) {
    fader[pIndex] = -fader[pIndex];
  }

}
}

void Complex()
{
for (int pIndex = 0; pIndex < pinCount; pIndex++) {

  // Set Brightness to 250 at first
  if (statusMode != priorMode){
      brightness[pIndex] = 250;
      pinStatus[pIndex]=0;
      priorMode = statusMode;  
  }

  // set the brightness of pin pIndex:
  analogWrite(ledPins[pIndex], brightness[pIndex]);

  if (pinStatus[pIndex]==0 && pIndex>0){
     randNumber = random(500);

     if (randNumber<=25){
      pinStatus[pIndex]=1;
      fader[pIndex]=-5;
     }
  }

  if (pinStatus[pIndex]==0 && pIndex==0){
     randNumber = random(500);
     newTime = millis();
     if (randNumber<=25 && (newTime-lastTime)>30000){
      pinStatus[pIndex]=1;
      fader[pIndex]=-5;
      lastTime = newTime;
     }
  }

  if (pinStatus[pIndex]==1){
  // change the brightness for next time through the loop:
     brightness[pIndex] = brightness[pIndex] + fader[pIndex];

  // reverse the direction of the fading at the ends of the fade:
  if (brightness[pIndex] <= 10) {
    fader[pIndex] = -fader[pIndex];
  }
  if (brightness[pIndex] >= 255) {
    brightness[pIndex] = 250;
    pinStatus[pIndex]=0;
  }
  
  }

  }

} // End Complex

void staticOn(){

for (int pIndex = 0; pIndex < pinCount; pIndex++) {

  // set the brightness of pin pIndex:
  analogWrite(ledPins[pIndex], 250);

}

}

void staticHalf(){

for (int pIndex = 0; pIndex < pinCount; pIndex++) {

  // set the brightness of pin pIndex:
  analogWrite(ledPins[pIndex], 100);

}

}
void staticOff(){

for (int pIndex = 0; pIndex < pinCount; pIndex++) {

  // set the brightness of pin pIndex:
  analogWrite(ledPins[pIndex], 0);

}

}

void Click(){
  
    clickTime = millis(); // Get the time of the button click
    if (clickTime - lastClickTime >= timeDebounce) {
       priorMode = statusMode;
       statusMode++;
       if (statusMode > totalModes) {
          statusMode = 0;
       }

    }
    
    lastClickTime = clickTime;
    //Serial.println(justChanged);
}
