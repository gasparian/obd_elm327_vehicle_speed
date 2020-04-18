#ifndef OBD_PIDS_H
#define OBD_PIDS_H

#define SET_LINE_BRK "atl1\r"
#define SET_NO_SPACES "ats0\r"
#define DEVICE_INFO "ati\r"
#define DEVICE_HARD_RESET "atz\r"
#define PID_SPEED "010D1\r" /* add 1 on the end to the set maximum */
                            /* expected number of responses        */
                            /* and so reduce the answer time       */
#define PID_RPM "010C\r"
#endif