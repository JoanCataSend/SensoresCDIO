#ifndef SENSOR_LUMINOSIDAD_H
#define SENSOR_LUMINOSIDAD_H

#include <Adafruit_ADS1X15.h>

// Usamos el mismo objeto ads definido previamente
extern Adafruit_ADS1115 ads;

// Variables globales
const int LUZ_CANAL = 3; // Canal A3 del ADS1115

// Función para inicializar el sensor de luminosidad
void iniciarSensorLuminosidad() {
  Serial.println("Sensor de luminosidad inicializado.");
}

// Función para leer la luminosidad
float leerLuminosidad() {
  // Leer la luminosidad del canal A3 del ADS1115
  int luzRaw = ads.readADC_SingleEnded(LUZ_CANAL);

  // Convertir la lectura a milivoltios
  float voltajeLuz = (luzRaw * 4.096) / 32768.0 * 1000;

  // Mostrar el resultado en el monitor serie con salto de línea
  Serial.print("Luminosidad: ");
  Serial.print(voltajeLuz);
  Serial.println(" mV");

  // Condiciones según el rango de voltaje
  if (voltajeLuz < 150) {
    Serial.println("OSCURIDAD");
  } else if (voltajeLuz >= 150 && voltajeLuz <= 319) {
    Serial.println("SOMBRA");
  } else if (voltajeLuz >= 329 && voltajeLuz <= 390) {
    Serial.println("LUZ NATURAL");
  } else if (voltajeLuz >= 391 && voltajeLuz <= 500) {
    Serial.println("LUZ LINTERNAS");
  } else if (voltajeLuz > 500) {
    Serial.println("FOCO");
  }

  Serial.println("---------------------------");
  return voltajeLuz;
}

#endif