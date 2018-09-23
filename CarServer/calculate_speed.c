#include <wiringPi.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <GlobalV.h>

int butPin = 15;
time_t now, later;
struct timespec start, end;

void detect_rotation(void)
{
	if (digitalRead(butPin) == 0) {
		printf("Now\n");
		now = time(NULL);

		//double diff = difftime(later,now);
		//double diff = later - now;
		//printf("%.20f seconds \n", diff);
		clock_gettime(1, &end);

		// Calculating total time taken by the program. 
		double diff;
		diff = (end.tv_sec - start.tv_sec) * 1e9;
		diff = (diff + (end.tv_nsec - start.tv_nsec)) * 1e-9;
		double elapsedinMinutes = diff * 0.0166667;
		double rpm = 1 / elapsedinMinutes;
		double distance = rpm * 22;
		double speed = distance / 1;
		double speedKms = speed * .0006;
		car_speed = speedKms;
		printf("%f km/h", speedKms);
	}
	else {
		clock_gettime(1, &start);
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

