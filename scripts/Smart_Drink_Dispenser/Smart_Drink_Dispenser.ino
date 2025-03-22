#include "HX711.h"
#include <ESP32Servo.h>

#define pumpPin1 13   // pin do sterowania pompą nr 1
#define pumpPin2 14   // pin do sterowania pompą nr 2

#define CLK 22        // pin od zegara
#define dataOUT1 27   // Dane z przetwornika nr 1
#define dataOUT2 26   // Dane z przetwornika nr 2
#define dataOUT3 25   // Dane z przetwornika nr 3
#define servoPin 33      // Pin do serwomechanizmu

#define calibration_factor1 -3350.0 // współczynnik kalibracji dla przetwornika nr 1
#define calibration_factor2 -3350.0 // współczynnik kalibracji dla przetwornika nr 2
#define calibration_factor3 -3350.0 // współczynnik kalibracji dla przetwornika nr 3

#define empty_glass 200.0
#define full_glass 450.0


HX711 scale1; // obiekt od przetwornika nr 1
HX711 scale2; // obiekt od przetwornika nr 2
HX711 scale3; // obiekt od przetwornika nr 3
Servo servo;  // obiekt od serwomechanizmu

/**
 * @brief Klasa obsługująca moduły nalewające ciecz.
 * 
 * Umożliwia sprawdzenie wagi, ruch serwomechanizmu i włączenie pomp.
 */
class Glass {
  public:
  static float amount_1; // Ilość cieczy nr 1
  static float amount_2; // Ilość cieczy nr 2
  int position_glass; // Pozycja szklanki
  static int position_servo; // Pozycja serwomechanizmu

  Glass(int pos) {
    this->position_glass = pos;
  }

 /**
 * @brief Funkcja obsługuje działaniem pomp, nalewa do jednej szklanki.
 */
  void pour () {
    moveServomechanism();
    float weight = returnWeight();
    float weight_empty_glass_and_amount_1;
    if (weight < empty_glass -0) {
      // brak szklanki
      return;
    }
    while (true) {
      weight = returnWeight();
      if (weight < empty_glass + Glass::amount_1) {
        digitalWrite(pumpPin1, HIGH);
      } else {
        digitalWrite(pumpPin1, LOW);
        weight_empty_glass_and_amount_1 = weight;
        break;
      }
    }
    while (true) {
      weight = returnWeight();
      if (weight < weight_empty_glass_and_amount_1 + Glass::amount_2) {
        digitalWrite(pumpPin2, HIGH);
      } else {
        digitalWrite(pumpPin2, LOW);
        break;
      }
    }
  }

  /**
  * @brief Funkcja obsługująca zwracanie wartości wagi.
  */
  static float returnWeight() {
    switch(Glass::position_servo) {
      case 0:
      return -1.f;
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
  * Serwomechanizm ustawia pozycję szklanki. Po ostatniej pozycji wraca do domyslnej.
  *
  * @param default_position Serwomechanizm wraca do domyślnej pozycji.
  * @return Jeśli serwomechanizm znajduje się na domyślnej pozycji zwraca TRUE, w przeciwnym razie zwraca FALSE.
  */
  bool moveServomechanism(bool default_position = false) {
    if (default_position) {
      servo.write(0);
      Glass::position_servo = 0;
      return true;

    } else if (this->position_glass == 1) {
      servo.write(45);

    } else if (this->position_glass == 2) {
      servo.write(90);

    } else if (this->position_glass == 3) {
      servo.write(135);

    }
    Glass::position_servo = this->position_glass;
    return false;
  }

  /**
  * @brief Funkcja ustawia hardware do pozycji domyślnej.
  */
  void default_position() {
    digitalWrite(pumpPin1, LOW);
    digitalWrite(pumpPin2, LOW);
    moveServomechanism(true);
  }
  
  /**
  * @brief Zmiana przepisu.
  *
  * @param amount_1 Ustawia ilość pierwszej cieczy.
  * @param amount_2 Ustawia ilość drugiej cieczy.
  */
  static bool change_recipt(float amount_1, float amount_2) {
    Glass::amount_1 = amount_1;
    Glass::amount_2 = amount_2;
    return true;
  }
};

float Glass::amount_1 = 0.0f;
float Glass::amount_2 = 0.0f;

void setup() {
  pinMode(pumpPin1, OUTPUT);
  pinMode(pumpPin2, OUTPUT);
  digitalWrite(pumpPin1, LOW);
  digitalWrite(pumpPin2, LOW);
  scale1.begin(dataOUT1, CLK); // przetwornik nr 1
  scale2.begin(dataOUT2, CLK); // przetwornik nr 2
  scale3.begin(dataOUT3, CLK); // przetwornik nr 3
  servo.attach(servoPin);
  Serial.begin(9600);
  Serial.println("Start programu");
  scale1.set_scale(calibration_factor1);  // Ustawienie kalibracji dla 1
  scale2.set_scale(calibration_factor2);  // Ustawienie kalibracji dla 2
  scale3.set_scale(calibration_factor3);  // Ustawienie kalibracji dla 3
  scale1.tare();  // Tarowanie wagi nr 1
  scale2.tare();  // Tarowanie wagi nr 2
  scale3.tare();  // Tarowanie wagi nr 3

  Glass::change_recipt(50.0f, 60.0f);
  Glass glass1(1);  // obiekt szklana1
  Glass glass2(2);  // obiekt szklana2
  Glass glass3(3);  // obiekt szklana3
}

void loop() {
  

}
