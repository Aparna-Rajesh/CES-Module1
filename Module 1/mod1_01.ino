#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#define TFT_BL   32            // LED back-light control pin

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

void setup() {
  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(1);

  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 5000, 8); // 0-15, 5000, 8
  ledcAttachPin(TFT_BL, 0); // TFT_BL, 0 - 15
  ledcWrite(0, 100); // 0-15, 0-255 (with 8 bit resolution); 0=totally dark;255=totally shiny
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long functionTime = 14400; // 60% of 24 seconds in milliseconds
  unsigned long sleepTime = 4800; // 20% of 24 seconds in milliseconds

  uint16_t textColor = random(0xFFFF); // Random 16-bit color
  uint16_t bgColor = random(0xFFFF); // Random 16-bit color

  unsigned long startTime = millis();

  while (millis() - startTime < functionTime) {
    // Display "It'll be okay" messages
    tft.fillScreen(bgColor);
    tft.setTextColor(textColor, bgColor);

    tft.drawString("It'll be okay.", 70, 44, 2);
    tft.drawString("It'll be okay.", 70, 64, 2);
    tft.drawString("It'll be okay.", 70, 84, 2);
    tft.drawString("It'll be okay.", 70, 104, 2);
    tft.drawString("It'll be okay.", 70, 124, 2);
    tft.drawString("It'll be okay.", 70, 144, 2);
    tft.drawString("It'll be okay.", 70, 164, 2);
    tft.drawString("It'll be okay.", 70, 184, 2);
    tft.drawString("It'll be okay.", 70, 204, 2);
    tft.drawString("It'll be okay.", 70, 224, 2);
    tft.drawString("It'll be okay.", 70, 244, 2);
    tft.drawString("It'll be okay.", 70, 264, 2);
    tft.drawString("It'll be okay.", 70, 284, 2);

    // Wait for a random period of time
    unsigned long displayTime = random(500, 2000); // Time between displaying "It'll be okay" messages
    delay(displayTime);

    // Check if it's time to display the anxiety-ridden message
    if (millis() - startTime < functionTime - 300) { // Ensure there's enough time left for the anxiety message
      // Display anxiety message
      tft.fillScreen(TFT_WHITE);
      tft.setTextColor(TFT_BLACK, TFT_WHITE);

      // Array of variations of the phrase
      String phrases[] = {
        "I'm running",
        "Time is",
        "Will I ever",
        "I'm running",
        "Tick-tock",
        "24 hrs is",
        "Is it too"
      };
      String seconds[] = {
        "out of time.",
        "slipping away",
        "accomplish enough?",
        "out of time.",
        "tick-tock!",
        "never enough",
        "late for me?"
      };

      // Select a random  phrase
      uint8_t phraseIndex = random(0, 7);

      // Display the randomly selected phrase with the randomly selected font
      tft.drawString(phrases[phraseIndex], 75, 174, 1);
      tft.drawString(seconds[phraseIndex], 75, 184, 1);

      delay(300); // Display the anxiety message for 0.5 seconds
    }
  }

  // Enter deep sleep for the rest of the loop
  esp_sleep_enable_timer_wakeup(sleepTime * 1000);
  esp_deep_sleep_start();
}
