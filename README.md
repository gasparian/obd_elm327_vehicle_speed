## ELM327 communication on Linux  

The goal is to get a vehicle speed with high frequency, using a super-cheap elm327 controller and a standardized OBD API.  
The speed is usually encoded with two-digit hexadecimal number and can vary in range 0...255 km\h.  

Before begin:  
 - [obd communication cheatsheet](https://gist.github.com/gasparian/d8c24743e0e2527e2c1c3090a1bcf9df);  
 - [elm reference](https://www.elmelectronics.com/wp-content/uploads/2016/07/ELM327DS.pdf);  
 - [termios serial params](https://www.cmrr.umn.edu/~strupp/serial.html);  
 - [OBD-II PIDs](https://en.wikipedia.org/wiki/OBD-II_PIDs);  

### Configuring COM-port  

The main thing - is to configure serial port in a right way. The communication itself is straight forward.  
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
After finishing the screen session, the serial port "saves" the last configuration, so it becomes really useful for debugging. To get the last serial port configs for particular device, just run `stty`:  
```
sudo stty -F /dev/ttyUSB0 -a
```  

### Using the code  

All you need is stored in `obd_helpers.h`. The `elm_main.c` is only to show the functionality.  

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
sudo ./elm_main /dev/ttyUSB0 > /tmp/elm_speed.csv 2>&1
```  
To monitor the logfile while dumping:  
```
tail -f /tmp/elm_log.csv
```  

#### Skoda Octavia A5 2011 actual OBD PIDS  
Here are PIDs supported by tested car:  
```
{
    '03': 'Fuel system status',
    '04': 'Calculated engine load',
    '05': 'Engine coolant temperature',
    '06': 'Short term fuel trim—Bank 1',
    '07': 'Long term fuel trim—Bank 1',
    '0B': 'Intake manifold absolute pressure',
    '0C': 'Engine RPM',
    '0D': 'Vehicle speed',
    '0E': 'Timing advance',
    '0F': 'Intake air temperature',
    '11': 'Throttle position',
    '12': 'Commanded secondary air status',
    '13': 'Oxygen sensors present (in 2 banks)',
    '15': 'Oxygen Sensor 2 A: Voltage B: Short term fuel trim',
    '1C': 'OBD standards this vehicle conforms to',
    '20': 'PIDs supported [21 - 40]',
    '33': 'Absolute Barometric Pressure'
}
```  


### To do:  
 - set the elm<-->ecu communication via `AT ST hh` < 32 (32 ~= 200 ms) and try 10Hz transmission - seems like no effect...;  
 - try to set maximum expected number of responses as a single hex digit after request: `01 0D 1` or `01 0D 01`?;   
 - turn on the headers `AT H1` and try to request speed, you should see the ECU id: (`83 F1 10 41 0D 00 D2`, `84 F1 10 41 0C 00 00 D2`);  
  - check the protocol for the car: AT DP <- можно посмотреть, AT SP h <- найти протокол по номеру h;  
  - check the barometric pressure and temperature;  
  - try to set engine ECU header `AT SH 86 10 F1` and change the priority byte (`86`) to a higher value;  
