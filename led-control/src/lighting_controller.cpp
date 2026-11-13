#include "lighting_controller.h"

#include "config.h"

void LightingController::begin() {
  pinMode(PIN_LOW_BEAM, OUTPUT);
  pinMode(PIN_HIGH_BEAM, OUTPUT);
  pinMode(PIN_LEFT_TURN, OUTPUT);
  pinMode(PIN_RIGHT_TURN, OUTPUT);
  pinMode(PIN_BRAKE_POSITION, OUTPUT);
  pinMode(PIN_BRAKE_LAMP, OUTPUT);
  state_ = {};
  blinkOutput_ = false;
  writeOutputs();
}

void LightingController::setCommand(bool low, bool high, bool left, bool right,
                                    bool hazard, bool brake) {
  state_.hazard = hazard;
  if (hazard) {
    state_.left = false;
    state_.right = false;
  } else if (left) {
    state_.left = true;
    state_.right = false;
  } else if (right) {
    state_.left = false;
    state_.right = true;
  } else {
    state_.left = false;
    state_.right = false;
  }
  state_.high = high;
  state_.low = high ? false : low;
  // El freno es independiente de alta/baja: cambiar una luz no modifica la
  // otra función ni apaga PIN_BRAKE_LAMP.
  state_.brake = brake;
  writeOutputs();
}

void LightingController::update(unsigned long now) {
  if (state_.left || state_.right || state_.hazard) {
    if (now - lastBlinkAt_ >= BLINK_HALF_PERIOD_MS) {
      lastBlinkAt_ = now;
      blinkOutput_ = !blinkOutput_;
      // La intermitencia sólo actualiza las salidas de giro/baliza.
      writeTurnOutputs();
    }
  } else if (blinkOutput_) {
    blinkOutput_ = false;
    writeTurnOutputs();
  }
}

void LightingController::writeOutputs() {
  writeSteadyOutputs();
  writeTurnOutputs();
}

void LightingController::writeSteadyOutputs() {
  // Estas salidas no comparten la fase de intermitencia ni tienen exclusión
  // entre sí: alta/baja (mutuamente excluyentes) pueden coexistir con freno.
  digitalWrite(PIN_LOW_BEAM,
               state_.low ? LOW_BEAM_ON_LEVEL : LOW_BEAM_OFF_LEVEL);
  digitalWrite(PIN_HIGH_BEAM,
               state_.high ? HIGH_BEAM_ON_LEVEL : HIGH_BEAM_OFF_LEVEL);
  // Luz de posición de freno: permanece encendida durante toda la operación.
  digitalWrite(PIN_BRAKE_POSITION, HIGH);
  digitalWrite(PIN_BRAKE_LAMP, state_.brake ? HIGH : LOW);
}

void LightingController::writeTurnOutputs() {
  digitalWrite(PIN_LEFT_TURN,
               (state_.hazard || state_.left) && blinkOutput_ ? HIGH : LOW);
  digitalWrite(PIN_RIGHT_TURN,
               (state_.hazard || state_.right) && blinkOutput_ ? HIGH : LOW);
}
