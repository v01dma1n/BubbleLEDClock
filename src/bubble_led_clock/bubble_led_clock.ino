#include "blc_app.h"

#include <ESP32NTPClock.h>

SemaphoreHandle_t serialMutex = NULL;

// The single global instance of the application
BubbleLedClockApp& app = BubbleLedClockApp::getInstance();

void setup() {
  Serial.begin(115200);
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime < 2000)) {
    ; // Wait for serial port to connect, with a 2-second timeout 
  }
  // delay(200);
  Serial.println("\n>>> Starting...");

  // Create the mutex before any other tasks might use it
  serialMutex = xSemaphoreCreateMutex();

  app.setup();

  Serial.println("\n>>> ... setup complete");
}

void loop() {
  app.loop();
}