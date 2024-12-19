#ifndef SENSOR_HUMEDAD_H
#define SENSOR_HUMEDAD_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "leds.h"

// Declaración del ADC ADS1115
Adafruit_ADS1115 ads;

// Declaración de pines y variables
int16_t humedadCrudoSeco;
int16_t humedadCrudoMojado;

// Declaración de funciones
void calibrarSensorHumedad(); // Declarar antes de usarla
void iniciarSensorHumedad();
int leerHumedad();

// Definición de las funciones

// Inicializar el sensor de humedad
void iniciarSensorHumedad() {
  if (!ads.begin()) {
    Serial.println("No se encontró el ADS1115. Verifica la conexión.");
    while (1);
  }
  ads.setGain(GAIN_ONE);
  Serial.println("ADS1115 iniciado correctamente.");
  iniciarLEDs();
  calibrarSensorHumedad(); // Llamar después de declarar
}

// Función para calibrar el sensor de humedad
void calibrarSensorHumedad() {
  Serial.println("SECA EL SENSOR TOTALMENTE");
  for (int i = 5; i > 0; i--) {
    Serial.println(String(i) + "...");
    delay(1000);
  }
  humedadCrudoSeco = ads.readADC_SingleEnded(0);
  Serial.println("Valor seco: " + String(humedadCrudoSeco));

  Serial.println("PON EL SENSOR EN EL AGUA HASTA LA LÍNEA");
  for (int i = 5; i > 0; i--) {
    Serial.println(String(i) + "...");
    delay(1000);
  }
  humedadCrudoMojado = ads.readADC_SingleEnded(0);
  Serial.println("Valor mojado: " + String(humedadCrudoMojado));

  if (humedadCrudoSeco <= humedadCrudoMojado) {
    Serial.println("Error en la calibración.");
    while (1);
  }
}

// Leer la humedad
int leerHumedad() {
  int16_t humedadCrudo = ads.readADC_SingleEnded(0);
  int humedadPorcentaje = map(humedadCrudo, humedadCrudoMojado, humedadCrudoSeco, 100, 0);
  humedadPorcentaje = constrain(humedadPorcentaje, 0, 100);

  actualizarLEDs(humedadPorcentaje);

  Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
  return humedadPorcentaje;
}

#endif