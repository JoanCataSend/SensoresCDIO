#ifndef SENSOR_SALINIDAD_H
#define SENSOR_SALINIDAD_H
#define power_pin 5

const int numPoints = 4;
float valorDigital[numPoints];
float gramosSal[numPoints] = {0, 1, 5, 10};

// Inicializar el sensor de salinidad
void iniciarSensorSalinidad() {
  pinMode(power_pin, OUTPUT);
  digitalWrite(power_pin, LOW);
}

// Leer la salinidad
float leerSalinidad() {
  digitalWrite(power_pin, HIGH);
  delay(100);
  int adc0 = analogRead(A0);
  digitalWrite(power_pin, LOW);
  delay(100);
  float salinidad = 0.0;
  for (int i = 0; i < numPoints; i++) {
    float Li = 1.0;
    for (int j = 0; j < numPoints; j++) {
      if (j != i) {
        Li *= (adc0 - valorDigital[j]) / (valorDigital[i] - valorDigital[j]);
      }
    }
    salinidad += gramosSal[i] * Li;
  }
  if (adc0 < valorDigital[0]) {
    salinidad = 0.0;
  }
  Serial.println("Salinidad: " + String(salinidad) + " gramos");
  return salinidad;
}

// Calibrar el sensor de salinidad
void calibrarSensorSalinidad() {
  Serial.println("Calibrando sensor de salinidad...");
  for (int i = 0; i < numPoints; i++) {
    Serial.print("Coloca el sensor en la solución de ");
    Serial.print(gramosSal[i]);
    Serial.println(" gramos de sal y presiona Enter.");
    
    while (Serial.available() == 0) {} // Esperar a que el usuario presione Enter
    Serial.readStringUntil('\n'); // Leer la entrada del usuario

    digitalWrite(power_pin, HIGH);
    delay(100);
    valorDigital[i] = analogRead(A0);
    digitalWrite(power_pin, LOW);
    Serial.println("Valor digital: " + String(valorDigital[i]));
  }
  Serial.println("Calibración completada.");
}

#endif