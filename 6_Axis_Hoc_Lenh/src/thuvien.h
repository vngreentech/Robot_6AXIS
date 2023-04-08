#include "Arduino.h"
#include "Servo.h"
#include "EEPROM.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

/*
* Servo S1-8; S2-9; S3-10; S4-11; S5-12; Kep-13
* Bien tro: B1-A0; B2-A1; B3-A2; B4-A3; B5-A4 
* Nut nhan KEP - 4
*
* Menu: len-7; xuong-6; enter-5
*/
#define enter 5
#define len 7
#define xuong 6
byte gt_len; byte gt_xuong; byte gt_enter;
int dem_menu=0; int dem_lenxuong=0;
int macdinh1 = 1; int macdinh2 = 1; int macdinh3 = 1;

Servo servo_S1;
Servo servo_S2;
Servo servo_S3;
Servo servo_S4;
Servo servo_S5;
Servo servo_kep;

#define servoS1  8
#define servoS2  9
#define servoS3  10
#define servoS4  11
#define servoS5  12 
#define servokep  13

#define nutnhan 4
#define bientro_B1 A0
#define bientro_B2 A1
#define bientro_B3 A2
#define bientro_B4 A3
#define bientro_B5 A4

int gt_bientro_B1; int gt_bientro_B2; int gt_bientro_B3; int gt_bientro_B4; int gt_bientro_B5;
int goc_S1;  int goc_S2;  int goc_S3; int goc_S4; int goc_S5; int goc_S6;
int gt_nutnhan; 
int biendem = 0;
int macdinh = 1; 

unsigned int vitri=1;

unsigned int diachi_S1=14; unsigned int diachi_S2=15; unsigned int diachi_S3=16;
unsigned int diachi_S4=17; unsigned int diachi_S5=18; unsigned int diachi_kep=19;
unsigned int diachi_cuoi;

int batdau=20;
int ketthuc;

unsigned long time=0;
unsigned long dem_chay=1; boolean run;

void man_hinh()
{
  lcd.setCursor(0,2);
  lcd.print("  6-AXIS ROBOT ARM  ");
  lcd.setCursor(2,3);
  lcd.print("Green Technology");
}

void XD_VT(int goc, int VT1, int VT2, int VT3, int hang)
{
    if(goc < 10)
    {
        lcd.setCursor(VT1,hang); lcd.print(goc);
        lcd.setCursor(VT2,hang); lcd.print("  ");
    }
    else if (goc < 100)
    {
        lcd.setCursor(VT1,hang); lcd.print(goc);
        lcd.setCursor(VT3,hang); lcd.print(" ");        
    }
    else    {lcd.setCursor(VT1,hang); lcd.print(goc);}
}

void VT_HT_goc(int servo)
{
    byte goc;
    if(servo == 1)
    {
        goc = servo_S1.read();
        XD_VT(goc, 3,4,5,0);
    }
    else if(servo == 2)
    {
        goc = servo_S2.read();
        XD_VT(goc, 10,11,12,0);
    }
    else if(servo == 3)
    {
        goc = servo_S3.read();
        XD_VT(goc, 17,18,19,0);
    }
    else if(servo == 4)
    {
        goc = servo_S4.read();
        XD_VT(goc, 3,4,5,1);
    }
    else if(servo == 5)
    {
        goc = servo_S5.read();
        XD_VT(goc, 10,11,12,1);
    }
    else if(servo == 6)
    {
        goc = servo_kep.read();
        XD_VT(goc, 17,18,19,1);
    }
}

void HT_goc()
{
    lcd.setCursor(0,0);lcd.print("S1:");
    VT_HT_goc(1);
    lcd.setCursor(7,0);lcd.print("S2:");
    VT_HT_goc(2);
    lcd.setCursor(14,0);lcd.print("S3:");      
    VT_HT_goc(3);

    lcd.setCursor(0,1);lcd.print("S4:");
    VT_HT_goc(4);
    lcd.setCursor(7,1);lcd.print("S5:");
    VT_HT_goc(5);
    lcd.setCursor(14,1);lcd.print("S6:");
    VT_HT_goc(6);
}

