#include <Servo.h>  

Servo claw;    
int Delay = 50;  
int Step = 5;      
int Angle;

void setup() {
  Serial.begin(9600);     
  claw.attach(9);      
  delay(200);           
  claw.write(90); 
  
  Serial.println("Claw Control Ready! Commands: O(Open) C(Close) S(Slow) H(Fast)");
}

void loop(){
  char servoName = Serial.read(); 
  if (servoName != -1) {  
    switch(servoName){
      case 'O': { // Open
        for(Angle=0; Angle<=180; Angle++){
          claw.write(Angle);
          delay(Delay); 
        } 
        break;  
      }
      case 'C': { // Close
        for(Angle=180; Angle>=0; Angle--){
          claw.write(Angle);
          delay(Delay); 
        }
        break;
      }
      case 'S': { // Slow down
        if (Delay >= 100) {
          Serial.println("Too Slow!!!");
        } else {
          Delay += Step;
        }
        break;
      }       
      case 'H': { // Speed up
        if (Delay <= 5) {
          Serial.println("Too High!!!");
        } else {
          Delay -= Step;
        }
        break;
      }
      default:
        Serial.print("Unknown command: ");
        Serial.println(servoName);
        break;
    }
  }
  delay(50); 

}