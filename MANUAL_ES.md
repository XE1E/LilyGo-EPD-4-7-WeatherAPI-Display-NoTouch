# Manual Estacion Meteorologica LilyGo EPD 4.7" - Version NoTouch

## Tabla de Contenidos

1. [Introduccion](#1-introduccion)
2. [Especificaciones Tecnicas](#2-especificaciones-tecnicas)
3. [Arquitectura del Sistema](#3-arquitectura-del-sistema)
4. [Instalacion y Compilacion](#4-instalacion-y-compilacion)
5. [Configuracion](#5-configuracion)
6. [Uso del Dispositivo](#6-uso-del-dispositivo)
7. [API OpenWeatherMap](#7-api-openweathermap)
8. [Gestion de Energia](#8-gestion-de-energia)
9. [Solucion de Problemas](#9-solucion-de-problemas)
10. [Apendice](#10-apendice)

---

## 1. Introduccion

### 1.1 Descripcion General

La Estacion Meteorologica LilyGo EPD 4.7" NoTouch es un dispositivo basado en ESP32-S3 que muestra informacion meteorologica en tiempo real obtenida de OpenWeatherMap. Utiliza una pantalla e-paper (tinta electronica) de 4.7 pulgadas que ofrece excelente visibilidad bajo cualquier condicion de luz y bajo consumo de energia.

Esta version **NoTouch** esta disenada para pantallas sin capacidad tactil, mostrando unicamente la pantalla principal del clima y entrando en modo deep sleep automaticamente.

### 1.2 Caracteristicas Principales

- **Pantalla e-paper de 4.7 pulgadas** - 960x540 pixeles, escala de grises
- **Multi-WiFi** - Soporte para hasta 3 redes WiFi configurables
- **Modo AP (Access Point)** - Portal cautivo para configuracion inicial
- **Deep Sleep** - Bajo consumo para operacion con bateria
- **Multi-idioma** - Espanol, Ingles y Frances
- **Actualizacion automatica** - Intervalo configurable (5-120 minutos)
- **Indice UV y Calidad del Aire** - Datos adicionales de OpenWeatherMap

### 1.3 Diferencias con Version Touch

Esta version **no incluye**:
- Navegacion tactil
- Pantallas secundarias (pronostico detallado, graficos, historial)
- Almacenamiento de historial
- Soporte para tarjeta SD
- Configuracion Bluetooth

---

## 2. Especificaciones Tecnicas

### 2.1 Hardware

#### Microcontrolador
| Parametro | Especificacion |
|-----------|----------------|
| Chip | ESP32-S3 |
| CPU | Dual-core Xtensa LX7 @ 240MHz |
| RAM | 512KB SRAM + 8MB PSRAM (OPI) |
| Flash | 16MB |
| WiFi | 802.11 b/g/n (2.4GHz) |

#### Pantalla E-Paper
| Parametro | Especificacion |
|-----------|----------------|
| Tipo | E-Ink (tinta electronica) |
| Tamano | 4.7 pulgadas diagonal |
| Resolucion | 960 x 540 pixeles |
| Colores | 16 niveles de gris |
| Tecnologia | ED047TC1 |
| Tiempo de refresco | ~0.5 segundos |
| Angulo de vision | ~180 grados |

#### Alimentacion
| Parametro | Especificacion |
|-----------|----------------|
| Voltaje entrada USB | 5V |
| Voltaje bateria | 3.7V LiPo (3.2V-4.2V) |
| Consumo activo | ~150mA |
| Consumo deep sleep | ~10uA |
| Pin ADC bateria | GPIO 14 |

#### Botones
| Parametro | Especificacion |
|-----------|----------------|
| Boton BOOT | GPIO 0 (modo bootloader) |
| Boton RST | Reset hardware |

### 2.2 Tecnologia E-Paper

#### Como Funciona

La pantalla e-paper utiliza microesferas bicolores suspendidas en un fluido. Al aplicar voltaje, las particulas blancas o negras se mueven hacia la superficie, creando la imagen. Sin voltaje, la imagen se mantiene indefinidamente.

#### Ventajas
1. **Visibilidad** - Perfecta bajo luz solar directa
2. **Angulo de vision** - Casi 180 grados
3. **Consumo** - Solo consume energia al cambiar la imagen
4. **Confort visual** - Sin retroiluminacion, no cansa la vista

#### Limitaciones
1. **Velocidad** - Refresco mas lento que LCD (~0.5s)
2. **Ghosting** - Pueden quedar imagenes residuales
3. **Color** - Solo escala de grises
4. **Temperatura** - Funcionamiento optimo 0-50C

---

## 3. Arquitectura del Sistema

### 3.1 Diagrama de Flujo

```
+----------------+
|    INICIO      |
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
| FORCE_AP_MODE? |----Si----> Modo AP
+----------------+             |
       |No                     |
       v                       |
+----------------+             |
| StartWiFi()    |             |
+----------------+             |
       |                       |
  Conectado?                   |
  /        \                   |
Si          No-----------------+
|                              |
v                              v
+----------------+      +----------------+
| SetupTime()    |      | startAPMode()  |
+----------------+      +----------------+
       |                       |
       v                       v
+----------------+      +----------------+
| obtainWeather  |      |   LOOP()       |
| obtainUVIndex  |      | - handleAPMode |
| obtainAirQuality|     | - Retry WiFi   |
+----------------+      +----------------+
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
|    INICIO      |
+----------------+
```

### 3.2 Estructura de Archivos

```
LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch/
|
+-- LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch.ino  # Sketch principal
|
+-- owm_credentials.h     # Credenciales WiFi y API (valores por defecto)
|
+-- wifi_manager.h        # Modo AP, portal web, almacenamiento NVS
|
+-- forecast_record.h     # Estructura de datos meteorologicos
|
+-- lang.h                # Sistema multi-idioma (ES/EN/FR)
|
+-- opensans*.h           # Fuentes
|
+-- moon.h                # Imagen de la luna
+-- sunrise.h             # Icono amanecer
+-- sunset.h              # Icono anochecer
```

### 3.3 Almacenamiento de Configuracion (NVS)

La configuracion se almacena en el namespace "weather" de ESP32 Preferences:

| Clave | Tipo | Descripcion |
|-------|------|-------------|
| ssid1, pass1 | String | Red WiFi principal |
| ssid2, pass2 | String | Red WiFi secundaria |
| ssid3, pass3 | String | Red WiFi terciaria |
| apikey | String | API Key de OpenWeatherMap |
| city | String | Nombre de la ciudad |
| lat, lon | String | Coordenadas geograficas |
| lang | String | Idioma (ES, EN, FR) |
| hemi | String | Hemisferio (north, south) |
| units | String | Unidades (M=metrico, I=imperial) |
| tz | String | Zona horaria POSIX |
| gmt | Int | Offset GMT en segundos |
| dst | Int | Offset horario de verano |
| updint | Int | Intervalo actualizacion (min) |

---

## 4. Instalacion y Compilacion

### 4.1 Requisitos de Software

#### Arduino IDE
- Version 1.8.x o 2.x

#### Board Manager
- **URL**: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
- **Paquete**: esp32 by Espressif Systems **version 2.0.17**

#### Librerias Requeridas
Solo instalar estas dos librerias:

1. **EPD47-master**
   - URL: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip

2. **ArduinoJson**
   - Autor: Benoit Blanchon
   - Version: 6.19.0

### 4.2 Configuracion del Arduino IDE

| Parametro | Valor |
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

### 4.3 Proceso de Compilacion

1. Abrir el archivo `.ino` en Arduino IDE
2. Seleccionar el puerto COM correcto
3. Click en "Verify" para compilar
4. Click en "Upload" para cargar

### 4.4 Modo Bootloader (si falla la carga)

Si la carga falla:
1. Presionar y mantener el boton **BOOT**
2. Sin soltar BOOT, presionar **RST**
3. Soltar RST
4. Soltar BOOT
5. Intentar cargar nuevamente

---

## 5. Configuracion

### 5.1 Configuracion Inicial (Modo AP)

En el primer arranque o cuando no hay WiFi disponible:

1. El dispositivo crea la red: **WeatherStation-Setup**
2. Conectarse con contrasena: **weather123**
3. Abrir navegador: **http://192.168.4.1**
4. Completar el formulario de configuracion
5. Guardar - el dispositivo se reinicia

### 5.2 Parametros de Configuracion

#### Redes WiFi
- Hasta 3 redes WiFi con SSID y contrasena
- El dispositivo conecta automaticamente a la de mejor senal

#### API OpenWeatherMap
- Obtener API key gratuita en: https://openweathermap.org/
- La key gratuita permite ~1000 llamadas/dia

#### Ubicacion
- **Ciudad**: Nombre para mostrar en pantalla
- **Latitud/Longitud**: Coordenadas exactas para datos precisos

#### Zona Horaria
- Formato POSIX, ejemplo: `CST6CDT,M3.2.0,M11.1.0`
- El offset GMT se calcula automaticamente

#### Opciones de Actualizacion
- **Intervalo**: 5 a 120 minutos entre actualizaciones
- **Hora inicio**: Hora de inicio de actividad (0-23)
- **Hora fin**: Hora de suspension nocturna (0-23)

#### Idioma
- Espanol, Ingles o Frances

#### Unidades
- Metrico (Celsius, m/s, hPa)
- Imperial (Fahrenheit, mph, inHg)

---

## 6. Uso del Dispositivo

### 6.1 Operacion Normal

1. El dispositivo se enciende o despierta del deep sleep
2. Conecta a la red WiFi mas fuerte disponible
3. Sincroniza la hora via NTP
4. Obtiene datos del clima de OpenWeatherMap
5. Muestra la informacion en pantalla
6. Entra en deep sleep hasta la proxima actualizacion

### 6.2 Pantalla Principal

La pantalla muestra:

```
+----------------------------------------------------------+
|  Ciudad               Fecha y Hora           Bateria WiFi |
+----------------------------------------------------------+
|                                                           |
|    [Brujula Viento]        TEMPERATURA           [Icono]  |
|                            Humedad%                       |
|    Direccion               Max|Min  UV Index              |
|    Velocidad                                              |
|                            Descripcion del clima          |
|    [Sol/Luna]              Presion hPa                    |
|    Amanecer                                               |
|    Anochecer               Sensacion termica              |
|    Fase lunar                                             |
|                                                           |
|    +----------+  +----------+  +----------+               |
|    | Pronost. |  | Pronost. |  | Pronost. |               |
|    | Dia 1    |  | Dia 2    |  | Dia 3    |               |
|    +----------+  +----------+  +----------+               |
+----------------------------------------------------------+
```

### 6.3 Indicadores de Estado

- **Bateria**: Nivel de carga de la bateria LiPo
- **WiFi**: Intensidad de la senal en dB

---

## 7. API OpenWeatherMap

### 7.1 Endpoints Utilizados

| Endpoint | Descripcion |
|----------|-------------|
| /data/2.5/weather | Clima actual |
| /data/2.5/forecast | Pronostico 5 dias |
| /data/2.5/uvi | Indice UV |
| /data/2.5/air_pollution | Calidad del aire |

### 7.2 Datos Obtenidos

- Temperatura actual, maxima y minima
- Sensacion termica
- Humedad relativa
- Presion atmosferica y tendencia
- Velocidad y direccion del viento
- Probabilidad de precipitacion
- Cobertura de nubes
- Visibilidad
- Amanecer y anochecer
- Indice UV
- Indice de calidad del aire (ICA)

### 7.3 Limites de la API Gratuita

- ~1000 llamadas por dia
- Datos actualizados cada 10 minutos en el servidor
- Pronostico hasta 5 dias

---

## 8. Gestion de Energia

### 8.1 Modos de Energia

| Modo | Consumo | Descripcion |
|------|---------|-------------|
| Activo | ~150mA | Procesando, WiFi activo |
| Deep Sleep | ~10uA | Solo RTC activo |

### 8.2 Ciclo de Energia

1. **Wake up** - Timer o reset manual
2. **Activo** - WiFi, API, display (~5-10 segundos)
3. **Deep Sleep** - Hasta proxima actualizacion

### 8.3 Duracion de Bateria

Con bateria LiPo de 2000mAh:
- Intervalo 30 min: ~2-3 meses
- Intervalo 60 min: ~4-6 meses

### 8.4 Horas de Actividad

Configurar horas de inicio y fin para suspender actualizaciones durante la noche y ahorrar bateria.

---

## 9. Solucion de Problemas

### 9.1 No Conecta a WiFi

**Sintomas**: Entra en modo AP constantemente

**Soluciones**:
1. Verificar que el SSID y contrasena son correctos
2. Comprobar que la red esta en rango
3. La red debe ser 2.4GHz (no soporta 5GHz)

### 9.2 No Muestra Datos del Clima

**Sintomas**: Pantalla en blanco o datos antiguos

**Soluciones**:
1. Verificar API key de OpenWeatherMap
2. Comprobar coordenadas (lat/lon)
3. Verificar conexion a internet

### 9.3 Ghosting en Pantalla

**Sintomas**: Imagenes residuales visibles

**Soluciones**:
1. El ghosting es normal en e-paper
2. Se reduce con cada actualizacion
3. Presionar RST para forzar refresco

### 9.4 Falla la Carga del Firmware

**Sintomas**: Error de timeout o conexion

**Soluciones**:
1. Usar modo bootloader (BOOT + RST)
2. Verificar cable USB (usar cable de datos, no solo carga)
3. Probar otro puerto USB

### 9.5 Hora Incorrecta

**Sintomas**: La hora mostrada no coincide

**Soluciones**:
1. Verificar configuracion de zona horaria
2. El formato debe ser POSIX valido
3. Verificar offset GMT y DST

---

## 10. Apendice

### 10.1 Codigos de Iconos del Clima

| Codigo | Descripcion |
|--------|-------------|
| 01d/01n | Cielo despejado |
| 02d/02n | Pocas nubes |
| 03d/03n | Nubes dispersas |
| 04d/04n | Muy nublado |
| 09d/09n | Lluvia ligera |
| 10d/10n | Lluvia |
| 11d/11n | Tormenta |
| 13d/13n | Nieve |
| 50d/50n | Niebla |

### 10.2 Zonas Horarias POSIX

Ejemplos comunes:
- Mexico Central: `CST6CDT,M4.1.0,M10.5.0`
- Mexico Pacifico: `MST7MDT,M4.1.0,M10.5.0`
- Espana: `CET-1CEST,M3.5.0,M10.5.0`
- Argentina: `ART3`
- Colombia: `COT5`

### 10.3 Escala de Calidad del Aire (ICA)

| ICA | Descripcion | Color |
|-----|-------------|-------|
| 1 | Buena | Verde |
| 2 | Aceptable | Amarillo |
| 3 | Moderada | Naranja |
| 4 | Mala | Rojo |
| 5 | Muy mala | Morado |

### 10.4 Escala UV

| UV | Descripcion |
|----|-------------|
| 0-2 | Bajo |
| 3-5 | Moderado |
| 6-7 | Alto |
| 8-10 | Muy alto |
| 11+ | Extremo |

---

## Creditos

- Codigo original: David Bird 2021
- Modificaciones: Stefan Maetschke 2025
- Version NoTouch y mejoras: XE1E 2026

## Licencia

MIT License

---

73 de XE1E
