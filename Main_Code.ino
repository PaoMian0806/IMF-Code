/* 開始導入相關函式庫 */

#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "DHT.h"

/* 開始Define板子腳位、資料型態 */

#define DHTPIN 2  
#define DHTTYPE DHT22 
#define IN1 7
#define IN2 8
#define ENA 5
#define Relay 4

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial BT(11, 10); /* 應該是序列埠的通訊腳 */

float *LCD_FUNCTION(String Num1,String Num2,String Num3,String Num4);
int FAN_STATE(float *Num4);
void FAN_MODE (int Num5);
void MAIN_SCENE();
void CHECK();
void FAN_WORKING();

LiquidCrystal_PCF8574 lcd(0x27); /* 設定LCD螢幕最多顯示兩行16個字元 */

int LCD_Show = -1; /* 宣告LCD_Show初始值 */
int Normal_Temp = 25; /* 正常室溫 */
int PWMA = 0;

String Serial_String = "NULL";
String Main_String = "NULL";

String LCDS = "LCD-ON";
String DHTS = "DHT-ON";
String ESPS = "ESP-ON";

float *DHT_DATA = NULL; /* 宣告一指向DHT_DATA，且未被使用的指標變數 */

void setup()
{
  /* 設定LCD螢幕最多顯示兩行16個字元 */
  int error; /* 宣告整數型態ERROR以檢測運行錯誤 */

  Serial.begin(9600); /* 設定序列埠為9600，建議不更動 */
  BT.begin(9600); /* 藍芽序列埠同為9600，建議不更動 */
  Serial.setTimeout(1500); /* 防止序列埠多次Println時Memory Leak，以及部份情況下Println發生亂碼問題 */
  Serial.println("===================");
  Serial.println(F("Fans Debugging Menu")); /* 開始畫面 - 主選單 */
  Serial.println("===================");

  while (!Serial)
    ; /* 開發板需讀取到序列埠才執行程式，故添加之 */
    
  Serial.println("檢查LCD是否連接..."); /* 開始檢查是否讀取到LCD */

  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("狀態: ");
  Serial.println(error); /* 回傳錯誤值（1或0）*/

  if (error == 0){ /* 如果回傳值0則讀取成功 */
    Serial.println("成功偵測到LCD");
    pinMode (IN1, OUTPUT);
    pinMode (IN2, OUTPUT);
    pinMode (ENA, OUTPUT);
    pinMode (Relay, OUTPUT);
    LCD_Show = 0;
    lcd.begin(16, 2); /* 初始化LCD */
    dht.begin();

    digitalWrite(Relay, HIGH);
    MAIN_SCENE();

  }
  else{
    Serial.println("未偵測到LCD");
  }
}


