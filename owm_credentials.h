const bool DebugDisplayUpdate = false;

// ============================================================================
// NOTE: These are DEFAULT/FALLBACK values. You can configure all settings
// via the web interface when:
//   1. No configured WiFi network is found (automatic), OR
//   2. FORCE_AP_MODE is set to true in the main .ino file
// The device will create a WiFi hotspot "WeatherStation-Setup" for configuration.
// Web-configured values will override these defaults.
// ============================================================================

// Set to your WiFi credentials (multiple SSIDs supported)
// The device will scan and connect to the one with the highest signal strength
struct WiFiCredentials {
  const char* ssid;
  const char* password;
};

const WiFiCredentials wifiNetworks[] = {
  {"Your_WiFi_SSID", "Your_WiFi_Password"},
  // Add more networks below:
  // {"Network2", "password2"},
  // {"Network3", "password3"},
};

const int wifiNetworkCount = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);

// WeatherAPI.com - Get free API key at https://www.weatherapi.com/
String apikey       = "YOUR_WEATHERAPI_KEY";
const char server[] = "api.weatherapi.com";

//Set your location
String City             = "Your City";
String Latitude         = "0.0000";
String Longitude        = "0.0000";

String Language         = "es";                                // WeatherAPI language codes: es, en, fr, de, etc.
String Hemisphere       = "north";                             // or "south"
String Units            = "M";                                 // Use 'M' for Metric or I for Imperial

const char* Timezone    = "CST6";
const char* ntpServer   = "time.cloudflare.com";
int  gmtOffset_sec      = -21600;                              // -6 hours for Mexico City
int  daylightOffset_sec = 0;
