#ifndef DHTSERVICE_H
#define DHTSERVICE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <DHT.h> // https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h> // https://github.com/adafruit/DHT-sensor-library

#include "Configurable.h"

namespace esp8266util {

  // TODO Service
  class DHTService : public Configurable {

    public:

      struct config_t {
        uint8_t pin;
       /**
        Valid types: 
        DHT11 = 11
        DHT21 = 21
        DHT22 = 22 
        */
        uint8_t type;
      };

      bool begin(config_t config);

      bool begin(JsonObject& json);

      config_t getConfig();

      JsonObject& getConfigAsJson();

      DHT_Unified& getDHT();

      bool update();

      float getTemperature();

      float getHumidity();

      JsonArray& getDetails();

    private:

      DHT_Unified* _dht = NULL;
      config_t _config;

      float _temperature = NAN;
      float _humidity = NAN;
  };
}

#endif // DHTSERVICE_H
