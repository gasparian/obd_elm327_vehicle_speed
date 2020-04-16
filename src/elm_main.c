#include "obd_helpers.h"

void main( int argc, char** argv ) {
    setbuf(stdout, NULL); // disable stdout buffering

    printf("+----------------------------------------+\n");
    printf("|         Serial Port Read/Write         |\n");
    printf("+----------------------------------------+\n");

    /*------------------- Set initial params and vars ----------------*/
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

    char *msg = "%d,%lu,%lu,%lu,%zu,%s\n";
    char *msg_speed = "%d,%lu,%lu,%lu,%zu,%d\n";
    size_t bytes_read = 0;
    int16_t speed = INT16_MIN;
    unsigned long tsw, tsr, dt;
    int check = -1;
    int iter = 0;
    /*----------------------------------------------------------------*/

    /*----------------------- Configure serial port ------------------*/
    // open serial port file
    get_port(&fd, &device_name);

    // set up serial port
    serial_setup(&fd, vmin, vtime);
    /*----------------------------------------------------------------*/

    /*----------------------- Talking to the device ------------------*/

    char *answer = malloc(buff_size);
    if ( debug_mode ) {
        // elm hard reset
        bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_HARD_RESET, 3);
        check = answer_check(answer, "ELM327", 6);
    } else {   
        bytes_read = elm_talk(&fd, answer, buff_size, PID_SPEED, 2);
        check = answer_check(answer, "41 0D", 5);
    }
    if (check != 0) {
        fprintf(stderr, "Elm bad response!\n");
        close(fd); // Close serial port
        exit(1);
    }
    printf("\nReady to talk!\n");
    /*----------------------------------------------------------------*/

    /*----------------------- Log the output -------------------------*/
    printf("\nid,wtime,rtime,dt,bytes_read,data\n");
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
		dt = tsr - tsw;

        if ( debug_mode ) {
            if (bytes_read < 0) { 
                fprintf(stderr, msg, iter-1, tsw, tsr, dt, 0, "");
                continue;
            }
            printf(msg, iter-1, tsw, tsr, dt, bytes_read, answer);
        } else {
            int check = answer_check(answer, "41 0D", 5);
            if ( (bytes_read < 0) || (check != 0) ) {
                fprintf(stderr, msg, iter-1, tsw, tsr, dt, bytes_read, "");
                continue;
            }
            speed = get_vehicle_speed(answer);
            printf(msg_speed, iter-1, tsw, tsr, dt, bytes_read, speed);
            // printf(msg, iter-1, tsw, tsr, bytes_read, answer);
        }
    }
    /*----------------------------------------------------------------*/

    // Release memory and close serial port if the loop is finite
    free(answer); 
    close(fd);
    printf("\n +----------------------------------+\n\n\n");
}
