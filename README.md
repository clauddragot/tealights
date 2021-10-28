# TeaLights
Device that shows the current temperature of the liquid inside of an object and also estimates when is the ideal time to drink it.

The project consist in the implementation of a device (PCB composed of a NTC thermistor, connected to a development board NodeMCU Lua WIFI, both integrated with ESP8266 microcontroller). This device is used for measuring and informing the user about the temperature of liquids in real time through a web application and also a mobile application. The problem stems from the fact that often the liquid in a container is too cold or too hot, and the latter inconvenience can even be harmful. Thus, the experience would be a much more pleasant one with a device that informs us when is the ideal time to serve the drink.

This system work with a PCB that is composed of a temperature sensor (NTC thermistor) connected to a development board (NodeMCU Lua WIFI), both integrated with ESP8266 microcontroller. The novelty of this electronic equipment, compared to those already on the market, is given by the fact that the user is able to interact with the device in real time through a web page / Android application. 

The mentioned development boards have the integrated TCP / IP protocol, but also have 802.11 support: b / g / n, and thus the user will be able to connect to the device via WiFi and will be able to see the updates related to the current liquid temperature, and also the time remaining until it no longer has a temperature considered optimal. 

At the same time, the user will be able to select, through the web page / mobile application, an optimal temperature considered by him and will be notified when the liquid has reached that temperature.
