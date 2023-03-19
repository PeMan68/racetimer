#include <Arduino.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 10

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int pinStart = 2;
int pinStop1 = 3;
int pinStop2 = 4;
int pinLEDGreen = 5;
int pinLEDYellow = 6;
int pinLEDRed = 7;

unsigned long runningStartTime;
unsigned long runningPassedTime;
unsigned long lastRunningPastTime;
unsigned long minutes;
unsigned long seconds;
unsigned long tenths;
String raceTime1 = "";
String raceTime2 = "";
bool toggleResults = true;
char cRaceTime1[20];
char cRaceTime2[20];

bool running = false;

void rollingSequence();
void startSequence();
void stopSequence();
void redToGreenSequence();
void greenToRedSequence();
void buttonPress();
void displayTest();
unsigned long getPassedTime();
String makeStringFromTime(unsigned long a);
void updateDisplayWithTime();

void setup()
{
	pinMode(pinStart, INPUT);
	pinMode(pinStop1, INPUT);
	pinMode(pinStop2, INPUT);
	pinMode(pinLEDGreen, OUTPUT);
	pinMode(pinLEDYellow, OUTPUT);
	pinMode(pinLEDRed, OUTPUT);
	Serial.begin(9600); // remove after tests
						// Intialize the display
	myDisplay.begin();

	// Set the intensity (brightness) of the display (0-15)
	myDisplay.setIntensity(0);

	// Clear the display
	myDisplay.displayClear();
}

void loop()
{
	if (running)
	{
		// sub Visa tid
		updateDisplayWithTime();
		int buttonStatus1 = digitalRead(pinStop1);
		if (buttonStatus1 == HIGH && raceTime1 == "") // ingång stopp1 och inte lagrad tid för 1
		{
			// Sub Lagra tid#1
			raceTime1 = "BANA 1: " + makeStringFromTime(runningPassedTime);
			Serial.println(raceTime1);
			raceTime1.toCharArray(cRaceTime1, sizeof(cRaceTime1));
		}
		else
		{
			int buttonStatus2 = digitalRead(pinStop2);	  
			if (buttonStatus2 == HIGH && raceTime2 == "") // ingång stopp2 och inte lagrad tid för 2
			{
				// Sub Lagra tid#2
				raceTime2 = "BANA 2: " + makeStringFromTime(runningPassedTime);
				Serial.println(raceTime2);
				raceTime2.toCharArray(cRaceTime2, sizeof(cRaceTime2));
			}
			else
			{
				// Båda i mål?
				if (raceTime1 != "" && raceTime2 != "")
				{
					running = false;
					toggleResults = true;
					stopSequence();
				}
				else
				{
					// Serial.println("sub växla diplay mellan runtimer och första måltid");
				}
			}
		}
	}
	else
	{
		// Startknapp tryckt??
		int buttonStatus = digitalRead(pinStart); // Anropa debounce-funktionen med knappens pinnummer som argument
		if (buttonStatus == HIGH)
		{ // Om en knapptryckning har detekterats

			// starta race
			Serial.println("starta race");
			redToGreenSequence();
			running = true;
			raceTime1 = "";
			raceTime2 = "";
			runningStartTime = millis();
		}
		else
		{
			// Sub visa senaste lagrade tider
			if (raceTime1 != "" || raceTime2 != "")
			{
				if (toggleResults)
				{
					if (myDisplay.displayAnimate())
					{
						toggleResults = !toggleResults;
						myDisplay.displayClear();
						myDisplay.displayText(cRaceTime1, PA_LEFT, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
						myDisplay.displayReset();
					}
				}
				else
				{
					if (myDisplay.displayAnimate())
					{
						toggleResults = !toggleResults;
						myDisplay.displayClear();
						myDisplay.displayText(cRaceTime2, PA_LEFT, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
						myDisplay.displayReset();
					}
				}
			}
		}
	}
}

unsigned long getPassedTime()
{
	return (millis() - runningStartTime) / 100; // Passerade tiondelar sedan start
}

String makeStringFromTime(unsigned long a)
{
	minutes = a / 600;
	seconds = (a - minutes * 600) / 10;
	tenths = a - minutes * 600 - seconds * 10;
	String result = "";

	result += String(minutes);
	result += ":";
	if (seconds < 10)
	{
		result += "0";
	}
	result += String(seconds);
	result += ".";
	result += String(tenths);
	return result;
}

void updateDisplayWithTime()
{
	runningPassedTime = getPassedTime(); // Passerade tiondelar
	if (runningPassedTime > 20 && runningPassedTime != lastRunningPastTime)
	{
		myDisplay.setTextAlignment(PA_LEFT);
		String result = makeStringFromTime(runningPassedTime);
		myDisplay.print(result);
		lastRunningPastTime = runningPassedTime;
	}
}

void stopSequence()
{
	digitalWrite(pinLEDGreen, LOW);
	digitalWrite(pinLEDYellow, LOW);
	digitalWrite(pinLEDRed, LOW);
}
void startSequence()
{
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
}

void redToGreenSequence()
{
	digitalWrite(pinLEDRed, HIGH);
	digitalWrite(pinLEDGreen, LOW);
	digitalWrite(pinLEDYellow, LOW);

	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.print("READY!");

	delay(2000);
	digitalWrite(pinLEDYellow, HIGH);
	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.print("STEADY");
	delay(2000);
	digitalWrite(pinLEDGreen, HIGH);
	digitalWrite(pinLEDRed, LOW);
	digitalWrite(pinLEDYellow, LOW);
	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.print("GO!");
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

void displayTest()
{
	myDisplay.setTextAlignment(PA_LEFT);
	myDisplay.print("Left");
	delay(2000);

	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.print("Center");
	delay(2000);

	myDisplay.setTextAlignment(PA_RIGHT);
	myDisplay.print("Right");
	delay(2000);

	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.setInvert(true);
	myDisplay.print("Invert");
	delay(2000);

	myDisplay.setInvert(false);
	myDisplay.print(1234);
	delay(2000);
}