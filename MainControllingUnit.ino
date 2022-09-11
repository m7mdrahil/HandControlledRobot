#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
SoftwareSerial Arduino(D1,D2);//D1 to 11 and D2 to 10
int x;
String data1, data2, data3;
String dataIn;
char c;
int8_t indexOfA, indexOfB, indexOfC;
/////////////////////////////////////////////////////////////////////////

char ssid[] = "AdelBaqi";
char pass[] = "123456789";

typedef struct struct_message 
{
  String a,b,c;
}
struct_message;

struct_message myData;


unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer





//////////////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(115200);
  Arduino.begin(115200);
    WiFi.mode(WIFI_STA);

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
         Serial.println("WaterTemp = " + data1);
         Serial.println("WaterLevel = " + data2);
         Serial.println("pHLevel = " + data3);
         c=0;
         dataIn="";
       }
}

void Parse_the_Data()
{
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");
  data1 = dataIn.substring (0 ,indexOfA);
  data2 = dataIn.substring (indexOfA+1 ,indexOfB);
  data3 = dataIn.substring (indexOfB+1 ,indexOfC);
  myData.a =data1;
  myData.b =data2;
  myData.c =data3;
}
