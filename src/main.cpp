#include <Arduino.h>

const int PUMP_PIN = 9;       // pin PWM ke Gate IRLZ44N
const int RAMP_STEP = 5;      // langkah naik/turun PWM
const int RAMP_DELAY = 20;    // jeda antar langkah (ms)

// ----------------------------
// SETUP
// ----------------------------
void setup() {
  pinMode(PUMP_PIN, OUTPUT);
  analogWrite(PUMP_PIN, 0); // pastikan pompa mati di awal
  Serial.begin(115200);
  Serial.println("=== Kontrol Pompa HEX + Timer ===");
  Serial.println("Format:");
  Serial.println("  #RRGGBB,durasi  -> nyala dgn intensitas warna & waktu");
  Serial.println("  ON,durasi       -> nyala default (200 PWM)");
  Serial.println("  OFF             -> matikan pompa");
  Serial.println("Contoh: #00B7EB,5  atau  ON,3");
}

// ----------------------------
// KONVERSI WARNA HEX KE INTENSITAS PWM
// ----------------------------
int hexToIntensity(const String &hex) {
  if (hex.length() < 7 || hex[0] != '#') return 0;
  int r = strtol(hex.substring(1,3).c_str(), NULL, 16);
  int g = strtol(hex.substring(3,5).c_str(), NULL, 16);
  int b = strtol(hex.substring(5,7).c_str(), NULL, 16);
  return constrain((r + g + b) / 3, 0, 255);
}

// ----------------------------
// TRANSISI PWM HALUS
// ----------------------------
void rampTo(int target) {
  static int current = 0; // ingat nilai terakhir
  target = constrain(target, 0, 255);
  
  if (target > current) {
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
  current = target;
  analogWrite(PUMP_PIN, target);
}

// ----------------------------
// LOOP UTAMA
// ----------------------------
void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    int commaIndex = input.indexOf(',');
    String cmd = input;
    int duration = 0; // dalam detik

    if (commaIndex != -1) {
      cmd = input.substring(0, commaIndex);
      duration = input.substring(commaIndex + 1).toInt();
    }

    // --- Perintah ON ---
    if (cmd.equalsIgnoreCase("ON")) {
      Serial.print("Pompa ON (PWM 200)");
      if (duration > 0) Serial.print(" selama "); Serial.print(duration); Serial.println(" detik");
      rampTo(200);
      if (duration > 0) {
        delay(duration * 1000);
        rampTo(0);
        Serial.println("Pompa OFF otomatis");
      }
    }
    // --- Perintah OFF ---
    else if (cmd.equalsIgnoreCase("OFF") || cmd.equalsIgnoreCase("STOP")) {
      rampTo(0);
      Serial.println("Pompa dimatikan");
    }
    // --- Input HEX warna ---
    else if (cmd.startsWith("#")) {
      int inten = hexToIntensity(cmd);
      Serial.print("HEX "); Serial.print(cmd);
      Serial.print(" → PWM "); Serial.println(inten);
      rampTo(inten);
      if (duration > 0) {
        delay(duration * 1000);
        rampTo(0);
        Serial.println("Pompa OFF otomatis");
      }
    }
    // --- Input tidak dikenal ---
    else {
      Serial.println("Format salah! Contoh: #00B7EB,5  atau  ON,3  atau  OFF");
    }
  }
}
