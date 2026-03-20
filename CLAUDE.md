# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESP32-based weather display for the LilyGo EPD 4.7" e-paper display - **NoTouch version** (no touch screen navigation). Fetches weather data from OpenWeatherMap API and displays current conditions plus forecast. Features multi-WiFi support, AP mode captive portal for configuration, and deep sleep for battery operation.

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
- **LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch.ino**: Main sketch - WiFi connection, API calls, display rendering, sleep management
- **owm_credentials.h**: Default configuration (WiFi SSIDs, API key, location, timezone). Values can be overridden via web config
- **wifi_manager.h**: AP mode captive portal, web server, preferences storage (ESP32 NVS)
- **forecast_record.h**: `Forecast_record_type` struct for weather data
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

## Configuration

### AP Mode Access
- SSID: `WeatherStation-Setup`
- Password: `weather123`
- URL: `http://192.168.4.1`

### Required API Key
Get a free API key from https://openweathermap.org/ and set in `owm_credentials.h` or via web interface.
