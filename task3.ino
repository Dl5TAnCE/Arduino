#include <Servo.h>

Servo base, rArm, fArm, claw; 
const int pinBtn1 = 2, pinBtn2 = 3, pinBtn3 = 4, pinBtn4 = 5;
const int pinJoyX = A0, pinJoyY = A1, pinJoyZ = A2, pinJoyG = A3; 

const int PICK_A[4] = {60, 60, 130, 180};  
const int PLACE_A[4] = {60, 90, 90, 0};    
const int PICK_B[4] = {90, 55, 125, 180};  
const int PLACE_B[4] = {90, 90, 90, 0};    
const int PICK_C[4] = {120, 60, 130, 180}; 
const int PLACE_C[4] = {120, 90, 90, 0};   
const int HOME[4] = {90, 90, 90, 90};      

void setup() {
  base.attach(11); rArm.attach(10); fArm.attach(9); claw.attach(6);
  pinMode(pinBtn1, INPUT_PULLUP); pinMode(pinBtn2, INPUT_PULLUP);
  pinMode(pinBtn3, INPUT_PULLUP); pinMode(pinBtn4, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.println("MeArm Multi-Function System Ready!");
  moveTo(HOME[0], HOME[1], HOME[2], HOME[3]);
}

void loop(){
  if (isReplaying) {
    replayActions();
    return; 
  }

  if (isRecording) {
    controlWithJoystick();
    recordActions();
    return;
  }

  if (digitalRead(pinBtn1) == LOW) {
    delay(50); 
    if (digitalRead(pinBtn1) == LOW) executePickPlaceCycle();
    while(digitalRead(pinBtn1) == LOW); 
}

void recordActions() {
  if (millis() - recordStartTime >= RECORD_DURATION) {
    Serial.println(">>> Auto-stopped: Reached 10s limit.");
    isRecording = false; 
    return;
  }
  static unsigned long lastRecordTime = 0;
  if (millis() - lastRecordTime >= RECORD_INTERVAL && recordedCount < MAX_RECORDS) {
    recordedActions[recordedCount].posBase = curB;
    recordedActions[recordedCount].posRArm = curR;
    recordedActions[recordedCount].posFArm = curF;
    recordedActions[recordedCount].posClaw = curC;
    recordedCount++;
    lastRecordTime = millis();
  }
}

void replayActions() {
  for (int i = 0; i < recordedCount; i++) {
    moveTo(recordedActions[i].posBase, recordedActions[i].posRArm, 
           recordedActions[i].posFArm, recordedActions[i].posClaw);
    delay(RECORD_INTERVAL); // 按照录制时的间隔还原时间轴
  }
  Serial.println(">>> REPLAY COMPLETE.");
  isReplaying = false;
}

void controlWithJoystick() {
  int x = analogRead(pinJoyX), y = analogRead(pinJoyY);
  int z = analogRead(pinJoyZ), g = analogRead(pinJoyG);
  
  if (x < 400) curB = constrain(curB + 1, 0, 180);
  if (x > 600) curB = constrain(curB - 1, 0, 180);
  if (y < 400) curR = constrain(curR + 1, 0, 180);
  if (y > 600) curR = constrain(curR - 1, 0, 180);
  if (z < 400) curF = constrain(curF + 1, 0, 180);
  if (z > 600) curF = constrain(curF - 1, 0, 180);
  if (g < 400) curC = constrain(curC + 1, 0, 180);
  if (g > 600) curC = constrain(curC - 1, 0, 180);

  base.write(curB); rArm.write(curR); fArm.write(curF); claw.write(curC);
}