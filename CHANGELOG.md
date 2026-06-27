# Changelog

Todos los cambios notables de este proyecto se documentan en este archivo.

El formato está basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/)
y el proyecto sigue un versionado de tipo `MAYOR.MENOR`.

## [2.9] - 2026-06-26

### Añadido
- Escaneo de redes WiFi y botón para mostrar/ocultar contraseña en el portal de configuración.
- Mejoras en el flujo de configuración web.
- Guía de instalación y mejoras de visibilidad del **Web Flasher**.

### Cambiado
- Pantalla del modo AP: título subido 15px para un mejor espaciado vertical.
- Sketch renombrado para coincidir con el nombre de la carpeta (compatibilidad con Arduino IDE); workflow ajustado al nuevo nombre.

### Corregido
- Corregido el timeout de conexión WiFi.

### Notas
- Se intentó traducir los mensajes de la herramienta esp-web-tools del Web Flasher; el cambio se revirtió, por lo que esos mensajes permanecen en inglés.

## [2.8] - 2026-05-26

- Primera versión etiquetada y publicada con compilación automática de firmware (CI) y Web Flasher.
- Estación meteorológica para LilyGo EPD 4.7" (versión sin táctil) con integración WeatherAPI.com, soporte multi-WiFi, portal cautivo en modo AP, actualización OTA y deep sleep para operación con batería.

[2.9]: https://github.com/XE1E/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/releases/tag/v2.9
[2.8]: https://github.com/XE1E/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/releases/tag/v2.8
