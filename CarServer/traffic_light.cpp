#include <wiringPi.h>
#include <stdlib.h> 

void init_traffic_light(int green, int red, int yellow)
{
	if (wiringPiSetup() == -1)
		exit(1);
	pinMode(green, OUTPUT);
	pinMode(red, OUTPUT);
	pinMode(yellow, OUTPUT);
	while (1)
	{
		digitalWrite(green, HIGH);
		delayMicroseconds((rand() % 15 + 1) * 1000000);
		digitalWrite(green, LOW);
		digitalWrite(yellow, HIGH);
		delayMicroseconds((rand() % 15 + 1) * 1000000);
		digitalWrite(yellow, LOW);
		digitalWrite(red, HIGH);
		delayMicroseconds((rand() % 15 + 1) * 1000000);
		digitalWrite(red, LOW);
	}
}

int main(void) {
	init_traffic_light(29, 27, 28);
}