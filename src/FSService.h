#ifndef FSSERVICE_H
#define FSSERVICE_H

#include <Logger.h> // https://github.com/hunsalz/log4arduino
#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

using log4arduino::LOG;

namespace esp8266util {

  class FSService : public Service {

    public:

      FSService();

      ~FSService();

      bool available();

      bool end();

      FS& getFileSystem();

      JsonObject& getStorageDetails();

      JsonArray& getFileListing();

      static String formatBytes(size_t bytes);

    private:

      bool begin(); // called implicitly

      bool _available = false;
  };

  extern FSService FILESYSTEM;
}

#endif // FSSERVICE_H