# Manual Estacion Meteorologica LilyGo EPD 4.7" - Version WeatherAPI

## Tabla de Contenidos

1. [Introduccion](#1-introduccion)
2. [Especificaciones Tecnicas](#2-especificaciones-tecnicas)
3. [Arquitectura del Sistema](#3-arquitectura-del-sistema)
4. [Instalacion y Compilacion](#4-instalacion-y-compilacion)
5. [Configuracion](#5-configuracion)
6. [Uso del Dispositivo](#6-uso-del-dispositivo)
7. [API WeatherAPI.com](#7-api-weatherapicom)
8. [Gestion de Energia](#8-gestion-de-energia)
9. [Solucion de Problemas](#9-solucion-de-problemas)
10. [Apendice](#10-apendice)

---

## 1. Introduccion

### 1.1 Descripcion General

La Estacion Meteorologica LilyGo EPD 4.7" es un dispositivo basado en ESP32-S3 que muestra informacion meteorologica en tiempo real obtenida de **WeatherAPI.com**. Utiliza una pantalla e-paper (tinta electronica) de 4.7 pulgadas que ofrece excelente visibilidad bajo cualquier condicion de luz y bajo consumo de energia.

Esta version **NoTouch** esta disenada para pantallas sin capacidad tactil, mostrando unicamente la pantalla principal del clima y entrando en modo deep sleep automaticamente.

### 1.2 Caracteristicas Principales

- **Pantalla e-paper de 4.7 pulgadas** - 960x540 pixeles, escala de grises
- **Multi-WiFi** - Soporte para hasta 3 redes WiFi configurables
- **Modo AP (Access Point)** - Portal cautivo para configuracion inicial
- **Deep Sleep** - Bajo consumo para operacion con bateria
- **Multi-idioma** - Espanol, Ingles y Frances
- **Actualizacion automatica** - Intervalo configurable (5-120 minutos)
- **Indice UV y Calidad del Aire** - Datos de WeatherAPI incluidos en una sola llamada
- **Fase Lunar Real** - Datos astronomicos directos de la API
- **Amanecer/Atardecer** - Horarios reales de la ubicacion

### 1.3 Ventajas de WeatherAPI vs OpenWeatherMap

| Caracteristica | WeatherAPI | OpenWeatherMap |
|---------------|------------|----------------|
| Llamadas API | 1 (todo incluido) | 3+ (separadas) |
| Fase Lunar | Datos reales | Calculada |
| Capa Gratuita | 1M llamadas/mes | 1K llamadas/dia |
| Calidad del Aire | Incluida | API separada |
| Alertas | Disponibles | API separada |

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
| Tiempo de refresco | ~0.5 segundos |

#### Alimentacion
| Parametro | Especificacion |
|-----------|----------------|
| Voltaje entrada USB | 5V |
| Voltaje bateria | 3.7V LiPo |
| Consumo activo | ~150mA |
| Consumo deep sleep | ~10uA |

### 2.2 Software

| Componente | Version |
|------------|---------|
| Arduino Core ESP32 | 2.0.17 |
| ArduinoJson | 6.19.0 |
| EPD47-master | Latest |

---

## 3. Arquitectura del Sistema

### 3.1 Flujo Principal

```
Inicio/Despertar
       |
       v
  Inicializar Sistema
       |
       v
  Cargar Configuracion (NVS)
       |
       v
  Escanear Redes WiFi
       |
       +---> Sin red conocida ---> Modo AP (Portal Web)
       |
       v
  Conectar a WiFi
       |
       v
  Obtener Datos (WeatherAPI)
       |
       v
  Actualizar Pantalla
       |
       v
  Deep Sleep (X minutos)
```

### 3.2 Estructura de Archivos

```
LilyGo-EPD-4-7-WeatherAPI-Display/
|
+-- LilyGo-EPD-4-7-WeatherAPI-Display.ino  # Sketch principal
|
+-- owm_credentials.h     # Credenciales WiFi y API (valores por defecto)
+-- wifi_manager.h        # Portal web y modo AP
+-- forecast_record.h     # Estructuras de datos del clima
+-- lang.h                # Cadenas multi-idioma
|
+-- opensans*.h           # Fuentes (tamanos 8-24)
+-- moon.h, sunrise.h, sunset.h  # Iconos bitmap
```

### 3.3 Datos del Clima

WeatherAPI proporciona todos los datos en una sola llamada:

| Campo | Descripcion | Unidad |
|-------|-------------|--------|
| Temperature | Temperatura actual | C / F |
| Feelslike | Sensacion termica | C / F |
| Humidity | Humedad relativa | % |
| Pressure | Presion atmosferica | mb / hPa |
| Wind | Velocidad y direccion | kph / mph |
| UV Index | Indice ultravioleta | 0-11+ |
| AQI | Indice de calidad del aire | 1-6 |
| Sunrise/Sunset | Horarios solares | HH:MM AM/PM |
| Moon Phase | Fase lunar | Texto + % iluminacion |
| Forecast | Pronostico 3 dias | Por hora |

---

## 4. Instalacion y Compilacion

### 4.1 Requisitos

1. **Arduino IDE** 1.8.x o 2.x
2. **ESP32 Board Support** version 2.0.17
3. **Librerias requeridas**

### 4.2 Instalacion de Librerias

#### ESP32 Board Manager
1. Abrir Arduino IDE
2. Ir a Archivo > Preferencias
3. En "URLs adicionales de gestor de tarjetas":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Ir a Herramientas > Placa > Gestor de tarjetas
5. Buscar "esp32" e instalar version **2.0.17**

#### EPD47-master
1. Descargar: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip
2. En Arduino IDE: Sketch > Incluir libreria > Agregar libreria .ZIP
3. Seleccionar el archivo descargado

#### ArduinoJson
1. Herramientas > Gestionar librerias
2. Buscar "ArduinoJson"
3. Instalar version **6.19.0** de Benoit Blanchon

### 4.3 Configuracion Arduino IDE

| Opcion | Valor |
|--------|-------|
| Placa | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| USB DFU On Boot | Disable |
| Flash Size | 16MB (128Mb) |
| Flash Mode | QIO 80MHz |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |
| USB Mode | Hardware CDC and JTAG |

### 4.4 Subir Firmware

1. Conectar dispositivo via USB
2. Seleccionar puerto COM correcto
3. Compilar y subir

**Si falla la subida:**
1. Mantener presionado BOOT
2. Sin soltar BOOT, presionar RST
3. Soltar RST
4. Soltar BOOT
5. Intentar subir nuevamente

---

## 5. Configuracion

### 5.1 Portal Web (Modo AP)

Cuando no hay red WiFi configurada o disponible, el dispositivo crea un punto de acceso:

- **SSID:** WeatherStation-Setup
- **Password:** weather123
- **URL:** http://192.168.4.1

### 5.2 Opciones de Configuracion

#### Pestana WiFi
| Campo | Descripcion |
|-------|-------------|
| SSID 1-3 | Nombre de la red WiFi |
| Password 1-3 | Contrasena de la red |

#### Pestana Clima
| Campo | Descripcion |
|-------|-------------|
| API Key | Clave de WeatherAPI.com |
| Ciudad | Nombre para mostrar |
| Latitud | Coordenada (-90 a 90) |
| Longitud | Coordenada (-180 a 180) |
| Zona Horaria | Formato POSIX (ej: CST6CDT) |

#### Pestana Sistema
| Campo | Descripcion |
|-------|-------------|
| Idioma | ES, EN, FR |
| Unidades | M (Metrico), I (Imperial) |
| Intervalo | Minutos entre actualizaciones |

### 5.3 Obtener API Key de WeatherAPI

1. Ir a [weatherapi.com](https://www.weatherapi.com/)
2. Crear cuenta gratuita
3. Ir al Dashboard
4. Copiar la API Key
5. Pegarla en la configuracion web

---

## 6. Uso del Dispositivo

### 6.1 Pantalla Principal

La pantalla muestra:

```
+--------------------------------------------------+
|  Ciudad, Region                    Fecha y Hora  |
+--------------------------------------------------+
|                                                  |
|   [Icono]   Temperatura    Humedad%              |
|             Max/Min        Indice UV             |
|                                                  |
|   Rosa de   Presion        Visibilidad           |
|   Vientos   Tendencia      Sensacion   ICA       |
|                                                  |
|   Luna      Amanecer                             |
|   Fase      Atardecer                            |
|                                                  |
+--------------------------------------------------+
|  [Pronostico por horas - 7 periodos de 3h]       |
+--------------------------------------------------+
|  [Graficas: Temp | Presion | Humedad | Precip]   |
+--------------------------------------------------+
```

### 6.2 Iconos del Clima

| Icono | Condicion |
|-------|-----------|
| Sol | Despejado |
| Sol/Nube | Parcialmente nublado |
| Nubes | Nublado |
| Lluvia | Lluvia |
| Tormenta | Tormenta electrica |
| Nieve | Nevada |
| Niebla | Niebla/Neblina |

### 6.3 Indicadores

- **Rosa de Vientos:** Direccion y velocidad del viento actual
- **Fase Lunar:** Icono y nombre de la fase (datos reales de WeatherAPI)
- **ICA (Indice Calidad Aire):** 1-Bueno a 6-Peligroso

---

## 7. API WeatherAPI.com

### 7.1 Endpoint Utilizado

```
https://api.weatherapi.com/v1/forecast.json
  ?key={API_KEY}
  &q={LAT},{LON}
  &days=3
  &aqi=yes
  &lang={LANG}
```

### 7.2 Datos Recibidos

Una sola llamada incluye:
- **location** - Informacion de ubicacion
- **current** - Condiciones actuales + calidad del aire
- **forecast** - 3 dias con datos por hora
- **astro** - Datos astronomicos (sol, luna)

### 7.3 Limites del Plan Gratuito

| Limite | Valor |
|--------|-------|
| Llamadas/mes | 1,000,000 |
| Dias pronostico | 3 |
| Historial | No incluido |
| Alertas | Disponibles |

### 7.4 Codigos de Condicion

WeatherAPI usa codigos numericos (1000, 1003, etc.) que se mapean a iconos internos compatibles con el sistema de iconos original.

---

## 8. Gestion de Energia

### 8.1 Modos de Operacion

| Modo | Consumo | Duracion |
|------|---------|----------|
| Activo | ~150mA | ~10-15 segundos |
| Deep Sleep | ~10uA | Configurable |

### 8.2 Autonomia Estimada

Con bateria de 2000mAh:
- Actualizacion cada 60 min: ~6 meses
- Actualizacion cada 30 min: ~3 meses
- Actualizacion cada 15 min: ~6 semanas

### 8.3 Indicador de Bateria

Ubicado en esquina superior derecha:
- Verde: >75%
- Amarillo: 25-75%
- Rojo: <25%

---

## 9. Solucion de Problemas

### 9.1 Problemas Comunes

| Problema | Solucion |
|----------|----------|
| No sube firmware | Usar modo bootloader (BOOT + RST) |
| No conecta WiFi | Verificar 2.4GHz, acercar al router |
| Sin datos clima | Verificar API key en weatherapi.com |
| Error NoMemory | Normal, se reintenta automaticamente |
| Pantalla no actualiza | Verificar alimentacion |

### 9.2 Mensajes de Error Serial

| Mensaje | Causa | Solucion |
|---------|-------|----------|
| Connection failed 401/403 | API key invalida | Verificar key |
| deserializeJson NoMemory | Buffer insuficiente | Usar 64KB buffer |
| WiFi connection failed | Sin red disponible | Verificar credenciales |

### 9.3 Reinicio de Fabrica

Para borrar toda la configuracion:
1. Agregar en setup():
   ```cpp
   Preferences prefs;
   prefs.begin("weather", false);
   prefs.clear();
   prefs.end();
   ```
2. Subir, ejecutar una vez, quitar el codigo

---

## 10. Apendice

### 10.1 Zonas Horarias POSIX

| Region | Codigo |
|--------|--------|
| Mexico Centro | CST6CDT |
| Mexico Pacifico | MST7MDT |
| Espana | CET-1CEST |
| Argentina | ART3 |
| Colombia | COT5 |

### 10.2 Codigos de Idioma

| Codigo | Idioma |
|--------|--------|
| ES | Espanol |
| EN | Ingles |
| FR | Frances |

### 10.3 Creditos

- Proyecto original: David Bird
- Port ESP32: LilyGo
- Adaptacion WeatherAPI: XE1E
- Datos meteorologicos: [WeatherAPI.com](https://www.weatherapi.com/)

---

*Manual version 1.0 - WeatherAPI*
