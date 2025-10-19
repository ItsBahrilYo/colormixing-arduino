#include <Arduino.h>
#include <Servo.h>

Servo mixerServo;  // Servo untuk pengaduk

void setup() {
  mixerServo.attach(9);   // Pin sinyal servo ke pin D9
  Serial.begin(9600);
  Serial.println("Servo pengaduk siap!");
}

void loop() {
  // Putar searah jarum jam selama 3 detik
  mixerServo.write(180);   // 180 = CW (penuh)
  delay(3000);             // putar selama 3 detik
  
  // Berhenti 1 detik
  mixerServo.write(90);    // 90 = stop
  delay(1000);

  // Putar berlawanan jarum jam selama 3 detik
  mixerServo.write(0);     // 0 = CCW (penuh)
  delay(3000);

  // Berhenti lagi
  mixerServo.write(90);    
  delay(2000);
}