bool KT_co_lenh()
{
  boolean co;
  if(EEPROM.read(1) == 1) //Da co lenh duoc luu
    co = true;        
  else
    co = false;
  return co;
}

void Xem_lenh()
{
  Serial.print("1="); Serial.print(EEPROM.read(1));
  Serial.print("  - CUOI="); Serial.println(EEPROM.read(2));
  for(int i=10; i<40; i++)
  {
    Serial.print(i); Serial.print("="); Serial.print(EEPROM.read(i)); Serial.print("; ");
  }
}

void menu()
{
  if (dem_lenxuong == 0)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(">BACK");
    lcd.setCursor(0,1);
    lcd.print(" THEM LENH");
    lcd.setCursor(0,2);
    lcd.print(" XOA LENH");  
    lcd.setCursor(0,3);
    lcd.print(" CHAY LENH");
  }
  else if (dem_lenxuong == 1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" BACK");
    lcd.setCursor(0,1);
    lcd.print(">THEM LENH");
    lcd.setCursor(0,2);
    lcd.print(" XOA LENH");  
    lcd.setCursor(0,3);
    lcd.print(" CHAY LENH");     
  }
  else if(dem_lenxuong == 2)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" BACK");
    lcd.setCursor(0,1);
    lcd.print(" THEM LENH");
    lcd.setCursor(0,2);
    lcd.print(">XOA LENH");  
    lcd.setCursor(0,3);
    lcd.print(" CHAY LENH");    
  }
  else if(dem_lenxuong == 3)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" BACK");
    lcd.setCursor(0,1);
    lcd.print(" THEM LENH");
    lcd.setCursor(0,2);
    lcd.print(" XOA LENH");  
    lcd.setCursor(0,3);
    lcd.print(">CHAY LENH");    
  }
}

void dk_goc(int bientro, int servo)
{
  int gt_bt = analogRead(bientro);
  byte goc = map(gt_bt, 0, 1023, 0, 180);
  if (servo == 1){
    servo_S1.write(goc);
  }
  else if (servo == 2){
    servo_S2.write(goc);
  }
  else if (servo == 3){
    servo_S3.write(goc);
  }
  else if (servo == 4){
    servo_S4.write(goc);
  }
  else if (servo == 5){
    servo_S5.write(goc);
  }
  else {}
}

void dk_servo()
{
    gt_nutnhan = digitalRead(nutnhan);
    if (gt_nutnhan != macdinh){
    if (gt_nutnhan == 0){ biendem++;  }
    macdinh = gt_nutnhan; 
    }

    if (biendem%2==0){
    servo_kep.write(180);
    } else {servo_kep.write(0);}    

    dk_goc(bientro_B1, 1);
    dk_goc(bientro_B2, 2);
    dk_goc(bientro_B3, 3);
    dk_goc(bientro_B4, 4);
    dk_goc(bientro_B5, 5);    
}

void xoa_lenh()
{
  if(KT_co_lenh() == true) //Co LENH
  {
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("DANG XOA LENH...");        
    lcd.setCursor(4,2);
    lcd.print("VUI LONG CHO!"); 

    dem_menu = 0; dem_lenxuong = 0;

    for (int i = 0 ; i < 2000 ; i++) 
    {
      EEPROM.write(i, 0);
    } 

    // Xem_lenh();

    delay(500);

    lcd.clear();
    man_hinh();
  }
  else //Hien khong co lenh trong EEPROM
  {
    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("KHONG CO LENH NAO");
    lcd.setCursor(4,2);
    lcd.print("DUOC LUU...!");   

    dem_menu = 0; dem_lenxuong = 0;

    delay(3000);
    lcd.clear();
    man_hinh();    
  }
}