void loop() /* 迴圈函數，中止程式之前將不斷重複 */
{
  while(BT.available()){  /* 藍芽 */

      delay(100);
      Main_String = BT.readString();

      if(Main_String.toInt()!=0){
        if(Main_String.toInt()>= 0 && Main_String.toInt()<=255){     
          PWMA=Main_String.toInt();
          Serial.flush();
         } 
      }
      else{
        /* 此處程式碼需要優化，勿用大量If..Else */
        if(Main_String=="0"){
          PWMA=0;
          Serial.flush();
         }
        else if((Serial_String=="Set" || Serial_String=="Set\n") && (Main_String=="LCD-ON\n" || Main_String=="LCD-ON" || Main_String=="LCD-OFF\n" || Main_String=="LCD-OFF")){
          LCDS=Main_String;
          Serial.flush();
         }
        else if((Serial_String=="Set" || Serial_String=="Set\n") && (Main_String=="DHT-ON\n" || Main_String=="DHT-ON" || Main_String=="DHT-OFF\n" || Main_String=="DHT-OFF")){  
          DHTS=Main_String;
          Serial.flush();
         }
        else if((Serial_String=="Set" || Serial_String=="Set\n") && (Main_String=="ESP-ON\n" || Main_String=="ESP-ON" || Main_String=="ESP-OFF\n" || Main_String=="ESP-OFF")){ 
          ESPS=Main_String;
          Serial.flush();
         }
        else{
          Serial_String=Main_String;
          Serial.flush();
         }
      } 
  }   
    
  while(Serial.available()){ /* 監控視窗 */
    
      delay(100);
      Main_String = Serial.readString();
      
      if(Main_String.toInt()!=0){
        if(Main_String.toInt()>= 0 && Main_String.toInt()<=255){
          PWMA=Main_String.toInt();
          Serial.flush();
          Serial.println("調整PWM值...");
         }
      }
      else{
        if(Main_String=="0\n" || Main_String=="0"){
          PWMA=0;
          Serial.flush();
          Serial.println("調整PWM值...");
         }
        else{
          if((Serial_String=="Set" || Serial_String=="Set\n") && (Main_String=="LCD-ON\n" || Main_String=="LCD-ON" || Main_String=="LCD-OFF\n" || Main_String=="LCD-OFF")){
            LCDS=Main_String;
            Serial.flush();
            Serial.println("調整LCD背板...");
           }
          else if((Serial_String=="Set" || Serial_String=="Set\n") && (Main_String=="DHT-ON\n" || Main_String=="DHT-ON" || Main_String=="DHT-OFF\n" || Main_String=="DHT-OFF")){
            DHTS=Main_String;
            Serial.flush();
            Serial.println("調整DHT資料顯示...");
           }
          else if((Serial_String=="Set" || Serial_String=="Set\n") && (Main_String=="ESP-ON\n" || Main_String=="ESP-ON" || Main_String=="ESP-OFF\n" || Main_String=="ESP-OFF")){
            ESPS=Main_String;
            Serial.flush();
            Serial.println("調整ESP32的使用狀態...");
           }
          else{
            Serial_String = Main_String; 
           }   
         }
      }
      if(Serial_String.equals("Auto\n")){ /* 待處理：輸入字串時結尾會多出換行符號 */
         Serial.println("載入自動模式...");
         Serial.flush();
      }
      else if(Serial_String.equals("Set\n")){
         Serial.println("載入手動模式...");
         Serial.flush();
      }
      else if(Serial_String.equals("Exit\n")){
         Serial.println("正在退出...");
         Serial.flush();
      }
  }
  if(Serial_String.equals("Auto\n") || Serial_String.equals("Auto")){ 
    float *Temp_LCD = LCD_FUNCTION(LCDS,DHTS,Serial_String,ESPS);
    if(Temp_LCD != NULL){
       delay(50);
       digitalWrite(Relay, LOW);
       int Placeholder = FAN_STATE(Temp_LCD);
       delay(500);
       FAN_WORKING();
       CHECK();
       delay(100);
      }
  }
  if(Serial_String.equals("Set\n") || Serial_String.equals("Set")){
    float *Temp_LCD = LCD_FUNCTION(LCDS,DHTS,Serial_String,ESPS);
    if(Temp_LCD != NULL){
       FAN_WORKING();
       delay(500);
       CHECK();
      }
  }    
  if(Serial_String.equals("Exit\n") || Serial_String.equals("Exit")){
    digitalWrite(Relay, HIGH);
    lcd.setBacklight(0);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0, 0);
    MAIN_SCENE();
    PWMA = 0;
    delay(1000);
    FAN_WORKING();
    Serial_String = "NULL";
    Main_String = "NULL";
  }
}

