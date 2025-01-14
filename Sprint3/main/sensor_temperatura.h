#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include <Adafruit_ADS1X15.h>

// Usamos el mismo objeto ads definido previamente
extern Adafruit_ADS1115 ads;

// Definimos las constantes del circuito
const float m = 34.61e-3;      // Pendiente del circuito acondicionador (en V/°C)
const float b = 7.88e-3;       // Offset del circuito acondicionador (en V)
const float Vmax = 4.096;      // Rango máximo de entrada del ADS1115 (según la ganancia)
const int resolucionADC = 32768; // (2^15 - 1): Resolución del ADS1115

// Función para inicializar el sensor de temperatura
void iniciarSensorTemperatura() {
  Serial.println("Sensor de temperatura inicializado.");
}

// Función para leer la temperatura
float leerTemperatura() {
  // Leer valor digital desde el canal A2 del ADS1115
  int16_t valorDigital = ads.readADC_SingleEnded(1);

  // Calcular la temperatura usando la fórmula derivada
  float temperatura = ((valorDigital * Vmax) / (m * resolucionADC)) - (b / m);

  // Mostrar en el monitor serial
  Serial.println("Temperatura: " + String(temperatura) + " °C");
  return temperatura;
}

#endif