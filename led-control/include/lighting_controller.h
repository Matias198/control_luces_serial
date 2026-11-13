#pragma once

#include <Arduino.h>

struct LightingState {
  bool low = false;
  bool high = false;
  bool left = false;
  bool right = false;
  bool hazard = false;
  bool brake = false;
};

class LightingController {
 public:
  void begin();
  void update(unsigned long now);
  void setCommand(bool low, bool high, bool left, bool right, bool hazard,
                  bool brake);
  const LightingState& state() const { return state_; }

 private:
  LightingState state_;
  bool blinkOutput_ = false;
  unsigned long lastBlinkAt_ = 0;
  void writeOutputs();
  void writeSteadyOutputs();
  void writeTurnOutputs();
};
