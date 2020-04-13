#include "obd_helpers.h"

void main( int argc, char** argv ) {
	setbuf(stdout, NULL); // disable stdout buffering

	printf("\n +----------------------------------------+");
	printf("\n |         Serial Port Read/Write         |");
	printf("\n +----------------------------------------+");

	/*------------------------ Set initial params --------------------*/
	int fd; /*File Descriptor*/
	size_t buff_size = 20; // nbytes, 11 for device check, 2 for speed (?)
    size_t vmin = 20; // min characters to read
	size_t vtime = 8; // in deciseconds
	char *device_name = "/dev/ttyUSB0";	
	if (argc > 1) {
		device_name = argv[1];
	}
	/*----------------------------------------------------------------*/

    /*----------------------- Configure serial port ------------------*/
	// open serial port file
	get_port(&fd, &device_name);

	// set up serial port
	serial_setup(&fd, vmin, vtime);
	/*----------------------------------------------------------------*/

	/*----------------------- Talking to the device ------------------*/
	char *answer = malloc(buff_size);
	size_t bytes_read = 0;
	int speed = 0;
	unsigned long ts;
	// warming up
	bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_INFO);
	int check = answer_check(answer, "ELM327 v1.5", bytes_read);
	if (check != 0) {
		fprintf(stderr, "Elm bad response!\n");
		close(fd); // Close serial port
		exit(1);
	}

	printf("\nReady to talk!\n");
	int iter = 0;
	while (++iter) {
		// clean the buff array
		bzero(answer, buff_size);

		// write command and read result
		bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_INFO); // for debugging without a car
		// bytes_read = elm_talk(&fd, answer, buff_size, PID_SPEED); // for `prod` <<<<<<<<<<<<<<<<<<<<<<
		ts = get_time();

		
		// int check = answer_check("41 0D 00", "41 0D", 8); <<<<<<<<<<<<<<<<<<<<<<
		// if ( (bytes_read < 0) || (check != 0) ) {
		// 	printf("Reading error!");
		// 	continue;
		// }

		if (bytes_read < 0) {
			printf("Reading error!");
			continue;
		}

        // DEBUG
		printf("\n[%d] time (us): %lu; Bytes Rxed: %zu; \nAnswer: \n", 
		       iter-1, ts, bytes_read);
		printf("%s", answer);
		printf("\n");
		//

		// usleep(1000000); // sleep for 1 sec.
		
		// get the vehicle speed from string <<<<<<<<<<<<<<<<<<<<<<
		// speed = get_vehicle_speed(answer);
	    // printf("\n[%d] time (us): %lu; Bytes Rxed: %zu; Speed: %d\n", 
		//        iter-1, ts, bytes_read, speed);
		
	}
	free(answer);
	/*----------------------------------------------------------------*/

	close(fd); // Close serial port
	printf("\n +----------------------------------+\n\n\n");
}
