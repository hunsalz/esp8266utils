#include "WiFiAPService.h"

namespace esp8266util {

  WiFiAPService::~WiFiAPService() {
    stop();
  }

  bool WiFiAPService::isSetup() {
    return setupDone;
  }

  bool WiFiAPService::isRunning() {
    return WiFi.status();
  }

  bool WiFiAPService::start() {

    if (isSetup()) {
      WiFi.softAPdisconnect();
      // TODO reflect changes to MDNS
      if (WiFi.softAP(ssid, passphrase, channel, ssid_hidden, max_connection)) {
        Log.notice(F("Soft AP established successful. IP address of AP is: %s" CR), WiFi.softAPIP().toString().c_str());
      } else {
        Log.error(F("Couldn't establish a soft access point." CR));
      }
    } else {
      Log.error("Call setup() first.");
    }

    return isRunning();
  }

  bool WiFiAPService::stop() {

    WiFi.softAPdisconnect();

    return isRunning();
  }

  ESP8266WiFiClass* WiFiAPService::getWiFi() {
    return &WiFi;
  }

  bool WiFiAPService::setup(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection, bool autoConnect, bool persistent) {

    this->ssid = ssid;
    this->passphrase = passphrase;
    this->channel = channel;
    this->ssid_hidden = ssid_hidden;
    this->max_connection = max_connection;
    // general settings
    WiFi.enableAP(true);
    WiFi.setAutoConnect(true);
    WiFi.persistent(false);

    setupDone = true;

    return isSetup();
  }

  JsonObject& WiFiAPService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("softAPgetStationNum")] = getWiFi()->softAPgetStationNum();
    json[F("softAPIP")] = getWiFi()->softAPIP().toString();
    json[F("softAPmacAddress")] = getWiFi()->softAPmacAddress();

    return json;
  }
}
