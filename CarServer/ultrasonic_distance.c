#include <wiringPi.h>
#include <stdio.h>

#define GPIO22     23
#define GPIO23     22


static int trigger = GPIO22;
static int echo = GPIO23;

static volatile long startTimeUsec;
static volatile long endTimeUsec;

void recordPulseLength() {
	startTimeUsec = micros();
	while (digitalRead(echo) == HIGH);
	endTimeUsec = micros();
}


int calculate_distance()
{
	double speedOfSoundMetersPerSecond = 340.29;

	wiringPiSetup();

	pinMode(trigger, OUTPUT);
	pinMode(echo, INPUT);

	// Initialize the sensor's trigger pin to low. If we don't pause
	// after setting it to low, sometimes the sensor doesn't work right.
	digitalWrite(trigger, LOW);
	delay(500); // .5 seconds

	// Triggering the sensor for 10 microseconds will cause it to send out
	// 8 ultrasonic (40Khz) bursts and listen for the echos.
	digitalWrite(trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigger, LOW);

	// The sensor will raise the echo pin high for the length of time that it took
	// the ultrasonic bursts to travel round trip.
	// Doesn't work; endTimeUsec and startTimeUsec are always the same.
	//wiringPiISR(echo, INT_EDGE_RISING, &recordPulseLength);
	while (digitalRead(echo) == LOW);
	recordPulseLength();

	long travelTimeUsec = endTimeUsec - startTimeUsec;
	double distanceMeters = ((travelTimeUsec / 1000000.0)*speedOfSoundMetersPerSecond) / 2;

	printf("Distance is %s cm", distanceMeters * 100);

	return 0;
}
