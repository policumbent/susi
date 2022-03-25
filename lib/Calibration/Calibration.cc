#include "Calibration.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiAP.h>

const char *ssid = "Prova";
const char *password = "";

Calibration::Calibration(GearVec *s1, GearVec *s2) {
  servo1 = std::unique_ptr<GearVec>(s1);
  servo2 = std::unique_ptr<GearVec>(s2);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void Calibration::init() {
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "API Not Found");
  });

  /** REST API **/

  // GET /api/info
  server.on("/api/info", HTTP_GET, [](AsyncWebServerRequest *request) {
    auto response = new AsyncJsonResponse();
    auto json = response->getRoot();

    json["name"] = "TaurusX";
    json["servo"] = SERVO;
    json["gear"] = MAX_GEAR;
    json["last_calibration"] = nullptr;

    response->setLength();
    response->setCode(200);

    request->send(response);
  });

  // GET /api/gear/configuration
  server.on("/api/gear/configuration", HTTP_GET,
            [this](AsyncWebServerRequest *request) {
              auto response = new AsyncJsonResponse(false, 2048);
              auto json = response->getRoot();

              json["servo1"] = *this->servo1;
              json["servo2"] = *this->servo2;

              response->setLength();
              response->setCode(200);

              request->send(response);
            });

  // POST /api/gear/configuration (as server handle due json body)
  server.addHandler(new AsyncCallbackJsonWebHandler(
      "/api/gear/configuration",
      [this](AsyncWebServerRequest *request, JsonVariant &body) {
        // check invalid request
        if (request->method() != HTTP_POST || !body.containsKey("servo1") ||
            !body.containsKey("servo2") || !body["servo1"].is<GearVec>() ||
            !body["servo2"].is<GearVec>()) {
          request->send(403);
          return;
        }

        try {
          *this->servo1 = body["servo1"].as<GearVec>();
          *this->servo2 = body["servo2"].as<GearVec>();

          Memory::save_config(*this->servo1, *this->servo2);

          request->send(200);
        } catch (...) {
          request->send(502);
        }
      },
      2048));
  //  server.on("/api/gear/configuration", HTTP_POST,[](AsyncWebServerRequest
  //  *request){ request->send(202, "text", "ciao");});

  // GET /api/gear?id=<int:gear_id>
  server.on("/api/gear", HTTP_GET, [this](AsyncWebServerRequest *request) {
    // check invalid request
    if (request->params() != 1 || !request->hasParam("id") ||
        request->getParam("id")->value().toInt() < MIN_GEAR ||
        request->getParam("id")->value().toInt() > MAX_GEAR) {
      request->send(422);
      return;
    }

    auto id = request->getParam("id")->value().toInt();
    auto response = new AsyncJsonResponse(false, 2048);
    auto json = response->getRoot();

    json["servo1"] = (*this->servo1)[id - 1];
    json["servo2"] = (*this->servo2)[id - 1];

    response->setLength();
    response->setCode(200);
    request->send(response);
  });

  // PUT /api/gear?id=<int:gear_id>
  server.addHandler(new AsyncCallbackJsonWebHandler(
      "/api/gear",
      [this](AsyncWebServerRequest *request, JsonVariant &body) {
        // check invalid request
        if (request->method() != HTTP_PUT || request->params() != 1 ||
            !request->hasParam("id") ||
            request->getParam("id")->value().toInt() < MIN_GEAR ||
            request->getParam("id")->value().toInt() > MAX_GEAR ||
            !body.containsKey("servo1") || !body["servo1"].is<Gear>() ||
            !body.containsKey("servo2") || !body["servo2"].is<Gear>()) {
          request->send(422);
          return;
        }
        auto id = request->getParam("id")->value().toInt();

        try {
          (*this->servo1)[id - 1] = body["servo1"].as<Gear>();
          (*this->servo2)[id - 1] = body["servo2"].as<Gear>();

          Memory::save_config(*this->servo1, *this->servo2);
          request->send(200);
        } catch (...) {
          request->send(502);
        }
      },
      2048));

  /** STATIC FILES **/

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
}

bool Calibration::isActive() const { return _isActive; }

void Calibration::begin() {
  if (!_isActive) {
    WiFiClass::mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    SPIFFS.begin();

    this->init();
    server.begin();

    _isActive = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void Calibration::end() {
  if (_isActive) {
    server.end();
    server.reset();
    SPIFFS.end();
    WiFi.softAPdisconnect(true);

    _isActive = false;
    digitalWrite(LED_BUILTIN, LOW);
  }
}
