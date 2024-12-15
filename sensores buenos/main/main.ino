#include <Arduino.h>
#include "config.h"
#include "sensor_humedad.h"
#include "sensor_salinidad.h"
#include "sensor_temperatura.h"  // Incluir el archivo de temperatura

void setup() {
  Serial.begin(9600);
  connectWiFi();           // Conectar al Wi-Fi
  iniciarSensorHumedad();  // Inicializar el sensor de humedad
  iniciarSensorSalinidad();// Inicializar el sensor de salinidad
  iniciarSensorTemperatura(); // Inicializar el sensor de temperatura
}

void loop() {
  // Leer y procesar datos del sensor de humedad
  int humedadPorcentaje = leerHumedad();
  
  // Leer y procesar datos del sensor de salinidad
  float salinidad = leerSalinidad();

  // Leer y procesar datos del sensor de temperatura
  float temperatura = leerTemperatura();

  // Enviar datos al servidor
  enviarDatosNube(humedadPorcentaje, salinidad, temperatura);

  delay(1000); // Espera 1 segundo antes de la próxima iteración
}
