# AirSim_Terminal_Control

Terminal controller for AirSim drone, including free movement (W,A,S,D...)

## Supporting OS
* macOS
* ~~Windows(Testing)~~

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
   Pressing R,T at once resets the drone
   ~~~

