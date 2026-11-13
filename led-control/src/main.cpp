#include <Arduino.h>

#include "config.h"
#include "lighting_controller.h"
#include "serial_protocol.h"

LightingController lighting;
SerialProtocol protocol(lighting);

void setup() {
  lighting.begin();
  Serial.begin(SERIAL_BAUD);
  protocol.begin(Serial);
}

void loop() {
  const unsigned long now = millis();
  protocol.update();
  lighting.update(now);
}
