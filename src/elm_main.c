#include "obd_helpers.h"

void main( int argc, char** argv ) {
    setbuf(stdout, NULL); // disable stdout buffering

    printf("\n +----------------------------------------+");
    printf("\n |         Serial Port Read/Write         |");
    printf("\n +----------------------------------------+");

    /*------------------------ Set initial params --------------------*/
    int fd; /*File Descriptor*/
    char *device_name = "/dev/ttyUSB0";    
    if (argc > 1) {
        device_name = argv[1];
    }
    int debug_mode = 0;
    if (argc > 2) {
        if (strcmp(argv[2], "debug") == 0) {
            debug_mode = 1;
        }
    }
    size_t vmin = 18;
    if ( debug_mode ) {
        vmin = 21;
    }
    size_t buff_size = 21; // nbytes
    size_t vtime = 2; // blocking read time in deciseconds
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
    int16_t speed = INT16_MIN;
    unsigned long tsw, tsr;

    // elm hard reset
    int check = -1;
    bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_HARD_RESET, 1);
    if ( debug_mode ) {
        check = answer_check(answer, "ELM327 v1.5", bytes_read);
    } else {
        bzero(answer, buff_size);
        bytes_read = elm_talk(&fd, answer, buff_size, PID_SPEED, 1);
        check = answer_check(answer, "41 0D", 5);
    }
    if (check != 0) {
        fprintf(stderr, "Elm bad response!\n");
        close(fd); // Close serial port
        exit(1);
    }
    printf("Ready to talk!\n");

    printf("\nid,wtime,rtime,bytes_read,data\n");
    int iter = 0;
    while (++iter) {
        // clean the buff array
        bzero(answer, buff_size);

        // write command and read result
        tsw = get_time();
        if ( debug_mode ) {
            bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_INFO, 0);
        } else {
            bytes_read = elm_talk(&fd, answer, buff_size, PID_SPEED, 0);
        }
        tsr = get_time();

        if ( debug_mode ) {
            if (bytes_read < 0) { 
                fprintf(stderr, "%d, %lu, %lu, 0, Reading error!\n", 
                        iter-1, tsw, tsr);
                continue;
            }
            printf("%d, %lu, %lu, %zu, %s\n", 
                   iter-1, tsw, tsr, bytes_read, answer);
        } else {
            int check = answer_check(answer, "41 0D", 5);
            if ( (bytes_read < 0) || (check != 0) ) {
                fprintf(stderr, "%d, %lu, %lu, %zu, Reading error!\n", 
                        iter-1, tsw, tsr, bytes_read);
                continue;
            }
            speed = get_vehicle_speed(answer);
            printf("%d, %lu, %lu, %zu, %d\n", 
                   iter-1, tsw, tsr, bytes_read, speed);
        }
        
    }
    free(answer);
    /*----------------------------------------------------------------*/

    close(fd); // Close serial port
    printf("\n +----------------------------------+\n\n\n");
}
