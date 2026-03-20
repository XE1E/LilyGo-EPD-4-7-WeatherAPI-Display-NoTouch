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
  {"YourWiFi_SSID", "YourWiFi_Password"},
  // Add more networks below:
  // {"SecondNetwork", "password2"},
  // {"ThirdNetwork", "password3"},
};

const int wifiNetworkCount = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);

// Get API key by signing up for a free developer account at https://openweathermap.org/
String apikey       = "YOUR_API_KEY_HERE";                     // See: https://openweathermap.org/
const char server[] = "api.openweathermap.org";

//Set your location
String City             = "Your City";                         // Your home city name
String Latitude         = "0.0000";                            // Latitude of your location in decimal degrees
String Longitude        = "0.0000";                            // Longitude of your location in decimal degrees

String Language         = "EN";                                // NOTE: Only the weather description is translated by OWM
                                                               // Examples: Arabic (AR) Czech (CZ) English (EN) Spanish (ES) French (FR)
                                                               // German (DE) Italian (IT) Portuguese (PT) Russian (RU) Japanese (JA)
String Hemisphere       = "north";                             // or "south"
String Units            = "M";                                 // Use 'M' for Metric or I for Imperial

const char* Timezone    = "UTC0";                              // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* ntpServer   = "pool.ntp.org";                      // Choose a time server close to you, or use pool.ntp.org
int  gmtOffset_sec      = 0;                                   // UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically -18000
int  daylightOffset_sec = 0;                                   // In the UK DST is +1hr or 3600-secs, other countries may use different values
