#include <Arduino.h>
#include <Servo.h>

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6; //Kep

#define Servo_1 3
#define Servo_2 4
#define Servo_3 5
#define Servo_4 6
#define Servo_5 7
#define Servo_6 8 //Kep

#define bientro_1 A0
#define bientro_2 A1
#define bientro_3 A2
#define bientro_4 A3
#define bientro_5 A4
#define nutnhan 2
byte gt_nutnhan; int dem=0; byte macdinh = 1;

void setup() 
{
  Serial.begin(9600);

  pinMode(nutnhan, INPUT_PULLUP);
  servo_1.attach(Servo_1);
  servo_2.attach(Servo_2);
  servo_3.attach(Servo_3);
  servo_4.attach(Servo_4);
  servo_5.attach(Servo_5);
  servo_6.attach(Servo_6);  
}

void dk_goc(int bientro, int servo)
{
  int gt_bt = analogRead(bientro);
  byte goc = map(gt_bt, 0, 1023, 0, 180);
  if (servo == 1){
    servo_1.write(goc);
  }
  else if (servo == 2){
    servo_2.write(goc);
  }
  else if (servo == 3){
    servo_3.write(goc);
  }
  else if (servo == 4){
    servo_4.write(goc);
  }
  else if (servo == 5){
    servo_5.write(goc);
  }
  else {}
}

void loop() 
{
  gt_nutnhan = digitalRead(nutnhan);
  if (gt_nutnhan != macdinh){
    if (gt_nutnhan == 0){ dem++;  }
    macdinh = gt_nutnhan; 
  }
  
  if (dem%2==0){
    servo_6.write(180);
  } else {servo_6.write(0);}

  // servo_1.write(90);
  dk_goc(bientro_1, 1);
  dk_goc(bientro_2, 2);
  dk_goc(bientro_3, 3);
  dk_goc(bientro_4, 4);
  dk_goc(bientro_5, 5);
                   
  Serial.print("S1: "); Serial.print(servo_1.read());
  Serial.print("  - S2: "); Serial.print(servo_2.read());
  Serial.print("  - S3: "); Serial.print(servo_3.read());
  Serial.print("  - S4: "); Serial.print(servo_4.read());
  Serial.print("  - S5: "); Serial.print(servo_5.read());                    
  Serial.print("  - S6: "); Serial.print(servo_6.read());

  Serial.println();
  delay(30);                           
}