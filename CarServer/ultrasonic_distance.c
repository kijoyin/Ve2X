#include <wiringPi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <GlobalV.h>


int TRIG = 4, ECHO = 5;
struct timespec start, end, timeoutStart, timeoutEnd;

int ping()
{
	long ping = 0, timeoutStart_s = 0;
	long pong = 0, timeoutEnd_s = 0;
	float distance = 0;
	long timeout = 500000; // 0.5 sec ~ 171 m

	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);

	// Ensure trigger is low.
	digitalWrite(TRIG, LOW);
	delay(50);

	// Trigger the ping.
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);

	clock_gettime(1, &timeoutStart);
	timeoutStart_s = timeoutStart.tv_nsec *0.001;
	// Wait for ping response, or timeout.
	while (digitalRead(ECHO) == LOW) {
		clock_gettime(1, &timeoutEnd);
		timeoutEnd_s = timeoutEnd.tv_nsec *0.001;
		if ((timeoutEnd_s - timeoutStart_s) > timeout)
		{
			return -1;
		}
	}
	clock_gettime(1, &start);
	ping = start.tv_nsec *0.001;

	// Wait for pong response, or timeout.
	clock_gettime(1, &timeoutStart);
	timeoutStart_s = timeoutStart.tv_nsec *0.001;
	while (digitalRead(ECHO) == HIGH) {
		clock_gettime(1, &timeoutEnd);
		timeoutEnd_s = timeoutEnd.tv_nsec *0.001;
		if ((timeoutEnd_s - timeoutStart_s) > timeout)
		{
			return -1;
		}
	}
	clock_gettime(1, &end);
	pong = end.tv_nsec *0.001;


	// Convert ping duration to distance.
	distance_to_crash = (float)(pong - ping) * 0.017150;

	//printf("Distance: %.2f cm.\n", distance_to_crash);

	return 1;
}
