#include <Arduino.h>
#include "config.h"
#include "sensor_humedad.h"
#include "sensor_salinidad.h"
#include "sensor_temperatura.h"

void setup() {
  Serial.begin(9600);
  connectWiFi();           // Conectar al Wi-Fi
  iniciarSensorHumedad();  // Inicializar el sensor de humedad
  iniciarSensorSalinidad();// Inicializar el sensor de salinidad
  iniciarSensorTemperatura(); // Inicializar el sensor de temperatura
}

void loop() {
  Serial.println("Selecciona el sensor a utilizar:");
  Serial.println("1. Sensor de Humedad");
  Serial.println("2. Sensor de Salinidad");
  Serial.println("3. Sensor de Temperatura");
  Serial.println("4. Leer y enviar todos los sensores");
  Serial.println("Escribe 'salir' para volver al menú de opciones.");

  while (Serial.available() == 0) {} // Esperar a que el usuario ingrese una opción
  String input = Serial.readStringUntil('\n');
  input.trim(); // Eliminar espacios en blanco

  if (input.equalsIgnoreCase("salir")) {
    return; // Volver al menú de opciones
  }

  int opcion = input.toInt();

  while (true) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command.equalsIgnoreCase("salir")) {
        break; // Salir del bucle y volver al menú de opciones
      }
    }

    int humedadPorcentaje = leerHumedad();
    float salinidad = leerSalinidad();
    float temperatura = leerTemperatura();

    switch (opcion) {
      case 1:
        Serial.println("Leyendo sensor de humedad...");
        Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
        enviarDatosNube(humedadPorcentaje, salinidad, temperatura); // Enviar todos los datos
        break;
      case 2:
        Serial.println("Leyendo sensor de salinidad...");
        Serial.println("Salinidad: " + String(salinidad) + " gramos");
        enviarDatosNube(humedadPorcentaje, salinidad, temperatura); // Enviar todos los datos
        break;
      case 3:
        Serial.println("Leyendo sensor de temperatura...");
        Serial.println("Temperatura: " + String(temperatura) + " °C");
        enviarDatosNube(humedadPorcentaje, salinidad, temperatura); // Enviar todos los datos
        break;
      case 4:
        Serial.println("Leyendo y enviando todos los sensores...");
        Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
        Serial.println("Salinidad: " + String(salinidad) + " gramos");
        Serial.println("Temperatura: " + String(temperatura) + " °C");
        enviarDatosNube(humedadPorcentaje, salinidad, temperatura); // Enviar todos los datos
        break;
      default:
        Serial.println("Opción no válida. Por favor, selecciona una opción del 1 al 4.");
        break;
    }

    delay(1000); // Espera 1 segundo antes de la próxima lectura
  }
}