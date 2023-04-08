#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte diachi[6] = "12345";

// nrf24l01: IRQ-D8 CE-D9 CSN-D10 MOSI-D11 MISO-D12 SCK-13

byte nhan[30];

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_KEP;

#define servo1  3
#define servo2  4
#define servo3  5 
#define servo4  6
#define servo5  7
#define servoKEP  8

byte macdinh = 1; unsigned int dem = 0;

void home()
{
  servo_1.write(90);
  servo_2.write(30);
  servo_3.write(0);
  servo_4.write(90);
  servo_5.write(180);
  servo_KEP.write(180); //MO
}

void DK_1()
{
  byte goc; byte cong; byte tru;

  goc = servo_1.read();
  if(nhan[0]>=50 && nhan[0]<=100) //phai
  {
    if(goc > 0)
    {
      tru = map(nhan[0], 50, 99, 0, 1);
      goc = goc - tru;
      servo_1.write(goc);
      cong = 0;
    }
    else //Neu goc_A < 0
    {
      cong = 0; tru = 0;
      servo_1.write(0);
    }
  }
  else //trai.....0-49
  {
    if(goc < 180) //lon hon 0
    {
      cong = map(nhan[0], 49, 0, 0, 1);
      goc = goc + cong;
      servo_1.write(goc);
      tru = 0;
    }
    else //lon hon 180
    {
      cong = 0; tru = 0;
      servo_1.write(180);
    }
  }   
}

void DK_2()
{
  byte goc; byte cong; byte tru;

  goc = servo_2.read();
  if(nhan[1]>=50 && nhan[1]<=100) //len
  {
    if(goc > 0)
    {
      tru = map(nhan[1], 50, 99, 0, 1);
      goc = goc - tru;
      servo_2.write(goc);
      cong = 0;
    }
    else //Neu goc_A < 0
    {
      cong = 0; tru = 0;
      servo_2.write(0);
    }
  }
  else //xuong.....0-49
  {
    if(goc < 180) //lon hon 0
    {
      cong = map(nhan[1], 49, 0, 0, 1);
      goc = goc + cong;
      servo_2.write(goc);
      tru = 0;
    }
    else //nho hon 0
    {
      cong = 0; tru = 0;
      servo_2.write(180);
    }
  }   
}

byte cong(byte goc)
{
  if(goc < 180) goc = goc + 1;
  else  goc = 180;
  return goc;
}

byte tru(byte goc)
{
  if(goc > 0) goc = goc - 1;
  else  goc = 0;
  return goc;
}

void control(byte servo)
{
  byte goc;
  if(servo == 3)
  {
    goc = servo_3.read();
    if(nhan[2] == 0)  servo_3.write(tru(goc));
    else if(nhan[3] == 0) servo_3.write(cong(goc));   
  }
  else if(servo == 4)
  {
    goc = servo_4.read();
    if(nhan[4] == 0)  servo_4.write(cong(goc));
    else if(nhan[5] == 0) servo_4.write(tru(goc));       
  }
  else if(servo == 5)
  {
    goc = servo_5.read();
    if(nhan[6] == 0)  servo_5.write(cong(goc));
    else if(nhan[7] == 0) servo_5.write(tru(goc));       
  }  
}

void setup() 
{
  Serial.begin(115200);

  servo_1.attach(servo1);
  servo_2.attach(servo2); 
  servo_3.attach(servo3);
  servo_4.attach(servo4);
  servo_5.attach(servo5);
  servo_KEP.attach(servoKEP);

  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  }   

  radio.openReadingPipe(1, diachi);
  radio.setPALevel(RF24_PA_MIN); //Cài bộ khuyết địa công suất ở mức MIN
  radio.setChannel(120);
  radio.setDataRate(RF24_250KBPS);   
  radio.startListening(); //Bta dau nhan

  if (!radio.available()) //khong ket noi
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }     

  home();
}

void loop() 
{
  if (radio.available()) 
  {
    radio.read(&nhan, sizeof(nhan));
  }

  if(nhan[4]==0 && nhan[5]==0)
    home();
  else 
  {
    if(nhan[8] != macdinh)
    {
      if(nhan[8] == 0)
        dem++;
      macdinh = nhan[8];
    }
    if(dem%2 == 0)
      servo_KEP.write(180);
    else servo_KEP.write(0);

    DK_1();
    DK_2();
    control(3);
    control(4);
    control(5);
  }

  Serial.print("X= "); Serial.print(nhan[0]); Serial.print("  - Y= "); Serial.print(nhan[1]);
  Serial.print("  - A= "); Serial.print(nhan[2]); Serial.print("  - C= "); Serial.print(nhan[3]);
  Serial.print("  - B= "); Serial.print(nhan[4]); Serial.print("  - D= "); Serial.print(nhan[5]);
  Serial.print("  - E= "); Serial.print(nhan[6]); Serial.print("  - F= "); Serial.print(nhan[7]);
  Serial.print("  - KEP= "); Serial.print(nhan[8]);
  Serial.print("  - DEM= "); Serial.print(dem);

  Serial.println();  
  delay(30);
}