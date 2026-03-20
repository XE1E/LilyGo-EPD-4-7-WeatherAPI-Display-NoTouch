# LilyGo EPD 4.7" Weather Display - NoTouch Version

Weather display for LilyGo T5 4.7" e-paper display (non-touch version).

## Features

- **Current weather** - Temperature, humidity, pressure, wind, UV Index
- **3-day forecast** - Weather conditions for upcoming days
- **Moon phase** - Current lunar phase display
- **Sunrise/sunset** - Daily solar times
- **Web configuration** - Configure via WiFi AP, no recompilation needed
- **Multi-language** - Spanish, English, French (selectable in config)
- **Deep sleep** - Battery-friendly operation with configurable update interval
- **Multi-WiFi** - Connects to strongest available network from configured list

## Hardware

**Required:** LilyGo T5 4.7" S3 (ESP32-S3, 960x540 e-paper)

This version does NOT require touch capability - it displays weather and goes to deep sleep.

## Quick Start

### 1. Get OpenWeatherMap API Key

1. Go to https://openweathermap.org/
2. Create a free account
3. Go to API Keys section
4. Copy your API key

### 2. Upload Firmware

**Arduino IDE Settings:**
| Setting | Value |
|---------|-------|
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| Flash Size | 16MB (128Mb) |
| Partition Scheme | 16M Flash (3M APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |

**Required Libraries:**
- Board Manager: esp32 by Espressif Systems 2.0.17
- EPD47-master: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip
- ArduinoJson: by Benoit Blanchon 6.19.0

### 3. First Boot Configuration

On first power-up (or when no WiFi is available), the device enters configuration mode:

1. Connect to WiFi network: `WeatherStation-Setup`
2. Password: `weather123`
3. Open browser: `http://192.168.4.1`
4. Enter your settings:
   - OpenWeatherMap API key
   - City name
   - Latitude/Longitude
   - Timezone
   - Update interval
   - Language
5. Click Save - device restarts and displays weather

### 4. Normal Operation

After configuration, the device:
1. Connects to WiFi
2. Fetches weather data
3. Displays weather on screen
4. Enters deep sleep for configured interval
5. Wakes up and repeats

## Configuration Options

| Field | Description | Example |
|-------|-------------|---------|
| API Key | OpenWeatherMap API key | abc123... |
| City | City name for display | Mexico City |
| Latitude | Location latitude | 19.4326 |
| Longitude | Location longitude | -99.1332 |
| Timezone | POSIX timezone string | CST6CDT,M3.2.0,M11.1.0 |
| Update interval | Minutes between updates | 30 |
| Language | Interface language | es / en / fr |
| Units | Metric (C) or Imperial (F) | M / I |

## Troubleshooting

### Upload fails
1. Press and hold BOOT button
2. While holding BOOT, press RST
3. Release RST, then release BOOT
4. Upload should now work

### Can't connect to configuration WiFi
- Make sure no other device is connected to `WeatherStation-Setup`
- Try moving closer to the device
- Press RST to restart and try again

### Weather not updating
- Check your API key is valid
- Verify latitude/longitude are correct
- Check WiFi credentials

### Display shows old data
- Press RST to force a refresh
- Check update interval setting

## Files

| File | Description |
|------|-------------|
| `LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch.ino` | Main sketch |
| `owm_credentials.h` | Default configuration |
| `wifi_manager.h` | AP mode and web server |
| `lang.h` | Multi-language strings |
| `forecast_record.h` | Weather data structure |
| `opensans*.h` | Font files |

## Technical Details

- **Display:** 960x540 pixels, 4-bit grayscale
- **MCU:** ESP32-S3 with PSRAM
- **Power:** Deep sleep between updates (~10uA)
- **Storage:** Configuration saved in ESP32 NVS (persists across power cycles)
- **Wake source:** Timer

## Credits

- Original code by David Bird 2021
- Modified by Stefan Maetschke 2025
- Modified by XE1E 2026 - Multi-WiFi, AP config, UV/AQI, multi-language

Based on:
- https://github.com/Xinyuan-LilyGO/LilyGo-EPD-4-7-OWM-Weather-Display
- https://github.com/DzikuVx/LilyGo-EPD-4-7-OWM-Weather-Display

## License

MIT License - See LICENSE file

---

73 de XE1E
