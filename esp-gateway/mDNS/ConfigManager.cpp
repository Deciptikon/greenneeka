#include "ConfigManager.h"

void ConfigManager::begin() {
  _preferences.begin("greenneeka", false);
}

void ConfigManager::end() {
  _preferences.end();
}

// WiFi credentials
bool ConfigManager::saveWiFiCredentials(const String &ssid, const String &password) {
  begin();
  _preferences.putString("wifi_ssid", ssid);
  _preferences.putString("wifi_pass", password);
  end();
  return true;
}

bool ConfigManager::loadWiFiCredentials(String &ssid, String &password) {
  begin();
  ssid = _preferences.getString("wifi_ssid", "");
  password = _preferences.getString("wifi_pass", "");
  end();
  return ssid != "";
}

void ConfigManager::clearWiFiCredentials() {
  begin();
  _preferences.remove("wifi_ssid");
  _preferences.remove("wifi_pass");
  end();
}

bool ConfigManager::hasWiFiCredentials() {
  String ssid, password;
  return loadWiFiCredentials(ssid, password);
}

// Generic data methods
void ConfigManager::putString(const char* key, const String &value) {
  begin();
  _preferences.putString(key, value);
  end();
}

String ConfigManager::getString(const char* key, const String &defaultValue) {
  begin();
  String value = _preferences.getString(key, defaultValue);
  end();
  return value;
}

void ConfigManager::putInt(const char* key, int value) {
  begin();
  _preferences.putInt(key, value);
  end();
}

int ConfigManager::getInt(const char* key, int defaultValue) {
  begin();
  int value = _preferences.getInt(key, defaultValue);
  end();
  return value;
}

void ConfigManager::putBool(const char* key, bool value) {
  begin();
  _preferences.putBool(key, value);
  end();
}

bool ConfigManager::getBool(const char* key, bool defaultValue) {
  begin();
  bool value = _preferences.getBool(key, defaultValue);
  end();
  return value;
}
