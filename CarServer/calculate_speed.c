#include <wiringPi.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

int butPin = 15;
time_t now, later;

void detect_rotation(void)
{
	if (digitalRead(butPin) == 0) {
		printf("Received SIGINT. Exiting Application\n");
		time(&now);
		double diff = difftime(now, later);
		double elapsedinMinutes = diff * 0.0166667;
		double rpm = rpm = 1 / elapsedinMinutes;
		double distance = rpm * 22;
		double speed = distance / 1;
		double speedKms = speed * .0006;
		printf("%.f km/h", diff);
	}
	else {
		time(&later);
	}
}

void init_sensor()
{
	pinMode(butPin, INPUT);
	pullUpDnControl(butPin, PUD_UP);
	wiringPiISR(butPin, INT_EDGE_BOTH, detect_rotation);
	while (1) {}
}

