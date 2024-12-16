#define power_pin 5 

const int numPoints = 5; 
float valorDigital[numPoints] = {462, 840, 860, 900, 930}; 
float gramosSal[numPoints] = {0, 5, 10, 15, 20 };         

void setup() {
  pinMode(power_pin, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  int16_t adc0;

  
  digitalWrite(power_pin, HIGH);
  delay(100);

  adc0 = analogRead(A0);
  digitalWrite(power_pin, LOW);
  delay(100);

  Serial.print("Lectura digital = ");
  Serial.println(adc0, DEC);

  // Convertir a gramos de sal usando el polinomio de Lagrange
  float salinidad = calcularSalinidad(adc0);
  Serial.print("Gramos de sal = ");
  Serial.println(salinidad, 2); 
}
float calcularSalinidad(int adc0){
  float salinidad=0.0;

  for (int i=0; i<numPoints; i++) {
   float Li = 1.0;
   for (int j=0; j<numPoints; j++) {
     if (j != i) {
      Li *= (adc0- valorDigital[j]) / (valorDigital[i] - valorDigital[j]);   //Lagrange
      }
    }
    salinidad += gramosSal[i] * Li;
  }

  
  if (adc0 < valorDigital[0]) {
    salinidad = 0.0;
  }

  return salinidad;
}
