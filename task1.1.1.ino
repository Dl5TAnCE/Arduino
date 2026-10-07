#include <Servo.h>

Servo base, fArm, rArm, claw;

void setup() {
  Serial.begin(9600);
  base.attach(11);
  rArm.attach(10);
  fArm.attach(9);
  claw.attach(6);
  
  base.write(90);
  fArm.write(90);
  rArm.write(90);
  claw.write(90);
  
  Serial.println("MeArm Control Ready! Format: b90, r60, f120, c45");
}

void loop() {
  char servoName = 0;  // 
  
  if (Serial.available() > 0) {
    servoName = Serial.read();
    int angle = Serial.parseInt();
    
    Serial.print("servoName = ");
    Serial.print(servoName);
    Serial.print(" , angle = ");
    Serial.print(angle);
    Serial.println(" is moving.");
    
    // 
    switch(servoName) {
      case 'b':
        base.write(constrain(angle, 0, 180));
        break;
      case 'r':
        rArm.write(constrain(angle, 0, 180));
        break;
      case 'f':
        fArm.write(constrain(angle, 0, 180));
        break;
      case 'c':
        claw.write(constrain(angle, 0, 180));
        break;
      default:
        Serial.print("Unknown command: ");
        Serial.println(servoName);
        break;
    }
  }
  
  delay(50);
}