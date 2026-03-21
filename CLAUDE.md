# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESP32-based weather display for the LilyGo EPD 4.7" e-paper display - **NoTouch version** (no touch screen navigation). Fetches weather data from **WeatherAPI.com** and displays current conditions plus 3-day forecast. Features multi-WiFi support, AP mode captive portal for configuration, and deep sleep for battery operation.

This is a simplified version without:
- Touch screen navigation
- Secondary screens (forecast details, conditions, history, config display)
- Weather history storage
- SD card support
- BLE configuration

## Build & Upload

### Arduino IDE Settings
- **Board**: ESP32S3 Dev Module
- **USB CDC On Boot**: Enable
- **USB DFU On Boot**: Disable
- **Flash Size**: 16MB (128Mb)
- **Flash Mode**: QIO 80MHz
- **Partition Scheme**: 16M Flash (3M APP/9.9MB FATFS)
- **PSRAM**: OPI PSRAM
- **Upload Mode**: UART0/Hardware CDC
- **USB Mode**: Hardware CDC and JTAG

### Required Libraries
- **Board Manager**: esp32 by Espressif Systems 2.0.17
- **EPD47-master**: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip
- **ArduinoJson**: by Benoit Blanchon 6.19.0

Only install these two libraries in the Arduino libraries folder to avoid conflicts.

### Force Upload Mode
If upload fails, enter bootloader mode:
1. Press and hold BOOT(IO0) button
2. While holding BOOT, press RST
3. Release RST
4. Release BOOT

## Architecture

### Main Flow
`setup()` → `InitialiseSystem()` → `loadConfig()` → WiFi connect → fetch weather → `DisplayWeather()` → deep sleep

If WiFi fails or `FORCE_AP_MODE=true`, enters AP mode with captive portal for configuration.

### Key Files
- **LilyGo-EPD-4-7-WeatherAPI-Display.ino**: Main sketch - WiFi connection, API calls, display rendering, sleep management
- **owm_credentials.h**: Default configuration (WiFi SSIDs, API key, location, timezone). Values can be overridden via web config
- **wifi_manager.h**: AP mode captive portal, web server, preferences storage (ESP32 NVS)
- **forecast_record.h**: `Forecast_record_type` struct for weather data (extended for WeatherAPI fields)
- **lang.h**: UI text strings (Spanish/English/French)
- **opensans*.h**: Font definitions (sizes 6-24)
- **moon.h, sunrise.h, sunset.h**: Icon bitmaps

### Configuration Storage
Web-configured values are stored in ESP32 preferences (NVS) namespace "weather" and override defaults in `owm_credentials.h`.

### WiFi Connection Logic
1. Scans available networks
2. Matches against stored config + hardcoded credentials
3. Connects to network with strongest signal
4. Falls back to AP mode ("WeatherStation-Setup") if no connection
5. After 5 failed retries, enters permanent deep sleep (requires manual reset)

### Display
Display is 960x540 pixels. Shows main weather screen only (no navigation). Updates based on configured interval then returns to deep sleep.

## WeatherAPI Integration

### API Endpoint
```
https://api.weatherapi.com/v1/forecast.json
  ?key={API_KEY}
  &q={LAT},{LON}
  &days=3
  &aqi=yes
  &lang={LANG}
```

### Single API Call
WeatherAPI returns all data in one response (~50KB JSON):
- **location**: City, region, country, coordinates
- **current**: Temperature, humidity, pressure, wind, UV, condition, air quality
- **forecast.forecastday[]**: 3 days of hourly data
- **astro**: Sunrise, sunset, moonrise, moonset, moon phase, illumination

### Key Functions
- `obtainWeatherData()`: HTTPS call to WeatherAPI (port 443, WiFiClientSecure)
- `DecodeWeatherAPI()`: Parses JSON into WxConditions[] and WxForecast[]
- `mapWeatherAPIIcon()`: Maps WeatherAPI condition codes to OWM-style icon codes (01d, 02d, etc.)
- `TranslateMoonPhase()`: Translates English moon phase names to current language
- `DrawMoonFromAPI()`: Draws moon using illumination percentage from API

### Differences from OpenWeatherMap Version
| Feature | WeatherAPI | OWM |
|---------|------------|-----|
| API calls | 1 | 3+ |
| Connection | HTTPS required | HTTP allowed |
| Moon phase | Real data from API | Calculated from date |
| Sunrise/Sunset | String format ("06:40 AM") | Unix timestamp |
| Air quality | Included | Separate call |
| JSON size | ~50KB | ~10KB per call |

## Configuration

### AP Mode Access
- SSID: `WeatherStation-Setup`
- Password: `weather123`
- URL: `http://192.168.4.1`

### Required API Key
Get a free API key from https://www.weatherapi.com/ and set in `owm_credentials.h` or via web interface.

### Pressure Units
- Spanish (ES): Shows "mb"
- English/French (EN/FR): Shows "hPa"
- Values are identical (1 mb = 1 hPa), only labels differ

## Troubleshooting

### Common Issues
| Error | Cause | Fix |
|-------|-------|-----|
| Connection failed 401/403 | Invalid API key | Verify key at weatherapi.com |
| deserializeJson NoMemory | Response too large | Buffer is 64KB, alerts disabled |
| WiFi connection failed | No network | Check credentials, 2.4GHz only |

### Memory Management
- JSON buffer: 64KB (DynamicJsonDocument)
- Response read as String before parsing (more reliable with HTTPS)
- Alerts disabled (`alerts=no`) to reduce response size
