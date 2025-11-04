#include "HX711.h"

#define DT  2
#define SCK 3

HX711 scale;

void setup() {
  Serial.begin(115200);
  scale.begin(DT, SCK);
  scale.set_scale(1063); // belum tahu faktor kalibrasi
  scale.tare();      // nolkan berat

  Serial.println("Taruh beban diketahui (misal 100g)...");
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.get_units(10);
    Serial.print("Reading: ");
    Serial.println(reading);
    delay(1000);
  }
}
