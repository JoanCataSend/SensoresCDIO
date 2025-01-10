#include <SoftwareSerial.h>
#include "FuncionesExtras.h"
//#include <Adafruit_ADS1015.h>

class SensorSalinidad
{

    //ATRIBUTOS
  private:
    int power_pin;
    int input_pin;
    int nave;
    int maxSalinidad;
    int minSalinidad;
    Adafruit_ADS1115 ads1;

    //FUNCIONES EXTRA
    FuncionesExtras func;
    const int direccionMemoriaPP = 13;
    const int direccionMemoriaPP2 = 14;

    const int direccionMemoriaIP = 15;
    const int direccionMemoriaIP2 = 16;

    const int direccionMemoriaN = 17;
    const int direccionMemoriaN2 = 18;

    const int direccionMemoriaMAXS = 19;
    const int direccionMemoriaMAXS2 = 20;

    const int direccionMemoriaMINS = 21;
    const int direccionMemoriaMINS2 = 22;

  public:

    //CONSTRUCTOR
    SensorSalinidad(Adafruit_ADS1115 ads1) {
      //DEVOLVEMOS LOS VALORES DE LA EEPROM A LAS VARIABLES DE ESTA CLASE
      this->power_pin = this->func.getDatos(direccionMemoriaPP, direccionMemoriaPP2);
      this->input_pin = this->func.getDatos(direccionMemoriaIP, direccionMemoriaIP2);
      this->nave = this->func.getDatos(direccionMemoriaN, direccionMemoriaN2);
      this->maxSalinidad = this->func.getDatos(direccionMemoriaMAXS, direccionMemoriaMAXS2);
      this->minSalinidad = this->func.getDatos(direccionMemoriaMINS, direccionMemoriaMINS2);

      this->ads1 = ads1;

    }

    //CONSTRUCTOR POR PARÁMETROS
    SensorSalinidad(int power_pin, int input_pin, int nave, int maxSalinidad, int minSalinidad, Adafruit_ADS1115 ads1) {
      //GUARDAMOS LOS DATOS QUE NOS PASAN POR PARÁMETROS
      //ASIGNAMOS A LAS VARIABLES DE LA CLASE LOS VALORES DE LOS PARÁMETROS

      this->power_pin = power_pin;
      this->func.setDatos(power_pin, direccionMemoriaPP, direccionMemoriaPP2);

      this->input_pin = input_pin;
      this->func.setDatos(input_pin, direccionMemoriaIP, direccionMemoriaIP2);

      this->nave = nave;
      this->func.setDatos(nave, direccionMemoriaN, direccionMemoriaN2);

      this->maxSalinidad = maxSalinidad;
      this->func.setDatos(maxSalinidad, direccionMemoriaMAXS, direccionMemoriaMAXS2);

      this->minSalinidad = minSalinidad;
      this->func.setDatos(minSalinidad, direccionMemoriaMINS, direccionMemoriaMINS2);

      this->ads1 = ads1;

    }

    int getSalinidad() {//PASAMOS COMO PARAMETROS EL PIN DIGITAL, EL ANALÓGICO Y EL NÚMERO DE VECES QUE QUEREMOS QUE HAGA EL FOR

      int i;
      float reading, sum; // USAMOS VALORES DECIMALES
      int16_t salinidad;
      int16_t adc1;

      sum = 0.0;

      for ( i = 1; i <= this->nave; i++ ) {//ESTE FOR SIRVE PARA ENCENDER Y APAGAR VARIAS VECES "EL SENSOR", ENTRE CADA VEZ QUE APAGAMOS Y ENCENDEMOS LEEMOS LOS DATOS DEL SENSOR DE SALINIDAD

        digitalWrite( this->power_pin, HIGH ); // ENCENDEMOS
        delay(100); // ESPERAMOS
        adc1 = this->ads1.readADC_SingleEnded(this->input_pin); // LEEMOS LOS DATOS
        salinidad = adc1;
        digitalWrite( this->power_pin, LOW ); // APAGAMOS
        delay(10); // ESPERAMOS

      }

      if (func.getDebug()) {
        Serial.print("Datos leídos del sensor: ");
        Serial.println(salinidad);

        Serial.println(" Datos guardados en la EPROM: ");
        Serial.print("\t MaxSalinidad: ");
        Serial.println(this->maxSalinidad);
        Serial.print("\t MinSalinidad: ");
        Serial.println(this->minSalinidad);
        Serial.print("\t PinDigital: ");
        Serial.println(this->power_pin);
      }


      int porcentaje, rango;
      rango = this->maxSalinidad - this->minSalinidad; //RANGO ENTRE MAX Y MINIMO
      porcentaje = ((salinidad - this->minSalinidad) * 100) / rango; //CACULAMOS EL PORCENTAJE

      if (porcentaje < 0) { //SI ES MENOR QUE 0 LO IGUALAMOS A 0
        porcentaje = 0;
      } else if (porcentaje > 100) { //SI ES MAYOR QUE DEVUELVA 100
        porcentaje = 100;
      }

      return porcentaje;//DEVOLVEMOS EL PORCENTAJE
    }


};
