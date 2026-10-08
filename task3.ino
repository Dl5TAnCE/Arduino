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



}