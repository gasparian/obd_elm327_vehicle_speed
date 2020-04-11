#include "obd_helpers.h"

void main( int argc, char** argv ) {
	int fd; /*File Descriptor*/
	size_t buff_size = 32; // some big number for tests
	size_t wait_time = 10;
	char *device_name = "/dev/ttyUSB0";	
	if (argc > 1) {
		device_name = argv[1];
	}
	
	fprintf(stdout, "\n +----------------------------------------+");
	fprintf(stdout, "\n |         Serial Port Read/Write         |");
	fprintf(stdout, "\n +----------------------------------------+");

	// open serial port file
	get_port(&fd, &device_name);

	// set up serial port
	serial_setup(&fd, buff_size, wait_time);

	char read_buffer[buff_size]; // Buffer to store the data received
	int bytes_read = 0; // Number of bytes read by the read() system call

	// talking to the device
	int i;
	while (1) {
		// write command and read result
		bytes_read = elm_talk(&fd, &read_buffer);

		printf("\n\n  Bytes Rxed: %d\n", bytes_read);
		// print the answer only, length of bytes_read
		for( i=0; i < bytes_read; i++ )
			printf("%c", read_buffer[i]);
	}

	printf("\n +----------------------------------+\n\n\n");
	close(fd); // Close the serial port
}
