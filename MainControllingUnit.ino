#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
SoftwareSerial Arduino(D1,D2);
int x;
String data1, data2, data3, data4, data5, data6;
String dataIn;
char c;
int8_t indexOfA, indexOfB, indexOfC, indexOfD, indexOfE, indexOfF;
/////////////////////////////////////////////////////////////////////////


uint8_t broadcastAddress[] = {0x84, 0xCC, 0xA8, 0xA5, 0x03, 0xFD};


typedef struct struct_message 
{
  String a,b,c,d,e,f;
}
struct_message;

struct_message myData;


unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
//  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
   // Serial.println("Delivery success");
  }
  else{
   // Serial.println("Delivery fail");
}


}
//////////////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(57600);
  Arduino.begin(57600);
    WiFi.mode(WIFI_STA);


     // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop()
{
  while(Arduino.available()>0)
    {
  c=Arduino.read();

    if(c=='\n')    {break;}
    else   {dataIn+=c;}
  
    }
     if(c=='\n')
      {
        Parse_the_Data();
        Serial.println("Data1 = " + data1);
         Serial.println("Data2 = " + data2);
         Serial.println("Data3 = " + data3);
         Serial.println("Data4 = " + data4);
         Serial.println("Data5 = " + data5);
         Serial.println("Data6 = " + data6);
         c=0;
         dataIn="";
       }
}

void Parse_the_Data()
{
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");
  indexOfD = dataIn.indexOf("D");
  indexOfE = dataIn.indexOf("E");
  indexOfF = dataIn.indexOf("F");
  data1 = dataIn.substring (0 ,indexOfA);
  data2 = dataIn.substring (indexOfA+1 ,indexOfB);
  data3 = dataIn.substring (indexOfB+1 ,indexOfC);
  data4 = dataIn.substring (indexOfC+1 ,indexOfD);
  data5 = dataIn.substring (indexOfD+1 ,indexOfE);
  data6 = dataIn.substring (indexOfE+1 ,indexOfF);
  myData.a =data1;
  myData.b =data2;
  myData.c =data3;
  myData.d =data4;
  myData.e =data5;
  myData.f =data6;
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

