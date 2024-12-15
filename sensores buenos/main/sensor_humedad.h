#ifndef SENSOR_HUMEDAD_H
#define SENSOR_HUMEDAD_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Declaración del ADC ADS1115
Adafruit_ADS1115 ads;

// Declaración de pines y variables
const int ledMid = 0;   // LED indicador de 0% a 50%
const int ledHigh = 4;  // LED indicador de 50% a 100%
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
  pinMode(ledMid, OUTPUT);
  pinMode(ledHigh, OUTPUT);
  digitalWrite(ledMid, LOW);
  digitalWrite(ledHigh, LOW);
  calibrarSensorHumedad(); // Llamar después de declarar
}

// Función para calibrar el sensor de humedad
void calibrarSensorHumedad() {
  Serial.println("SECA EL SENSOR TOTALMENTE");
  delay(5000);
  humedadCrudoSeco = ads.readADC_SingleEnded(0);
  Serial.println("Valor seco: " + String(humedadCrudoSeco));

  Serial.println("PON EL SENSOR EN EL AGUA HASTA LA LÍNEA");
  delay(5000);
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

  if (humedadPorcentaje <= 50) {
    digitalWrite(ledMid, HIGH);
    digitalWrite(ledHigh, LOW);
  } else {
    digitalWrite(ledMid, LOW);
    digitalWrite(ledHigh, HIGH);
  }

  Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
  return humedadPorcentaje;
}

#endif
