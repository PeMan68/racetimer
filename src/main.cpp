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

long currentLightValue = 0;
long startLightValue = 0;
unsigned long runningStartTime;
unsigned long runningPassedTime;
unsigned long minutes;
unsigned long seconds;
unsigned long tenths;
String raceTime1 = "";
String raceTime2 = "";

bool running = false;
bool buttonPressed = false;
bool buttonPressedLong = false;
// int button;
// int lastButtonState = LOW;
// unsigned long buttonPressedStartTime = 0;
// unsigned long lastDebounceTime = 0;
// unsigned long debounceDelay = 50; // 50ms debounce filter

int pinStart = 2;
int pinStop1 = 3;
int pinStop2 = 4;
int pinLEDGreen = 5;
int pinLEDYellow = 6;
int pinLEDRed = 7;

int pinLight = A0;

void rollingSequence();
void startSequence();
void stopSequence();
void redToGreenSequence();
void greenToRedSequence();
void buttonPress();
void displayTest();
String makeString(long int a, long int b, long int c);

unsigned int debounceDelay = 50; // Ange en lämplig debounce-fördröjning (i millisekunder)

int buttonState;					// Aktuell knappstatus
int lastButtonState = LOW;			// Senaste knappstatus
unsigned long lastDebounceTime = 0; // Tidpunkt för senaste debounce

// Funktion för debounce av knapptryckning
int debounce(int buttonPin)
{
	int reading = digitalRead(buttonPin); // Läs av knappens status
	if (reading != lastButtonState)
	{								 // Om knappstatusen har ändrats
		lastDebounceTime = millis(); // Uppdatera senaste debounce-tidpunkt
	}
	if ((millis() - lastDebounceTime) > debounceDelay)
	{ // Om det har gått tillräckligt med tid sedan senaste debounce
		if (reading != buttonState)
		{						   // Om det är en ny knapptryckning
			buttonState = reading; // Uppdatera aktuell knappstatus
			return buttonState;	   // Returnera aktuell knappstatus
		}
	}
	lastButtonState = reading; // Spara senaste knappstatus
	return -1;				   // Om ingen ny knapptryckning har detekterats
}

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

// void loop_old()
// {
// 	int reading = digitalRead(pinStart);

// 	if (reading != lastButtonState)
// 	{
// 		lastDebounceTime = millis();
// 	}
// 	if ((millis() - lastButtonState) > debounceDelay)
// 	{
// 		if (reading != button)
// 		{
// 			button = reading;
// 		}
// 		if (button == HIGH)
// 		{
// 			buttonPress();
// 		}
// 		else
// 		{

// 			if (buttonPressed)
// 			{
// 				// button released
// 				buttonPressed = false;
// 				Serial.println("knapp släppt");

// 				if (!running && !buttonPressedLong)
// 				// short press
// 				// start new race
// 				{
// 					Serial.println("Starta nytt race");
// 					startSequence();
// 					running = true;
// 					startLightValue = analogRead(pinLight);
// 					runningStartTime = millis();
// 				}
// 			}
// 			if (running)
// 			// Race running
// 			{
// 				runningPassedTime = (millis() - runningStartTime) / 100; // Passerade tiondelar
// 				minutes = runningPassedTime / 600;
// 				seconds = (runningPassedTime - minutes * 600) / 10;
// 				tenths = runningPassedTime - minutes * 600 - seconds * 10;
// 				currentLightValue = analogRead(pinLight);
// 				if (runningPassedTime > 20)
// 				{
// 					myDisplay.setTextAlignment(PA_LEFT);
// 					String result = makeString(minutes, seconds, tenths);
// 					myDisplay.print(result);
// 					// myDisplay.print(String(minutes) + ":" + String(seconds) + "," + String(tenths));
// 				}
// 				if ((currentLightValue - startLightValue) * 100 / startLightValue > 10)
// 				// lightvalue up 10%
// 				{
// 					running = false;
// 					stopSequence();
// 					String result = makeString(minutes, seconds, tenths);
// 					Serial.println(result);
// 				}
// 			}
// 		}
// 	}
// 	lastButtonState = reading;
// 	// displayTest();
// }

void loop()
{
	if (running)
	{
		// sub Visa tid
		if (1) // knapp stopp1 och inte lagrad tid för 1
		{
			// Sub Lagra tid#1
		}
		else
		{
			if (1) // knapp stopp2 och inte lagrad tid för 2
			{
				// Sub Lagra tid#2
			}
			else
			{
				// Båda i mål?
				if (1)
				{
					running = false;
				}
				else
				{
					// sub växla diplay mellan runtimer och första måltid
				}
			}
		}
	}
	else
	{
		// Startknapp tryckt??
		int buttonStatus = debounce(pinStart); // Anropa debounce-funktionen med knappens pinnummer som argument
		if (buttonStatus == HIGH)
		{ // Om en knapptryckning har detekterats

			// starta race
			Serial.println("starta race");
			running = true;
		}
		else
		{
			// Sub visa senaste lagrade tider
			Serial.println("Sub visa senaste lagrade tider");
		}
	}
}

String makeString(long int a, long int b, long int c)
{
	String result = "";

	result += String(a);
	result += ":";
	if (b < 10)
	{
		result += "0";
	}
	result += String(b);
	result += ".";
	result += String(c);
	return result;
}

// // void buttonPress()
// {
// 	if (!buttonPressed)
// 	{
// 		// button pressed
// 		buttonPressed = true;
// 		buttonPressedLong = false;
// 		buttonPressedStartTime = millis();
// 		Serial.println("knapp tryckt");
// 	}
// 	else
// 	{
// 		if (!buttonPressedLong && millis() - buttonPressedStartTime > 2000)
// 		{
// 			// long press
// 			buttonPressedLong = true;
// 			// buttonPressed = false;
// 			if (running)
// 			{
// 				// race started, just reset
// 				myDisplay.setTextAlignment(PA_RIGHT);
// 				myDisplay.print("STOPPA RACE");
// 				Serial.println("Stoppa race, tjuvstart");
// 				running = false;
// 				stopSequence();
// 			}
// 			else
// 			{
// 				// delete previous racetime
// 				Serial.println("Radera föregånde racetid");
// 			}
// 		}
// 	}
// }

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