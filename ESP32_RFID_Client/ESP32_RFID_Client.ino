#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// RFID RC522 pins
#define RST_PIN 4
#define SS_PIN 5

// LCD ST7735 pins
#define TFT_CS 5
#define TFT_RST 17
#define TFT_DC 16

MFRC522 mfrc522(SS_PIN, RST_PIN); // RFID object
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); // LCD object

// Replace with your AP credentials
const char* ssid = "ESP32-AP";
const char* password = "12345678";

const char* serverIP = "192.168.4.1"; // IP of the Access Point ESP32
const int serverPort = 80;

String currentFile = "Idle"; // Current file being requested
int countdownTime = 0;       // Countdown time from the server

void setup() {
  Serial.begin(115200);
  SPI.begin();

  // Initialize RFID
  mfrc522.PCD_Init();
  Serial.println("RFID Initialized");

  // Initialize LCD
  tft.initR(INITR_BLACKTAB); // Black tab initialization
  tft.setRotation(1);        // Adjust rotation as needed
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1.5);
  tft.setCursor(0, 0);
  tft.println("Connecting to WiFi...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  tft.fillScreen(ST77XX_BLACK);
  tft.println("Connected to AP");
}

void loop() {
  // Check for a new card and send UID to the server
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println("RFID UID: " + uid);
    sendUID(uid);
    delay(1000); // Debounce delay
  }

  // Fetch the current file request and countdown from the server
  fetchRequestInfo();
  displayInfo(); // Display the file and countdown on the LCD
  delay(1000);   // Update every second
}

void sendUID(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("http://") + serverIP + "/sendUID";

    http.begin(url);
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST(uid);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error in sending UID: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void fetchRequestInfo() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Fetch file name
    String urlFile = String("http://") + serverIP + "/getRequestedFile";
    http.begin(urlFile);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Fetched Requested File: " + response);
      if (response.isEmpty() || response == "Idle") {
        currentFile = "Idle";
      } else {
        currentFile = response;
      }
    } else {
      Serial.println("Error fetching requested file");
    }
    http.end();

    // Fetch countdown
    String urlCountdown = String("http://") + serverIP + "/getCountdown";
    http.begin(urlCountdown);
    httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      countdownTime = response.toInt();
      Serial.println("Fetched Countdown: " + response);
    } else {
      Serial.println("Error fetching countdown");
      countdownTime = 0;
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    currentFile = "Idle";
    countdownTime = 0;
  }
}


void displayInfo() {
  // Clear the entire screen
  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(0, 0);

  // Show "Idle" status if no file is requested
  if (currentFile == "Idle") {
    tft.println("System Status:");
    tft.setCursor(0, 20);
    tft.println("Idle");
  } else {
    // Display requested file and countdown
    tft.println("Requested File:");
    tft.setCursor(0, 20);
    tft.println(currentFile);

    tft.setCursor(0, 50);
    tft.println("Countdown: " + String(countdownTime) + "s");
  }
}

