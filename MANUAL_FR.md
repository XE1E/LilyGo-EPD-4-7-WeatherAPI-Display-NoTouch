# Manuel Station Meteo LilyGo EPD 4.7" - Version WeatherAPI

## Table des Matieres

1. [Introduction](#1-introduction)
2. [Specifications Techniques](#2-specifications-techniques)
3. [Architecture du Systeme](#3-architecture-du-systeme)
4. [Installation et Compilation](#4-installation-et-compilation)
5. [Configuration](#5-configuration)
6. [Utilisation de l'Appareil](#6-utilisation-de-lappareil)
7. [API WeatherAPI.com](#7-api-weatherapicom)
8. [Gestion de l'Energie](#8-gestion-de-lenergie)
9. [Resolution de Problemes](#9-resolution-de-problemes)
10. [Annexe](#10-annexe)

---

## 1. Introduction

### 1.1 Description Generale

La Station Meteo LilyGo EPD 4.7" est un appareil base sur ESP32-S3 qui affiche des informations meteorologiques en temps reel obtenues de **WeatherAPI.com**. Elle utilise un ecran e-paper (encre electronique) de 4,7 pouces offrant une excellente visibilite dans toutes les conditions d'eclairage et une faible consommation d'energie.

Cette version **NoTouch** est concue pour les ecrans sans capacite tactile, affichant uniquement l'ecran meteo principal et entrant automatiquement en mode veille profonde.

### 1.2 Caracteristiques Principales

- **Ecran e-paper de 4,7 pouces** - 960x540 pixels, niveaux de gris
- **Multi-WiFi** - Support jusqu'a 3 reseaux WiFi configurables
- **Mode AP (Point d'Acces)** - Portail captif pour la configuration initiale
- **Veille profonde** - Faible consommation pour le fonctionnement sur batterie
- **Multi-langue** - Francais, Espagnol et Anglais
- **Mise a jour automatique** - Intervalle configurable (5-120 minutes)
- **Indice UV et Qualite de l'Air** - Inclus dans un seul appel WeatherAPI
- **Phase Lunaire Reelle** - Donnees astronomiques directement de l'API
- **Lever/Coucher du Soleil** - Horaires reels pour votre emplacement

### 1.3 Avantages WeatherAPI vs OpenWeatherMap

| Caracteristique | WeatherAPI | OpenWeatherMap |
|-----------------|------------|----------------|
| Appels API | 1 (tout inclus) | 3+ (separes) |
| Phase Lunaire | Donnees reelles | Calculee |
| Forfait Gratuit | 1M appels/mois | 1K appels/jour |
| Qualite de l'Air | Incluse | API separee |
| Alertes | Disponibles | API separee |

---

## 2. Specifications Techniques

### 2.1 Materiel

#### Microcontroleur
| Parametre | Specification |
|-----------|---------------|
| Puce | ESP32-S3 |
| CPU | Double coeur Xtensa LX7 @ 240MHz |
| RAM | 512KB SRAM + 8MB PSRAM (OPI) |
| Flash | 16MB |
| WiFi | 802.11 b/g/n (2,4GHz) |

#### Ecran E-Paper
| Parametre | Specification |
|-----------|---------------|
| Type | E-Ink (encre electronique) |
| Taille | 4,7 pouces diagonale |
| Resolution | 960 x 540 pixels |
| Couleurs | 16 niveaux de gris |
| Temps de rafraichissement | ~0,5 secondes |

#### Alimentation
| Parametre | Specification |
|-----------|---------------|
| Tension d'entree USB | 5V |
| Tension batterie | 3,7V LiPo |
| Consommation active | ~150mA |
| Consommation veille profonde | ~10uA |

### 2.2 Logiciel

| Composant | Version |
|-----------|---------|
| Arduino Core ESP32 | 2.0.17 |
| ArduinoJson | 6.19.0 |
| EPD47-master | Derniere |

---

## 3. Architecture du Systeme

### 3.1 Flux Principal

```
Demarrage/Reveil
       |
       v
  Initialiser Systeme
       |
       v
  Charger Configuration (NVS)
       |
       v
  Scanner Reseaux WiFi
       |
       +---> Aucun reseau connu ---> Mode AP (Portail Web)
       |
       v
  Connecter au WiFi
       |
       v
  Obtenir Donnees (WeatherAPI)
       |
       v
  Mettre a Jour l'Ecran
       |
       v
  Veille Profonde (X minutes)
```

### 3.2 Structure des Fichiers

```
LilyGo-EPD-4-7-WeatherAPI-Display/
|
+-- LilyGo-EPD-4-7-WeatherAPI-Display.ino  # Sketch principal
|
+-- owm_credentials.h     # Identifiants WiFi et API (valeurs par defaut)
+-- wifi_manager.h        # Portail web et mode AP
+-- forecast_record.h     # Structures de donnees meteo
+-- lang.h                # Chaines multi-langues
|
+-- opensans*.h           # Polices (tailles 8-24)
+-- moon.h, sunrise.h, sunset.h  # Icones bitmap
```

### 3.3 Donnees Meteorologiques

WeatherAPI fournit toutes les donnees en un seul appel:

| Champ | Description | Unite |
|-------|-------------|-------|
| Temperature | Temperature actuelle | C / F |
| Ressenti | Temperature ressentie | C / F |
| Humidite | Humidite relative | % |
| Pression | Pression atmospherique | mb / hPa |
| Vent | Vitesse et direction | kph / mph |
| Indice UV | Indice ultraviolet | 0-11+ |
| IQA | Indice qualite de l'air | 1-6 |
| Lever/Coucher | Horaires solaires | HH:MM AM/PM |
| Phase Lunaire | Phase de la lune | Texte + % illumination |
| Previsions | Previsions 3 jours | Par heure |

---

## 4. Installation et Compilation

### 4.1 Prerequis

1. **Arduino IDE** 1.8.x ou 2.x
2. **Support Carte ESP32** version 2.0.17
3. **Bibliotheques requises**

### 4.2 Installation des Bibliotheques

#### Gestionnaire de Cartes ESP32
1. Ouvrir Arduino IDE
2. Aller a Fichier > Preferences
3. Dans "URLs de gestionnaire de cartes supplementaires":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Aller a Outils > Carte > Gestionnaire de cartes
5. Rechercher "esp32" et installer version **2.0.17**

#### EPD47-master
1. Telecharger: https://github.com/DFRobotdl/EPD47/archive/refs/heads/master.zip
2. Dans Arduino IDE: Croquis > Inclure bibliotheque > Ajouter bibliotheque .ZIP
3. Selectionner le fichier telecharge

#### ArduinoJson
1. Outils > Gerer les bibliotheques
2. Rechercher "ArduinoJson"
3. Installer version **6.19.0** de Benoit Blanchon

### 4.3 Configuration Arduino IDE

| Option | Valeur |
|--------|--------|
| Carte | ESP32S3 Dev Module |
| USB CDC On Boot | Enable |
| USB DFU On Boot | Disable |
| Flash Size | 16MB (128Mb) |
| Flash Mode | QIO 80MHz |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0/Hardware CDC |
| USB Mode | Hardware CDC and JTAG |

### 4.4 Televersement du Firmware (USB)

1. Connecter l'appareil via USB
2. Selectionner le bon port COM
3. Compiler et televerser

**Si le televersement echoue:**
1. Maintenir le bouton BOOT
2. Sans relacher BOOT, appuyer sur RST
3. Relacher RST
4. Relacher BOOT
5. Reessayer le televersement

### 4.5 Mises a Jour OTA (Over-The-Air)

Le firmware peut etre mis a jour sans fil, sans cable USB.

#### Methode 1: Web OTA (Recommande)

Mise a jour depuis le navigateur pendant que l'appareil est connecte au WiFi:

1. Se connecter au meme reseau WiFi que l'appareil
2. Ouvrir dans le navigateur: `http://[IP_APPAREIL]/ota`
3. Glisser le fichier `.bin` ou cliquer pour selectionner
4. Cliquer sur "Update Firmware"
5. Attendre la fin (ne pas deconnecter pendant le processus)
6. L'appareil redemarrera automatiquement

**Note**: L'IP de l'appareil est affichee sur l'ecran d'information.

#### Methode 2: Arduino OTA

Mise a jour directement depuis Arduino IDE via WiFi:

1. S'assurer que l'appareil et le PC sont sur le meme reseau
2. Dans Arduino IDE: `Outils` → `Port`
3. Selectionner "WeatherStation-NoTouch at [IP]" (apparait comme port reseau)
4. Cliquer sur Televerser comme d'habitude

**Prerequis**:
- Appareil allume et connecte au WiFi
- PC sur le meme reseau local
- Arduino IDE avec support ESP32

#### Methode 3: Web Flasher (GitHub)

Flasher depuis le navigateur sans rien installer:

1. Visiter: `https://xe1e.github.io/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/`
2. Connecter l'appareil via USB
3. Cliquer sur "Instalar Firmware"
4. Selectionner le port serie
5. Attendre la fin de l'installation

**Prerequis**:
- Navigateur Chrome, Edge ou Opera (necessite Web Serial API)
- Cable USB connecte a l'appareil

#### Telecharger le Firmware

Les fichiers .bin compiles sont disponibles sur:
`https://github.com/XE1E/LilyGo-EPD-4-7-WeatherAPI-Display-NoTouch/releases`

---

## 5. Configuration

### 5.1 Portail Web (Mode AP)

Lorsqu'aucun reseau WiFi n'est configure ou disponible, l'appareil cree un point d'acces:

- **SSID:** WeatherStation-Setup
- **Mot de passe:** weather123
- **URL:** http://192.168.4.1

### 5.2 Options de Configuration

#### Onglet WiFi
| Champ | Description |
|-------|-------------|
| SSID 1-3 | Nom du reseau WiFi |
| Mot de passe 1-3 | Mot de passe du reseau |

#### Onglet Meteo
| Champ | Description |
|-------|-------------|
| Cle API | Cle WeatherAPI.com |
| Ville | Nom a afficher |
| Latitude | Coordonnee (-90 a 90) |
| Longitude | Coordonnee (-180 a 180) |
| Fuseau horaire | Format POSIX (ex: CET-1CEST) |

#### Onglet Systeme
| Champ | Description |
|-------|-------------|
| Langue | ES, EN, FR |
| Unites | M (Metrique), I (Imperial) |
| Intervalle | Minutes entre mises a jour |

### 5.3 Obtenir une Cle API WeatherAPI

1. Aller sur [weatherapi.com](https://www.weatherapi.com/)
2. Creer un compte gratuit
3. Aller au Tableau de bord
4. Copier la Cle API
5. La coller dans la configuration web

---

## 6. Utilisation de l'Appareil

### 6.1 Ecran Principal

L'ecran affiche:

```
+--------------------------------------------------+
|  Ville, Region                      Date & Heure |
+--------------------------------------------------+
|                                                  |
|   [Icone]   Temperature    Humidite%             |
|             Max/Min        Indice UV             |
|                                                  |
|   Rose des  Pression       Visibilite            |
|   Vents     Tendance       Ressenti    IQA       |
|                                                  |
|   Lune      Lever du soleil                      |
|   Phase     Coucher du soleil                    |
|                                                  |
+--------------------------------------------------+
|  [Previsions horaires - 7 periodes de 3h]        |
+--------------------------------------------------+
|  [Graphiques: Temp | Pression | Humidite | Precip]|
+--------------------------------------------------+
```

### 6.2 Icones Meteo

| Icone | Condition |
|-------|-----------|
| Soleil | Degage |
| Soleil/Nuage | Partiellement nuageux |
| Nuages | Nuageux |
| Pluie | Pluie |
| Orage | Orage |
| Neige | Neige |
| Brouillard | Brouillard/Brume |

### 6.3 Indicateurs

- **Rose des Vents:** Direction et vitesse actuelles du vent
- **Phase Lunaire:** Icone et nom de la phase (donnees reelles WeatherAPI)
- **IQA (Indice Qualite Air):** 1-Bon a 6-Dangereux

---

## 7. API WeatherAPI.com

### 7.1 Point d'Acces Utilise

```
https://api.weatherapi.com/v1/forecast.json
  ?key={CLE_API}
  &q={LAT},{LON}
  &days=3
  &aqi=yes
  &lang={LANGUE}
```

### 7.2 Donnees Recues

Un seul appel inclut:
- **location** - Informations de localisation
- **current** - Conditions actuelles + qualite de l'air
- **forecast** - 3 jours avec donnees horaires
- **astro** - Donnees astronomiques (soleil, lune)

### 7.3 Limites du Forfait Gratuit

| Limite | Valeur |
|--------|--------|
| Appels/mois | 1 000 000 |
| Jours de prevision | 3 |
| Historique | Non inclus |
| Alertes | Disponibles |

### 7.4 Codes de Condition

WeatherAPI utilise des codes numeriques (1000, 1003, etc.) qui sont mappes aux icones internes compatibles avec le systeme d'icones original.

---

## 8. Gestion de l'Energie

### 8.1 Modes de Fonctionnement

| Mode | Consommation | Duree |
|------|--------------|-------|
| Actif | ~150mA | ~10-15 secondes |
| Veille profonde | ~10uA | Configurable |

### 8.2 Autonomie Estimee

Avec batterie de 2000mAh:
- Mise a jour toutes les 60 min: ~6 mois
- Mise a jour toutes les 30 min: ~3 mois
- Mise a jour toutes les 15 min: ~6 semaines

### 8.3 Indicateur de Batterie

Situe dans le coin superieur droit:
- Vert: >75%
- Jaune: 25-75%
- Rouge: <25%

---

## 9. Resolution de Problemes

### 9.1 Problemes Courants

| Probleme | Solution |
|----------|----------|
| Le firmware ne se televerse pas | Utiliser mode bootloader (BOOT + RST) |
| WiFi ne se connecte pas | Verifier 2,4GHz, se rapprocher du routeur |
| Pas de donnees meteo | Verifier cle API sur weatherapi.com |
| Erreur NoMemory | Normal, reessaie automatiquement |
| L'ecran ne se met pas a jour | Verifier l'alimentation |

### 9.2 Messages d'Erreur Serie

| Message | Cause | Solution |
|---------|-------|----------|
| Connection failed 401/403 | Cle API invalide | Verifier la cle |
| deserializeJson NoMemory | Tampon insuffisant | Utilise tampon 64KB |
| WiFi connection failed | Aucun reseau disponible | Verifier identifiants |

### 9.3 Reinitialisation d'Usine

Pour effacer toute la configuration:
1. Ajouter dans setup():
   ```cpp
   Preferences prefs;
   prefs.begin("weather", false);
   prefs.clear();
   prefs.end();
   ```
2. Televerser, executer une fois, supprimer le code

---

## 10. Annexe

### 10.1 Fuseaux Horaires POSIX

| Region | Code |
|--------|------|
| France | CET-1CEST |
| Belgique | CET-1CEST |
| Suisse | CET-1CEST |
| Canada Quebec | EST5EDT |
| Maroc | WET0 |

### 10.2 Codes de Langue

| Code | Langue |
|------|--------|
| FR | Francais |
| ES | Espagnol |
| EN | Anglais |

### 10.3 Credits

- Projet original: David Bird
- Port ESP32: LilyGo
- Adaptation WeatherAPI: XE1E
- Donnees meteorologiques: [WeatherAPI.com](https://www.weatherapi.com/)

---

*Manuel version 1.0 - WeatherAPI*
