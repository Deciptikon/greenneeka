#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Preferences.h>

class ConfigManager {
  public:
    void begin();
    void end();
    
    // WiFi credentials
    bool saveWiFiCredentials(const String &ssid, const String &password);
    bool loadWiFiCredentials(String &ssid, String &password);
    void clearWiFiCredentials();
    bool hasWiFiCredentials();
    
    // Generic data
    void putString(const char* key, const String &value);
    String getString(const char* key, const String &defaultValue = "");
    void putInt(const char* key, int value);
    int getInt(const char* key, int defaultValue = 0);
    void putBool(const char* key, bool value);
    bool getBool(const char* key, bool defaultValue = false);

  private:
    Preferences _preferences;
};

#endif
