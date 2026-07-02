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

## Functions

The following functionality is implemented for the crossover;-
- The crossover controls two servos according to the parameters passed into the class Crossover constructor.
- The crossover has two positions, Thrown and Closed.
- Each position has an event. When this event is received both servos will start to move to that position.
- As well as explicitly moving to one of the two positions a toggle event is implements. When this event is received the servos will move according to the following;-
    - if currently at the Thrown position, both servos will move to the Closed position.
    - if currently at the Closed position, both servos will move to the Thrown position.
- When the first servo in the crossover strats moving away from a position a leaving event is sent for that position. When the last servo reaches a position a reached event is sent for that position.
- To cater for the situation where the LCC node is running before the LCC hub starts this software component can be commanded to send the current state of all its events when the hub is connected.
- Responds to a JMRI query with the current state of the requested event.
- A test event can be configured which, when received, causes the crossover to continuously move between the Thrown and Closed positions.

