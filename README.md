#serial_tool_ver_0.01
---------------------------------
##build
1. `$ cd build`  
2. `$ cmake ..`  
3. `$ make`  
---------------------------------

## serial send
1. run  
```
$ ./SerialSend ${Port} ${Baudrate} ${Sendmode} ${Message} $[Looptime]
```
```
$ ./SerialSend /dev/ttyUSB0 9600 hex AAFF01 1000
```
send 0xAA 0xFF 0x01 9600 baudrate at port /dev/ttyUSB0 every 1000ms  
${Port} :/dev/ttyUSB0 /dev/ttyS1 ...  
${Baudrate} :4800 9600 ...  
${Sendmode} :ascii or hex  
${Message} :string or Hex byte  
$[Looptime]: default once , ms  
