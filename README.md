# LilyGo EPD 4.7" Weather Display - NoTouch

Weather station for LilyGo T5 4.7" e-paper display (non-touch version).

## Features

- **Current weather** - Temperature, humidity, pressure, wind, UV Index, Air Quality
- **3-day forecast** - Weather conditions for upcoming days
- **Moon phase** - Current lunar phase with icon
- **Sunrise/sunset** - Daily solar times
- **Web configuration** - Configure via WiFi AP, no recompilation needed
- **Multi-language** - Spanish, English, French (selectable in config)
- **Multi-WiFi** - Connects to strongest available network from up to 3 configured
- **Deep sleep** - Battery-friendly operation with configurable update interval
- **No touch required** - Works with non-touch LilyGo T5 4.7" S3

## Hardware

**Required:** LilyGo T5 4.7" S3 (ESP32-S3, 960x540 e-paper)

The display does NOT require touch capability - shows weather and enters deep sleep.

## Quick Start

### 1. Upload Firmware

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

### 2. First Boot Configuration

On first power-up (or when no WiFi available), the device enters configuration mode:

1. Connect to WiFi network: `WeatherStation-Setup`
2. Password: `weather123`
3. Open browser: `http://192.168.4.1`
4. Enter your settings (API key, location, etc.)
5. Click Save - device restarts and displays weather

### 3. Normal Operation

After configuration, the device:
1. Connects to WiFi
2. Fetches weather from OpenWeatherMap
3. Displays weather on screen
4. Enters deep sleep (default: 1 hour)
5. Wakes up and repeats

## Configuration Options

| Field | Description | Example |
|-------|-------------|---------|
| WiFi (up to 3) | Network SSID and password | MyWiFi / password123 |
| API Key | OpenWeatherMap API key | abc123... |
| City | City name for display | Mexico City |
| Latitude | Location latitude | 19.4326 |
| Longitude | Location longitude | -99.1332 |
| Timezone | POSIX timezone string | CST6CDT,M3.2.0,M11.1.0 |
| Update interval | Minutes between updates | 60 |
| Language | Interface language | es / en / fr |
| Units | Metric or Imperial | M / I |

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

### No weather data
- Check your OpenWeatherMap API key is valid
- Verify latitude/longitude are correct
- Check WiFi credentials

## Files

| File | Description |
|------|-------------|
| `LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch.ino` | Main sketch |
| `wifi_manager.h` | AP mode and web server |
| `owm_credentials.h` | Default configuration |
| `lang.h` | Multi-language strings |
| `forecast_record.h` | Weather data structure |
| `opensans*.h` | Font files |

## Technical Details

- **Display:** 960x540 pixels, 4-bit grayscale
- **MCU:** ESP32-S3 with PSRAM
- **Power:** Deep sleep between updates (~10uA)
- **Storage:** Configuration saved in ESP32 NVS (persists across power cycles)
- **Wake source:** Timer

## Documentation

See detailed manuals in three languages:
- [MANUAL_ES.md](MANUAL_ES.md) - Español
- [MANUAL_EN.md](MANUAL_EN.md) - English
- [MANUAL_FR.md](MANUAL_FR.md) - Français

## License

MIT License - See LICENSE file

## Credits

- Original code by David Bird 2021
- Modified by Stefan Maetschke 2025
- NoTouch version by XE1E 2026
- EPD47 library by Vroland
- Weather data from OpenWeatherMap

---

73 de XE1E
