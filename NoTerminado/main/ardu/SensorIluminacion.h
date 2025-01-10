#include <SoftwareSerial.h>
#include "FuncionesExtras.h"
//#include <Adafruit_ADS1015.h>

class SensorIluminacion
{

    //ATRIBUTOS
  private:
    int pinAnalog;
    double maxVolt;
    double maxIlu;
    double minIlu;

    Adafruit_ADS1115 ads1;

    //FUNCIONES EXTRA
    FuncionesExtras func;
    const int direccionMemoriaPA = 9;
    const int direccionMemoriaPA2 = 10;

    const int direccionMemoriaMAXV = 11;
    const int direccionMemoriaMAXV2 = 12;
    
    const int direccionMemoriaMIN = 40;
    const int direccionMemoriaMIN2 = 41;

    const int direccionMemoriaMAX = 42;
    const int direccionMemoriaMAX2 = 43;

  public:

    //CONSTRUCTOR
    SensorIluminacion(Adafruit_ADS1115 ads1) {
      //DEVOLVEMOS LOS VALORES DE LA EEPROM A LAS VARIABLES DE ESTA CLASE
      this->pinAnalog = this->func.getDatos(direccionMemoriaPA, direccionMemoriaPA2);
      this->maxVolt = this->func.getDatos(direccionMemoriaMAXV, direccionMemoriaMAXV2) / 100;
      
      this->minIlu = this->func.getDatos(direccionMemoriaMIN, direccionMemoriaMIN2);
      this->maxIlu = this->func.getDatos(direccionMemoriaMAX, direccionMemoriaMAX2);

      this->ads1 = ads1;

    }

    //CONSTRUCTOR POR PARÁMETROS
    SensorIluminacion(int pinAnalog, double maxVolt, double minIlu, double maxIlu, Adafruit_ADS1115 ads1) {
      //GUARDAMOS LOS DATOS QUE NOS PASAN POR PARÁMETROS
      //ASIGNAMOS A LAS VARIABLES DE LA CLASE LOS VALORES DE LOS PARÁMETROS

      this->pinAnalog = pinAnalog;
      this->func.setDatos(pinAnalog, direccionMemoriaPA, direccionMemoriaPA2);

      this->maxVolt = maxVolt;
      this->func.setDatos(maxVolt, direccionMemoriaMAXV, direccionMemoriaMAXV2);

      this->minIlu = minIlu;
      this->func.setDatos(minIlu, direccionMemoriaMIN, direccionMemoriaMIN2);

      this->maxIlu = maxIlu;
      this->func.setDatos(maxIlu, direccionMemoriaMAX, direccionMemoriaMAX2);

      this->ads1 = ads1;

    }

    int getIluminacion() {
      double vILU;
      int16_t lectILU;
      double porcentajeILU;

      lectILU = ads1.readADC_SingleEnded(this->pinAnalog); // LEEMOS LOS DATOS
      vILU = ((lectILU * 4.096) / 32768); //SACAMOS LOS VOLT (EN VOLTIOS)

      if (func.getDebug()) {
        Serial.print("Datos leídos del sensor: ");
        Serial.println(lectILU);

        Serial.println(" Datos guardados en la EPROM: ");
        Serial.print("\t MaxVolt: ");
        Serial.println(this->maxVolt);
        Serial.print("\t Max: ");
        Serial.println(this->maxIlu);
        Serial.print("\t Min: ");
        Serial.println(this->minIlu);
      }

      if (vILU <= 0.0305) { //SI EL VOLTAJE ESTÁ ENTRE 0 Y 30.5mV
        Serial.println("\nOscuridad total, el sensor está tapado");
      } else if (vILU > 0.0305 && vILU <= 0.051) { //SI EL VOLTAJE ESTÁ ENTRE 30.5 Y 51mV
        Serial.println("\nSombra, el sensor está con luz ambiente");
      } else  {//(vILU >0.051)SI EL VOLTAJE ES SUPERIOR A 30.5mV
        Serial.println("\nLuz ambiente, el sensor no está con sombra");
      }

      //porcentajeILU = ((vILU) * 100) / this->maxVolt; //TOMANDO 3.71V el 100%
      porcentajeILU= lectILU*100/(this->maxIlu - this->minIlu);
      if (porcentajeILU>100){
        porcentajeILU=100;
      }
      
      return porcentajeILU;
    }


};
