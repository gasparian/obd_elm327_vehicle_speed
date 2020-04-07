## elm327 signals processor on linux  

### Configure virtual serial port for tests  
Install `socat`:  
```
sudo apt-get install socat
dmesg | grep tty
```  
Create new virtual serial port:  
```
socat -d pty,raw,echo=0
```  


