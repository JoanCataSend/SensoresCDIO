#include <SoftwareSerial.h>
#include "FuncionesExtras.h"

class SensorTemperatura
{

    //ATRIBUTOS
  private:
    int pin;
    Adafruit_ADS1115 ads1;
    double maxVolt;
    double minVolt;
    double margenError;

    //FUNCIONES EXTRA
    FuncionesExtras func;
    const int direccionMemoriaPin = 23;
    const int direccionMemoriaPin2 = 24;

    const int direccionMemoriaMAXV = 25;
    const int direccionMemoriaMAXV2 = 26;

    const int direccionMemoriaMINV = 27;
    const int direccionMemoriaMINV2 = 28;

    const int direccionMemoriaME = 29;
    const int direccionMemoriaME2 = 30;


  public:

    //CONSTRUCTOR
    SensorTemperatura(Adafruit_ADS1115 ads1) {

      //DEVOLVEMOS LOS VALORES DE LA EEPROM A LAS VARIABLES DE ESTA CLASE
      this->pin = this->func.getDatos(direccionMemoriaPin, direccionMemoriaPin2);
      this->minVolt = this->func.getDatos(direccionMemoriaMINV, direccionMemoriaMINV2) / 10000;
      this->maxVolt = this->func.getDatos(direccionMemoriaMAXV, direccionMemoriaMAXV2) / 1000;
      this->margenError = this->func.getDatos(direccionMemoriaME, direccionMemoriaME2) / 100;

      this->ads1 = ads1;

    }

    //CONSTRUCTOR POR PARÁMETROS
    SensorTemperatura(int pin, double minV, double maxV, double me, Adafruit_ADS1115 ads1) {

      //GUARDAMOS LOS DATOS QUE NOS PASAN POR PARÁMETROS
      this->func.setDatos(pin, direccionMemoriaPin, direccionMemoriaPin2);
      this->func.setDatos(minV, direccionMemoriaMINV, direccionMemoriaMINV2);
      this->func.setDatos(maxV, direccionMemoriaMAXV, direccionMemoriaMAXV2);
      this->func.setDatos(me, direccionMemoriaME, direccionMemoriaME2);

      //ASIGNAMOS A LAS VARIABLES DE LA CLASE LOS VALORES DE LOS PARÁMETROS
      this->ads1 = ads1;

    }

    //CAMBIAR LOS DATOS DE LA FUNCION POR PARAMETROS

    double getTemperatura() {

      double vo;
      double result;
      double calibracion;
      int16_t lect;

      lect = ads1.readADC_SingleEnded(this->pin); // LEEMOS LOS DATOS
      vo = ((lect * 4.096) / 32768); //SACAMOS LOS VOLT

      if (func.getDebug()) {
        Serial.print("Datos leídos del sensor: ");
        Serial.println(lect);

        Serial.println(" Datos guardados en la EPROM: ");
        Serial.print("\t MaxVolt: ");
        Serial.println(this->maxVolt);
        Serial.print("\t MinVolt: ");
        Serial.println(this->minVolt);
        Serial.print("\t MargenError: ");
        Serial.println(this->margenError);

      }



      result = (vo - this->maxVolt) / this->minVolt; //SACAMOS EL RESULTADO
      calibracion = result - this->margenError; //CALIBRAMOS EL SENSOR CON LA FORMULA

      return calibracion;

    }


};
