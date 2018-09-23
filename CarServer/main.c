/* Example code for starting
 * 2 threads and synchronizing
 * their operation using a message_queue.
 *
 * All code provided is as is
 * and not completely tested
 *
 * Author: Aadil Rizvi
 * Date: 6/1/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <util.h>
#include <control_car.h>
#include <bluez_server.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <calculate_speed.h>
#include <GlobalV.h>

extern void init_sensor();

#define CAR_CMD_NAME "/car_cmd"

char carCMD[1024];

pthread_t carReceiverThread;
pthread_t carControllerThread;
pthread_t calculateSpeedThread;
pthread_t getCarSpeedThread;

static struct mq_attr my_mq_attr;
static mqd_t my_mq;

static unsigned int counter;
static char *message = "abcd";

void carReceiver_main(void);
void carController_main(void);
void calculateSpeed_main(void);
void getCarSpeed_main(void);

void sig_handler(int signum) {
	if (signum != SIGINT) {
		printf("Received invalid signum = %d in sig_handler()\n", signum);
		ASSERT(signum == SIGINT);
	}

	printf("Received SIGINT. Exiting Application\n");

	AllLow();// Clean up the pins

	pthread_cancel(carReceiverThread);
	pthread_cancel(carControllerThread);
	pthread_cancel(calculateSpeedThread);
	pthread_cancel(getCarSpeedThread);

	mq_close(my_mq);
	mq_unlink(CAR_CMD_NAME);

	exit(0);
}

int main(void) {
	pthread_attr_t attr;
	int status;

	signal(SIGINT, sig_handler);

	counter = 0;
	init_car();

	my_mq_attr.mq_maxmsg = 10;
	my_mq_attr.mq_msgsize = sizeof(counter);

	my_mq = mq_open(CAR_CMD_NAME, \
		O_CREAT | O_RDWR | O_NONBLOCK, \
		0666, \
		&my_mq_attr);

	ASSERT(my_mq != -1);

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 1024 * 1024);

	printf("Creating Car cmd receiver\n");
	status = pthread_create(&carReceiverThread, &attr, (void*)&carReceiver_main, NULL);
	if (status != 0) {
		printf("Failed to create carReceiverThread with status = %d\n", status);
		ASSERT(status == 0);
	}

	printf("Creating car controller\n");
	status = pthread_create(&carControllerThread, &attr, (void*)&carController_main, NULL);
	if (status != 0) {
		printf("Failed to create carControllerThread with status = %d\n", status);
		ASSERT(status == 0);
	}

	printf("Calculate speed thread\n");
	status = pthread_create(&calculateSpeedThread, &attr, (void*)&calculateSpeed_main, NULL);
	if (status != 0) {
		printf("Failed to create calcuate speed with status = %d\n", status);
		ASSERT(status == 0);
	}

	printf("Calculate speed thread\n");
	status = pthread_create(&getCarSpeedThread, &attr, (void*)&getCarSpeed_main, NULL);
	if (status != 0) {
		printf("Failed to create calcuate speed with status = %d\n", status);
		ASSERT(status == 0);
	}

	pthread_join(carReceiverThread, NULL);
	pthread_join(carControllerThread, NULL);
	pthread_join(calculateSpeedThread, NULL);
	pthread_join(getCarSpeedThread, NULL);

	sig_handler(SIGINT);

	return 0;
}

void getCarSpeed_main(void)
{
	while (1)
	{
		usleep(1000000);
		printf("%f km/h from main", car_speed);
	}
}
void calculateSpeed_main(void) 
{
	car_speed = 0;
	init_sensor();
}
void carReceiver_main(void) {
	unsigned int exec_period_usecs;
	int status;

	exec_period_usecs = 1000000; /*in micro-seconds*/
	int client = init_server();
	printf("Client = %d \n", \
		client);
	printf("Thread 1 started. Execution period = %d uSecs\n", \
		exec_period_usecs);
	while (1) {
		int cmd = 0;// read_server(client, carCMD);
		int bytes_read = 0;
		bytes_read = read(client, carCMD, 1);
		if (bytes_read > 0) {
			printf("received [%s]\n", carCMD);
		}

		if (*carCMD == 'S')
		{
			//start_car();
			cmd = 1;
			printf("started");
		}
		else if (*carCMD == 'P')
		{
			cmd = 2;
			//stop_car();
		}
		else if (*carCMD == 'L')
		{
			cmd = 3;
			//turn_left();
		}
		else if (*carCMD == 'R')
		{
			cmd = 4;
			//turn_right();
		}
		else if (*carCMD == 'A')
		{
			cmd = 5;
			//accelerate(true);
		}
		else if (*carCMD == 'D')
		{
			cmd = 6;
			//accelerate(false);
		}
		printf("Message = %s \n", carCMD);
		status = mq_send(my_mq, (const char*)&cmd, sizeof(counter), 1);
		ASSERT(status != -1);
	}
}


void carController_main(void) {
	unsigned int exec_period_usecs;
	int status;
	int recv_counter;

	exec_period_usecs = 10000; /*in micro-seconds*/

	printf("Thread 2 started. Execution period = %d uSecs\n", \
		exec_period_usecs);

	while (1) {
		status = mq_receive(my_mq, (char*)&recv_counter, \
			sizeof(recv_counter), NULL);
		//status = mq_receive(my_mq, (char*)&recv_message, \
			sizeof(recv_message), NULL);
		//recv_counter = 0;
		if (status > 0) {
			printf("RECVd MSG in Car: %d\n", recv_counter);
			if (recv_counter == 1)
			{
				start_car();
				printf("started");
			}
			else if (recv_counter == 2)
			{
				stop_car();
			}
			else if (recv_counter == 3)
			{
				turn_left();
			}
			else if (recv_counter == 4)
			{
				turn_right();
			}
			else if (recv_counter == 5)
			{
				accelerate(true);
			}
			else if (recv_counter == 6)
			{
				accelerate(false);
			}
		}

		usleep(exec_period_usecs);
	}
}
