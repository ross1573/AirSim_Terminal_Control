# AirSim_Terminal_Control

Terminal controller for AirSim, including free movement (W,A,S,D...)  
AirLib is not included, please download from [AirSim](https://github.com/microsoft/AirSim.git).  
Now supports rpclib, string commands can be executed from anothor process.  

## Supporting OS
macOS  
Windows  
  
## Commands
* speed [ horizontal, vertical, yaw ]
* keyboard
   ~~~
   W,A,S,D for horizontal move
   Z,X for vertical move
   Q,E for yaw move
   F for teminate
   ~~~
* arm
* disarm
* takeoff [ timeout:10 ]
* land [ timeout:10 ]
* home [ timeout:60 ]
* move [ X, Y, Z ] [ duration ] [ drivetraintype ] [ yaw ]  
 move [ to ] [ X, Y, Z ] [ velocity:speed(horizontal)]  
 move [ path ] [ name of file ] [ velocity:speed(horizontal)]  
* rotate [ yaw ] [ duration ]  
rotate [ to ] [ yaw ] [ timeout:10 ]
* reset  
   ~~~
   Pressing R,T at once, resets the drone to initial condition
   ~~~  

## Rpclib
* 'TerminalController Name' [ ip:stdin ] [ port ]
   ~~~
   'stdin' is the standard mode, which rpc is not used.
   ~~~
* call 'str_cmd' function and pass 'Commands' to server
* client example is provided at [rpc_client_example.cpp](https://github.com/ross1573/AirSim_Terminal_Control/blob/main/rpc_client_example.cpp)
