class Speaker
{

    //ATRIBUTOS
  private:
    int pin;

    //FUNCIONES EXTRA
    const int direccionMemoriaPin = 31;
    const int direccionMemoriaPin2 = 32;

  public:

    //CONSTRUCTOR
    Speaker() {
      pinMode(pin, OUTPUT);

    }

    //CONSTRUCTOR POR PARÁMETROS
    Speaker(int pin) {
      //ASIGNAMOS A LAS VARIABLES DE LA CLASE LOS VALORES DE LOS PARÁMETROS
      this->pin = pin;
      pinMode(pin, OUTPUT);

    }

    void playSetup() {
      playSpeaker(1, 500, 50);
      playSpeaker(1, 750, 50);
      playSpeaker(1, 1000, 50);
    }

    void playSpeaker(int repeticiones, int tipo, int delayN) {

      for (int i = 0; i < repeticiones; i++) {
        tone(this->pin, tipo);
        delay(delayN);
        noTone(this->pin);
        delay(delayN);
      }

    }


};
