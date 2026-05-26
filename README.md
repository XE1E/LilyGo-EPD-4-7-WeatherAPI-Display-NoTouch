# LilyGo EPD 4.7" WeatherAPI Display (NoTouch)

![Weather Station Display](docs/weather-station.jpg)

**[English](README_EN.md)** | **[Francais](README_FR.md)**

Estacion meteorologica ESP32-S3 usando la pantalla e-paper LilyGo T5 4.7". Obtiene datos del clima de **WeatherAPI.com** y muestra condiciones actuales mas pronostico de 3 dias.

## Caracteristicas

- **Clima Actual** - Temperatura, humedad, presion, velocidad/direccion del viento, indice UV, calidad del aire (ICA)
- **Pronostico 3 Dias** - Temperaturas por hora con iconos del clima
- **Datos Astronomicos** - Fase lunar, amanecer/atardecer (datos reales de WeatherAPI)
- **Graficas de Tendencia** - Temperatura, presion, humedad, precipitacion en 3 dias
- **Configuracion Web** - Configuracion facil via portal cautivo (sin cambios de codigo)
- **Multi-Idioma** - Espanol, Ingles, Frances
- **Multi-WiFi** - Se conecta a la red mas fuerte disponible
- **Deep Sleep** - Eficiente en bateria, intervalo de actualizacion configurable
- **Sin Touch** - Pantalla unica, sin navegacion necesaria

## Hardware

- **LilyGo T5 4.7" E-Paper (ESP32-S3)** - [Link del Producto](http://www.lilygo.cc/products/t5-4-7-inch-e-paper-v2-3)
- Pantalla: 960x540 pixeles, 16 niveles de gris
- Esta es la version **sin touch**

## Inicio Rapido

### Configuracion Arduino IDE

| Configuracion | Valor |
|---------------|-------|
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| Flash Size | 16MB (128Mb) |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |

### Librerias Requeridas

| Libreria | Version | Fuente |
|----------|---------|--------|
| esp32 (Board Manager) | 2.0.17 | Espressif Systems |
| EPD47-master | Ultima | [Descargar ZIP](https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip) |
| ArduinoJson | 6.19.0 | Benoit Blanchon |

**Importante:** Solo instalar EPD47-master y ArduinoJson en la carpeta de librerias para evitar conflictos.

**Alternativa: Instalar via Web (sin Arduino IDE)**

