/*
 * The following code demonstrates how to make
 * one Grove ChainableLED blink using the fastled library.
 *
 * STEPS:
 * -Make sure you connected your chainable LED
 * to the right pin on your Seeeduino Lotus (note that
 * a ChainableLED allocates two pins actually).
 *
 * TODO:
 * -Try to attach multiple ChainableLEDs in series
 *  and write code to control them all
 */

#include <FastLED.h>

//connections
#define DATA_PIN 5
#define CLOCK_PIN 4

//how many leds
#define NUM_LEDS 1

//an array to hold the led data
CRGB leds[NUM_LEDS];

void setup() {
	//start the led library
	//this example uses an esoteric c++ mechanism called "Templates"
	//so do not worry to much about the syntax, just make sure a line
	//such as the line below is in your setup() if you use FastLED.
	FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
	// Turn the LED on, then pause
	leds[0].setRGB( 255, 255, 2255);
	FastLED.show();
	delay(500);
	// Now turn the LED off, then pause
	leds[0].setRGB( 0, 0, 0);
	FastLED.show();
	delay(500);
}
