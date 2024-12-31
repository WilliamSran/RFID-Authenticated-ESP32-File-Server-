# ESP32 RFID-Authenticated File Server and Client

This repository demonstrates a system using two ESP32 modules: one as an RFID-authenticated file server and the other as a client. The server hosts files and serves them upon authentication using an RFID card, while the client communicates with the server to send RFID data and display status on an LCD.

## Features

### Server
- Operates as an Access Point (AP) with a web interface.
- Maps RFID UIDs to specific files.
- Serves files securely only when a valid RFID UID is presented.
- Provides a dynamic HTML page to:
  - Request access to files.
  - Display the current status and remaining access time.
- Times out access requests and granted sessions after a set period.

### Client
- Connects to the server as a WiFi station.
- Uses an RFID reader (MFRC522) to read UID cards and sends data to the server.
- Displays the current requested file and countdown timer on an LCD (Adafruit ST7735).

---

## Getting Started

### Prerequisites
- Two ESP32 boards (one for the server, one for the client).
- MFRC522 RFID module.
- Adafruit ST7735 or compatible TFT LCD module.
- LittleFS library for file storage on the server.

### Libraries Required
Install the following libraries in your Arduino IDE:
- `ESPAsyncWebServer`
- `AsyncTCP`
- `LittleFS`
- `Adafruit_GFX`
- `Adafruit_ST7735`
- `MFRC522`

---

## Installation

### Server Setup
1. Upload the `server` code to an ESP32 module.
2. Create the file to be served in the LittleFS partition:
   - Use the Arduino IDE LittleFS plugin to upload files.
   - Example: `SecretFile_1.txt` must exist in the `/data` directory.
3. Power on the server and connect to the AP (e.g., `ESP32-AP` with password `12345678`).

### Client Setup
1. Upload the `client` code to another ESP32 module.
2. Connect the MFRC522 RFID module and Adafruit ST7735 LCD module to the ESP32 as per the pin definitions in the code.
3. Power on the client. It will connect to the server AP automatically.

---

## Usage

### Server
1. Open the web interface by navigating to the server's IP (e.g., `192.168.4.1`) in a browser.
2. Request access to a file by clicking the respective button.
3. Scan the RFID card with the client module. The server will authenticate the UID and grant access if it matches the mapping.
4. Download the file within the granted access time via the web interface.

### Client
1. Scan RFID cards on the MFRC522 reader.
2. Observe the current file and countdown displayed on the LCD.
3. If the RFID UID is valid, the server grants access.

---

## Hardware Connections

### RFID (MFRC522)
| RFID Pin | ESP32 Pin |
|----------|-----------|
| RST      | 4         |
| SS       | 5         |
| MOSI     | 23        |
| MISO     | 19        |
| SCK      | 18        |
| GND      | GND       |
| VCC      | 3.3V      |

### LCD (Adafruit ST7735)
| LCD Pin | ESP32 Pin |
|---------|-----------|
| CS      | 5         |
| DC      | 16        |
| RST     | 17        |
| SCK     | 18        |
| MOSI    | 23        |
| GND     | GND       |
| VCC     | 3.3V      |

---

## File Mapping

In the server code, modify the `fileAccessMapping` array to map UIDs to files. Example:
```cpp
FileAccess fileAccessMapping[] = {
  { "a33e1af5", "SecretFile_1.txt" },
  { "b45f2b34", "Confidential_2.pdf" }
};
```

---

## Demo

1. Start the server and connect to the web interface.
2. Request a file, then scan an RFID card on the client.
3. If authenticated, download the file.

---

## Limitations
- Supports only predefined file-UID mappings.
- Timeout durations are fixed but can be adjusted in the code.
- Requires a stable WiFi connection between the client and server.

---

## Future Improvements
- Add dynamic file-UID mapping via the web interface.
- Support for multiple clients accessing the server simultaneously.
- Enhanced security with encryption for UID and file transfers.

---

## License
This project is open-source under the [MIT License](LICENSE).

---

## Contributions
Contributions are welcome! Feel free to fork this repository and create pull requests for improvements or additional features. 

--- 

Enjoy building your secure RFID-authenticated file server! 🚀
