#include <wiringPi.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <GlobalV.h>

int butPin = 15;
time_t now, later;
struct timespec speed_start, speed_end;

void detect_rotation(void)
{
	if (digitalRead(butPin) == 0) {
		printf("Now\n");
		now = time(NULL);

		//double diff = difftime(later,now);
		//double diff = later - now;
		//printf("%.20f seconds \n", diff);
		clock_gettime(1, &speed_end);

		// Calculating total time taken by the program. 
		double diff;
		diff = (speed_end.tv_sec - speed_start.tv_sec) * 1e9;
		diff = (diff + (speed_end.tv_nsec - speed_start.tv_nsec)) * 1e-9;
		double elapsedinMinutes = diff * 0.0166667;
		double rpm = 1 / elapsedinMinutes;
		double distance = rpm * 22;
		double speed = distance / 1;
		double speedKms = speed * .6;
		car_speed = speedKms;
		//printf("%d km/h", speedKms);
	}
	else {
		clock_gettime(1, &speed_start);
	}
}

void init_sensor()
{
	pinMode(butPin, INPUT);
	pullUpDnControl(butPin, PUD_UP);
	wiringPiISR(butPin, INT_EDGE_BOTH, detect_rotation);
	//while (1) {
	//}
}

