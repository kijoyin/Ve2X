#include <wiringPi.h>
#include <stdbool.h>
#include <softPwm.h>
#include "include\control_car.h"

static int leftForwardPin = 28;
static int leftBackwardPin = 29;
static int rightForwardPin = 24;
static int rightBackwardPin = 25;
static int pmwPin = 1;
static int speed = 800;
static bool forward = true;

void init_car()
{
	if (wiringPiSetup() == -1)
		exit(1);

	pinMode(leftForwardPin, OUTPUT);
	pinMode(leftBackwardPin, OUTPUT);
	pinMode(rightForwardPin, OUTPUT);
	pinMode(rightBackwardPin, OUTPUT);
	AllLow();
	pwmWrite(pmwPin, speed);
}
void start_car()
{
	speed = 50;
	forward = true;
	pwmWrite(pmwPin, speed);
	digitalWrite(leftBackwardPin, LOW);
	digitalWrite(leftForwardPin, HIGH);
	digitalWrite(rightBackwardPin, LOW);
	digitalWrite(rightForwardPin, HIGH);
}
void stop_car()
{
	forward = true;
	AllLow();

}
void reverse_car()
{
	forward = false;
	digitalWrite(leftBackwardPin, HIGH);
	digitalWrite(leftForwardPin, LOW);
	digitalWrite(rightBackwardPin, HIGH);
	digitalWrite(rightForwardPin, LOW);
}

void turn_left()
{
	if (forward)
	{
		stop_start_left();
	}
	else
	{
		stop_start_right();
	}
}

void turn_right()
{
	if (forward)
	{
		stop_start_right();
	}
	else
	{
		stop_start_left();
	}
}

void stop_start_left()
{
	digitalWrite(leftBackwardPin, LOW);
	digitalWrite(leftForwardPin, LOW);
	delayMicroseconds(10);
	if (forward)
	{
		digitalWrite(leftForwardPin, HIGH);
	}
	else
	{
		digitalWrite(leftBackwardPin, HIGH);
	}
}


void stop_start_right()
{
	digitalWrite(rightBackwardPin, LOW);
	digitalWrite(rightForwardPin, LOW);
	delayMicroseconds(10);
	if (forward)
	{
		digitalWrite(rightForwardPin, HIGH);
	}
	else
	{
		digitalWrite(rightBackwardPin, HIGH);
	}
}

void accelerate(bool acc)
{
	if (acc && speed < 1000)
	{
		speed = speed + 50;
		pwmWrite(pmwPin, speed);
	}
	else if (!acc && speed < 200)
	{
		speed = speed - 50;
		pwmWrite(pmwPin, speed);
	}
}

void AllLow()
{
	digitalWrite(leftBackwardPin, LOW);
	digitalWrite(leftForwardPin, LOW);
	digitalWrite(rightBackwardPin, LOW);
	digitalWrite(rightForwardPin, LOW);
}