#include <Arduino.h>
#include "config.h"
#include "sensor_humedad.h"
#include "sensor_salinidad.h"
#include "sensor_temperatura.h"
#include "leds.h"

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
  Serial.println("5. Calibrar sensores");
  Serial.println("Escribe 'salir' para volver al menú de opciones.");

  while (Serial.available() == 0) {} // Esperar a que el usuario ingrese una opción
  String input = Serial.readStringUntil('\n');
  input.trim(); // Eliminar espacios en blanco

  if (input.equalsIgnoreCase("salir")) {
    return; // Volver al menú de opciones
  }

  int opcion = input.toInt();

  if (opcion == 5) {
    calibrarSensores();
    return;
  }

  while (true) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command.equalsIgnoreCase("salir")) {
        break; // Salir del bucle y volver al menú de opciones
      }
    }

    switch (opcion) {
      case 1:
        {
          int humedadPorcentaje = leerHumedad();
          Serial.println("Leyendo sensor de humedad...");
          Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
          enviarDatosNube(humedadPorcentaje, 0, 0); // Enviar solo los datos de humedad
        }
        break;
      case 2:
        {
          float salinidad = leerSalinidad();
          Serial.println("Leyendo sensor de salinidad...");
          Serial.println("Salinidad: " + String(salinidad) + " gramos");
          enviarDatosNube(0, salinidad, 0); // Enviar solo los datos de salinidad
        }
        break;
      case 3:
        {
          float temperatura = averageSample(10); // Obtener la media de 10 muestras
          Serial.println("Leyendo sensor de temperatura...");
          Serial.println("Temperatura: " + String(temperatura) + " °C");
          enviarDatosNube(0, 0, temperatura); // Enviar solo los datos de temperatura
        }
        break;
      case 4:
        {
          int humedadPorcentaje = leerHumedad();
          float salinidad = leerSalinidad();
          float temperatura = averageSample(10); // Obtener la media de 10 muestras
          Serial.println("Leyendo y enviando todos los sensores...");
          Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
          Serial.println("Salinidad: " + String(salinidad) + " gramos");
          Serial.println("Temperatura: " + String(temperatura) + " °C");
          enviarDatosNube(humedadPorcentaje, salinidad, temperatura); // Enviar todos los datos
        }
        break;
      default:
        Serial.println("Opción no válida. Por favor, selecciona una opción del 1 al 4.");
        break;
    }

    delay(1000); // Espera 1 segundo antes de la próxima lectura
  }
}

void calibrarSensores() {
  Serial.println("Selecciona el sensor a calibrar:");
  Serial.println("1. Sensor de Humedad");
  Serial.println("2. Sensor de Salinidad");
  Serial.println("Escribe 'salir' para volver al menú de opciones.");

  while (Serial.available() == 0) {} // Esperar a que el usuario ingrese una opción
  String input = Serial.readStringUntil('\n');
  input.trim(); // Eliminar espacios en blanco

  if (input.equalsIgnoreCase("salir")) {
    return; // Volver al menú de opciones
  }

  int opcion = input.toInt();

  switch (opcion) {
    case 1:
      calibrarSensorHumedad();
      break;
    case 2:
      calibrarSensorSalinidad();
      break;
    default:
      Serial.println("Opción no válida. Por favor, selecciona una opción del 1 al 2.");
      break;
  }
}