#include <wiringPi.h>
#include <stdbool.h>

static int leftForwardPin = 29;
static int leftBackwardPin = 28;
static int rightForwardPin = 25;
static int rightBackwardPin = 24;
static int pmwPin = 1;
static int speed = 90;
static bool forward = true;

void init_car()
{
	pinMode(leftForwardPin, OUTPUT);
	pinMode(leftBackwardPin, OUTPUT);
	pinMode(rightForwardPin, OUTPUT);
	pinMode(rightBackwardPin, OUTPUT);
	AllLow();
	pwmWrite(pmwPin, speed);
}
void start_car()
{
	speed = 90;
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
	AllLow();
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
	delayMicroseconds(1000000);
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
	delayMicroseconds(1000000);
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
	if (acc && speed <= 100)
	{
		speed = speed + 10;
		if (speed > 100)
		{
			speed = 100;
		}
		pwmWrite(pmwPin, speed);
		printf("Increasing speed to %d", speed);
	}
	else if (!acc && speed > 20)
	{
		speed = speed - 10;
		if (speed < 20)
		{
			speed = 20;
		}
		pwmWrite(pmwPin, speed);
		printf("Recuding speed to %d", speed);
	}
}

void AllLow()
{
	digitalWrite(leftBackwardPin, LOW);
	digitalWrite(leftForwardPin, LOW);
	digitalWrite(rightBackwardPin, LOW);
	digitalWrite(rightForwardPin, LOW);
}