#pragma once

#include "ArduinoJson.h"
#include "AsyncJson.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

// littlefs lib
#include <LITTLEFS.h>
#define SPIFFS LITTLEFS

#include "Gear.h"
#include "Memory.h"

class Calibration {
  AsyncWebServer server = AsyncWebServer(80);

  std::unique_ptr<GearVec> servo1 = nullptr;
  std::unique_ptr<GearVec> servo2 = nullptr;

  bool _isActive = false;
  void init();

public:
  Calibration(GearVec *servo1, GearVec *servo2);

  bool isActive() const;
  void begin();
  void end();
};

/**
 * Serialization/Deserialization rules
 */
namespace ARDUINOJSON_NAMESPACE {
template <> struct Converter<Position> {
  static void toJson(const Position &src, JsonVariant dst) {
    dst["up"] = src.up;
    dst["down"] = src.down;
  }

  static Position fromJson(JsonVariantConst src) {
    return Position{src["up"], src["down"]};
  }

  static bool checkJson(JsonVariantConst src) {
    return src["up"].is<unsigned int>() && src["down"].is<unsigned int>();
  }
};

template <> struct Converter<Gear> {
  static void toJson(const Gear &src, JsonVariant dst) {
    dst["id"] = src.id;
    dst["position"] = src.pos;
  }

  static Gear fromJson(JsonVariantConst src) {
    return Gear{src["id"], src["position"]};
  }

  static bool checkJson(JsonVariantConst src) {
    return src["position"].is<Position>() && src["id"].is<uint8_t>() &&
           src["id"].as<uint8_t>() >= MIN_GEAR &&
           src["id"].as<uint8_t>() <= MAX_GEAR;
  }
};

template <> struct Converter<GearVec> {
  static void toJson(const GearVec &src, JsonVariant dst) {
    for (auto i = 0; i < src.size(); i++) {
      dst[i] = src[i];
    }
  }

  static GearVec fromJson(JsonVariantConst src) {
    auto vec = GearVec{MAX_GEAR};
    for (auto i = 0; i < vec.size(); i++) {
      vec[i] = Gear{src[i]};
    }
    return vec;
  }

  static bool checkJson(JsonVariantConst src) {
    if (src.size() != MAX_GEAR) {
      return false;
    }

    for (auto i = 0; i < src.size(); i++) {
      if (!src[i].is<Gear>() || src[i].as<Gear>().id != i + 1) {
        return false;
      }
    }
    return true;
  }
};
} // namespace ARDUINOJSON_NAMESPACE
