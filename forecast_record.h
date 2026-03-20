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
  float    Windspeed;
  float    Rainfall;
  float    Snowfall;
  float    Pop;
  float    Pressure;
  int      Cloudcover;
  int      Visibility;
  int      Sunrise;
  int      Sunset;
  int      Timezone;
  // UV Index and Air Quality
  float    UVIndex;
  int      AQI;          // Air Quality Index (1-5)
  float    PM2_5;        // PM2.5 ug/m3
  float    PM10;         // PM10 ug/m3
  float    CO;           // Carbon monoxide ug/m3
  float    NO2;          // Nitrogen dioxide ug/m3
  float    O3;           // Ozone ug/m3
  float    SO2;          // Sulphur dioxide ug/m3
} Forecast_record_type;

#endif /* ifndef FORECAST_RECORD_H_ */
