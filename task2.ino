#include <Servo.h>

Servo base, rArm, fArm, claw;

int curBase = 90, curR = 90, curF = 90, curC = 90;
int tgtBase = 90, tgtR = 90, tgtF = 90, tgtC = 90;

const int HOME[4] = {90, 90, 90, 90};
const int PLACE[4] = {45, 70, 110, 0};

const int PICK[3][4] = {
  {90,  60, 130, 180},  
  {60,  55, 125, 180},   
  {120, 55, 125, 180}   
};

void setup() {
  base.attach(11);
  rArm.attach(10);
  fArm.attach(9);
  claw.attach(6);

  Serial.begin(9600);
  Serial.println("MeArm 3-Object Pick-Place Ready!");
  Serial.println("Send 'A' to start auto cycle, 'S' to stop");

  moveTo(HOME[0], HOME[1], HOME[2], HOME[3]);
}

bool running = false;

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'A' || cmd == 'a') {
      running = true;
      Serial.println(">>> Starting 3-object pick-place cycle...");
    }
    if (cmd == 'S' || cmd == 's') {
      running = false;
      Serial.println(">>> Stopped.");
      return;
    }
  }

  if (!running) return;

  for (int i = 0; i < 3; i++) {
    if (!running) break;  }
