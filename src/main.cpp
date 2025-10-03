#include "HX711.h"

#define DT  2   // HX711 DOUT
#define SCK 3   // HX711 SCK
#define PUMP_PIN 5  // MOSFET Gate

HX711 scale;

// Kalibrasi awal (ubah sesuai hasil kalibrasi dengan beban known weight)
float scale_factor = 2280.0; 
float density_g_per_ml = 1.0; // default: 1 ml = 1 g

void setup() {
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  scale.begin(DT, SCK);
  scale.set_scale(scale_factor);
  scale.tare(); // nolkan timbangan
  Serial.println("Sistem siap. Ketik jumlah ml di Serial Monitor, lalu Enter.");
}

void loop() {
  if (Serial.available()) {
    float target_ml = Serial.parseFloat(); 
    if (target_ml > 0) {
      dispense_ml(target_ml);
    }
  }
}

void dispense_ml(float target_ml) {
  float baseline = scale.get_units(10); // berat awal
  float target_g = target_ml * density_g_per_ml;
  float target_total = baseline + target_g;

  Serial.print("Dispense ");
  Serial.print(target_ml);
  Serial.print(" ml (~");
  Serial.print(target_g);
  Serial.println(" g)");

  // Nyalakan pompa
  digitalWrite(PUMP_PIN, HIGH);

  while (scale.get_units(5) < target_total) {
    Serial.print("Berat sekarang: ");
    Serial.println(scale.get_units(5));
    delay(100); // kurangi noise
  }

  // Matikan pompa
  digitalWrite(PUMP_PIN, LOW);
  Serial.println("Selesai dispense.");
  Serial.print("Berat akhir: ");
  Serial.println(scale.get_units(10));
}
