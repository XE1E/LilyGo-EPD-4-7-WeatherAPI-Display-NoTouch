#ifndef FORECAST_RECORD_H_
#define FORECAST_RECORD_H_

#include <Arduino.h>

typedef struct { // For current Day and Day 1, 2, 3, etc
  int      Dt;
  String   Period;
  String   Icon;
  String   Trend;
  String   Main0;
  String   Forecast0;
  String   Forecast1;
  String   Forecast2;
  String   Description;
  String   Time;
  String   Country;
  float    lat;
  float    lon;
  float    Temperature;
  float    Feelslike;
  float    Humidity;
  float    High;
  float    Low;
  float    Winddir;
  String   WinddirStr;    // NEW: Wind direction as text ("N", "NE", etc.)
  float    Windspeed;
  float    Gust;          // NEW: Wind gust speed
  float    Rainfall;
  float    Snowfall;
  float    Pop;           // Probability of precipitation
  int      ChanceOfRain;  // NEW: Chance of rain %
  int      ChanceOfSnow;  // NEW: Chance of snow %
  float    Pressure;
  int      Cloudcover;
  int      Visibility;
  float    Dewpoint;      // NEW: Dew point
  int      Sunrise;
  int      Sunset;
  String   SunriseStr;    // NEW: "06:40 AM" format
  String   SunsetStr;     // NEW: "06:48 PM" format
  String   Moonrise;      // NEW: Moon rise time
  String   Moonset;       // NEW: Moon set time
  String   MoonPhase;     // NEW: "Waxing Crescent", etc.
  int      MoonIllum;     // NEW: Moon illumination %
  int      Timezone;
  // UV Index and Air Quality
  float    UVIndex;
  int      AQI;           // Air Quality Index (1-5 EPA)
  int      AQI_DEFRA;     // NEW: UK DEFRA index
  float    PM2_5;         // PM2.5 ug/m3
  float    PM10;          // PM10 ug/m3
  float    CO;            // Carbon monoxide ug/m3
  float    NO2;           // Nitrogen dioxide ug/m3
  float    O3;            // Ozone ug/m3
  float    SO2;           // Sulphur dioxide ug/m3
  // Location (WeatherAPI has more detail)
  String   Region;        // NEW: State/Region
  String   Neighborhood;  // NEW: Colonia/Neighborhood
} Forecast_record_type;

#endif /* ifndef FORECAST_RECORD_H_ */
