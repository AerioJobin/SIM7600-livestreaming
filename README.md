# SIM7600 Livestreaming

A project for live streaming using the SIM7600 module with Arduino/ESP32 microcontrollers.

## Overview

This repository contains code and resources for implementing live streaming capabilities using the SIM7600 LTE/4G communication module. The project is designed for IoT applications requiring reliable data transmission and streaming capabilities.

## Files

- **livestreamwithsave.ino** - Arduino/ESP32 sketch for SIM7600 livestreaming with local storage support

## Hardware Requirements

- SIM7600 Module (LTE/4G)
- Arduino or ESP32 Microcontroller
- Appropriate development board
- USB cable for programming
- Active SIM card with data plan

## Features

- Live streaming over LTE/4G network
- Local data storage capability
- Easy-to-use Arduino sketch
- Compatible with both Arduino and ESP32 platforms

## Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/AerioJobin/SIM7600-livestreaming.git
   ```

2. Open the `.ino` file in the Arduino IDE

3. Configure your SIM7600 module settings and network credentials in the sketch

4. Upload the sketch to your microcontroller

## Configuration

Update the following parameters in the code:
- SIM7600 UART pins
- APN (Access Point Name) for your carrier
- Server address for streaming
- Storage settings if using local save functionality

## Usage

After uploading the sketch to your microcontroller:
1. Power on the SIM7600 module
2. Wait for network connection
3. Begin livestreaming according to your configuration
4. Monitor serial output for debugging information

## Troubleshooting

- Ensure proper power supply to the SIM7600 module (typically 3.3V)
- Verify SIM card is active and has data plan
- Check UART connection between microcontroller and SIM7600
- Monitor serial console for error messages
- Ensure antenna is properly connected to SIM7600 module

## License

MIT License

## Author

AerioJobin

## Support

For issues, questions, or contributions, please open an issue on GitHub.
