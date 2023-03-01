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

void rollingSequence();
void startSequence();
void redToGreenSequence();
void greenToRedSequence();

void setup() {
	pinMode(pinStartReset, INPUT);
	pinMode(pinLEDGreen, OUTPUT);
	pinMode(pinLEDYellow, OUTPUT);
	pinMode(pinLEDRed, OUTPUT);
	Serial.begin(9600); // remove after tests
}


void startSequence()
{
	// rollingSequence();
	for (int i = 0; i < 5; i++)
	{
		digitalWrite(pinLEDGreen, HIGH);
		digitalWrite(pinLEDYellow, HIGH);
		digitalWrite(pinLEDRed, HIGH);
		delay(500 - i * 100);
		digitalWrite(pinLEDGreen, LOW);
		digitalWrite(pinLEDYellow, LOW);
		digitalWrite(pinLEDRed, LOW);
		delay(500 - i * 100);
	}
		redToGreenSequence();
		delay(2000);
		greenToRedSequence();

}

void redToGreenSequence()
{
		digitalWrite(pinLEDRed, HIGH);
		digitalWrite(pinLEDGreen, LOW);
		digitalWrite(pinLEDYellow, LOW);
		delay(2000);
		digitalWrite(pinLEDYellow, HIGH);
		delay(2000);
		digitalWrite(pinLEDGreen, HIGH);
		digitalWrite(pinLEDRed, LOW);
		digitalWrite(pinLEDYellow, LOW);
}

void greenToRedSequence()
{
		digitalWrite(pinLEDGreen, HIGH);
		digitalWrite(pinLEDRed, LOW);
		digitalWrite(pinLEDYellow, LOW);
		delay(2000);
		digitalWrite(pinLEDGreen, LOW);
		digitalWrite(pinLEDYellow, HIGH);
		delay(2000);
		digitalWrite(pinLEDRed, HIGH);
		digitalWrite(pinLEDYellow, LOW);
}

void rollingSequence()
{
	for (int i = 0; i < 5; i++)
	{
		digitalWrite(pinLEDGreen, HIGH);
		delay(100);
		digitalWrite(pinLEDYellow, HIGH);
		delay(100);
		digitalWrite(pinLEDRed, HIGH);
		delay(100);
		digitalWrite(pinLEDGreen, LOW);
		delay(100);
		digitalWrite(pinLEDYellow, LOW);
		delay(100);
		digitalWrite(pinLEDRed, LOW);
		delay(100);
	}

}

void loop() {
	button = digitalRead(pinStartReset);
	if (button == HIGH)
	{
		startSequence();
	}
	
}