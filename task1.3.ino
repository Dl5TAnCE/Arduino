#include <Servo.h>

Servo base, rArm, fArm, claw; 

void setup() {
  base.attach(11);  
  rArm.attach(10);  
  fArm.attach(9);   
  claw.attach(6);   
  
  Serial.begin(9600);
  Serial.println("MeArm Multi-Servo Control Ready!");
  Serial.println("Format: base,rArm,fArm,claw (e.g., 90,60,120,60)");
  
  base.write(90); rArm.write(90); fArm.write(90); claw.write(90);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n'); 
    
    int angles[4];
    base.write(angles[0]);
    rArm.write(angles[1]);
    fArm.write(angles[2]);
    claw.write(angles[3]);
    
    Serial.print("Finish!: B="); Serial.print(angles[0]);
    Serial.print(", R="); Serial.print(angles[1]);
    Serial.print(", F="); Serial.print(angles[2]);
    Serial.print(", C="); Serial.println(angles[3]);
  }
}
