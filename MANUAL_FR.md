# Manuel Station Meteo LilyGo EPD 4.7" - Version NoTouch

## Table des Matieres

1. [Introduction](#1-introduction)
2. [Specifications Techniques](#2-specifications-techniques)
3. [Architecture du Systeme](#3-architecture-du-systeme)
4. [Installation et Compilation](#4-installation-et-compilation)
5. [Configuration](#5-configuration)
6. [Utilisation de l'Appareil](#6-utilisation-de-lappareil)
7. [API OpenWeatherMap](#7-api-openweathermap)
8. [Gestion de l'Energie](#8-gestion-de-lenergie)
9. [Resolution de Problemes](#9-resolution-de-problemes)
10. [Annexe](#10-annexe)

---

## 1. Introduction

### 1.1 Description Generale

La Station Meteo LilyGo EPD 4.7" NoTouch est un appareil base sur ESP32-S3 qui affiche des informations meteorologiques en temps reel obtenues d'OpenWeatherMap. Elle utilise un ecran e-paper (encre electronique) de 4,7 pouces offrant une excellente visibilite dans toutes les conditions d'eclairage et une faible consommation d'energie.

Cette version **NoTouch** est concue pour les ecrans sans capacite tactile, affichant uniquement l'ecran meteo principal et entrant automatiquement en mode veille profonde.

### 1.2 Caracteristiques Principales

- **Ecran e-paper de 4,7 pouces** - 960x540 pixels, niveaux de gris
- **Multi-WiFi** - Support de jusqu'a 3 reseaux WiFi configurables
- **Mode AP (Point d'Acces)** - Portail captif pour configuration initiale
- **Veille Profonde** - Faible consommation pour fonctionnement sur batterie
- **Multi-langue** - Espagnol, Anglais et Francais
- **Mises a jour automatiques** - Intervalle configurable (5-120 minutes)
- **Indice UV et Qualite de l'Air** - Donnees supplementaires OpenWeatherMap

### 1.3 Differences avec la Version Tactile

Cette version **n'inclut pas**:
- Navigation tactile
- Ecrans secondaires (previsions detaillees, graphiques, historique)
- Stockage d'historique
- Support carte SD
- Configuration Bluetooth

---

## 2. Specifications Techniques

### 2.1 Materiel

#### Microcontroleur
| Parametre | Specification |
|-----------|---------------|
| Puce | ESP32-S3 |
| CPU | Dual-core Xtensa LX7 @ 240MHz |
| RAM | 512KB SRAM + 8MB PSRAM (OPI) |
| Flash | 16MB |
| WiFi | 802.11 b/g/n (2.4GHz) |

#### Ecran E-Paper
| Parametre | Specification |
|-----------|---------------|
| Type | E-Ink (encre electronique) |
| Taille | 4,7 pouces diagonale |
| Resolution | 960 x 540 pixels |
| Couleurs | 16 niveaux de gris |
| Technologie | ED047TC1 |
| Temps de rafraichissement | ~0,5 secondes |
| Angle de vision | ~180 degres |

#### Alimentation
| Parametre | Specification |
|-----------|---------------|
| Tension entree USB | 5V |
| Tension batterie | 3,7V LiPo (3,2V-4,2V) |
| Consommation active | ~150mA |
| Consommation veille | ~10uA |
| Pin ADC batterie | GPIO 14 |

#### Boutons
| Parametre | Specification |
|-----------|---------------|
| Bouton BOOT | GPIO 0 (mode bootloader) |
| Bouton RST | Reset materiel |

### 2.2 Technologie E-Paper

#### Fonctionnement

L'ecran e-paper utilise des microspheres bicolores suspendues dans un fluide. Lorsqu'une tension est appliquee, les particules blanches ou noires se deplacent vers la surface, creant l'image. Sans tension, l'image reste indefiniment.

#### Avantages
1. **Visibilite** - Parfaite en plein soleil
2. **Angle de vision** - Presque 180 degres
3. **Consommation** - N'utilise de l'energie qu'au changement d'image
4. **Confort visuel** - Pas de retroeclairage, ne fatigue pas les yeux

#### Limitations
1. **Vitesse** - Rafraichissement plus lent que LCD (~0,5s)
2. **Ghosting** - Des images residuelles peuvent rester
3. **Couleur** - Niveaux de gris uniquement
4. **Temperature** - Fonctionnement optimal 0-50C

---

## 3. Architecture du Systeme

### 3.1 Diagramme de Flux

```
+----------------+
|    DEMARRAGE   |
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
| FORCE_AP_MODE? |----Oui----> Mode AP
+----------------+              |
       |Non                     |
       v                        |
+----------------+              |
| StartWiFi()    |              |
+----------------+              |
       |                        |
  Connecte?                     |
  /        \                    |
Oui         Non-----------------+
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
|   DEMARRAGE    |
+----------------+
```

### 3.2 Structure des Fichiers

```
LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch/
|
+-- LilyGo-EPD-4-7-OWM-Weather-Display-NoTouch.ino  # Sketch principal
|
+-- owm_credentials.h     # Identifiants WiFi et API (valeurs par defaut)
|
+-- wifi_manager.h        # Mode AP, portail web, stockage NVS
|
+-- forecast_record.h     # Structure de donnees meteo
|
+-- lang.h                # Systeme multi-langue (ES/EN/FR)
|
+-- opensans*.h           # Polices
|
+-- moon.h                # Image de la lune
+-- sunrise.h             # Icone lever de soleil
+-- sunset.h              # Icone coucher de soleil
```

### 3.3 Stockage de Configuration (NVS)

La configuration est stockee dans le namespace "weather" des Preferences ESP32:

| Cle | Type | Description |
|-----|------|-------------|
| ssid1, pass1 | String | Reseau WiFi principal |
| ssid2, pass2 | String | Reseau WiFi secondaire |
| ssid3, pass3 | String | Reseau WiFi tertiaire |
| apikey | String | Cle API OpenWeatherMap |
| city | String | Nom de la ville |
| lat, lon | String | Coordonnees geographiques |
| lang | String | Langue (ES, EN, FR) |
| hemi | String | Hemisphere (north, south) |
| units | String | Unites (M=metrique, I=imperial) |
| tz | String | Fuseau horaire POSIX |
| gmt | Int | Decalage GMT en secondes |
| dst | Int | Decalage heure d'ete |
| updint | Int | Intervalle mise a jour (min) |

---

## 4. Installation et Compilation

### 4.1 Logiciels Requis

#### Arduino IDE
- Version 1.8.x ou 2.x

#### Board Manager
- **URL**: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
- **Paquet**: esp32 by Espressif Systems **version 2.0.17**

#### Bibliotheques Requises
Installer uniquement ces deux bibliotheques:

1. **EPD47-master**
   - URL: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip

2. **ArduinoJson**
   - Auteur: Benoit Blanchon
   - Version: 6.19.0

### 4.2 Configuration Arduino IDE

| Parametre | Valeur |
|-----------|--------|
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| USB DFU On Boot | Disable |
| Flash Size | 16MB (128Mb) |
| Flash Mode | QIO 80MHz |
| Partition Scheme | 16M Flash (3M APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |
| USB Mode | Hardware CDC and JTAG |

### 4.3 Processus de Compilation

1. Ouvrir le fichier `.ino` dans Arduino IDE
2. Selectionner le port COM correct
3. Cliquer sur "Verify" pour compiler
4. Cliquer sur "Upload" pour telecharger

### 4.4 Mode Bootloader (si echec du telechargement)

Si le telechargement echoue:
1. Appuyer et maintenir le bouton **BOOT**
2. En maintenant BOOT, appuyer sur **RST**
3. Relacher RST
4. Relacher BOOT
5. Reessayer le telechargement

---

## 5. Configuration

### 5.1 Configuration Initiale (Mode AP)

Au premier demarrage ou quand aucun WiFi n'est disponible:

1. L'appareil cree le reseau: **WeatherStation-Setup**
2. Se connecter avec mot de passe: **weather123**
3. Ouvrir navigateur: **http://192.168.4.1**
4. Completer le formulaire de configuration
5. Sauvegarder - l'appareil redemarre

### 5.2 Parametres de Configuration

#### Reseaux WiFi
- Jusqu'a 3 reseaux WiFi avec SSID et mot de passe
- L'appareil se connecte automatiquement au signal le plus fort

#### API OpenWeatherMap
- Obtenir une cle API gratuite sur: https://openweathermap.org/
- La cle gratuite permet ~1000 appels/jour

#### Localisation
- **Ville**: Nom a afficher sur l'ecran
- **Latitude/Longitude**: Coordonnees exactes pour donnees precises

#### Fuseau Horaire
- Format POSIX, exemple: `CET-1CEST,M3.5.0,M10.5.0`
- Le decalage GMT est calcule automatiquement

#### Options de Mise a Jour
- **Intervalle**: 5 a 120 minutes entre les mises a jour
- **Heure debut**: Heure de debut d'activite (0-23)
- **Heure fin**: Heure de suspension nocturne (0-23)

#### Langue
- Espagnol, Anglais ou Francais

#### Unites
- Metrique (Celsius, m/s, hPa)
- Imperial (Fahrenheit, mph, inHg)

---

## 6. Utilisation de l'Appareil

### 6.1 Fonctionnement Normal

1. L'appareil s'allume ou se reveille de la veille profonde
2. Se connecte au reseau WiFi le plus fort disponible
3. Synchronise l'heure via NTP
4. Obtient les donnees meteo d'OpenWeatherMap
5. Affiche les informations sur l'ecran
6. Entre en veille profonde jusqu'a la prochaine mise a jour

### 6.2 Ecran Principal

L'ecran affiche:

```
+----------------------------------------------------------+
|  Ville              Date et Heure          Batterie WiFi  |
+----------------------------------------------------------+
|                                                           |
|    [Boussole Vent]      TEMPERATURE            [Icone]    |
|                         Humidite%                         |
|    Direction            Max|Min  Indice UV                |
|    Vitesse                                                |
|                         Description meteo                 |
|    [Soleil/Lune]        Pression hPa                      |
|    Lever                                                  |
|    Coucher              Ressenti                          |
|    Phase lunaire                                          |
|                                                           |
|    +----------+  +----------+  +----------+               |
|    | Prevision|  | Prevision|  | Prevision|               |
|    | Jour 1   |  | Jour 2   |  | Jour 3   |               |
|    +----------+  +----------+  +----------+               |
+----------------------------------------------------------+
```

### 6.3 Indicateurs d'Etat

- **Batterie**: Niveau de charge de la batterie LiPo
- **WiFi**: Intensite du signal en dB

---

## 7. API OpenWeatherMap

### 7.1 Points de Terminaison Utilises

| Point de terminaison | Description |
|----------------------|-------------|
| /data/2.5/weather | Meteo actuelle |
| /data/2.5/forecast | Previsions 5 jours |
| /data/2.5/uvi | Indice UV |
| /data/2.5/air_pollution | Qualite de l'air |

### 7.2 Donnees Recuperees

- Temperature actuelle, max et min
- Temperature ressentie
- Humidite relative
- Pression atmospherique et tendance
- Vitesse et direction du vent
- Probabilite de precipitation
- Couverture nuageuse
- Visibilite
- Lever et coucher du soleil
- Indice UV
- Indice de Qualite de l'Air (IQA)

### 7.3 Limites de l'API Gratuite

- ~1000 appels par jour
- Donnees mises a jour toutes les 10 minutes sur le serveur
- Previsions jusqu'a 5 jours

---

## 8. Gestion de l'Energie

### 8.1 Modes d'Energie

| Mode | Consommation | Description |
|------|--------------|-------------|
| Actif | ~150mA | Traitement, WiFi actif |
| Veille Profonde | ~10uA | Seul RTC actif |

### 8.2 Cycle d'Energie

1. **Reveil** - Timer ou reset manuel
2. **Actif** - WiFi, API, affichage (~5-10 secondes)
3. **Veille Profonde** - Jusqu'a prochaine mise a jour

### 8.3 Autonomie de la Batterie

Avec batterie LiPo 2000mAh:
- Intervalle 30 min: ~2-3 mois
- Intervalle 60 min: ~4-6 mois

### 8.4 Heures d'Activite

Configurer les heures de debut et de fin pour suspendre les mises a jour pendant la nuit et economiser la batterie.

---

## 9. Resolution de Problemes

### 9.1 Ne Se Connecte Pas au WiFi

**Symptomes**: Entre constamment en mode AP

**Solutions**:
1. Verifier que le SSID et mot de passe sont corrects
2. Verifier que le reseau est a portee
3. Le reseau doit etre 2,4GHz (5GHz non supporte)

### 9.2 Pas de Donnees Meteo

**Symptomes**: Ecran vide ou anciennes donnees

**Solutions**:
1. Verifier la cle API OpenWeatherMap
2. Verifier les coordonnees (lat/lon)
3. Verifier la connexion internet

### 9.3 Ghosting sur l'Ecran

**Symptomes**: Images residuelles visibles

**Solutions**:
1. Le ghosting est normal pour e-paper
2. Se reduit a chaque mise a jour
3. Appuyer sur RST pour forcer le rafraichissement

### 9.4 Echec du Telechargement du Firmware

**Symptomes**: Erreur de timeout ou connexion

**Solutions**:
1. Utiliser le mode bootloader (BOOT + RST)
2. Verifier le cable USB (utiliser cable de donnees, pas charge seule)
3. Essayer un autre port USB

### 9.5 Heure Incorrecte

**Symptomes**: L'heure affichee ne correspond pas

**Solutions**:
1. Verifier la configuration du fuseau horaire
2. Le format doit etre POSIX valide
3. Verifier les decalages GMT et DST

---

## 10. Annexe

### 10.1 Codes d'Icones Meteo

| Code | Description |
|------|-------------|
| 01d/01n | Ciel degage |
| 02d/02n | Quelques nuages |
| 03d/03n | Nuages epars |
| 04d/04n | Tres nuageux |
| 09d/09n | Pluie legere |
| 10d/10n | Pluie |
| 11d/11n | Orage |
| 13d/13n | Neige |
| 50d/50n | Brouillard |

### 10.2 Fuseaux Horaires POSIX

Exemples courants:
- France: `CET-1CEST,M3.5.0,M10.5.0`
- Belgique: `CET-1CEST,M3.5.0,M10.5.0`
- Suisse: `CET-1CEST,M3.5.0,M10.5.0`
- Quebec: `EST5EDT,M3.2.0,M11.1.0`
- Maroc: `WET0`

### 10.3 Echelle de Qualite de l'Air (IQA)

| IQA | Description | Couleur |
|-----|-------------|---------|
| 1 | Bonne | Vert |
| 2 | Acceptable | Jaune |
| 3 | Moderee | Orange |
| 4 | Mauvaise | Rouge |
| 5 | Tres mauvaise | Violet |

### 10.4 Echelle UV

| UV | Description |
|----|-------------|
| 0-2 | Faible |
| 3-5 | Modere |
| 6-7 | Eleve |
| 8-10 | Tres eleve |
| 11+ | Extreme |

---

## Credits

- Code original: David Bird 2021
- Modifications: Stefan Maetschke 2025
- Version NoTouch et ameliorations: XE1E 2026

## Licence

MIT License

---

73 de XE1E
