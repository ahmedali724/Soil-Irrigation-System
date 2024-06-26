# Soil Irrigation System using ATmega32 Microcontroller

## Overview
This project aims to create an efficient and automated soil irrigation system to optimize crop growth through precise soil moisture management. Utilizing the ATmega32 microcontroller, soil moisture sensors, water pump, LCD display, keypad, relay, and power supply, the system accurately monitors soil moisture levels and regulates irrigation accordingly.

## Features
- **Autonomous Mode**: Automatically senses soil moisture levels and activates the water pump when moisture falls below a predefined threshold.
- **User-Defined Interval Mode**: Allows users to set irrigation intervals using the keypad. The system checks soil moisture levels and activates the water pump based on the user-defined schedule.
- **Data Communication**: Uses UART to transmit soil moisture data to a connected laptop, where Python scripts capture and visualize the data.

## Hardware Components
- ATmega32 Microcontroller
- Soil Moisture Sensors
- Water Pump
- LCD Display
- Keypad
- Relay
- Power Supply

## Internal Peripherals of ATmega32
- ADC (Analog-to-Digital Converter): Converts analog sensor data to digital values.
- Timers: Used for interval timing in user-defined mode.
- UART (Universal Asynchronous Receiver-Transmitter): Facilitates serial communication with external devices.

## Software Components
- Microcontroller Firmware: Written in C, handles sensor readings, pump control, user input, and UART communication.
- Python Scripts: Capture data from the microcontroller and visualize soil moisture trends.

## Getting Started

### Prerequisites
- ATmega32 Microcontroller and Programmer
- Soil Moisture Sensors
- Water Pump and Relay Module
- LCD Display (16x2)
- Keypad (4x4)
- Power Supply
- Laptop with Python installed

### Usage
- **Autonomous Mode**: The system will automatically monitor soil moisture levels and activate the pump when needed.
- **User-Defined Interval Mode**: Use the keypad to set the desired irrigation interval. The system will irrigate based on this schedule and soil moisture readings.

## Circuit Diagram
![image](https://github.com/ahmedali724/Soil-Irrigation-System/assets/111287464/f62cb5dc-4794-45cb-b0c1-c650f4ea9258)


