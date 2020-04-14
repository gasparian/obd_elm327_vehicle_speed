## ELM327 signals processing on Linux  

The goal is to get a speed from vehicle using a super-cheap elm327 controller and a standardized OBD API.  

Before begin:  
 - [obd communication cheatsheet](https://gist.github.com/gasparian/d8c24743e0e2527e2c1c3090a1bcf9df);  
 - [elm reference](https://www.elmelectronics.com/wp-content/uploads/2016/07/ELM327DS.pdf);  
 - [termios serial params](https://www.cmrr.umn.edu/~strupp/serial.html);  

### Configuring COM-port  

The main thing - is to configure serial port in right way. The communication itself is straight forward.  
Here are some basic serial port parameters for elm327 devices:  
 - Baudrate - 38400; 
 - 8 data bits;  
 - Stop bits - 1;  
 - No Parity;  

Sellecting the Serial port Number on Linux:  
 - `/dev/ttyUSBx` - when using USB to Serial Converter, where x can be 0,1,2...etc;  
 - `/dev/ttySx`   - for PC hardware based Serial ports, where x can be 0,1,2...etc;  
Remember, that in Linux, everything is a file - so you can apply standart read/write operations on `/dev/tty*`.  

Also, for quick tests, I use `screen`:  
```
sudo screen -L /dev/ttyUSB0 38400,cs8
```  
After finishing the screen session, the serial port "saves" the last configuration, so it becomes really useful for debugging. To get the last serial port configs for particular device, jsut run `stty`:  
```
sudo stty -F /dev/ttyUSB0 -a
```  

### Using the code  

All you need is stored in `obd_helpers.h`. The `elm_main.c` needed only to show the functionality.  

Compile:  
```
gcc -o elm_main elm_main.c
```  
Run, passing the needed device name (you need to be root or superuser):  
```
sudo ./elm_main /dev/ttyUSB0
```  
Or run with `debug` flag to test the code without a car:  
```
sudo ./elm_main /dev/ttyUSB0 debug
```  
The output of the main script is csv-like formatted, so it convenient to dump it to file and analyze:  
```
sudo ./elm_main /dev/ttyUSB0 > /tmp/elm_speed.csv
```  



