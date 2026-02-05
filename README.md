# SIM7600 Livestreaming

A real-time camera livestreaming project with SD card snapshot saving capability. Stream camera feed via web browser and capture photos directly to an SD card through a web interface.

## Overview

This ESP32/Arduino-based project enables live video streaming from an ArduCam camera over WiFi (or LTE/4G via SIM7600). The integrated web server provides a clean interface to:
- **View live MJPEG stream** from the connected camera in real-time
- **Capture and save snapshots** to an SD card card with a single button click
- Manage streaming without external dependencies

Perfect for IoT monitoring, time-lapse photography, and remote surveillance applications.

## Key Features

✅ **Live MJPEG Streaming** - Real-time camera feed accessible via web browser
✅ **One-Click Photo Capture** - Save high-quality JPEG snapshots directly to SD card
✅ **Responsive Web Interface** - Mobile-friendly HTML/CSS interface with live status updates
✅ **Dual SPI Bus Support** - Independent SPI channels for camera and SD card operations
✅ **Non-blocking Stream** - Stream continues even during photo capture operations
✅ **Timestamp Filenames** - Automatically named snapshots using millisecond precision
✅ **WiFi/LTE Ready** - WiFi AP mode with SIM7600 LTE support for extended connectivity

## Hardware Requirements

- **MCU**: ESP32 or Arduino Mega with SPI support
- **Camera**: ArduCam Mega (320x320 JPEG capable)
- **Storage**: Micro SD Card (FAT32 formatted)
- **Communication**: 
  - WiFi (ESP32 built-in or external module)
  - Optional: SIM7600 LTE/4G module for remote connectivity
- **Power**: Stable 5V/3.3V supply (>500mA recommended)
- **Peripherals**:
  - SPI Header for camera
  - SPI Header for SD card module
  - USB cable for programming

## Pin Configuration

```
ESP32 Pin Configuration:
- CAM_CS (Camera Chip Select):  GPIO 5
- SD_CS (SD Card Chip Select):  GPIO 13 (or 15 if 13 fails)
- SPI MOSI:                     GPIO 23
- SPI MISO:                     GPIO 19
- SPI CLK:                      GPIO 18
```

## Project Files

- **livestreamwithsave.ino** - Main Arduino sketch containing:
  - Camera initialization and frame capture
  - MJPEG streaming server on port 80
  - SD card write functions with error handling
  - Responsive web interface with one-click capture button

## Installation & Setup

### 1. Hardware Assembly

- Connect ArduCam to SPI (MOSI, MISO, CLK, CS-5)
- Connect SD Card module to SPI (MOSI, MISO, CLK, CS-13)
- Connect 5V/GND power to both modules
- Ensure antenna is connected to SIM7600 (if using LTE)

### 2. Software Setup

```bash
# Clone the repository
git clone https://github.com/AerioJobin/SIM7600-livestreaming.git
```

### 3. Arduino Configuration

1. Open `livestreamwithsave.ino` in Arduino IDE
2. Install required libraries:
   - `WiFi.h` (ESP32 built-in)
   - `WebServer.h` (ESP32 built-in)
   - `Arducam_Mega.h` (from Arducam)
   - `SD.h` (ESP32 built-in)
   - `SPI.h` (ESP32 built-in)

3. Update WiFi credentials (line 15-16):
   ```cpp
   const char* ssid = "YourSSID";
   const char* password = "YourPassword";
   ```

4. Verify pin configuration matches your setup (CAM_CS, SD_CS)
5. Select ESP32 Dev Board from Tools > Board
6. Upload sketch to your board

### 4. First Run

1. Connect to the WiFi network (SSID/Password from sketch)
2. Monitor Serial output (115200 baud) for the IP address
3. Open browser and navigate to: `http://<IP>/`
4. You should see:
   - Live camera stream at 320x320 resolution
   - "📸 SNAP TO SD" button below the stream

## Usage

### Accessing the Livestream

1. **Connect to WiFi**: Network name and password are defined in the sketch
2. **Open Browser**: Navigate to `http://<ESP32-IP>/`
3. **View Stream**: Live MJPEG stream displays in real-time
4. **Save Photo**: Click "📸 SNAP TO SD" button to capture and save

### Capturing Photos

