#include <SoftwareSerial.h>
#include "FuncionesExtras.h"

class SensorHumedad
{

    //ATRIBUTOS
  private:
    int pin;
    double waterValue;
    double airValue;
    Adafruit_ADS1115 ads1;

    //FUNCIONES EXTRA
    FuncionesExtras func;
    const int direccionMemoriaPin = 3;
    const int direccionMemoriaPin2 = 4;
    const int direccionMemoriaWV = 5;
    const int direccionMemoriaWV2 = 6;
    const int direccionMemoriaAV = 7;
    const int direccionMemoriaAV2 = 8;

  public:

    //CONSTRUCTOR
    SensorHumedad(Adafruit_ADS1115 ads1) {
      //DEVOLVEMOS LOS VALORES DE LA EEPROM A LAS VARIABLES DE ESTA CLASE
      //ASIGNAMOS A LAS VARIABLES DE LA CLASE LOS VALORES DE LOS PARÁMETROS

      this->pin = this->func.getDatos(direccionMemoriaPin, direccionMemoriaPin2);
      this->waterValue = this->func.getDatos(direccionMemoriaWV, direccionMemoriaWV2);
      this->airValue = this->func.getDatos(direccionMemoriaAV, direccionMemoriaAV2);

      this->ads1 = ads1;

    }

    //CONSTRUCTOR POR PARÁMETROS
    SensorHumedad(int pin, double waterValue, double airValue, Adafruit_ADS1115 ads1) {
      //GUARDAMOS LOS DATOS QUE NOS PASAN POR PARÁMETROS

      this->pin = pin;
      this->func.setDatos(pin, direccionMemoriaPin, direccionMemoriaPin2);

      this->waterValue = waterValue;
      this->func.setDatos(waterValue, direccionMemoriaWV, direccionMemoriaWV2);

      this->airValue = airValue;
      this->func.setDatos(airValue, direccionMemoriaAV, direccionMemoriaAV2);

      this->ads1 = ads1;

    }

    int getHumedad() {

      int16_t adc;
      int16_t humedad;

      adc = this->ads1.readADC_SingleEnded(pin);//LEEMOS LOS DATOS DEL PIN ANALOGICO
      humedad = 100 * this->airValue / (this->airValue - this->waterValue) - adc * 100 / (this->airValue - this->waterValue); // CALCULAMOS EL PORCENTAJE DE HUMEDAD

      if (func.getDebug()) {
        Serial.print("Datos leídos del sensor: ");
        Serial.println(adc);

        Serial.println(" Datos guardados en la EPROM: ");
        Serial.print("\t Aire: ");
        Serial.println(this->airValue);
        Serial.print("\t Agua: ");
        Serial.println(this->waterValue);
        Serial.print("\t Pin: ");
        Serial.println(this->pin);
      }

      if (humedad > 100) {//SI ES MAYOR QUE DEVUELVA 100
        humedad = 100;
      } else if (humedad < 0) {//SI ES MENOR QUE 0 LO IGUALAMOS A 0
        humedad = 0;
      }

      return humedad;//DEVOLVEMOS EL PORCENTAJE

    }


};
