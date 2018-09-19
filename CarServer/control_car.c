#include <wiringPi.h>
#include <stdbool.h>
#include <softPwm.h>

static int leftForwardPin = 28;
static int leftBackwardPin = 29;
static int rightForwardPin = 24;
static int rightBackwardPin = 25;
static int pmwPin = 1;
static int speed = 5;
static bool forward = true;

void init_car()
{
	if (wiringPiSetup() == -1)
		exit(1);

	pinMode(leftForwardPin, OUTPUT);
	pinMode(leftBackwardPin, OUTPUT);
	pinMode(rightForwardPin, OUTPUT);
	pinMode(rightBackwardPin, OUTPUT);
	digitalWrite(leftBackwardPin, LOW);
	digitalWrite(leftForwardPin, LOW);
	digitalWrite(rightBackwardPin, LOW);
	digitalWrite(rightForwardPin, LOW);
	pwmWrite(pmwPin, 800);
}
void start_car()
{
	speed = 50;
	forward = true;
	pwmWrite(pmwPin, 800);
	digitalWrite(leftBackwardPin, LOW);
	digitalWrite(leftForwardPin, HIGH);
	digitalWrite(rightBackwardPin, LOW);
	digitalWrite(rightForwardPin, HIGH);
	//pwmWrite(pmwPin, speed);
}
void stop_car()
{
	speed = 50;
	forward = true;
	digitalWrite(leftForwardPin, LOW);
	digitalWrite(rightForwardPin, LOW);
	//pwmWrite(pmwPin, 0);
}