- Click the "📸 SNAP TO SD" button while viewing the stream
- Status message appears: "Saving..."
- Message updates with filename: "Saved: /snap_1234567890.jpg"
- Image is automatically saved to SD card root directory
- Photos are JPEG format, 320x320 pixels
- Filenames include millisecond timestamps for unique identification

### Web Interface Details

The embedded web interface provides:
- **Full-width camera stream** (responsive, up to 600px max-width)
- **One-click capture button** with visual feedback
- **Real-time status updates** (saves/errors)
- **Dark theme** for comfortable viewing
- **Mobile-friendly** viewport scaling
- **No external dependencies** - pure HTML/CSS/JavaScript

## Configuration Options

Editable parameters in the sketch:

```cpp
// Camera resolution (options: 320x320, 640x480, etc.)
CAM_IMAGE_MODE_320X320

// Image format (currently JPEG)
CAM_IMAGE_PIX_FMT_JPG

// WiFi SSID and Password
const char* ssid = "Aerio4048";
const char* password = "aerio12345678";

// SPI Chip Select Pins
const int CAM_CS = 5;
const int SD_CS = 13;
```

## Web API Endpoints

```
GET  /              → Web interface (HTML/CSS/JS)
GET  /stream        → MJPEG stream (multipart/x-mixed-replace)
GET  /save          → Capture and save photo to SD card
```

## Troubleshooting

### Livestream Not Working

- **Check Serial Output**: Look for "Stream Ready: http://" message
- **Verify WiFi Connection**: Ensure device connects to AP
- **Camera Initialization**: Check for "Camera Init Failed!" in serial
- **SPI Conflicts**: Ensure both CAM_CS and SD_CS pins are correct

### Photos Not Saving to SD

- **SD Card Not Detected**: Check serial for "SD Init Failed!" message
- **Card Formatting**: Ensure SD card is FAT32 formatted
- **Write Permissions**: Verify SD card is not write-protected
- **File System Error**: Try reformatting SD card and re-inserting
- **Pin Configuration**: Double-check SD_CS pin matches your setup
- **Power Supply**: Ensure stable power to SD card module

### Poor Stream Quality

- **Network Congestion**: Move closer to WiFi router
- **Camera Focus**: Ensure proper lighting and camera focus
- **Frame Rate**: Adjust delay(10) in handleStream() to reduce CPU load
- **Memory**: Restart device if performance degrades

### Camera Not Capturing

- **Power Supply**: Ensure stable 5V for camera module
- **Connection Check**: Verify all SPI connections are secure
- **Driver Installation**: Reinstall Arducam library from GitHub
- **Serial Debug**: Monitor serial output for error codes

## Performance Specifications

- **Stream Resolution**: 320x320 JPEG
- **Frame Size**: ~15-20 KB per frame
- **Frame Rate**: ~10 FPS at 320x320 resolution
- **Capture Time**: ~100-200ms per photo
- **Memory Usage**: ~50KB frame buffer + stack
- **SD Card Speed**: FAT32 optimized for ESP32 writes

## SIM7600 LTE Integration (Optional)

For remote connectivity beyond WiFi range:

1. Connect SIM7600 module to UART (RX/TX pins)
2. Initialize SIM7600 in setup() with AT commands
3. Configure APN for your carrier
4. Route server traffic through LTE tunnel
5. Access stream from anywhere with data connection

*Note: LTE functionality requires additional code modifications*

## Future Enhancements

- [ ] Multiple resolution options in web UI
- [ ] Video recording to SD card
- [ ] FTP server for remote file access
- [ ] Motion detection with automatic saving
- [ ] Web dashboard with capture history
- [ ] Email notifications
- [ ] Temperature/humidity sensor integration

## License

MIT License - Free for personal and commercial use

## Author

**AerioJobin** - Electronics & IoT Enthusiast

## Support & Contributions

- **Issues**: Report bugs and request features on GitHub Issues
- **Discussions**: Join discussions for tips and troubleshooting
- **Pull Requests**: Contributions welcome! Please follow standard practices
- **Documentation**: Help improve docs and examples

## Acknowledgments

- ArduCam for excellent camera modules and libraries
- ESP32 community for WebServer implementation
- GitHub community for feedback and improvements

---

**Happy Streaming! 📷✨**
