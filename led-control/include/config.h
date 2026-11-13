#pragma once

#include <Arduino.h>

// GPIO definidos para la maqueta. Ver docs/CONEXION_ELECTRICA.md.
constexpr uint8_t PIN_BRAKE_POSITION = 28;
constexpr uint8_t PIN_BRAKE_LAMP = 29;
constexpr uint8_t PIN_LEFT_TURN = 22;
constexpr uint8_t PIN_RIGHT_TURN = 23;
constexpr uint8_t PIN_HIGH_BEAM = 24;
constexpr uint8_t PIN_LOW_BEAM = 25;

// El módulo de altas/bajas es activo en HIGH: HIGH enciende y LOW apaga.
constexpr uint8_t HIGH_BEAM_ON_LEVEL = HIGH;
constexpr uint8_t HIGH_BEAM_OFF_LEVEL = LOW;
constexpr uint8_t LOW_BEAM_ON_LEVEL = HIGH;
constexpr uint8_t LOW_BEAM_OFF_LEVEL = LOW;

constexpr unsigned long SERIAL_BAUD = 115200UL;
constexpr unsigned long BLINK_HALF_PERIOD_MS = 500UL;
constexpr size_t SERIAL_LINE_MAX = 96;