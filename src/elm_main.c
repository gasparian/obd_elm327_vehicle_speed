#include "obd_helpers.h"

void main( int argc, char** argv ) {

	/*------------------------ Set initial params --------------------*/
	int fd; /*File Descriptor*/
	size_t buff_size = 32;
    size_t vmin = 20; // min characters to read
	size_t vtime = 10; // in deciseconds
	char *device_name = "/dev/ttyUSB0";	
	if (argc > 1) {
		device_name = argv[1];
	}
	/*----------------------------------------------------------------*/
	
	fprintf(stdout, "\n +----------------------------------------+");
	fprintf(stdout, "\n |         Serial Port Read/Write         |");
	fprintf(stdout, "\n +----------------------------------------+");

    /*----------------------- Configure serial port ------------------*/
	// open serial port file
	get_port(&fd, &device_name);

	// set up serial port
	serial_setup(&fd, vmin, vtime);
	/*----------------------------------------------------------------*/

	/*----------------------- Talking to the device ------------------*/
	char *answer = malloc(buff_size);
	// warming up
	elm_talk(&fd, answer, buff_size);
	printf("\nReady to talk!\n");
	while ( 1 ) {
		// write command and read result
		elm_talk(&fd, answer, buff_size);

		printf("\n\n  Bytes Rxed: %zu\n", sizeof(answer));
		// print the answer only, length of bytes_read
		printf("  %s\n", answer);
		printf("  %d\n", *answer);
	}
	free(answer);
	/*----------------------------------------------------------------*/

	printf("\n +----------------------------------+\n\n\n");
	close(fd); // Close the serial port
}
