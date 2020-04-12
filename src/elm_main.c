#include "obd_helpers.h"

void main( int argc, char** argv ) {
	setbuf(stdout, NULL); // disable stdout buffering

	printf("\n +----------------------------------------+");
	printf("\n |         Serial Port Read/Write         |");
	printf("\n +----------------------------------------+");

	/*------------------------ Set initial params --------------------*/
	int fd; /*File Descriptor*/
	size_t buff_size = 20; // nbytes, 11 for device check, 2 for speed
    size_t vmin = 20; // min characters to read
	size_t vtime = 10; // in deciseconds
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
	elm_talk(&fd, answer, buff_size, DEVICE_INFO);
	printf("\nReady to talk!\n");
	int iter = 0;
	while (++iter) {
		// clean the buff char array
		bzero(answer, buff_size);

		// write command and read result
		bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_INFO); // for debugging without a car
		// bytes_read = elm_talk(&fd, answer, buff_size, PID_SPEED); // for `prod`

		if (bytes_read < 0) {
			printf("Reading error!");
			continue;
		}
		ts = get_time();

        // DEBUG
		printf("\n[%d] time (us): %lu; Bytes Rxed: %zu; Answer: %s\n", 
		       iter-1, ts, bytes_read, answer);
		usleep(1000000); // sleep
		
		// get the vehicle speed from string
		// size_t answer_size = sizeof(answer)/sizeof(answer[0]);
		// speed = get_vehicle_speed(answer, answer_size);
	    // printf("\n[%d] time (us): %lu; Bytes Rxed: %zu; Speed: %d\n", 
		//        iter-1, ts, bytes_read, speed);
		
	}
	free(answer);
	/*----------------------------------------------------------------*/

	close(fd); // Close the serial port
	printf("\n +----------------------------------+\n\n\n");
}
