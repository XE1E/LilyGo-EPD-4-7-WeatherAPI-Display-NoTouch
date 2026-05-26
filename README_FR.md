# LilyGo EPD 4.7" WeatherAPI Display (NoTouch)

![Weather Station Display](docs/weather-station.jpg)

**[Espanol](README.md)** | **[English](README_EN.md)**

Station meteo ESP32-S3 utilisant l'ecran e-paper LilyGo T5 4.7". Recupere les donnees meteo de **WeatherAPI.com** et affiche les conditions actuelles plus les previsions sur 3 jours.

## Fonctionnalites

- **Meteo Actuelle** - Temperature, humidite, pression, vitesse/direction du vent, indice UV, qualite de l'air (IQA)
- **Previsions 3 Jours** - Temperatures horaires avec icones meteo
- **Donnees Astronomiques** - Phase lunaire, lever/coucher du soleil (donnees reelles de WeatherAPI)
- **Graphiques de Tendance** - Temperature, pression, humidite, precipitation sur 3 jours
- **Configuration Web** - Configuration facile via portail captif (sans modification de code)
- **Multi-Langue** - Espagnol, Anglais, Francais
- **Multi-WiFi** - Se connecte au reseau le plus fort disponible
- **Deep Sleep** - Efficace en batterie, intervalle de mise a jour configurable
- **Sans Touch** - Ecran unique, pas de navigation necessaire

## Materiel

- **LilyGo T5 4.7" E-Paper (ESP32-S3)** - [Lien Produit](http://www.lilygo.cc/products/t5-4-7-inch-e-paper-v2-3)
- Ecran: 960x540 pixels, 16 niveaux de gris
- C'est la version **sans touch**

## Demarrage Rapide

### Configuration Arduino IDE

| Parametre | Valeur |
|-----------|--------|
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| Flash Size | 16MB (128Mb) |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |

### Bibliotheques Requises

| Bibliotheque | Version | Source |
|--------------|---------|--------|
| esp32 (Board Manager) | 2.0.17 | Espressif Systems |
| EPD47-master | Derniere | [Telecharger ZIP](https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip) |
| ArduinoJson | 6.19.0 | Benoit Blanchon |

**Important:** N'installez que EPD47-master et ArduinoJson dans le dossier des bibliotheques pour eviter les conflits.

**Alternative: Installer via Web (sans Arduino IDE)**

[![Installer Firmware](https://img.shields.io/badge/Installer-Firmware-blue?style=for-the-badge)](https://xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/)

Utiliser Chrome, Edge ou Opera et connecter l'appareil via USB.

### Mises a Jour du Firmware (OTA)

| Methode | URL / Comment utiliser |
|---------|------------------------|
| **Web OTA** | `http://[IP_APPAREIL]/ota` - Televerser .bin depuis le navigateur |
| **Arduino OTA** | Selectionner le port reseau "WeatherStation-NoTouch" dans Arduino IDE |
| **Web Flasher** | [xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch](https://xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/) |
| **Releases** | [Telecharger fichiers .bin](https://github.com/XE1E/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/releases) |

### Configuration Initiale

1. Televerser le sketch sur l'appareil
2. L'appareil cree un hotspot WiFi: **WeatherStation-Setup** (mot de passe: `weather123`)
3. Se connecter au hotspot et ouvrir `http://192.168.4.1`
4. Configurer les identifiants WiFi, la cle API WeatherAPI, et l'emplacement
5. Sauvegarder et l'appareil redemarrera

### Obtenir votre Cle API

1. Aller sur [weatherapi.com](https://www.weatherapi.com/)
2. Creer un compte gratuit
3. Copier votre cle API depuis le tableau de bord
4. L'entrer dans la configuration web

### Fonctionnement Normal

1. Se reveille du deep sleep
2. Se connecte au reseau WiFi le plus fort configure
3. Recupere la meteo de WeatherAPI.com (actuel + previsions 3 jours + IQA)
4. Met a jour l'ecran e-paper
5. Retourne en deep sleep

## Options de Configuration

| Option | Description | Exemple |
|--------|-------------|---------|
| WiFi SSID/Password | Jusqu'a 3 reseaux | MonReseau / monmotdepasse |
| Cle API | Cle API WeatherAPI.com | abc123def456... |
| Ville | Nom de l'emplacement (affichage seulement) | Paris |
| Latitude | Latitude de l'emplacement | 48.8566 |
| Longitude | Longitude de l'emplacement | 2.3522 |
| Fuseau Horaire | Chaine POSIX de fuseau horaire | CET-1CEST |
| Intervalle | Minutes entre les mises a jour | 60 |
| Langue | ES, EN, ou FR | FR |
| Unites | M (Metrique) ou I (Imperial) | M |

## Depannage

| Probleme | Solution |
|----------|----------|
| Echec du televersement | Maintenir BOOT, appuyer RST, relacher RST, relacher BOOT, puis televerser |
| Pas de connexion WiFi | Verifier les identifiants, s'assurer d'un reseau 2.4GHz, se rapprocher du routeur |
| Pas de donnees meteo | Verifier la cle API sur weatherapi.com, verifier la connexion internet |
| Ecran ne se met pas a jour | Verifier l'intervalle de deep sleep, verifier l'alimentation |
| Erreur "NoMemory" | Normal pour les reponses tres grandes, alertes desactivees pour economiser la memoire |

## Fichiers du Projet

| Fichier | Description |
|---------|-------------|
| `LilyGo-EPD-4-7-WeatherAPI-Display.ino` | Sketch principal |
| `owm_credentials.h` | Configuration par defaut WiFi/API |
| `wifi_manager.h` | Portail web et mode AP |
| `forecast_record.h` | Structures de donnees meteorologiques |
| `lang.h` | Chaines multi-langue |

## Details Techniques

| Specification | Valeur |
|---------------|--------|
| Ecran | 960x540 pixels, 16 gris |
| MCU | ESP32-S3 avec PSRAM |
| Consommation | ~15mA actif, ~10uA deep sleep |
| Appels API | 1 par mise a jour (inclut actuel + previsions + IQA) |
| Reponse Max | ~50KB JSON (buffer 64KB) |

## WeatherAPI vs OpenWeatherMap

Ce projet utilise **WeatherAPI.com** au lieu d'OpenWeatherMap:

| Fonctionnalite | WeatherAPI | OpenWeatherMap |
|----------------|------------|----------------|
| Appels API | 1 (toutes les donnees) | 3+ (actuel, previsions, IQA) |
| Phase Lunaire | Donnees reelles | Calculee |
| Lever/Coucher | Format chaine | Timestamp Unix |
| Tier Gratuit | 1M appels/mois | 1K appels/jour |
| Alertes | Oui (desactivees pour memoire) | API separee |

## Documentation

- [Manual Espanol](MANUAL_ES.md)
- [English Manual](MANUAL_EN.md)
- [Manuel Francais](MANUAL_FR.md)

## Licence

Licence MIT - Voir [Licence.txt](Licence.txt)

## Credits

- Projet original par David Bird
- Port ESP32 par LilyGo
- Adaptation WeatherAPI par XE1E
- Donnees meteo de [WeatherAPI.com](https://www.weatherapi.com/)