float *LCD_FUNCTION(String Str1,String Str2,String SetMode,String Str3){ /* LCD以及DHT1602功能函數 */
  delay(500);
  
  if (DHT_DATA != NULL){
    delete [] DHT_DATA; /* 判斷DHT_DATA內部是否被分配到記憶體位址，並全數清除 */
  }

  DHT_DATA = new float[5];
  
  DHT_DATA[2] = dht.readHumidity();
  DHT_DATA[0] = dht.readTemperature();
  DHT_DATA[1] = dht.readTemperature(true);

  if (isnan(DHT_DATA[2]) || isnan(DHT_DATA[0]) || isnan(DHT_DATA[1])){
    Serial.println(F("啟動失敗: 未偵測到DHT11"));
    delay(1000);
    return NULL;
  }

  DHT_DATA[4] = dht.computeHeatIndex(DHT_DATA[1], DHT_DATA[2]);
  DHT_DATA[3] = dht.computeHeatIndex(DHT_DATA[0], DHT_DATA[2], false);

  if(Str3=="ESP-ON\n" || Str3=="ESP-ON"){
    digitalWrite(Relay,LOW);
  }
  else if((SetMode=="Set" || SetMode=="Set\n") && (Str3=="ESP-OFF\n" || Str3=="ESP-OFF")){
    digitalWrite(Relay,HIGH);
  }

  if(Str1=="LCD-ON\n" || Str1=="LCD-ON"){
    if (LCD_Show == 0) { /* LCD_Show狀態為0 */
       lcd.setBacklight(255);
       lcd.home();
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("H:");
       lcd.print(DHT_DATA[2]);
       lcd.print("%");
       lcd.setCursor(0, 1);
       lcd.print("Temp:");
       lcd.print(DHT_DATA[0]);
       lcd.print((char)223);
       lcd.print("C");
    }
    else if (LCD_Show == 1){
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("HI:");
       lcd.print(DHT_DATA[3]);
       lcd.print((char)223);
       lcd.print("C");
       lcd.setCursor(0, 1);
       lcd.print(DHT_DATA[4]);
       lcd.print((char)223);
       lcd.print("F");
    }
    else if (LCD_Show == 2){
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("PWM:");
       lcd.print(PWMA);
       lcd.setCursor(0, 1);
      
      if(PWMA >= 180 && PWMA <= 255){
         lcd.print("Mode:High");
      }
      else if(PWMA >= 120 && PWMA < 180){
         lcd.print("Mode:Normal");
      }
      else if(PWMA != 0 && PWMA <120){
         lcd.print("Mode:Low");
      }
      else{
         lcd.print("Mode:OFF");  
      }
    }
  }
  else if((SetMode=="Set" || SetMode=="Set\n") && (Str1=="LCD-OFF\n" || Str1=="LCD-OFF")){
    delay(500);
    lcd.setBacklight(0);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0, 0);
  }
  
  delay(500);
  LCD_Show = (LCD_Show + 1) % 3;

  if(Str2=="DHT-ON\n" || Str2=="DHT-ON"){
    Serial.flush();
    Serial.println("================");
    Serial.print("當前溫度(攝): ");
    Serial.print(DHT_DATA[0]);
    Serial.println("°C");
    Serial.print("當前溫度(華): ");
    Serial.print(DHT_DATA[1]);
    Serial.println("°F");
    Serial.print("當前濕度: ");
    Serial.print(DHT_DATA[2]);
    Serial.println("%");
    Serial.print("本日最高溫度(攝): ");
    Serial.print(DHT_DATA[3]);
    Serial.println("°C");
    Serial.print("本日最高溫度(華): ");
    Serial.print(DHT_DATA[4]);
    Serial.println("°F");
    Serial.println("================");
  }
  else if((SetMode=="Set" || SetMode=="Set\n") && (Str2=="DHT-OFF\n" || Str2=="DHT-OFF")){
    /* NULL */
  }

  char Hum_BT[10], Tem_BT[10], Tem_BTF[10], High_BT[10], HIgh_BTF[10];
  dtostrf(DHT_DATA[2], 3, 2, Hum_BT);
  dtostrf(DHT_DATA[0], 3, 2, Tem_BT);
  dtostrf(DHT_DATA[1], 3, 2, Tem_BTF);
  dtostrf(DHT_DATA[3], 3, 2, High_BT);
  dtostrf(DHT_DATA[4], 3, 2, HIgh_BTF);
  
  for(int i=0; i<5; i++){
    BT.write(Hum_BT[i]);
  }
  for(int i=0; i<5; i++){
    BT.write(Tem_BT[i]);
  }
  for(int i=0; i<5; i++){
    BT.write(Tem_BTF[i]);
  }
  for(int i=0; i<5; i++){
    BT.write(High_BT[i]);
  }
  for(int i=0; i<5; i++){
    BT.write(HIgh_BTF[i]);
  }
  
  return DHT_DATA;
}

void MAIN_SCENE(){
  Serial.println("============================");
  Serial.println("====== Fans Mode Menu ======");
  Serial.println("============================");
  Serial.println("Auto - 自動模式");
  Serial.println("Set - 手動模式");
}

void CHECK(){
  static int Check_Point = 0; /* 初始化執行次數 */
  
  Check_Point += 1;
  if (Check_Point % 5 == 0){
    Serial.println("如要返回至主選單，請輸入'Exit'指令。");
    Check_Point -=5;
  }
}
    

int FAN_STATE(float *x){ /* 風扇Mode的判斷函數 */
  
  if(x[0] < Normal_Temp){
    Serial.println("系統: 當前氣溫較低");
    PWMA = map(x[2], 0, 100, 0, 179);

    Serial.println(PWMA);
    Serial.print("比例:");
    Serial.println((PWMA/x[2]),3); 
  }
  else if(x[0] >= Normal_Temp){
    Serial.println("系統: 當前氣溫較高");
    PWMA = map(x[2], 0, 100, 180, 255);
    
    Serial.println(PWMA);
    Serial.print("比例:");
    Serial.println((PWMA/x[2]),3);
  }

  return 0;
}


void FAN_WORKING(){
  analogWrite(ENA, PWMA);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}
