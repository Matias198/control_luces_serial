#pragma once

#include <Arduino.h>

#include "lighting_controller.h"

class SerialProtocol {
 public:
  explicit SerialProtocol(LightingController& controller)
      : controller_(controller) {}
  void begin(Stream& serial);
  void update();
  void sendState();

 private:
  Stream* serial_ = nullptr;
  LightingController& controller_;
  char buffer_[96] = {};
  size_t length_ = 0;
  bool parseBoolean(const char* value, bool& result);
  void processLine(char* line);
  void sendAck();
  void sendError(const __FlashStringHelper* code);
};
