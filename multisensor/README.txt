                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


https://www.thingiverse.com/thing:2239142
NodeMCU Multisensor - temp, humidity, light, motion, RGB Led by bruhautomation is licensed under the Creative Commons - Attribution - Non-Commercial license.
http://creativecommons.org/licenses/by-nc/3.0/

# Summary

UPDATE 10 JUN 2017 - I added a second version of my enclosure that moves the DHT22 sensor outside the case, removes the standoff for the LDR sensor, and adds a pocket for the LED. This should reduce most of the high temperatures being reported by the board and the removal of the standoff allows you to use either the TEMT600 sensor or photoresistor module. A dab of hot glue can help hold everything in place until you can snap the case together. 

Video Tutorial - https://youtu.be/jpjfVc-9IrQ

This enclosure is for DIY Multisensor that can be built using a NodeMCU 1.0 ESP8266 development board. It works great with [Home Assistant](https://home-assistant.io/) - my favorite open-source, home automation platform. 

**no soldering** and **no breadboards** required! 

The code for the NodeMCU development board was written in the ArduinoIDE and is [availible here](https://github.com/bruhautomation/ESP-MQTT-JSON-Multisensor). 

A few people have reported that the floating PIR sensor issue is back. If it happens, wrap the PIR module in electrical tape, then aluminum foil, and then electrical tape again. Using the tape will make sure you don't short anything out with the foil on either the module or other components in the system. 

### Supported Features Include
- **DHT22** temperature sensor
- **DHT 22** humdidity sensor
- **AM312** PIR motion sensor 
- **photoresistor** or **TEMT600** light sensor
- **RGB led** with support for color, flash, fade, and transition
- **Over-the-Air (OTA)** upload from the ArduinoIDE


### Parts List

**Amazon Prime (fast shipping)**
- [NodeMCU 1.0](http://geni.us/cpmi)
- [DHT22 Module](http://geni.us/vAJWMXo)
- [LDR Photoresistor Module](http://geni.us/O0AO0)
    OR
- [TEMT6000](http://geni.us/aRYe)
- [Power Supply](http://geni.us/ZZ1r)
- [Common Cathode RGB Led](http://geni.us/nFcB)
- [Header Wires](http://geni.us/pvFNG)
- [Mini PIR Sensor](http://geni.us/dbGQ)

**Aliexpress (long shipping = cheap prices)**
- [NodeMCU 1.0](http://geni.us/EfYA)
- [DHT22 Module](http://geni.us/35Np8H)
- [LDR Photoresistor Module](http://geni.us/O5iv)
    OR
- [TEMT6000](http://geni.us/xAuLoy)
- [Power Supply](http://geni.us/NSYjvb)
- [Common Cathode RGB Led](http://geni.us/OfHbhZb)
- [Header Wires](http://geni.us/Iv6p9)
- [Mini PIR Sensor](http://geni.us/WBKyxhx)



### Wiring Diagram
![alt text](https://github.com/bruhautomation/ESP-MQTT-JSON-Multisensor/blob/master/wiring_diagram.png?raw=true "Wiring Diagram")

# Print Settings

Printer Brand: Prusa
Printer: Prusa Clone
Rafts: No
Supports: No
Resolution: 0.2 mm
Infill: 40%