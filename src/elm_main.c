#include "obd_helpers.h"

void main( int argc, char** argv ) {
    setbuf(stdout, NULL); // disable stdout buffering

    printf("\n +----------------------------------------+");
    printf("\n |         Serial Port Read/Write         |");
    printf("\n +----------------------------------------+");

    /*------------------------ Set initial params --------------------*/
    int fd; /*File Descriptor*/
    size_t buff_size = 21; // nbytes
    size_t vmin = 21; // min characters to read (can be <= buff_size)
    size_t vtime = 1; // blocking read time in deciseconds
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
    unsigned long ts;

    // elm hard reset
    bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_HARD_RESET, 1);
    int check = answer_check(answer, "ELM327 v1.5", bytes_read);
    if (check != 0) {
        fprintf(stderr, "Elm bad response!\n");
        close(fd); // Close serial port
        exit(1);
    }
    printf("Ready to talk!\n");

    printf("\nid, time, bytes_read, data\n");
    int iter = 0;
    while (++iter) {
        // clean the buff array
        bzero(answer, buff_size);

        // write command and read result
        if ( debug_mode ) {
            bytes_read = elm_talk(&fd, answer, buff_size, DEVICE_INFO, 0);
        } else {
            bytes_read = elm_talk(&fd, answer, buff_size, PID_SPEED, 0);
        }
        ts = get_time();

        if ( debug_mode ) {
            if (bytes_read < 0) { 
                fprintf(stderr, "%d, %lu, 0, Reading error!\n", 
                        iter-1, ts);
                continue;
            }
            printf("%d, %lu, %zu, %s\n", 
                   iter-1, ts, bytes_read, answer);
        } else {
            int check = answer_check(answer, "41 0D", bytes_read); 
            if ( (bytes_read < 0) || (check != 0) ) {
                fprintf(stderr, "%d, %lu, %zu, Reading error!\n", 
                        iter-1, ts, bytes_read);
                continue;
            speed = get_vehicle_speed(answer);
            printf("%d, %lu, %zu, %d\n", 
                   iter-1, ts, bytes_read, speed);
            }
        }
        
    }
    free(answer);
    /*----------------------------------------------------------------*/

    close(fd); // Close serial port
    printf("\n +----------------------------------+\n\n\n");
}
