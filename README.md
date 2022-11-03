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
${Port} :/dev/ttyUSB0 /dev/ttyS1 ...
${Baudrate} :4800 9600 ...
${Sendmode} :ascii or hex
${Message} :string or Hex byte
$[Looptime]: default once , ms
