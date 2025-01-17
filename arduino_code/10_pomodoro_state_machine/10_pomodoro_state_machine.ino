// ---------------
// -- Libraries --
// ---------------
#include <Servo.h>
#include <FastLED.h>


// ---------------
// -- Variables --
// ---------------

//pin Variables
const int buttonPin = 2;
const int potPin = A0;
const int servoPin = 6;

const int ledPin1 = 4;
const int ledPin2 = 5;

//create a servo object
Servo myServo;

// Connections
#define DATA_PIN 5
#define CLOCK_PIN 4

// How many leds
#define NUM_LEDS 1

// An array to hold the led data
CRGB leds[NUM_LEDS];


// State Machine Variables
const int stateIdle = 0;
const int stateWork = 1;
const int stateBreakAlarm = 2;
const int stateShortBreak = 3;
const int stateLongBreak = 4;
const int stateWorkAlarm = 5;

int currentState = stateIdle;

// Timer variables
unsigned long timerDuration;
unsigned long lastTimerStarted;

// const long focusWorkDuration=1000*60*20; //20 minutes
// const long shortBreakDuration=1000*60*5; //20 minutes
// const long longBreakDuration=1000*60*15; //20 minutes

const long focusWorkDuration=5000; //20 minutes
const long shortBreakDuration=5000; //20 minutes
const long longBreakDuration=5000; //20 minutes

//buttons Variables
boolean lastButtonPressed;

//break Variables
int nBreaks=0;

// ------------------
// -- Setup & Loop --
// ------------------

void setup() {
	//define input/output pins
	pinMode(buttonPin, INPUT);
	//initialize the led
	FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
	//initialize the servo
	myServo.attach(servoPin);
	startServoMoveToAnimation(0,0,1);

	Serial.begin(9600);
}

void loop() {

	updateStateMachine();
	//timing checks and led update is done inside this function
	updateLedAnimation();
	updateServo();
	updateMelody();
}

// ----------------
// -- Conditions --
// ----------------

// this function returns true when the button is pressed
boolean buttonPress(){
	boolean buttonStatus=false;
	boolean buttonPressed=digitalRead(buttonPin);
	if (buttonPressed&&!lastButtonPressed) {
		buttonStatus=true;
	}
	lastButtonPressed=buttonPressed;
	return(buttonStatus);
}

boolean timerExpired() {
	if (millis()-lastTimerStarted>timerDuration) {
		return true;
	}else{
		return false;
	}
}

int checkBreak(){
	return nBreaks;
}


// -------------
// -- Actions --
// -------------

void startTimer(int msec) {
	timerDuration = msec;
	lastTimerStarted = millis();
}

void recalculateServoPosition(){
	int timeRemaining=lastTimerStarted+timerDuration-millis();
	int servoPosition=map(timeRemaining, 0,timerDuration,0,180);
	startServoMoveToAnimation(servoPosition, 0, 1);
}



// -------------
// -- Utility --
// -------------

void goToState(int state){
	currentState=state;
	Serial.print("going to state:");
	Serial.print('\t');
	Serial.println(currentState);
}

// --------------------
// -- State Machines --
// --------------------

void updateStateMachine() {
	switch (currentState) {

	case stateIdle:
		if (buttonPress()) {
			startTimer(focusWorkDuration);
			setRGB(255,255,255);
			startFadeAnimation(1000,100);
			goToState(stateWork);
		}
		break;

	case stateWork:
		recalculateServoPosition();
		if (timerExpired()) {
			goToState(stateBreakAlarm);
			startHappyMelody(1000,true);
			startServoSweepAnimation(60,120,200,10);
			setRGB(22,200,22);
			startFadeAnimation(500,1);
		}
		break;

	case stateBreakAlarm:
		if (buttonPress()) {
			nBreaks++;
			if (checkBreak()<4) {
				startTimer(shortBreakDuration);
				goToState(stateShortBreak);
				setBrightness(100);
				startRainbowAnimation(5000,10);
				stopMelody();
			}
			if (checkBreak()==4) {
				startTimer(longBreakDuration);
				goToState(stateLongBreak);
				setBrightness(255);
				startRainbowAnimation(5000,100);
				stopMelody();
			}
		}
		break;

	case stateShortBreak:
		recalculateServoPosition();
		if (timerExpired()) {
			goToState(stateWorkAlarm);
			startSadMelody(2000,true);
			startServoSweepAnimation(0,120,1000,10);
			setRGB(200,0,200);
			startFadeAnimation(500,5);
		}
		break;

	case stateLongBreak:
		recalculateServoPosition();
		if (timerExpired()) {
			goToState(stateIdle);
			stopLedAnimations();
			startSongMelody(1000,false);
		}
		break;

	case stateWorkAlarm:
		if (buttonPress()) {
			startTimer(focusWorkDuration);
			setRGB(255,255,255);
			startFadeAnimation(1000,100);
			goToState(stateWork);
			stopMelody();
		}
		break;
	}
}
