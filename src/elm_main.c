#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
#include <stdlib.h>
#include "obd_helpers.h"

void main( int argc, char** argv ) {
	int fd;/*File Descriptor*/
	
	fprintf(stdout, "\n +----------------------------------+");
	fprintf(stdout, "\n |        Serial Port Read          |");
	fprintf(stdout, "\n +----------------------------------+");

	/*------------------------------- Opening the Serial Port -------------------------------*/

	/* Change /dev/ttyUSB0 to the one corresponding to your system */
	char *device_name = "/dev/ttyUSB0";
	if (argc > 1) {
		device_name = argv[1];
	}

	fd = open(device_name, O_RDWR | O_NOCTTY);	/* O_RDWR   - Read/Write access to serial port       */
												/* O_NOCTTY - No terminal will control the process   */
												/* Open in blocking mode, read will wait             */                           
								
	if(fd < 0) {	 
		printf("\n  Error! Cannot open %s \n", device_name);
		exit(0);
	} 
	printf("\n %s Opened Successfully! \n", device_name);
	// close(fd); // DELETE IT FURTHER

	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	
	struct termios SerialPortSettings;	/* Create the structure                          */

	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

	/* Setting the Baud rate for reading and writing */
	cfsetispeed(&SerialPortSettings,B38400);
	cfsetospeed(&SerialPortSettings,B38400);

	/* 8N1 Mode */
	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity    */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits, here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;    /* Clears the mask for setting the data size              */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                  */
	// SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                       */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver, Ignore Modem Control lines       */ 
	
	
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST; /*No Output Processing*/
	
	/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] = 2; /* characher count 0-255 */
	SerialPortSettings.c_cc[VTIME] = 2; /* in deciseconds (0.1 sec.); if == 0 - Wait indefinetly */

	if((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		printf("\n  ERROR ! in Setting attributes");
	else
		printf("\n  Parameters set:\n    BaudRate = 38400 \n    StopBits = 1 \n    Parity   = none\n");
		
	// /*------------------------------- Read data from serial port -----------------------------*/

	size_t buff_size = 12;
	long sleep_time = 3000; // polling interval in ms 
	char read_buffer[buff_size];   /* Buffer to store the data received       */
	int  bytes_read = 0;    /* Number of bytes read by the read() system call */

	int i;
	while (1) {
		tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer  */

		// send control word to elm327 and sleep
		char *control = "atz";
		write(fd, control, sizeof(control));
		msleep(sleep_time);

		// read answer from device
		bytes_read = read(fd, &read_buffer, buff_size); /* Read the data       */
		printf("\n\n  Bytes Rxed: %d", bytes_read); /* Print the number of bytes read */
		printf("\n\n  ");

		for( i=0; i < bytes_read; i++ )	 /*printing only the received characters*/
			printf("%c", read_buffer[i]);
	}

	printf("\n +----------------------------------+\n\n\n");
	close(fd); /* Close the serial port */
}
