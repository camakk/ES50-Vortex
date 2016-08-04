/*
* vortex_master.ino
* ES50 Final Project - Fall 2014
* by Cameron Akker, Bahlakoana Mabetha, Kyle Devine
*/

// ********************************************************************
// Pin Assignments
const byte clockPin = 2; // clock for all shift registers
const byte latchPin = 3; // latch for all shift registers
const byte dataPin[3] = {4, 5, 6}; // pins for 3 sections of 24-lights
const byte jackpotDataPin = 7; // data pin for jackpot lights
const byte jackpotDataPin[3] = {11, 12, 13};
const byte buttonPin_red = 8; // pin to see if red button pressed
const byte buttonPin_green = 9; // pin if green button pressed
const byte buttonPin_blue = 10; // pin if blue button pressed
// ********************************************************************
int DELAY = 18;
int JACKPOT_DELAY = 2;
int DIVISOR = 2;
boolean jackpot = false;
int section = 0; // can have value of 0, 1, or 2
int LEDnum = 0; // values 0 through 23
// ********************************************************************

void setup() {
 pinMode((int)clockPin, OUTPUT);
 pinMode((int)latchPin, OUTPUT);
 pinMode((int)dataPin[0], OUTPUT);
 pinMode((int)dataPin[1], OUTPUT);
 pinMode((int)dataPin[2], OUTPUT);
 pinMode((int)buttonPin_red, INPUT);
 pinMode((int)buttonPin_green, INPUT);
 pinMode((int)buttonPin_blue, INPUT);
 pinMode((int)jackpotDataPin[0], OUTPUT);
 pinMode((int)jackpotDataPin[1], OUTPUT);
 pinMode((int)jackpotDataPin[2], OUTPUT);
}
// returns binary number of LED to light up
unsigned long binary(int num) {
 unsigned long result = 1;
 for(int i = 0; i < num; i++) {
 result *= 2;
 }
 return result;
}
// returns if a button is pressed
boolean buttonPressed() {
 if(digitalRead(buttonPin_red) == HIGH && section == 0) {
 return true;
 } else if(digitalRead(buttonPin_green) == HIGH && section == 1) {
 return true;
 } else if(digitalRead(buttonPin_blue) == HIGH && section == 2) {
 return true;
 } else {
 return false;
 }
}
int addOneWrap(int num) {
 num++;
 if(num > 2) {
 num = 0;
 }
 return num;
}
// based on current section and LEDnum, writes to the LEDs without delay
void writeLEDs(int sect, unsigned long val) {
 shiftOut((int)dataPin[sect], (int)clockPin, MSBFIRST, val);
 shiftOut((int)dataPin[sect], (int)clockPin, MSBFIRST, val>>8);
 shiftOut((int)dataPin[sect], (int)clockPin, MSBFIRST, val>>16);
}
void loop() {
 if(jackpot == true){
 digitalWrite(jackpotDataPin[addOneWrap(section)], HIGH);
 jackpot = false;
 for(int i = 0; i < DIVISOR; i++) {
 delay(JACKPOT_DELAY/DIVISOR);
 if(buttonPressed()) {
 delay(4000);
 break;
 }
 }
 } else {
 digitalWrite(jackpotDataPin[addOneWrap(section)], LOW);
 digitalWrite(latchPin, LOW);
 switch(section) {
 case 0:
 writeLEDs(0, binary(LEDnum));
 writeLEDs(1, 0);
 writeLEDs(2, 0);
 break;
 case 1:
 writeLEDs(0, 0);
 writeLEDs(1, binary(LEDnum));
 writeLEDs(2, 0);
 break;
 case 2:
 writeLEDs(0, 0);
 writeLEDs(1, 0);
 writeLEDs(2, binary(LEDnum));
 break;
 }
 digitalWrite(latchPin, HIGH);
 LEDnum++;

 if(LEDnum == 17) {
 jackpot = true;
 }

 if(LEDnum > 23) {
 section++;
 LEDnum = 0;
 }

 if(section > 2) {
 section = 0;
 }
 for(int i = 0; i < DIVISOR; i++) {
 delay(DELAY/DIVISOR);
 if(buttonPressed()) {
 delay(2000);
 break;
 }
 }
}

