#include <WiFi.h>
#include <WiFiUdp.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#define VRX_PIN  33 
#define VRY_PIN  39 
#define TFT_BL   32            // LED back-light control pin

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const char* ssid = "boldbeautiful";
const char* password = "Barnard3A!";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // Port to listen on
char incomingPacket[255];  // buffer for incoming packets

const int analogInPin = 36; // ESP32 Analog Pin (example: GPIO 36)
const int buttonPin = 15;   // GPIO number for button
int sensorValue = 0;        // value read from the analog pin
int buttonState = 0;        // state of the button
int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value

uint16_t textColor = 0x000000; // Random 16-bit color
uint16_t bgColor = 0xFFFF; // Random 16-bit color

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);

  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 5000, 8); // 0-15, 5000, 8
  ledcAttachPin(TFT_BL, 0); // TFT_BL, 0 - 15
  ledcWrite(0, 100); // 0-15, 0-255 (with 8 bit resolution); 0=totally dark;255=totally shiny


  pinMode(buttonPin, INPUT_PULLUP);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start UDP
  Udp.begin(localUdpPort);
  Serial.println("UDP server started");
}

void loop() {
  tft.fillScreen(bgColor);
  tft.setTextColor(textColor, bgColor);

  tft.drawString("Turn the dial to change the", 30, 10, 2);
  tft.drawString("  brightness of the lava lamp.", 30, 30, 2);
  tft.drawString(" ", 30, 40, 2);
  tft.drawString("Click the button to rotate", 30, 55, 2);
  tft.drawString("  through color schemes.", 30, 75, 2);
  tft.drawString(" ", 30, 85, 2);
  tft.drawString("Push the joystick L/R", 30, 100, 2);
  tft.drawString("  to adjust the speed.", 30, 120, 2);

  // Check if a packet is received
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Process the received packet
    Serial.printf("Received packet of size %d from %s:%d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0; // Null-terminate the string
    }
  }

  // Always read sensor and button state
  sensorValue = analogRead(analogInPin); // Read the analog input
  buttonState = digitalRead(buttonPin);  // Read the button state
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // Prepare the packet to send
  String message = String(sensorValue) + "---";
  message += String(valueX) + "---" + String(valueY) + "---";
  message += (buttonState == LOW) ? "Pressed" : "Unpressed";

  // Send the packet back to the last known remote IP and port
  if (packetSize) {
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(message);
    Udp.endPacket();
    Serial.println("Sent: " + message);
  }

  delay(200); // Ensure this is outside any condition to run always
}
