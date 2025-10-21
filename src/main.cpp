#include "HX711.h"

// Pin HX711
#define DOUT  12
#define CLK   13

// Pin pompa (via MOSFET)
#define PUMP_R 3
#define PUMP_G 4
#define PUMP_B 5

HX711 scale;

float calibration_factor = -2280.0;  // sesuaikan setelah kalibrasi loadcell
float total_target = 30.0;           // total target campuran dalam gram

void pumpToWeight(int pumpPin, float target);

void setup() {
  Serial.begin(9600);
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare(); // nolkan timbangan

  pinMode(PUMP_R, OUTPUT);
  pinMode(PUMP_G, OUTPUT);
  pinMode(PUMP_B, OUTPUT);

  Serial.println("Masukkan kode warna HEX (misal #33CCFF):");
}

void loop() {
  if (Serial.available()) {
    String hexColor = Serial.readStringUntil('\n');
    hexColor.trim();
    if (hexColor.startsWith("#")) hexColor.remove(0, 1);

    if (hexColor.length() == 6) {
      int r = strtol(hexColor.substring(0, 2).c_str(), NULL, 16);
      int g = strtol(hexColor.substring(2, 4).c_str(), NULL, 16);
      int b = strtol(hexColor.substring(4, 6).c_str(), NULL, 16);

      Serial.print("RGB: ");
      Serial.print(r); Serial.print(", ");
      Serial.print(g); Serial.print(", ");
      Serial.println(b);

      // Hitung total dan proporsi
      int total = r + g + b;
      float targetR = total_target * (r / (float)total);
      float targetG = total_target * (g / (float)total);
      float targetB = total_target * (b / (float)total);

      Serial.println("Target gram:");
      Serial.print("R: "); Serial.println(targetR);
      Serial.print("G: "); Serial.println(targetG);
      Serial.print("B: "); Serial.println(targetB);

      // Reset berat awal
      scale.tare();

      // --- Pompa Merah ---
      pumpToWeight(PUMP_R, targetR);
      // --- Pompa Hijau ---
      pumpToWeight(PUMP_G, targetG);
      // --- Pompa Biru ---
      pumpToWeight(PUMP_B, targetB);

      Serial.println("Campuran selesai!\n");
      Serial.println("Masukkan warna berikutnya:");
    } else {
      Serial.println("Format salah! Gunakan format #RRGGBB");
    }
  }
}

// ------------------------
// Fungsi untuk pompa hingga berat target
// ------------------------
void pumpToWeight(int pumpPin, float target) {
  scale.tare();
  digitalWrite(pumpPin, HIGH);
  while (scale.get_units() < target) {
    Serial.print("Berat: ");
    Serial.println(scale.get_units());
    delay(100);
  }
  digitalWrite(pumpPin, LOW);
  Serial.print("Selesai pompa, berat akhir: ");
  Serial.println(scale.get_units());
}
