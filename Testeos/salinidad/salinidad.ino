#define power_pin 5 // Pin para alimentar el sensor de salinidad

void setup() {
  // Configuramos el pin de alimentación como salida
  pinMode(power_pin, OUTPUT); 
  Serial.begin(9600); // Iniciamos comunicación serial
}

void loop() {
  int16_t adc0;

  // Alimentamos la sonda con un tren de pulsos
  digitalWrite(power_pin, HIGH);
  delay(100); // Esperamos a que el sensor se estabilice

  // Leemos la señal analógica cuando hay un nivel alto
  adc0 = analogRead(A0);
  
  // Apagamos la alimentación de la sonda
  digitalWrite(power_pin, LOW);
  delay(100); // Pequeña espera antes de la siguiente lectura

  // Presentamos la lectura en el monitor serial
  Serial.print("Lectura digital = ");
  Serial.println(adc0, DEC);

  delay(500); // Espera para evitar lecturas demasiado rápidas
}
