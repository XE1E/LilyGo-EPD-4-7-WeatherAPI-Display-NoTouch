# LilyGo EPD 4.7" Weather Station Manual - WeatherAPI Version

## Table of Contents

1. [Introduction](#1-introduction)
2. [Technical Specifications](#2-technical-specifications)
3. [System Architecture](#3-system-architecture)
4. [Installation and Compilation](#4-installation-and-compilation)
5. [Configuration](#5-configuration)
6. [Device Usage](#6-device-usage)
7. [WeatherAPI.com](#7-weatherapicom)
8. [Power Management](#8-power-management)
9. [Troubleshooting](#9-troubleshooting)
10. [Appendix](#10-appendix)

---

## 1. Introduction

### 1.1 General Description

The LilyGo EPD 4.7" Weather Station is an ESP32-S3 based device that displays real-time weather information from **WeatherAPI.com**. It uses a 4.7-inch e-paper (electronic ink) display that offers excellent visibility under any lighting conditions and low power consumption.

This **NoTouch** version is designed for displays without touch capability, showing only the main weather screen and entering deep sleep mode automatically.

### 1.2 Main Features

- **4.7-inch e-paper display** - 960x540 pixels, grayscale
- **Multi-WiFi** - Support for up to 3 configurable WiFi networks
- **AP Mode (Access Point)** - Captive portal for initial configuration
- **Deep Sleep** - Low consumption for battery operation
- **Multi-language** - English, Spanish, and French
- **Automatic update** - Configurable interval (5-120 minutes)
- **UV Index and Air Quality** - Included in single WeatherAPI call
- **Real Moon Phase** - Astronomical data directly from API
- **Sunrise/Sunset** - Real times for your location

### 1.3 WeatherAPI vs OpenWeatherMap Advantages

| Feature | WeatherAPI | OpenWeatherMap |
|---------|------------|----------------|
| API Calls | 1 (all included) | 3+ (separate) |
| Moon Phase | Real data | Calculated |
| Free Tier | 1M calls/month | 1K calls/day |
| Air Quality | Included | Separate API |
| Alerts | Available | Separate API |

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
| Refresh time | ~0.5 seconds |

#### Power
| Parameter | Specification |
|-----------|---------------|
| USB input voltage | 5V |
| Battery voltage | 3.7V LiPo |
| Active consumption | ~150mA |
| Deep sleep consumption | ~10uA |

### 2.2 Software

| Component | Version |
|-----------|---------|
| Arduino Core ESP32 | 2.0.17 |
| ArduinoJson | 6.19.0 |
| EPD47-master | Latest |

---

## 3. System Architecture

### 3.1 Main Flow

```
Start/Wake
    |
    v
Initialize System
    |
    v
Load Configuration (NVS)
    |
    v
Scan WiFi Networks
    |
    +---> No known network ---> AP Mode (Web Portal)
    |
    v
Connect to WiFi
    |
    v
Get Data (WeatherAPI)
    |
    v
Update Display
    |
    v
Deep Sleep (X minutes)
```

### 3.2 File Structure

```
LilyGo-EPD-4-7-WeatherAPI-Display/
|
+-- LilyGo-EPD-4-7-WeatherAPI-Display.ino  # Main sketch
|
+-- owm_credentials.h     # WiFi and API credentials (defaults)
+-- wifi_manager.h        # Web portal and AP mode
+-- forecast_record.h     # Weather data structures
+-- lang.h                # Multi-language strings
|
+-- opensans*.h           # Fonts (sizes 8-24)
+-- moon.h, sunrise.h, sunset.h  # Bitmap icons
```

### 3.3 Weather Data

WeatherAPI provides all data in a single call:

| Field | Description | Unit |
|-------|-------------|------|
| Temperature | Current temperature | C / F |
| Feelslike | Feels like temperature | C / F |
| Humidity | Relative humidity | % |
| Pressure | Atmospheric pressure | mb / hPa |
| Wind | Speed and direction | kph / mph |
| UV Index | Ultraviolet index | 0-11+ |
| AQI | Air quality index | 1-6 |
| Sunrise/Sunset | Solar times | HH:MM AM/PM |
| Moon Phase | Lunar phase | Text + % illumination |
| Forecast | 3-day forecast | Hourly |

---

## 4. Installation and Compilation

### 4.1 Requirements

1. **Arduino IDE** 1.8.x or 2.x
2. **ESP32 Board Support** version 2.0.17
3. **Required libraries**

### 4.2 Library Installation

#### ESP32 Board Manager
1. Open Arduino IDE
2. Go to File > Preferences
3. In "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to Tools > Board > Board Manager
5. Search "esp32" and install version **2.0.17**

#### EPD47-master
1. Download: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip
2. In Arduino IDE: Sketch > Include Library > Add .ZIP Library
3. Select the downloaded file

#### ArduinoJson
1. Tools > Manage Libraries
2. Search "ArduinoJson"
3. Install version **6.19.0** by Benoit Blanchon

### 4.3 Arduino IDE Settings

| Option | Value |
|--------|-------|
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| USB DFU On Boot | Disable |
| Flash Size | 16MB (128Mb) |
| Flash Mode | QIO 80MHz |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |
| USB Mode | Hardware CDC and JTAG |

### 4.4 Upload Firmware (USB)

1. Connect device via USB
2. Select correct COM port
3. Compile and upload

**If upload fails:**
1. Hold BOOT button
2. While holding BOOT, press RST
3. Release RST
4. Release BOOT
5. Try uploading again

### 4.5 OTA Updates (Over-The-Air)

Firmware can be updated wirelessly without a USB cable.

#### Method 1: Web OTA (Recommended)

Update from browser while device is connected to WiFi:

1. Connect to the same WiFi network as the device
2. Open in browser: `http://[DEVICE_IP]/ota`
3. Drag the `.bin` file or click to select
4. Click "Update Firmware"
5. Wait for completion (do not disconnect during process)
6. Device will restart automatically

**Note**: Device IP is shown on the info screen.

#### Method 2: Arduino OTA

Update directly from Arduino IDE over WiFi:

1. Ensure device and PC are on the same network
2. In Arduino IDE: `Tools` → `Port`
3. Select "WeatherStation-NoTouch at [IP]" (appears as network port)
4. Click Upload as usual

**Requirements**:
- Device powered on and connected to WiFi
- PC on same local network
- Arduino IDE with ESP32 support

#### Method 3: Web Flasher (GitHub)

Flash from browser without installing anything:

1. Visit: `https://xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/`
2. Connect device via USB
3. Click "Install Firmware"
4. Select serial port
5. Wait for installation to complete

**Requirements**:
- Chrome, Edge or Opera browser (requires Web Serial API)
- USB cable connected to device

#### Download Firmware

Compiled .bin files are available at:
`https://github.com/XE1E/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/releases`

---

## 5. Configuration

### 5.1 Web Portal (AP Mode)

When no WiFi network is configured or available, the device creates an access point:

- **SSID:** WeatherStation-Setup
- **Password:** weather123
- **URL:** http://192.168.4.1

### 5.2 Configuration Options

#### WiFi Tab
| Field | Description |
|-------|-------------|
| SSID 1-3 | WiFi network name |
| Password 1-3 | Network password |

#### Weather Tab
| Field | Description |
|-------|-------------|
| API Key | WeatherAPI.com key |
| City | Display name |
| Latitude | Coordinate (-90 to 90) |
| Longitude | Coordinate (-180 to 180) |
| Timezone | POSIX format (e.g., EST5EDT) |

#### System Tab
| Field | Description |
|-------|-------------|
| Language | ES, EN, FR |
| Units | M (Metric), I (Imperial) |
| Interval | Minutes between updates |

### 5.3 Get WeatherAPI Key

1. Go to [weatherapi.com](https://www.weatherapi.com/)
2. Create free account
3. Go to Dashboard
4. Copy the API Key
5. Paste it in web configuration

---

## 6. Device Usage

### 6.1 Main Screen

The display shows:

```
+--------------------------------------------------+
|  City, Region                       Date & Time  |
+--------------------------------------------------+
|                                                  |
|   [Icon]    Temperature    Humidity%             |
|             High/Low       UV Index              |
|                                                  |
|   Wind      Pressure       Visibility            |
|   Rose      Trend          Feels Like   AQI      |
|                                                  |
|   Moon      Sunrise                              |
|   Phase     Sunset                               |
|                                                  |
+--------------------------------------------------+
|  [Hourly Forecast - 7 periods of 3h]             |
+--------------------------------------------------+
|  [Graphs: Temp | Pressure | Humidity | Precip]   |
+--------------------------------------------------+
```

### 6.2 Weather Icons

| Icon | Condition |
|------|-----------|
| Sun | Clear |
| Sun/Cloud | Partly cloudy |
| Clouds | Cloudy |
| Rain | Rain |
| Storm | Thunderstorm |
| Snow | Snow |
| Fog | Fog/Mist |

### 6.3 Indicators

- **Wind Rose:** Current wind direction and speed
- **Moon Phase:** Icon and phase name (real WeatherAPI data)
- **AQI (Air Quality Index):** 1-Good to 6-Hazardous

---

## 7. WeatherAPI.com

### 7.1 Endpoint Used

```
https://api.weatherapi.com/v1/forecast.json
  ?key={API_KEY}
  &q={LAT},{LON}
  &days=3
  &aqi=yes
  &lang={LANG}
```

### 7.2 Data Received

A single call includes:
- **location** - Location information
- **current** - Current conditions + air quality
- **forecast** - 3 days with hourly data
- **astro** - Astronomical data (sun, moon)

### 7.3 Free Plan Limits

| Limit | Value |
|-------|-------|
| Calls/month | 1,000,000 |
| Forecast days | 3 |
| History | Not included |
| Alerts | Available |

### 7.4 Condition Codes

WeatherAPI uses numeric codes (1000, 1003, etc.) which are mapped to internal icons compatible with the original icon system.

---

## 8. Power Management

### 8.1 Operating Modes

| Mode | Consumption | Duration |
|------|-------------|----------|
| Active | ~150mA | ~10-15 seconds |
| Deep Sleep | ~10uA | Configurable |

### 8.2 Estimated Battery Life

With 2000mAh battery:
- Update every 60 min: ~6 months
- Update every 30 min: ~3 months
- Update every 15 min: ~6 weeks

### 8.3 Battery Indicator

Located in upper right corner:
- Green: >75%
- Yellow: 25-75%
- Red: <25%

---

## 9. Troubleshooting

### 9.1 Common Problems

| Problem | Solution |
|---------|----------|
| Firmware won't upload | Use bootloader mode (BOOT + RST) |
| WiFi won't connect | Check 2.4GHz, move closer to router |
| No weather data | Verify API key at weatherapi.com |
| NoMemory error | Normal, retries automatically |
| Display not updating | Check power supply |

### 9.2 Serial Error Messages

| Message | Cause | Solution |
|---------|-------|----------|
| Connection failed 401/403 | Invalid API key | Verify key |
| deserializeJson NoMemory | Insufficient buffer | Uses 64KB buffer |
| WiFi connection failed | No network available | Check credentials |

### 9.3 Factory Reset

To clear all configuration:
1. Add in setup():
   ```cpp
   Preferences prefs;
   prefs.begin("weather", false);
   prefs.clear();
   prefs.end();
   ```
2. Upload, run once, remove the code

---

## 10. Appendix

### 10.1 POSIX Timezones

| Region | Code |
|--------|------|
| US Eastern | EST5EDT |
| US Pacific | PST8PDT |
| UK | GMT0BST |
| Central Europe | CET-1CEST |
| Japan | JST-9 |

### 10.2 Language Codes

| Code | Language |
|------|----------|
| EN | English |
| ES | Spanish |
| FR | French |

### 10.3 Credits

- Original project: David Bird
- ESP32 port: LilyGo
- WeatherAPI adaptation: XE1E
- Weather data: [WeatherAPI.com](https://www.weatherapi.com/)

---

*Manual version 1.0 - WeatherAPI*
