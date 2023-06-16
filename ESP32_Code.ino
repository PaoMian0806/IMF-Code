#include <WiFiClientSecure.h>
#define FBJS_ENABLE_WIFI_CLIENT_SECURE

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#ifndef DEFAULT_FLASH_FS
#include <FS.h>
#include <SPIFFS.h>
#define DEFAULT_FLASH_FS SPIFFS
#endif

#include <addons/TokenHelper.h>

#include <addons/RTDBHelper.h>

#define WIFI_SSID "XXXXXX" /* 輸入你的WiFi SSID */
#define WIFI_PASSWORD "XXXXXX" /* 輸入你的WiFi密碼 */

#define API_KEY "AIzaSyD8wZZWSAf1sxcVX5fCBaTJQ6Ae9rHBpCo"

#define DATABASE_URL "https://XXXXXX.firebaseio.com" /* 輸入你的FireBase Database網址 */

/* Define 擁有資料庫存取權限的使用者信箱、密碼 */
#define USER_EMAIL "XXXXXX"
#define USER_PASSWORD "XXXXXX"

/* Define FireBaseJson以及FireBaseData Type的變數 */
FirebaseJson WebJsonData;
FirebaseData fbdo;

/* Define 資料庫用戶的有效憑證 */
FirebaseAuth auth;
FirebaseConfig config;

/* GET HTTPS以及傳送資料的逾時秒數 */
unsigned long sendDataPrevMillis = 0;
unsigned long ms = 0;

unsigned long count = 0;

void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("正在連接目標WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("正在連接目標IP： ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    config.api_key = API_KEY;

    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    config.database_url = DATABASE_URL;

    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);
    Firebase.enableClassicRequest(fbdo, true);
    
    fbdo.setResponseSize(8192);
    Firebase.setMaxRetry(fbdo, 3);
}

void loop()
{
        
  if (millis() - ms > 15000 || ms == 0)
    {
        ms = millis();
        
        WiFiClientSecure sslClient;
        /* 跳過SSL證書認證 */
        sslClient.setInsecure();

        Serial.print("正在連接目標伺服器...");

        if (sslClient.connect("opendata.cwb.gov.tw", 443))
        {
            Serial.println(" 成功");
            /* 輸入你GET POST的網址 */
            sslClient.print("GET XXXXXX");
            sslClient.print("Host: opendata.cwb.gov.tw\n");
            sslClient.print("Content-Type: application/WebJsonData\n");
            sslClient.print("\n\n");
          
            if (WebJsonData.readFrom(sslClient))
            {
                Serial.println("資料讀取完成！讀取內容如下：");
                WebJsonData.remove("result/fields");
                WebJsonData.remove("records/location/[0]/lat");
                WebJsonData.remove("records/location/[0]/lon");
                WebJsonData.remove("records/location/[0]/locationName");
                WebJsonData.remove("records/location/[0]/stationId");
                WebJsonData.toString(Serial, true);
                Serial.println("");
            }
            
        }
        else{
            Serial.println(" 失敗\n");
        }

        sslClient.stop();

    }
    
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

        Serial.println("成功連接FireBase目標資料庫");

        Serial.print("資料反序列化所需緩衝(Buffer)之大小：");
        Serial.println(WebJsonData.serializedBufferLength());

        Serial.printf("正在將資料(WebJsonData)更新至FireBase資料庫... %s\n", Firebase.updateNode(fbdo, "OnlineData", WebJsonData) ? "完成" : fbdo.errorReason().c_str());
        Serial.println("\n");

        

    //}
  }
}
