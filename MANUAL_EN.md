# LilyGo EPD 4.7" Weather Station Manual - NoTouch Version

## Table of Contents

1. [Introduction](#1-introduction)
2. [Technical Specifications](#2-technical-specifications)
3. [System Architecture](#3-system-architecture)
4. [Installation and Compilation](#4-installation-and-compilation)
5. [Configuration](#5-configuration)
6. [Device Usage](#6-device-usage)
7. [OpenWeatherMap API](#7-openweathermap-api)
8. [Power Management](#8-power-management)
9. [Troubleshooting](#9-troubleshooting)
10. [Appendix](#10-appendix)

---

## 1. Introduction

### 1.1 General Description

The LilyGo EPD 4.7" NoTouch Weather Station is an ESP32-S3 based device that displays real-time weather information from OpenWeatherMap. It uses a 4.7-inch e-paper (electronic ink) display that offers excellent visibility under any lighting conditions and low power consumption.

This **NoTouch** version is designed for displays without touch capability, showing only the main weather screen and automatically entering deep sleep mode.

### 1.2 Main Features

- **4.7-inch e-paper display** - 960x540 pixels, grayscale
- **Multi-WiFi** - Support for up to 3 configurable WiFi networks
- **AP Mode (Access Point)** - Captive portal for initial configuration
- **Deep Sleep** - Low consumption for battery operation
- **Multi-language** - Spanish, English and French
- **Automatic updates** - Configurable interval (5-120 minutes)
- **UV Index and Air Quality** - Additional OpenWeatherMap data

### 1.3 Differences from Touch Version

This version **does not include**:
- Touch navigation
- Secondary screens (detailed forecast, graphs, history)
- History storage
- SD card support
- Bluetooth configuration

---

## 2. Technical Specifications

### 2.1 Hardware

#### Microcontroller
| Parameter | Specification |
|-----------|---------------|
| Chip | ESP32-S3 |
| CPU | Dual-core Xtensa LX7 @ 240MHz |
| RAM | 512KB SRAM + 8MB PSRAM (OPI) |
| Flash | 16MB |
| WiFi | 802.11 b/g/n (2.4GHz) |

#### E-Paper Display
| Parameter | Specification |
|-----------|---------------|
| Type | E-Ink (electronic ink) |
| Size | 4.7 inches diagonal |
| Resolution | 960 x 540 pixels |
| Colors | 16 grayscale levels |
| Technology | ED047TC1 |
| Refresh time | ~0.5 seconds |
| Viewing angle | ~180 degrees |

#### Power
| Parameter | Specification |
|-----------|---------------|
| USB input voltage | 5V |
| Battery voltage | 3.7V LiPo (3.2V-4.2V) |
| Active consumption | ~150mA |
| Deep sleep consumption | ~10uA |
| Battery ADC pin | GPIO 14 |

#### Buttons
| Parameter | Specification |
|-----------|---------------|
| BOOT button | GPIO 0 (bootloader mode) |
| RST button | Hardware reset |

### 2.2 E-Paper Technology

#### How It Works

The e-paper display uses bicolor microspheres suspended in fluid. When voltage is applied, white or black particles move to the surface, creating the image. Without voltage, the image remains indefinitely.

#### Advantages
1. **Visibility** - Perfect under direct sunlight
2. **Viewing angle** - Almost 180 degrees
3. **Consumption** - Only uses power when changing the image
4. **Visual comfort** - No backlight, doesn't strain eyes

#### Limitations
1. **Speed** - Slower refresh than LCD (~0.5s)
2. **Ghosting** - Residual images may remain
3. **Color** - Grayscale only
4. **Temperature** - Optimal operation 0-50C

---

## 3. System Architecture

### 3.1 Flow Diagram

```
+----------------+
|     START      |
+----------------+
       |
       v
+----------------+
| InitialiseSystem|
+----------------+
       |
       v
+----------------+
| loadConfig()    |
+----------------+
       |
       v
+----------------+
| FORCE_AP_MODE? |----Yes----> AP Mode
+----------------+              |
       |No                      |
       v                        |
+----------------+              |
| StartWiFi()    |              |
+----------------+              |
       |                        |
  Connected?                    |
  /        \                    |
Yes         No------------------+
|                               |
v                               v
+----------------+       +----------------+
| SetupTime()    |       | startAPMode()  |
+----------------+       +----------------+
       |                        |
       v                        v
+----------------+       +----------------+
| obtainWeather  |       |    LOOP()      |
| obtainUVIndex  |       | - handleAPMode |
| obtainAirQuality|      | - Retry WiFi   |
+----------------+       +----------------+
       |
       v
+----------------+
| DisplayWeather |
+----------------+
       |
       v
+----------------+
| BeginSleep()   |
| - deep_sleep   |
+----------------+
       |
       v (timer)
+----------------+
|     START      |
+----------------+
```

### 3.2 File Structure

```
LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch/
|
+-- LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch.ino  # Main sketch
|
+-- owm_credentials.h     # WiFi credentials and API (default values)
|
+-- wifi_manager.h        # AP mode, web portal, NVS storage
|
+-- forecast_record.h     # Weather data structure
|
+-- lang.h                # Multi-language system (ES/EN/FR)
|
+-- opensans*.h           # Fonts
|
+-- moon.h                # Moon image
+-- sunrise.h             # Sunrise icon
+-- sunset.h              # Sunset icon
```

### 3.3 Configuration Storage (NVS)

Configuration is stored in the ESP32 Preferences "weather" namespace:

| Key | Type | Description |
|-----|------|-------------|
| ssid1, pass1 | String | Primary WiFi network |
| ssid2, pass2 | String | Secondary WiFi network |
| ssid3, pass3 | String | Tertiary WiFi network |
| apikey | String | OpenWeatherMap API Key |
| city | String | City name |
| lat, lon | String | Geographic coordinates |
| lang | String | Language (ES, EN, FR) |
| hemi | String | Hemisphere (north, south) |
| units | String | Units (M=metric, I=imperial) |
| tz | String | POSIX timezone |
| gmt | Int | GMT offset in seconds |
| dst | Int | Daylight saving offset |
| updint | Int | Update interval (min) |

---

## 4. Installation and Compilation

### 4.1 Software Requirements

#### Arduino IDE
- Version 1.8.x or 2.x

#### Board Manager
- **URL**: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
- **Package**: esp32 by Espressif Systems **version 2.0.17**

#### Required Libraries
Install only these two libraries:

1. **EPD47-master**
   - URL: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip

2. **ArduinoJson**
   - Author: Benoit Blanchon
   - Version: 6.19.0

### 4.2 Arduino IDE Configuration

| Parameter | Value |
|-----------|-------|
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| USB DFU On Boot | Disable |
| Flash Size | 16MB (128Mb) |
| Flash Mode | QIO 80MHz |
| Partition Scheme | 16M Flash (3M APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |
| USB Mode | Hardware CDC and JTAG |

### 4.3 Compilation Process

1. Open the `.ino` file in Arduino IDE
2. Select the correct COM port
3. Click "Verify" to compile
4. Click "Upload" to flash

### 4.4 Bootloader Mode (if upload fails)

If upload fails:
1. Press and hold **BOOT** button
2. While holding BOOT, press **RST**
3. Release RST
4. Release BOOT
5. Try uploading again

---

## 5. Configuration

### 5.1 Initial Configuration (AP Mode)

On first boot or when no WiFi is available:

1. Device creates network: **WeatherStation-Setup**
2. Connect with password: **weather123**
3. Open browser: **http://192.168.4.1**
4. Complete the configuration form
5. Save - device restarts

### 5.2 Configuration Parameters

#### WiFi Networks
- Up to 3 WiFi networks with SSID and password
- Device automatically connects to strongest signal

#### OpenWeatherMap API
- Get free API key at: https://openweathermap.org/
- Free key allows ~1000 calls/day

#### Location
- **City**: Name to display on screen
- **Latitude/Longitude**: Exact coordinates for precise data

#### Timezone
- POSIX format, example: `EST5EDT,M3.2.0,M11.1.0`
- GMT offset is calculated automatically

#### Update Options
- **Interval**: 5 to 120 minutes between updates
- **Start hour**: Activity start hour (0-23)
- **End hour**: Night suspension hour (0-23)

#### Language
- Spanish, English or French

#### Units
- Metric (Celsius, m/s, hPa)
- Imperial (Fahrenheit, mph, inHg)

---

## 6. Device Usage

### 6.1 Normal Operation

1. Device powers on or wakes from deep sleep
2. Connects to strongest available WiFi network
3. Syncs time via NTP
4. Gets weather data from OpenWeatherMap
5. Displays information on screen
6. Enters deep sleep until next update

### 6.2 Main Screen

The screen displays:

```
+----------------------------------------------------------+
|  City                Date and Time          Battery WiFi  |
+----------------------------------------------------------+
|                                                           |
|    [Wind Compass]       TEMPERATURE            [Icon]     |
|                         Humidity%                         |
|    Direction            Max|Min  UV Index                 |
|    Speed                                                  |
|                         Weather description               |
|    [Sun/Moon]           Pressure hPa                      |
|    Sunrise                                                |
|    Sunset               Feels like                        |
|    Moon phase                                             |
|                                                           |
|    +----------+  +----------+  +----------+               |
|    | Forecast |  | Forecast |  | Forecast |               |
|    | Day 1    |  | Day 2    |  | Day 3    |               |
|    +----------+  +----------+  +----------+               |
+----------------------------------------------------------+
```

### 6.3 Status Indicators

- **Battery**: LiPo battery charge level
- **WiFi**: Signal strength in dB

---

## 7. OpenWeatherMap API

### 7.1 Endpoints Used

| Endpoint | Description |
|----------|-------------|
| /data/2.5/weather | Current weather |
| /data/2.5/forecast | 5-day forecast |
| /data/2.5/uvi | UV Index |
| /data/2.5/air_pollution | Air quality |

### 7.2 Data Retrieved

- Current, max and min temperature
- Feels like temperature
- Relative humidity
- Atmospheric pressure and trend
- Wind speed and direction
- Precipitation probability
- Cloud coverage
- Visibility
- Sunrise and sunset
- UV Index
- Air Quality Index (AQI)

### 7.3 Free API Limits

- ~1000 calls per day
- Data updated every 10 minutes on server
- Forecast up to 5 days

---

## 8. Power Management

### 8.1 Power Modes

| Mode | Consumption | Description |
|------|-------------|-------------|
| Active | ~150mA | Processing, WiFi active |
| Deep Sleep | ~10uA | Only RTC active |

### 8.2 Power Cycle

1. **Wake up** - Timer or manual reset
2. **Active** - WiFi, API, display (~5-10 seconds)
3. **Deep Sleep** - Until next update

### 8.3 Battery Life

With 2000mAh LiPo battery:
- 30 min interval: ~2-3 months
- 60 min interval: ~4-6 months

### 8.4 Activity Hours

Configure start and end hours to suspend updates during night and save battery.

---

## 9. Troubleshooting

### 9.1 Won't Connect to WiFi

**Symptoms**: Constantly enters AP mode

**Solutions**:
1. Verify SSID and password are correct
2. Check network is in range
3. Network must be 2.4GHz (5GHz not supported)

### 9.2 No Weather Data

**Symptoms**: Blank screen or old data

**Solutions**:
1. Verify OpenWeatherMap API key
2. Check coordinates (lat/lon)
3. Verify internet connection

### 9.3 Screen Ghosting

**Symptoms**: Residual images visible

**Solutions**:
1. Ghosting is normal for e-paper
2. Reduces with each update
3. Press RST to force refresh

### 9.4 Firmware Upload Fails

**Symptoms**: Timeout or connection error

**Solutions**:
1. Use bootloader mode (BOOT + RST)
2. Check USB cable (use data cable, not charge-only)
3. Try different USB port

### 9.5 Wrong Time

**Symptoms**: Displayed time doesn't match

**Solutions**:
1. Verify timezone configuration
2. Format must be valid POSIX
3. Check GMT and DST offset

---

## 10. Appendix

### 10.1 Weather Icon Codes

| Code | Description |
|------|-------------|
| 01d/01n | Clear sky |
| 02d/02n | Few clouds |
| 03d/03n | Scattered clouds |
| 04d/04n | Overcast |
| 09d/09n | Light rain |
| 10d/10n | Rain |
| 11d/11n | Thunderstorm |
| 13d/13n | Snow |
| 50d/50n | Fog |

### 10.2 POSIX Timezones

Common examples:
- US Eastern: `EST5EDT,M3.2.0,M11.1.0`
- US Pacific: `PST8PDT,M3.2.0,M11.1.0`
- UK: `GMT0BST,M3.5.0/1,M10.5.0`
- Central Europe: `CET-1CEST,M3.5.0,M10.5.0`
- Australia Sydney: `AEST-10AEDT,M10.1.0,M4.1.0/3`

### 10.3 Air Quality Index (AQI) Scale

| AQI | Description | Color |
|-----|-------------|-------|
| 1 | Good | Green |
| 2 | Fair | Yellow |
| 3 | Moderate | Orange |
| 4 | Poor | Red |
| 5 | Very Poor | Purple |

### 10.4 UV Scale

| UV | Description |
|----|-------------|
| 0-2 | Low |
| 3-5 | Moderate |
| 6-7 | High |
| 8-10 | Very high |
| 11+ | Extreme |

---

## Credits

- Original code: David Bird 2021
- Modifications: Stefan Maetschke 2025
- NoTouch version and improvements: XE1E 2026

## License

MIT License

---

73 de XE1E
