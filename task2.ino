#include <Servo.h>

Servo base, rArm, fArm, claw;

// 当前角度 & 目标角度
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
    if (!running) break;  

    Serial.print("=== Picking Object #");
    Serial.print(i + 1);
    Serial.println(" ===");

    // 第1步：移动到拾取位置上方（夹爪保持张开）
    moveTo(PICK[i][0], PICK[i][1], PICK[i][2], PICK[i][3]);
    delay(300);

    // 第2步：闭合夹爪抓取
    moveTo(PICK[i][0], PICK[i][1], PICK[i][2], 0);
    delay(600);

    // 第3步：抬起（保持夹爪闭合）
    moveTo(PICK[i][0], 90, 90, 0);
    delay(300);

    // 第4步：旋转到底座到放置位置
    moveTo(PLACE[0], 90, 90, 0);
    delay(300);

    // 第5步：下探到放置位置
    moveTo(PLACE[0], PLACE[1], PLACE[2], 0);
    delay(300);

    // 第6步：张开夹爪释放
    moveTo(PLACE[0], PLACE[1], PLACE[2], 180);
    delay(600);

    // 第7步：抬臂并返回待机位
    moveTo(HOME[0], HOME[1], HOME[2], HOME[3]);

    Serial.print("Object #");
    Serial.print(i + 1);
    Serial.println(" placed. Moving to next...");
    delay(500);
  }

  running = false;
  Serial.println(">>> All 3 objects processed. Send 'A' to repeat.");
}

// 平滑插值函数
void moveTo(int b, int r, int f, int c) {
  tgtBase = b; tgtR = r; tgtF = f; tgtC = c;

  int steps = max(max(abs(tgtBase - curBase), abs(tgtR - curR)),
                  max(abs(tgtF - curF), abs(tgtC - curC)));
  if (steps == 0) return;

  float dB = (float)(tgtBase - curBase) / steps;
  float dR = (float)(tgtR - curR) / steps;
  float dF = (float)(tgtF - curF) / steps;
  float dC = (float)(tgtC - curC) / steps;

  for (int i = 1; i <= steps; i++) {
    curBase = constrain((int)(tgtBase - dB * (steps - i)), 0, 180);
    curR    = constrain((int)(tgtR    - dR * (steps - i)), 0, 180);
    curF    = constrain((int)(tgtF    - dF * (steps - i)), 0, 180);
    curC    = constrain((int)(tgtC    - dC * (steps - i)), 0, 180);

    base.write(curBase);
    rArm.write(curR);
    fArm.write(curF);
    claw.write(curC);

    delay(15);
  }
}