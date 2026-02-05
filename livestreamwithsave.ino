#include <WiFi.h>
#include <WebServer.h>
#include <Arducam_Mega.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

// --- HARDWARE CONFIGURATION ---
const int CAM_CS = 5;   // Camera Chip Select
const int SD_CS  = 13;  // SD Card Chip Select (Try 15 if 13 fails)

Arducam_Mega myCAM(CAM_CS);
WebServer server(80);

const char* ssid = "Aerio4048";
const char* password = "aerio12345678";

// Flag to control the stream loop
bool isCapturing = false;

// --- SAVE IMAGE TO SD CARD ---
void handleSave() {
  isCapturing = true; // Pause the stream logic
  delay(100);         // Give the SPI bus a moment to clear

  Serial.println("Interrupt: Capturing photo for SD...");
  
  // 1. Take a High-Res Picture for saving (320x320 for speed, higher if you want)
  uint8_t status = myCAM.takePicture(CAM_IMAGE_MODE_320X320, CAM_IMAGE_PIX_FMT_JPG);
  
  if (status == 0) {
    uint32_t len = myCAM.getTotalLength();
    String fileName = "/snap_" + String(millis()) + ".jpg";
    
    // 2. Open SD File
    File file = SD.open(fileName, FILE_WRITE);
    if (file) {
      // 3. Write to SD
      while (len--) {
        file.write(myCAM.readByte());
      }
      file.close();
      Serial.println("Saved: " + fileName);
      server.send(200, "text/plain", "Saved: " + fileName);
    } else {
      Serial.println("SD Error: Could not write file.");
      server.send(500, "text/plain", "SD Write Failed");
    }
  } else {
    server.send(500, "text/plain", "Camera Capture Failed");
  }
  
  isCapturing = false; // Resume stream capability
}

// --- LIVE STREAMING (MJPEG) ---
void handleStream() {
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  server.sendContent(response);

  while (client.connected()) {
    // If a capture was requested, pause the stream momentarily
    if (isCapturing) {
      delay(50);
      continue;
    }

    // Capture Frame for Stream
    uint8_t status = myCAM.takePicture(CAM_IMAGE_MODE_320X320, CAM_IMAGE_PIX_FMT_JPG);
    
    if (status == 0) {
      uint32_t len = myCAM.getTotalLength();
      String header = "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: " + String(len) + "\r\n\r\n";
      server.sendContent(header);
      
      while (len--) {
        client.write(myCAM.readByte());
      }
      server.sendContent("\r\n");
    }
    // Small delay to keep things stable
    delay(10);
  }
}

// --- WEB INTERFACE ---
void handleRoot() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body{background:#111; color:white; font-family:sans-serif; text-align:center;}";
  html += ".btn{background:#e74c3c; color:white; padding:15px 30px; border:none; border-radius:5px; font-size:18px; margin-top:20px;}";
  html += "</style></head><body>";
  html += "<h1>Live Monitor</h1>";
  html += "<img src='/stream' style='width:100%; max-width:600px; border:2px solid #555;'><br>";
  html += "<button class='btn' onclick='capture()'>📸 SNAP TO SD</button>";
  html += "<p id='status'></p>";
  
  // Javascript to handle the button click without reloading the page
  html += "<script>function capture(){";
  html += "document.getElementById('status').innerText = 'Saving...';";
  html += "fetch('/save').then(response => response.text()).then(data => {";
  html += "document.getElementById('status').innerText = data;";
  html += "setTimeout(() => { document.getElementById('status').innerText = ''; }, 3000);";
  html += "});}</script>";
  
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1. Init SPI
  SPI.begin(18, 19, 23);
  pinMode(SD_CS, OUTPUT); digitalWrite(SD_CS, HIGH);
  pinMode(CAM_CS, OUTPUT); digitalWrite(CAM_CS, HIGH);

  // 2. Init SD Card (Must be FAT32)
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Init Failed! (Stream will work, Save will fail)");
  } else {
    Serial.println("SD Card Ready.");
  }

  // 3. Init Camera
  if (myCAM.begin() != 0) {
    Serial.println("Camera Init Failed!");
  }

  // 4. Start Server
  WiFi.softAP(ssid, password);
  Serial.print("Stream Ready: http://"); Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/stream", handleStream);
  server.on("/save", handleSave);
  
  server.begin();
}

void loop() {
  server.handleClient();
}