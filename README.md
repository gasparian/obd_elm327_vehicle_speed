## elm327 signals processor on linux  

Check out the [obd communication cheatsheet](https://gist.github.com/gasparian/d8c24743e0e2527e2c1c3090a1bcf9df).  

### Configuring COM-port on linux  

/*----------------------------------------------------------------------------------------------------*/
/* Program reads a string from the serial port at 9600 bps 8N1 format                                 */
/* Baudrate - 9600                                                                                    */
/* Stop bits -1                                                                                       */
/* No Parity                                                                                          */
/*----------------------------------------------------------------------------------------------------*/

/*====================================================================================================*/
/* Running the executable                                                                             */
/* ---------------------------------------------------------------------------------------------------*/ 
/* 1) Compile the  serialport_read.c  file using gcc on the terminal (without quotes)                 */
/*                                                                                                    */
/*	" gcc -o serialport_read serialport_read.c "                                                      */
/*                                                                                                    */
/* 2) Linux will not allow you to access the serial port from user space,you have to be root.So use   */
/*    "sudo" command to execute the compiled binary as super user.                                    */
/*                                                                                                    */
/*       "sudo ./serialport_read"                                                                     */
/*                                                                                                    */
/*====================================================================================================*/

/*====================================================================================================*/
/* Sellecting the Serial port Number on Linux                                                         */
/* ---------------------------------------------------------------------------------------------------*/ 
/* /dev/ttyUSBx - when using USB to Serial Converter, where x can be 0,1,2...etc                      */
/* /dev/ttySx   - for PC hardware based Serial ports, where x can be 0,1,2...etc                      */
/*====================================================================================================*/

/*-------------------------------------------------------------*/
/* termios structure -  /usr/include/asm-generic/termbits.h    */ 
/* use "man termios" to get more info about  termios structure */
/*-------------------------------------------------------------*/    

### Running the code  

Compile:  
```
gcc -o elm_main elm_main.c
```  
Run, passing the needed device:  
```
./elm_main /dev/ttyUSB0
```  