void Them_lenh()
{
  if(KT_co_lenh() == true) //Da co lenh duoc luu
  {
    lcd.clear();
    lcd.setCursor(3,1);
    lcd.print("DA CO LENH...!"); 
    delay(5000);
    lcd.clear();
    man_hinh();
    dem_menu = 0; dem_lenxuong = 0;         
  }
  else //Chua co lenh nao duoc luu
  {
    lcd.clear();
    lcd.setCursor(1,2);
    lcd.print("XIN MOI NHAP LENH");
    lcd.setCursor(4,3);
    lcd.print("VI TRI: ");
    lcd.setCursor(12,3);
    lcd.print(vitri);          
  }  
}

void luu_lenh()
{ 
  //14,15,16,17,18,19 ;  20,21,22,23,24,25 ; 26,27,28,29,30,31 ; 32,33,34,35,36,37
  diachi_S1 += 6;
  diachi_S2 += 6;
  diachi_S3 += 6;
  diachi_S4 += 6;
  diachi_S5 += 6;
  diachi_kep += 6;

  EEPROM.write(diachi_S1, servo_S1.read());
  EEPROM.write(diachi_S2, servo_S2.read());
  EEPROM.write(diachi_S3, servo_S3.read());
  EEPROM.write(diachi_S4, servo_S4.read());
  EEPROM.write(diachi_S5, servo_S5.read());
  EEPROM.write(diachi_kep, servo_kep.read());
}

void chay_lenh()
{
  if(KT_co_lenh() == true) //Neu co lenh
  {
    lcd.clear();
    lcd.setCursor(4,2);
    lcd.print("CHUONG TRINH"); 
    lcd.setCursor(6,3);
    lcd.print("DANG CHAY");      

    byte doc_S1; byte doc_S2; byte doc_S3;
    byte doc_S4; byte doc_S5; byte doc_kep; 
    ketthuc = EEPROM.read(2); //doc o nho cuoi cung
    while(run == true)
    {
      time = millis();
      gt_enter = digitalRead(enter);
      if(gt_enter == 1 && dem_menu==2 && dem_lenxuong == 3) 
      {
        while(batdau < ketthuc)
        {
          gt_enter = digitalRead(enter);
          if(millis() - time > 500 && gt_enter == 1)
          {
            HT_goc();
            doc_S1 = EEPROM.read(batdau);
            doc_S2 = EEPROM.read(batdau + 1);
            doc_S3 = EEPROM.read(batdau + 2);
            doc_S4 = EEPROM.read(batdau + 3);
            doc_S5 = EEPROM.read(batdau + 4);
            doc_kep = EEPROM.read(batdau + 5);

            servo_S1.write(doc_S1);
            servo_S2.write(doc_S2);
            servo_S3.write(doc_S3);
            servo_S4.write(doc_S4);
            servo_S5.write(doc_S5);
            servo_kep.write(doc_kep);

            Serial.print("Dem_menu: "); Serial.print(dem_menu); Serial.print(" | GT_ENTER: "); Serial.print(gt_enter);
            Serial.print("  - Time: "); Serial.print(time); Serial.print(" | Bat dau: "); Serial.print(batdau);
            Serial.print("  - S1: "); Serial.print(doc_S1); Serial.print("  - S2: "); Serial.print(doc_S2);
            Serial.print("  - S3: "); Serial.print(doc_S3); Serial.print("  - S4: "); Serial.print(doc_S4); 
            Serial.print("  - S5: "); Serial.print(doc_S5); Serial.print("  - KEP: "); Serial.print(doc_kep);
            Serial.print("  - BatDau: "); Serial.print(batdau); Serial.print("  - KetThuc: "); Serial.println(ketthuc);

            time = millis();
            batdau += 6;              
          }
          else if(gt_enter == 0)
          {
            run = false;
            break;
          }  
        }
      }
      else if(gt_enter == 0 && dem_menu==2 && dem_lenxuong == 3)
      {
        run = false;        
        break;
      }

      batdau = 20;
    }    
  }
  else //KHONG CO LENH NAO DUOC LUU
  {
    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("KHONG CO LENH NAO");
    lcd.setCursor(4,2);
    lcd.print("DUOC LUU...!");   

    dem_menu = 0; dem_lenxuong = 0;

    delay(3000);
    lcd.clear();
    man_hinh();       
  }
}
