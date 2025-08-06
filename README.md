# LCC_CROSSOVER

This component is part of a suite of components which can be used as part of a program which implements an OpenLCB/LCC node. It has been developed using PlatformIO and has been tested on an Arduino Nano ESP32.

The file crossover.h contains two classes. Class Crossover represents one crosoover and class Position_Crossover represents one position for the crossover.

This component has one dependency;-
- LCC_NODE_COMPONENT_BASE: https://github.com/JohnCallingham/LCC_NODE_COMPONENT_BASE.git

This is automatically downloaded when using PlatformIO.


## PlatformIO installation

It can be installed in PlatformIO by adding the following line to platform.ini;-

lib_deps = https://github.com/JohnCallingham/LCC_CROSSOVER.git

## Arduino IDE installation

It can be installed into the Arduino IDE by downloading the .zip file and adding to the Arduino library using Sketch -> Include Library -> Add .Zip Library
