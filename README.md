# Greenhouse_project

Project to collect data (temperature, relative humidity, light) from greenhouse for optimised vegetable growth! The long term goal is to have an autonomous and off-the-grid IoT system with a mobile app for data visualisation and monitoring. 

## Phases

The project is split into different phases:
- ~~Phase 1: Design PCBs and cases for the hardware.~~
- Phase 2: Read data from sensors and show it on a display.
- Phase 3: Integreate wireless data transmission and cloud storage followed by an iOS app for data visualisation.
- Phase 4: Add solar pannels for battery charging.

## Hardware

### Sensors:

- Si7020: Temperature and relative humidity sensor
- OPT3001: Visible light sensor
- *Optional Accelerometer: The greenhouse has a window which opens when the temperature is too high. An accelerometer mounted on the window can tell us if the window is open or shut.*

### Microcontroler:
- PSoC-5LP
- Wifi Module for wireless data transmission.
- LCD Display
