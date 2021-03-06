#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <FS.h>           // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include "Logging.hpp"

namespace espUtils {

class ESP8266FS {
 
 public:
  
  bool begin() {

    bool available = SPIFFS.begin();
    if (available) {
      VERBOSE_FP(F("File system mounted."));
    } else {
      WARNING_FP(F("Mounting file system failed."));
    }

    return available;
  }

  void end() {
    
    SPIFFS.end();
    VERBOSE_FP(F("File system unmounted."));
  }

  size_t serializeInfo(String& output) {

    const size_t CAPACITY = JSON_OBJECT_SIZE(6) + 80;
    StaticJsonDocument<CAPACITY> doc;
    JsonObject object = doc.to<JsonObject>();
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    object[F("totalBytes")] = fs_info.totalBytes;
    object[F("usedBytes")] = fs_info.usedBytes;
    object[F("blockSize")] = fs_info.blockSize;
    object[F("pageSize")] = fs_info.pageSize;
    object[F("maxOpenFiles")] = fs_info.maxOpenFiles;
    object[F("maxPathLength")] = fs_info.maxPathLength;
    serializeJson(object, output);
    return measureJson(object);
  }

  size_t serializeListing(String& output) {
    
    DynamicJsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String name = dir.fileName();
      String size = formatBytes(dir.fileSize());
      JsonObject entry = array.createNestedObject();
      entry[F("name")] = name;
      entry[F("size")] = size;
      VERBOSE_FP(F("Found file: name=%s, size=%s"), name.c_str(), size.c_str());
    }
    serializeJson(array, output);
    return measureJson(array);
  }

  String formatBytes(size_t bytes) {
    
    if (bytes < 1024) {
      return String(bytes) + "B";
    } else if (bytes < (1024 * 1024)) {
      return String(bytes / 1024.0) + "KB";
    } else if (bytes < (1024 * 1024 * 1024)) {
      return String(bytes / 1024.0 / 1024.0) + "MB";
    } else {
      return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
    }
  }
};
}  // namespace espUtils