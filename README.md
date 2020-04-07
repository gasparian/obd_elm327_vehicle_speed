## elm327 signals processor on linux  

### Configure virtual serial port for tests  
Install [socat](http://www.dest-unreach.org/socat/doc/socat.html):  
```
sudo apt-get install socat
dmesg | grep tty
```  
Line below creates a pair of VSP’s:  
```
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```  
Connecting executable:  
```
socat -d -d pty,raw,echo=0 "exec:myprog ...,pty,raw,echo=0"
``` 
where the executable myprog will be connected with the VSP through stdio.  



