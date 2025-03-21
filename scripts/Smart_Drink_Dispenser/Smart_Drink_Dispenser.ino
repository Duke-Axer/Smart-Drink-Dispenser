#include "HX711.h"

#define pumpPin1 13   // pin do sterowania pompą nr 1
#define pumpPin2 14   // pin do sterowania pompą nr 2

#define CLK 22        // pin od zegara
#define dataOUT1 27   // Dane z przetwornika nr 1
#define dataOUT2 26   // Dane z przetwornika nr 2
#define dataOUT3 25   // Dane z przetwornika nr 3
#define servo 33      // Pin do serwomechanizmu

#define calibration_factor1 -3350.0 // współczynnik kalibracji dla przetwornika nr 1
#define calibration_factor2 -3350.0 // współczynnik kalibracji dla przetwornika nr 2
#define calibration_factor3 -3350.0 // współczynnik kalibracji dla przetwornika nr 3

#define empty_glass 200.0
#define full_glass 450.0

HX711 scale1(dataOUT1, CLK); // obiekt od przetwornika nr 1
HX711 scale2(dataOUT2, CLK); // obiekt od przetwornika nr 2
HX711 scale3(dataOUT3, CLK); // obiekt od przetwornika nr 3


/**
 * @brief Klasa obsługująca moduły nalewające ciecz.
 * 
 * Umożliwia sprawdzenie wagi, ruch serwomechanizmu i włączenie pomp.
 */
class Glass {
  private:
  float amount_1 = 50.0; // Ilość cieczy nr 1
  float amount_2 = 50.0; // Ilość cieczy nr 2
  float weight_empty_glass_and_amount_1;
  int position_glass; // Pozycja szklanki
  int position_servo; // Pozycja serwomechanizmu

  public:
  Glass(int pos) {
    position = pos;
  }
 /**
 * @brief Funkcja obsługuje działanie pomp
 */
  static void pour () {
    while (true) {
      if (returnWeight() < empty_glass + amount_1) {
        digitalWrite(pumpPin1, HIGH);
      } else {
        digitalWrite(pumpPin1, LOW);
        weight_empty_glass_and_amount_1 = returnWeight();
        break;
      }
    }
    while (true) {
      if (returnWeight() < weight_empty_glass_and_amount_1) {
        digitalWrite(pumpPin2, HIGH);
      } else {
        digitalWrite(pumpPin2, LOW);
        break;
      }
    }
  }

  /**
  * @brief Funkcja obsługująca zwracanie wartości wagi.
  * 
  * 
  */
  static float returnWeight() {
    switch(position_servo) {
      case 1:
      return scale1.get_units();
      case 2:
      return scale2.get_units();
      case 3:
      return scale3.get_units();
    }
  }

  /**
  * @brief Funkcja obsługująca sterowaniem serwomechanizmu.
  * 
  * Serwomechanizm ustawia następną pozycję. Po ostatniej pozycji wraca do domyslnej.
  *
  * @param default_position Serwomechanizm wraca do domyślnej pozycji.
  * @return Jeśli serwomechanizm znajduje się na domyślnej pozycji zwraca TRUE, w przeciwnym razie zwraca FALSE.
  */
  bool moveServomechanism(bool default_position = false) {
    if (default_position) {

    } else {

    }
  }
  static void default_position() {
    digitalWrite(pumpPin1, LOW);
    digitalWrite(pumpPin2, LOW);
    moveServomechanism(true)
  }
}

void setup() {
  pinMode(pumpPin1, OUTPUT);
  pinMode(pumpPin2, OUTPUT);
  digitalWrite(pumpPin1, LOW);
  digitalWrite(pumpPin2, LOW);
  Serial.begin(9600);
  Serial.println("Start programu");
  scale1.set_scale(calibration_factor1);  // Ustawienie kalibracji dla 1
  scale2.set_scale(calibration_factor2);  // Ustawienie kalibracji dla 2
  scale3.set_scale(calibration_factor3);  // Ustawienie kalibracji dla 3
  scale1.tare();  // Tarowanie wagi nr 1
  scale2.tare();  // Tarowanie wagi nr 2
  scale3.tare();  // Tarowanie wagi nr 3

}

void loop() {
  // put your main code here, to run repeatedly:

}
