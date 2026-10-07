#include <Servo.h>  

Servo base, fArm, rArm, claw;    

const int baseMin = 0, baseMax = 180;
const int rArmMin = 0, rArmMax = 180;
const int fArmMin = 0, fArmMax = 180;
const int clawMin = 0, clawMax = 180;

int Delay = 20;
int Step = 3; 

void setup() {
  Serial.begin(9600);
  base.attach(11);     
  rArm.attach(10);     
  fArm.attach(6);      
  claw.attach(9);      
  
  base.write(90); 
  fArm.write(90); 
  rArm.write(90); 
  claw.write(90);  
  
  Serial.println("MeArm Joystick Control Ready!");
  Serial.println("Commands: a(左) d(右) s(下) w(上) i(fArm上) k(fArm下) j(夹爪关) l(夹爪开)");
}

void loop(){
    if (Serial.available() > 0) {  
        servo();
    }
    delay(50);  
}

void servo(){
    char servoName = Serial.read();  
    
    int basePos, rArmPos, fArmPos, clawPos;
    
    switch(servoName){
        case 'a': // Base左
            Serial.println("Base左");                
            basePos = base.read() - Step;
            basePos = constrain(basePos, baseMin, baseMax);
            base.write(basePos);
            Serial.println(basePos); 
            delay(Delay);
            break;  
            
        case 'd': // Base右
            Serial.println("Base右");                
            basePos = base.read() + Step;
            basePos = constrain(basePos, baseMin, baseMax);
            base.write(basePos);
            Serial.println(basePos); 
            delay(Delay);
            break;        
            
        case 's': // rArm下
            Serial.println("rArm下");                
            rArmPos = rArm.read() + Step;
            rArmPos = constrain(rArmPos, rArmMin, rArmMax);
            rArm.write(rArmPos);
            Serial.println(rArmPos); 
            delay(Delay);
            break;  
            
        case 'w': // rArm上
            Serial.println("rArm上");     
            rArmPos = rArm.read() - Step;
            rArmPos = constrain(rArmPos, rArmMin, rArmMax);
            rArm.write(rArmPos);
            delay(Delay);
            Serial.println(rArmPos); 
            break;  
            
        case 'i': // fArm上
            Serial.println("fArm上");        
            fArmPos = fArm.read() + Step;
            fArmPos = constrain(fArmPos, fArmMin, fArmMax);
            fArm.write(fArmPos);
            delay(Delay);
            Serial.println(fArmPos); 
            break;  
            
        case 'k': // fArm下
            Serial.println("fArm下");        
            fArmPos = fArm.read() - Step;
            fArmPos = constrain(fArmPos, fArmMin, fArmMax);
            fArm.write(fArmPos);
            Serial.println(fArmPos); 
            delay(Delay);
            break;  
            
        case 'j': // Claw关闭
            Serial.println("Claw关闭");        
            clawPos = claw.read() + Step;
            clawPos = constrain(clawPos, clawMin, clawMax);
            claw.write(clawPos);
            Serial.println(clawPos);
            delay(Delay);
            break;  
            
        case 'l': // Claw打开
            Serial.println("Claw打开");     
            clawPos = claw.read() - Step;
            clawPos = constrain(clawPos, clawMin, clawMax);
            claw.write(clawPos);
            Serial.println(clawPos);
            delay(Delay);
            break;  
            
        default:
            Serial.print("Unknown command: ");
            Serial.println(servoName);
            break;
    }
}