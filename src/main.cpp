#include <Arduino.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 6

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

long currentLightValue = 0;
long startLightValue = 0;
unsigned long runningStartTime;
unsigned long runningPassedTime;
unsigned long minutes;
unsigned long seconds;
unsigned long tenths;

bool running = false;
bool buttonPressed = false;
bool buttonPressedLong = false;
int button;
int lastButtonState = LOW;
unsigned long buttonPressedStartTime = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50ms debounce filter

int pinStartReset = 2;
int pinLEDGreen = 3;
int pinLEDYellow = 4;
int pinLEDRed = 5;

int pinLight = A0;

void rollingSequence();
void startSequence();
void stopSequence();
void redToGreenSequence();
void greenToRedSequence();
void buttonPress();
void displayTest();

void setup()
{
	pinMode(pinStartReset, INPUT);
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

void buttonPress()
{
	if (!buttonPressed)
	{
		// button pressed
		buttonPressed = true;
		buttonPressedLong = false;
		buttonPressedStartTime = millis();
		Serial.println("knapp tryckt");
	}
	else
	{
		if (!buttonPressedLong && millis() - buttonPressedStartTime > 2000)
		{
			// long press
			buttonPressedLong = true;
			// buttonPressed = false;
			if (running)
			{
				// race started, just reset
				myDisplay.setTextAlignment(PA_RIGHT);
				myDisplay.print("STOPPA RACE");
				Serial.println("Stoppa race, tjuvstart");
				running = false;
				stopSequence();
			}
			else
			{
				// delete previous racetime
				Serial.println("Radera föregånde racetid");
			}
		}
	}
}

void loop()
{
	int reading = digitalRead(pinStartReset);

	if (reading != lastButtonState)
	{
		lastDebounceTime = millis();
	}
	if ((millis() - lastButtonState) > debounceDelay)
	{
		if (reading != button)
		{
			button = reading;
		}
		if (button == HIGH)
		{
			buttonPress();
		}
		else
		{

			if (buttonPressed)
			{
				// button released
				buttonPressed = false;
				Serial.println("knapp släppt");

				if (!running && !buttonPressedLong)
				// short press
				// start new race
				{
					Serial.println("Starta nytt race");
					startSequence();
					running = true;
					startLightValue = analogRead(pinLight);
					runningStartTime = millis();
				}
			}
			if (running)
			// Race running
			{
				runningPassedTime = (millis() - runningStartTime) / 100; // Passerade tiondelar
				minutes = runningPassedTime / 600;
				seconds = (runningPassedTime - minutes * 600) / 10;
				tenths = runningPassedTime - minutes * 600 - seconds * 10;
				currentLightValue = analogRead(pinLight);
				if (seconds > 2)
				{
					myDisplay.setTextAlignment(PA_LEFT);
					myDisplay.print(String(minutes) + ":" + String(seconds) + "," + String(tenths));
				}
				if ((currentLightValue - startLightValue) * 100 / startLightValue > 10)
				// lightvalue up 10%
				{
					running = false;
					stopSequence();
					Serial.println(String(minutes) + ":" + String(seconds) + "," + String(tenths));
				}
			}
		}
	}
	lastButtonState = reading;
	// displayTest();
}

void stopSequence()
{
	digitalWrite(pinLEDGreen, LOW);
	digitalWrite(pinLEDYellow, LOW);
	digitalWrite(pinLEDRed, HIGH);
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
}

void redToGreenSequence()
{
	digitalWrite(pinLEDRed, HIGH);
	digitalWrite(pinLEDGreen, LOW);
	digitalWrite(pinLEDYellow, LOW);

	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.print("KLARA...");

	delay(2000);
	digitalWrite(pinLEDYellow, HIGH);
	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.print("FÄRDIGA");
	delay(2000);
	digitalWrite(pinLEDGreen, HIGH);
	digitalWrite(pinLEDRed, LOW);
	digitalWrite(pinLEDYellow, LOW);
	myDisplay.setTextAlignment(PA_CENTER);
	myDisplay.print("GÅ!");
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