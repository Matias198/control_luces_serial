#include "serial_protocol.h"

#include <string.h>

#include "config.h"

void SerialProtocol::begin(Stream& serial) {
  serial_ = &serial;
  sendState();
}

void SerialProtocol::update() {
  if (serial_ == nullptr) return;
  while (serial_->available() > 0) {
    const char c = static_cast<char>(serial_->read());
    // Aceptar LF, CR o CRLF: los monitores seriales no usan todos el mismo
    // terminador de línea.
    if (c == '\n' || c == '\r') {
      buffer_[length_] = '\0';
      if (length_ > 0) processLine(buffer_);
      length_ = 0;
    } else {
      if (length_ < SERIAL_LINE_MAX - 1) {
        buffer_[length_++] = c;
      } else {
        length_ = 0;
        sendError(F("LINE_TOO_LONG"));
      }
    }
  }
}

bool SerialProtocol::parseBoolean(const char* value, bool& result) {
  if (strcmp(value, "0") == 0) { result = false; return true; }
  if (strcmp(value, "1") == 0) { result = true; return true; }
  return false;
}

void SerialProtocol::processLine(char* line) {
  if (strcmp(line, "GET") == 0) { sendState(); return; }
  if (strncmp(line, "SET;", 4) != 0) {
    sendError(F("INVALID_COMMAND"));
    return;
  }
  bool low = false, high = false, left = false, right = false, hazard = false, brake = false;
  bool seenLow = false, seenHigh = false, seenLeft = false, seenRight = false;
  bool seenHazard = false, seenBrake = false;
  char* token = strtok(line + 4, ";");
  while (token != nullptr) {
    char* equals = strchr(token, '=');
    if (equals == nullptr) { sendError(F("INVALID_FIELD")); return; }
    *equals = '\0';
    bool parsed = false;
    if (strcmp(token, "low") == 0) { parsed = parseBoolean(equals + 1, low); seenLow = parsed; }
    else if (strcmp(token, "high") == 0) { parsed = parseBoolean(equals + 1, high); seenHigh = parsed; }
    else if (strcmp(token, "left") == 0) { parsed = parseBoolean(equals + 1, left); seenLeft = parsed; }
    else if (strcmp(token, "right") == 0) { parsed = parseBoolean(equals + 1, right); seenRight = parsed; }
    else if (strcmp(token, "hazard") == 0) { parsed = parseBoolean(equals + 1, hazard); seenHazard = parsed; }
    else if (strcmp(token, "brake") == 0) { parsed = parseBoolean(equals + 1, brake); seenBrake = parsed; }
    else { sendError(F("UNKNOWN_FIELD")); return; }
    if (!parsed) { sendError(F("INVALID_VALUE")); return; }
    token = strtok(nullptr, ";");
  }
  if (!(seenLow && seenHigh && seenLeft && seenRight && seenHazard && seenBrake)) {
    sendError(F("MISSING_FIELD")); return;
  }
  controller_.setCommand(low, high, left, right, hazard, brake);
  sendAck();
  sendState();
}

void SerialProtocol::sendAck() {
  if (serial_ == nullptr) return;
  const LightingState& s = controller_.state();
  serial_->print(F("ACK;low=")); serial_->print(s.low);
  serial_->print(F(";high=")); serial_->print(s.high);
  serial_->print(F(";left=")); serial_->print(s.left);
  serial_->print(F(";right=")); serial_->print(s.right);
  serial_->print(F(";hazard=")); serial_->print(s.hazard);
  serial_->print(F(";brake=")); serial_->println(s.brake);
}

void SerialProtocol::sendState() {
  if (serial_ == nullptr) return;
  const LightingState& s = controller_.state();
  serial_->print(F("STATE;low=")); serial_->print(s.low);
  serial_->print(F(";high=")); serial_->print(s.high);
  serial_->print(F(";left=")); serial_->print(s.left);
  serial_->print(F(";right=")); serial_->print(s.right);
  serial_->print(F(";hazard=")); serial_->print(s.hazard);
  serial_->print(F(";brake=")); serial_->println(s.brake);
}

void SerialProtocol::sendError(const __FlashStringHelper* code) {
  if (serial_ != nullptr) { serial_->print(F("ERR;code=")); serial_->println(code); }
}
