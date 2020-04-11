#ifndef OBD_HELPERS_H
#define OBD_HELPERS_H

#define PID_SPEED "01 0D\r"
#define DEVICE_INFO "atz\r"

#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <fcntl.h>   /* File Control Definitions           */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h> /* POSIX Terminal Control Definitions */

void get_port(int *fd, char **device) {
	/*------------------------------- Opening the Serial Port -------------------------------*/

	*fd = open(*device, O_RDWR | O_NOCTTY);	/* O_RDWR   - Read/Write access to serial port       */
										    /* O_NOCTTY - No terminal will control the process   */
											/* Open in blocking mode, read will wait             */
	if(*fd < 0) {	 
		printf("\n  Error! Cannot open %s \n", *device);
		exit(1);
	} 
	printf("\n %s Opened Successfully! \n", *device);
}

void serial_setup(int *fd, size_t vmin, size_t vtime) {
	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	
	struct termios SerialPortSettings;	/* Create the structure                          */

	tcgetattr(*fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */
    
    SerialPortSettings.c_cflag = B38400 | CS8 | CLOCAL | CREAD | IGNPAR;
    /* B38400 - standart elm327 baud rate                             */
    /* CREAD | CLOCAL - Enable receiver, Ignore Modem Control lines   */
    /* CS8 - Set the data bits = 8                                    */

    // More detailed serial port configuration; may be helpful later
    // /* Setting the Baud rate for reading and writing */
	// cfsetispeed(&SerialPortSettings, B38400);
	// cfsetospeed(&SerialPortSettings, B38400);
	// /* 8N1 Mode */
	// SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity    */
	// SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits, here it is cleared so 1 Stop bit */
	// SerialPortSettings.c_cflag &= ~CSIZE;    /* Clears the mask for setting the data size              */
	// SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                  */
	// // SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                       */
	// SerialPortSettings.c_cflag |=  
	// SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	// SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
	// SerialPortSettings.c_oflag &= ~OPOST; /*No Output Processing*/

    // Setting blocking params
	SerialPortSettings.c_cc[VMIN] = vmin; // character count 0-255
	SerialPortSettings.c_cc[VTIME] = vtime; // in deciseconds (0.1 sec.); if == 0 - Wait indefinetly

	if((tcsetattr(*fd, TCSANOW, &SerialPortSettings)) != 0) { // Set the attributes to the termios structure
		printf("\n  ERROR ! in Setting attributes");
		exit(1);
	} else {
		printf("\n  Parameters set:\n    BaudRate = 38400 \n    StopBits = 1 \n    Parity   = none\n");
	}
}

int elm_talk(int *fd, char (*read_buffer)[32]) {
    /*---------- Send command to the device and wait for the answer --------- */
    int  bytes_read = 0;
    tcflush(*fd, TCIFLUSH); // Discards old data in the rx buffer

    // send control word to elm327
    write(*fd, DEVICE_INFO, sizeof(DEVICE_INFO)); // getr device name for debugging 
    // write(fd, PID_SPEED, sizeof(PID_SPEED)); // use in real life ;)

    // read answer from buffer file
    bytes_read = read(*fd, read_buffer, sizeof(*read_buffer)); 

    return bytes_read;
}

#endif