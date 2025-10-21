#include <Arduino.h>
const int PUMP_PIN = 9;      // PWM pin
const int RAMP_STEP = 5;     // langkah PWM per iterasi (0-255)
const int RAMP_DELAY = 20;   // ms delay antar langkah (atur untuk kelembutan)

void setup() {
  pinMode(PUMP_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Masukkan HEX (contoh: #00B7EB) atau perintah ON/OFF/STOP");
}

int hexToIntensity(const String &hex) {
  if (hex.length() != 7 || hex[0] != '#') return 0;
  int r = strtol(hex.substring(1,3).c_str(), NULL, 16);
  int g = strtol(hex.substring(3,5).c_str(), NULL, 16);
  int b = strtol(hex.substring(5,7).c_str(), NULL, 16);
  int intensity = (r + g + b) / 3; // 0..255
  return intensity;
}

void rampTo(int target) {
  target = constrain(target, 0, 255);
  int current = 0;
  // baca nilai saat ini (kita tak punya fungsi readPWM -> simpan jika perlu)
  // untuk kesederhanaan mulai dari 0 setiap kali
  if (target >= current) {
    for (int v = current; v <= target; v += RAMP_STEP) {
      analogWrite(PUMP_PIN, v);
      delay(RAMP_DELAY);
    }
  } else {
    for (int v = current; v >= target; v -= RAMP_STEP) {
      analogWrite(PUMP_PIN, v);
      delay(RAMP_DELAY);
    }
  }
  analogWrite(PUMP_PIN, target);
}

void loop() {
  if (Serial.available() > 0) {
    String in = Serial.readStringUntil('\n');
    in.trim();
    if (in.equalsIgnoreCase("ON")) {
      rampTo(200); // contoh default power
      Serial.println("Pompa ON (soft-start)");
    } else if (in.equalsIgnoreCase("OFF") || in.equalsIgnoreCase("STOP")) {
      rampTo(0);
      Serial.println("Pompa OFF (soft-stop)");
    } else if (in.startsWith("#") && in.length() == 7) {
      int inten = hexToIntensity(in); // 0..255
      Serial.print("HEX: "); Serial.print(in);
      Serial.print(" -> PWM: "); Serial.println(inten);
      rampTo(inten);
    } else {
      Serial.println("Perintah tidak dikenal. Contoh: #00B7EB  atau ON / OFF");
    }
  }
}
