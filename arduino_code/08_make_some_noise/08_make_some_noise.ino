/*
   The following code demonstrates how to create beautiful
   asynchronous LEDs fading animations and how to switch between
   the different modalities

   STEPS:
   -make sure you connected your chainable LED
   to the right pin on your Seeeduino Lotus board
   -use the potentiometer to change color
   -press the button to switch animation
 */

#include <FastLED.h>
#include <Servo.h>


// Connections
#define DATA_PIN 5
#define CLOCK_PIN 4

int potPin = A0;

// How many leds
#define NUM_LEDS 1

// An array to hold the led data
CRGB leds[NUM_LEDS];

// button
int buttonPin = 2;
// the value that the button had in the previous loop()
boolean lastButtonPressed;

// how many animations do we have
int numAnimations=5;
int activeMode=0;

Servo myServo;
int servoPin=6;


void setup() {
	//initialize the led
	FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
	//initialize the servo
	myServo.attach(servoPin);
	startServoMoveToAnimation(0,0,1);
}

void loop() {
	//if the button is pressed we change animation
	if (readButton()) {
		//check that we are within the range of the the animations we programmed
		if (activeMode<numAnimations) {
			activeMode++;
		}else{
			activeMode=0;
		}
		// start the right animation
		switch (activeMode) {
		case 0:  //nothing
			stopAnimations();
			startServoMoveToAnimation(0,10,1);
			break;
		case 1: //one color not fading
			startSingleColor();
			startServoMoveToAnimation(90,1000,100);
			startMelody(2000,false);
			break;
		case 2: //the blink
			// we can still use the initFadeAnimation setting the number of steps to 1
			startFadeAnimation(500,1);
			startServoSweepAnimations(0,180,500,1);
			stopMelody();
			break;
		case 3: //the fade
			startFadeAnimation(2000,100);
			startServoSweepAnimations(0,180,2000,100);
			startMelody(4000,true);
			break;
		case 4: //the rainbow
			startRainbowAnimation(1000,100);
			startServoSweepAnimations(0,180,2000,4);
			stopMelody();
			break;
		}
	}

	// if the animation is not of type rainbox set the color using the potentiometer
	if (activeMode!=4) {
		int potValue = analogRead(potPin);
		setHue(map(potValue, 0,1024,0,255));
	}

	//timing checks and led update is done inside this function
	updateLedAnimation();
	updateServo();
	updateMelody();
}

// this function returns true when the button is pressed
boolean readButton(){
	boolean buttonStatus=false;
	boolean buttonPressed=digitalRead(buttonPin);
	if (buttonPressed&&!lastButtonPressed) {
		buttonStatus=true;
	}
	lastButtonPressed=buttonPressed;
	return(buttonStatus);
}
