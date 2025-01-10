
//MEDIR TEMPERATURA
double sensorTemperatura(int pin, Adafruit_ADS1115 ads1) {

  //int16_t lect;
  double vo;
  double result;
  double calibracion;
  int16_t lect;

  lect = ads1.readADC_SingleEnded(pin); // LEEMOS LOS DATOS
  vo = ((lect * 4.096) / 32768); //SACAMOS LOS VOLT

  result = (vo - 0.786) / 0.0348; //SACAMOS EL RESULTADO
  calibracion = result - 0.31; //CALIBRAMOS EL SENSOR CON LA FORMULA

  return calibracion;

}


//MEDIR SALINIDAD
int sensorSalinidad( int power_pin, int input_pin, int nave, int maxSalinidad, int minSalinidad, Adafruit_ADS1115 ads1) {//PASAMOS COMO PARAMETROS EL PIN DIGITAL, EL ANALÓGICO Y EL NÚMERO DE VECES QUE QUEREMOS QUE HAGA EL FOR

  int i;
  float reading, sum; // USAMOS VALORES DECIMALES
  int16_t salinidad;
  int16_t adc1;

  sum = 0.0;

  for ( i = 1; i <= nave; i++ ) {//ESTE FOR SIRVE PARA ENCENDER Y APAGAR VARIAS VECES "EL SENSOR", ENTRE CADA VEZ QUE APAGAMOS Y ENCENDEMOS LEEMOS LOS DATOS DEL SENSOR DE SALINIDAD

    //int16_t adc1;

    digitalWrite( power_pin, HIGH ); // ENCENDEMOS
    delay(100); // ESPERAMOS
    adc1 = ads1.readADC_SingleEnded(input_pin); // LEEMOS LOS DATOS
    salinidad = adc1;
    digitalWrite( power_pin, LOW ); // APAGAMOS
    delay(10); // ESPERAMOS

  }

  //PARA CALIBRAR
  //Serial.println(salinidad);


  int porcentaje, rango;
  rango = maxSalinidad - minSalinidad; //RANGO ENTRE MAX Y MINIMO
  porcentaje = ((salinidad - minSalinidad) * 100) / rango; //CACULAMOS EL PORCENTAJE

  if (porcentaje < 0) { //SI ES MENOR QUE 0 LO IGUALAMOS A 0
    porcentaje = 0;
  } else if (porcentaje > 100) { //SI ES MAYOR QUE DEVUELVA 100
    porcentaje = 100;
  }

  return porcentaje;//DEVOLVEMOS EL PORCENTAJE
}


//MEDIR HUMEDAD
int sensorHumedad( int airValue, int waterValue, int pin, Adafruit_ADS1115 ads1) {

  int16_t adc0;
  int16_t humedad;

  adc0 = ads1.readADC_SingleEnded(pin);//LEEMOS LOS DATOS DEL PIN ANALOGICO 
  humedad = 100 * airValue / (airValue - waterValue) - adc0 * 100 / (airValue - waterValue); // CALCULAMOS EL PORCENTAJE DE HUMEDAD

  if (humedad > 100) {//SI ES MAYOR QUE DEVUELVA 100
    humedad = 100;
  } else if (humedad < 0) {//SI ES MENOR QUE 0 LO IGUALAMOS A 0
    humedad = 0;
  }

  return humedad;//DEVOLVEMOS EL PORCENTAJE
}
