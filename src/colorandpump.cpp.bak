#include <Arduino.h>
// Pin pompa
#define PUMP_R 3
#define PUMP_G 5
#define PUMP_B 6

String inputString = "";

void setup() {
  Serial.begin(9600);
  pinMode(PUMP_R, OUTPUT);
  pinMode(PUMP_G, OUTPUT);
  pinMode(PUMP_B, OUTPUT);

  digitalWrite(PUMP_R, LOW);
  digitalWrite(PUMP_G, LOW);
  digitalWrite(PUMP_B, LOW);
  
  Serial.println("Masukkan kode warna HEX (#RRGGBB):");
}

void loop() {
  if (Serial.available()) {
    inputString = Serial.readStringUntil('\n');
    inputString.trim();

    //Mengubah nilai HEX menjadi Decimal agar mudah dibaca MCU
    if (inputString.charAt(0) == '#') {
      long number = strtol(&inputString[1], NULL, 16);
      int red   = (number >> 16) & 0xFF;
      int green = (number >> 8) & 0xFF;
      int blue  = number & 0xFF;

      Serial.print("R: "); Serial.print(red);
      Serial.print("  G: "); Serial.print(green);
      Serial.print("  B: "); Serial.println(blue);

      // Ubah nilai RGB menjadi durasi pompa (0–255 → 0–5000 ms)
      int durR = map(red, 0, 255, 0, 5000);
      int durG = map(green, 0, 255, 0, 5000);
      int durB = map(blue, 0, 255, 0, 5000);

      // Aktifkan pompa
      if (durR > 0) { digitalWrite(PUMP_R, HIGH); delay(durR); digitalWrite(PUMP_R, LOW); }
      if (durG > 0) { digitalWrite(PUMP_G, HIGH); delay(durG); digitalWrite(PUMP_G, LOW); }
      if (durB > 0) { digitalWrite(PUMP_B, HIGH); delay(durB); digitalWrite(PUMP_B, LOW); }

      Serial.println("Campuran selesai!\n");
    } else {
      Serial.println("Format salah! Gunakan format #RRGGBB, contoh: #FF8040");
    }
  }
}
