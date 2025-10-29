#include "UDPBroadcast.h"

const char* ssid = "SSID";
const char* pass = "PASSWORD";

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  delay(1000);

  if (!UDPBroadcaster.begin(ssid, pass, 5000)) {
    Serial.println("Failed to initialize UDP broadcast!");
    while(1) delay(1000);
  }

  UDPBroadcaster.println("Hello from ESP32!");
}

void loop() {
  UDPBroadcaster.update();

  while (Serial2.available()) {
    uint8_t buf[64];
    int n = Serial2.read(buf, sizeof(buf)); // non-blocking
    if (n > 0) {
      UDPBroadcaster.sendRaw(buf, n);
    }
  }

  delay(10);
}
