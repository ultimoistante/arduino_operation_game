
/** Simple Arduino "Operation Game" sketch
 **
 ** Play a sample (stored in flash memory) when a "sense" pin goes low,
 ** and blinks a LED during sample playback.
 **
 ** Uses PCM library by damellis (https://github.com/damellis/PCM/)
 **
 ** Written by Salvatore Carotenuto of OpenMakersItaly
 ** on December 16, 2018
 **/

 // -----------------------------------------------------------------------------
 // -----------------------------------------------------------------------------
 //
 // Arduino Nano connections
 //                                    +-----+
 //                              +-----|=====|-----+
 //       Red LED ---> D13 (PB5) |o    |=====|    o| (PB4) D12
 //                          3v3 |o    +-----+    o| (PB3) D11 ---> Speaker
 //                         Vref |o               o| (PB2) D10 
 //                     A0 (PC0) |o               o| (PB1) D9
 //                     A1 (PC1) |o               o| (PB0) D8  
 //                     A2 (PC2) |o               o| (PD7) D7
 //                     A3 (PC3) |o    Arduino    o| (PD6) D6  
 //                     A4 (PC4) |o      Nano     o| (PD5) D5
 //                     A5 (PC5) |o               o| (PD4) D4
 //                    A6 (ADC6) |o               o| (PD3) D3
 //                    A7 (ADC7) |o               o| (PD2) D2  ---> Tweezers
 //                           5V |o               o| GND
 //                    RST (PC6) |o               o| (PC6) RST
 //                          GND |o     . . .     o| (PD0) RX
 //                          Vin |o     . . .     o| (PD1) TX
 //                              +-----------------+
 //
 // -----------------------------------------------------------------------------
 // -----------------------------------------------------------------------------


#include <PCM.h>
#include "sample_data.h"


// ---- pin definitions
int sensePin = 2;
int ledPin = LED_BUILTIN;

#define LED_BLINK_ON_TIME_MSEC	25 // time in milliseconds to keep LED on (during blinking)
#define LED_BLINK_OFF_TIME_MSEC	75 // time in milliseconds to keep LED off (during blinking)

// ---- variables
unsigned long playbackStartTime = 0; // variable used to avoid looping of playback start
unsigned long playbackEndTime = 0;   // variable used to stop led blinking
unsigned int  sampleDurationMillis = 1800; // approximate duration of sample, in milliseconds

// variables used for led blinking
byte blink_led = 0;
byte led_status = 0;
unsigned int led_on_time_counter = LED_BLINK_ON_TIME_MSEC;
unsigned int led_off_time_counter = LED_BLINK_OFF_TIME_MSEC;

// -----------------------------------------------------------------------------


// Interrupt handler called once a millisecond
SIGNAL(TIMER0_COMPA_vect)
	{
	if (blink_led) // if we have to blink...
		{
		if (!led_status)	// if LED is currently OFF
			{
			// decreases milliseconds counter
			led_off_time_counter--;
			if (led_off_time_counter == 0) // if has passed defined time
				{
				// activates LED, sets status to 1 and resets counter
				digitalWrite(ledPin, HIGH);
				led_status = 1;
				led_off_time_counter = LED_BLINK_OFF_TIME_MSEC;
				}
			}
		else // if LED is currently ON
			{
			// decreases milliseconds counter
			led_on_time_counter--;
			if (led_on_time_counter == 0) // if has passed defined time
				{
				// deactivates LED, sets status to 0 and resets counter
				digitalWrite(ledPin, LOW);
				led_status = 0;
				led_on_time_counter = LED_BLINK_ON_TIME_MSEC;
				}
			}
		}
	}


// -----------------------------------------------------------------------------


void setup()
	{
	// sets "sense" pin to INPUT and HIGH
	pinMode(sensePin, INPUT_PULLUP);
	// sets LED pin to output
	pinMode(ledPin, OUTPUT);
	//
	// setup of millisecond timer interrupt
	// Timer0 is already used for millis() - we'll just interrupt somewhere
	// in the middle and call the "Compare A" function below
	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);
	}


// -----------------------------------------------------------------------------


void loop()
	{
	// checks if it's passed enough time after last sample playback
	if (millis() > (playbackStartTime + sampleDurationMillis))
		{
		// if status of sensePin goes low...
		if (!digitalRead(sensePin))
			{
			// ...activates LED blinking
			blink_led = 1;
			led_status = 1;
			digitalWrite(ledPin, HIGH);
			//
			// ...start playing the sample
			// (and saves the start and end time of playing)
			playbackStartTime = millis();
			playbackEndTime = playbackStartTime + sampleDurationMillis;
			startPlayback(sample_data, sizeof(sample_data));
			//
			}
		}
	//
	// checks if it's time to stop LED blinking (sample playback end)
	if (blink_led && millis() > playbackEndTime)
		{
		blink_led = 0;
		digitalWrite(ledPin, LOW);
		}
	}
