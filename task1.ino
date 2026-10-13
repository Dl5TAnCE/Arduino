#include <Servo.h>

Servo base, rArm, fArm, claw;

const int baseMin = 0, baseMax = 180;
const int rArmMin = 0, rArmMax = 180;
const int fArmMin = 0, fArmMax = 180;
const int clawMin = 0, clawMax = 180;

int Delay = 20;    
int Step = 3;      

int clawDelay = 50;   
int clawStep = 1;     
int clawAngle;        

void parseData(String data, int* angles) {
  int index = 0;
  char* token = strtok((char*)data.c_str(), ","); 
  
  while (token != NULL && index < 4) {
    angles[index++] = atoi(token); 
    token = strtok(NULL, ",");    
  }
}

void servo() {
  char servoName = Serial.read();
  
  int basePos, rArmPos, fArmPos, clawPos;
  
  switch(servoName){
      case 'a': {// Base左
          Serial.println("Base左");                
          basePos = base.read() - Step;
          basePos = constrain(basePos, baseMin, baseMax);
          base.write(basePos);
          Serial.println(basePos); 
          delay(Delay);
          break;  }
          
      case 'd': {// Base右
          Serial.println("Base右");                
          basePos = base.read() + Step;
          basePos = constrain(basePos, baseMin, baseMax);
          base.write(basePos);
          Serial.println(basePos); 
          delay(Delay);
          break;}        
          
      case 's': {// rArm下
          Serial.println("rArm下");                
          rArmPos = rArm.read() + Step;
          rArmPos = constrain(rArmPos, rArmMin, rArmMax);
          rArm.write(rArmPos);
          Serial.println(rArmPos); 
          delay(Delay);
          break; } 
          
      case 'w': {// rArm上
          Serial.println("rArm上");     
          rArmPos = rArm.read() - Step;
          rArmPos = constrain(rArmPos, rArmMin, rArmMax);
          rArm.write(rArmPos);
          delay(Delay);
          Serial.println(rArmPos); 
          break;  }
          
      case 'i': {// fArm上
          Serial.println("fArm上");        
          fArmPos = fArm.read() + Step;
          fArmPos = constrain(fArmPos, fArmMin, fArmMax);
          fArm.write(fArmPos);
          delay(Delay);
          Serial.println(fArmPos); 
          break;  }
          
      case 'k': {// fArm下
          Serial.println("fArm下");        
          fArmPos = fArm.read() - Step;
          fArmPos = constrain(fArmPos, fArmMin, fArmMax);
          fArm.write(fArmPos);
          Serial.println(fArmPos); 
          delay(Delay);
          break;  }
          
      case 'j': {// Claw关闭
          Serial.println("Claw关闭");        
          clawPos = claw.read() + Step;
          clawPos = constrain(clawPos, clawMin, clawMax);
          claw.write(clawPos);
          Serial.println(clawPos);
          delay(Delay);
          break; } 
          
      case 'l': {// Claw打开
          Serial.println("Claw打开");     
          clawPos = claw.read() - Step;
          clawPos = constrain(clawPos, clawMin, clawMax);
          claw.write(clawPos);
          Serial.println(clawPos);
          delay(Delay);
          break;  }
          
  }
}

void setup() {
  
  base.attach(11);
  rArm.attach(10);
  fArm.attach(6);
  claw.attach(9);
  
  Serial.begin(9600);
  
  // 初始化舵机到90度
  base.write(90); delay(200);
  rArm.write(90); delay(200);
  fArm.write(90); delay(200);
  claw.write(90); delay(200);
  
  // 打印控制说明
  Serial.println("=== MeArm Integrated Control ===");
  Serial.println("Joystick(a/s/d/w/i/k/j/l): File3 joystick control");
  Serial.println("Multi-servo(90,60,120,60): File1 multi-servo control");
  Serial.println("Claw(O/C/S/H): File2 claw control");
  Serial.println("Single servo(b90/r60/f120/c45): File4 single servo control");
}

void loop() {
  if (Serial.available() > 0) {
    char first = Serial.peek();
    
    if (first == 'a' || first == 's' || first == 'd' || first == 'w' || 
        first == 'i' || first == 'k' || first == 'j' || first == 'l') {
      servo();
    } else {
      // 读取所有可用字符（设置短超时避免阻塞）
      Serial.setTimeout(1);
      String data = Serial.readString();
      
      if (data.length() > 0) {
        if (data.indexOf(',') >= 0) {
          int newlinePos = data.indexOf('\n');
          if (newlinePos >= 0) data.remove(newlinePos);
          int crPos = data.indexOf('\r');
          if (crPos >= 0) data.remove(crPos);
          
          int angles[4];
          parseData(data, angles);
          
          base.write(angles[0]);
          rArm.write(angles[1]);
          fArm.write(angles[2]);
          claw.write(angles[3]);
          
          Serial.print("Finish!: B="); Serial.print(angles[0]);
          Serial.print(", R="); Serial.print(angles[1]);
          Serial.print(", F="); Serial.print(angles[2]);
          Serial.print(", C="); Serial.println(angles[3]);
        } else {
          char c = data.charAt(0);

          // O=Open(开爪) C=Close(关爪) S=Slow(减速) H=Fast(加速)
          if (c == 'O' || c == 'C' || c == 'S' || c == 'H') {
            for (int i = 0; i < data.length(); i++) {
              char cmd = data.charAt(i);
              switch(cmd) {
                case 'O': { // Open
                  for(clawAngle=0; clawAngle<=180; clawAngle++){
                    claw.write(clawAngle);
                    delay(clawDelay);
                  }
                  break;
                }
                case 'C': { // Close
                  for(clawAngle=180; clawAngle>=0; clawAngle--){
                    claw.write(clawAngle);
                    delay(clawDelay);
                  }
                  break;
                }
                case 'S': { // Slow down
                  if (clawDelay >= 100) {
                    Serial.println("Too Slow!!!");
                  } else {
                    clawDelay += clawStep;
                  }
                  break;
                }
                case 'H': { // Speed up
                  if (clawDelay <= 5) {
                    Serial.println("Too High!!!");
                  } else {
                    clawDelay -= clawStep;
                  }
                  break;
                }
              }
            }
          }
          else if (c == 'b' || c == 'r' || c == 'f' || c == 'c') {
            char sName = data.charAt(0);
            int angle = 0;
            for (int i = 1; i < data.length(); i++) {
              if (isdigit(data.charAt(i))) {//判断数字字符
                angle = angle * 10 + (data.charAt(i) - '0');
              }
            }
            
            Serial.print("servoName = ");
            Serial.print(sName);
            Serial.print(" , angle = ");
            Serial.print(angle);
            Serial.println(" is moving.");
            
            switch(sName) {
              case 'b': base.write(constrain(angle, 0, 180)); break;
              case 'r': rArm.write(constrain(angle, 0, 180)); break;
              case 'f': fArm.write(constrain(angle, 0, 180)); break;
              case 'c': claw.write(constrain(angle, 0, 180)); break;
            }
          }
        }
      }
    }
  }
  delay(50);
}