[![Instalar Firmware](https://img.shields.io/badge/Instalar-Firmware-blue?style=for-the-badge)](https://xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/)

Usar navegador Chrome, Edge u Opera y conectar dispositivo via USB.

### Actualizaciones de Firmware (OTA)

| Metodo | URL / Como usar |
|--------|-----------------|
| **Web OTA** | `http://[IP_DISPOSITIVO]/ota` - Subir .bin desde navegador |
| **Arduino OTA** | Seleccionar puerto de red "WeatherStation-NoTouch" en Arduino IDE |
| **Web Flasher** | [xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch](https://xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/) |
| **Releases** | [Descargar archivos .bin](https://github.com/XE1E/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/releases) |

### Configuracion Inicial

En el primer encendido, el dispositivo detecta automaticamente que no hay configuracion y entra en **modo configuracion inicial** (sin limite de tiempo):

1. Conectarse a red WiFi: `WeatherStation-Setup`
2. Password: `weather123`
3. Abrir navegador: `http://192.168.4.1`
4. **Probar WiFi** - Boton para verificar que la red existe
5. **Probar API** - Boton para validar tu API key antes de guardar
6. Ingresar configuracion (API keys, ubicacion, etc.)
7. Click en Guardar - el dispositivo reinicia automaticamente en 5 segundos

**Modos de configuracion:**
| Modo | Cuando ocurre | Timeout |
|------|---------------|---------|
| Configuracion Inicial | Primer encendido, sin config | Sin limite |
| Modo Recuperacion | Falla conexion WiFi | 5 minutos |
| Modo Forzado | `FORCE_AP_MODE=true` | Sin limite |

### Obtener tu API Key

1. Ir a [weatherapi.com](https://www.weatherapi.com/)
2. Registrar una cuenta gratuita
3. Copiar tu API key del dashboard
4. Ingresarla en la configuracion web

### Operacion Normal

1. Despierta del deep sleep
2. Se conecta a la red WiFi mas fuerte configurada
3. Obtiene clima de WeatherAPI.com (actual + pronostico 3 dias + ICA)
4. Actualiza la pantalla e-paper
5. Regresa a deep sleep

## Opciones de Configuracion

| Opcion | Descripcion | Ejemplo |
|--------|-------------|---------|
| WiFi SSID/Password | Hasta 3 redes | MiRed / mipassword |
| API Key | API key de WeatherAPI.com | abc123def456... |
| Ciudad | Nombre de ubicacion (solo display) | Ciudad de Mexico |
| Latitud | Latitud de ubicacion | 19.4326 |
| Longitud | Longitud de ubicacion | -99.1332 |
| Zona Horaria | String POSIX de zona horaria | CST6CDT |
| Intervalo | Minutos entre actualizaciones | 60 |
| Idioma | ES, EN, o FR | ES |
| Unidades | M (Metrico) o I (Imperial) | M |

### Aplicacion de Cambios

| Aplica inmediatamente | Requiere reinicio |
|-----------------------|-------------------|
| Idioma | Credenciales WiFi |
| Unidades (C/F) | API Key |
| Intervalo de actualizacion | Ubicacion/Coordenadas |
| Horario de actividad | Zona horaria |

## Solucion de Problemas

| Problema | Solucion |
|----------|----------|
| Falla la subida | Mantener BOOT, presionar RST, soltar RST, soltar BOOT, luego subir |
| Sin conexion WiFi | Verificar credenciales, asegurar red 2.4GHz, acercarse al router |
| Sin datos del clima | Verificar API key en weatherapi.com, revisar conexion a internet |
| Pantalla no actualiza | Revisar intervalo de deep sleep, verificar fuente de poder |
| Error "NoMemory" | Normal para respuestas muy grandes, alertas deshabilitadas para ahorrar memoria |

## Archivos del Proyecto

| Archivo | Descripcion |
|---------|-------------|
| `LilyGo-EPD-4-7-WeatherAPI-Display.ino` | Sketch principal |
| `owm_credentials.h` | Configuracion por defecto WiFi/API |
| `wifi_manager.h` | Portal web y modo AP |
| `forecast_record.h` | Estructuras de datos meteorologicos |
| `lang.h` | Strings multi-idioma |

## Detalles Tecnicos

| Especificacion | Valor |
|----------------|-------|
| Pantalla | 960x540 pixeles, 16 grises |
| MCU | ESP32-S3 con PSRAM |
| Consumo | ~15mA activo, ~10uA deep sleep |
| Llamadas API | 1 por actualizacion (incluye actual + pronostico + ICA) |
| Respuesta Max | ~50KB JSON (buffer 64KB) |

## WeatherAPI vs OpenWeatherMap

Este proyecto usa **WeatherAPI.com** en lugar de OpenWeatherMap:

| Caracteristica | WeatherAPI | OpenWeatherMap |
|----------------|------------|----------------|
| Llamadas API | 1 (todos los datos) | 3+ (actual, pronostico, ICA) |
| Fase Lunar | Datos reales | Calculada |
| Amanecer/Atardecer | Formato string | Timestamp Unix |
| Tier Gratuito | 1M llamadas/mes | 1K llamadas/dia |
| Alertas | Si (deshabilitadas por memoria) | API separada |

## Documentacion

- [Manual Espanol](MANUAL_ES.md)
- [English Manual](MANUAL_EN.md)
- [Manuel Francais](MANUAL_FR.md)

## Licencia

Licencia MIT - Ver [Licence.txt](Licence.txt)

## Creditos

- Proyecto original por David Bird
- Port ESP32 por LilyGo
- Adaptacion WeatherAPI por XE1E
- Datos del clima de [WeatherAPI.com](https://www.weatherapi.com/)
