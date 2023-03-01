#include <Arduino.h>

long currentLightValue = 0;
long startLightValue = 0;
unsigned long startTime;
unsigned long passedTime;
unsigned long minutes;
unsigned long seconds;
unsigned long tenths;

bool running = false;

int pinStartReset = 2;
int button = LOW;

int pinLEDGreen = 3;
int pinLEDYellow = 4;
int pinLEDRed = 5;

int pinLight = A0;

void setup() {
	pinMode(pinStartReset, INPUT);
	pinMode(pinLEDGreen, OUTPUT);
	pinMode(pinLEDYellow, OUTPUT);
	pinMode(pinLEDRed, OUTPUT);
	Serial.begin(9600); // remove after tests
}

void loop() {
  // put your main code here, to run repeatedly:
}