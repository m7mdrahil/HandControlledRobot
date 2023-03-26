// Define pins used for controlling the motors
#define ENA   14 // Enable/speed motors Right    GPIO14(D5)
#define ENB   12 // Enable/speed motors Left     GPIO12(D6)
#define IN_1  15 // L298N in1 motors Right       GPIO15(D8)
#define IN_2  13 // L298N in2 motors Right       GPIO13(D7)
#define IN_3  2  // L298N in3 motors Left        GPIO2(D4)
#define IN_4  16 // L298N in4 motors Left        GPIO16(D0)

// Include required libraries
#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure for receiving data - must match the sender structure
typedef struct struct_message 
{
  String a, b, c, d, e, f;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Initialize the speed of the car
int speedCar = 5;

// Initialize strings to store data from sensors
String rdata1, rdata2, rdata3, rdata4, rdata5, rdata6;

// Initialize integers to store sensor data for comparison
int Sen1, Sen2, Sen3, Sen4, Sen5, Sen6;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t* mac, uint8_t* incomingData, uint8_t len)
{
  // Copy incomingData into myData struct
  memcpy(&myData, incomingData, sizeof(myData));

  // Print the data received from each sensor to the serial monitor
  Serial.print("Sensor1: ");
  Serial.println(myData.a);
  Serial.print("Sensor2: ");
  Serial.println(myData.b);
  Serial.print("Sensor3: ");
  Serial.println(myData.c);
  Serial.print("Sensor4: ");
  Serial.println(myData.d);
  Serial.print("Sensor5: ");
  Serial.println(myData.e);
  Serial.print("Sensor6: ");
  Serial.println(myData.f);
  Serial.println();
}

void setup()
{
  // Initialize motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  // Initialize serial monitor
  Serial.begin(57600);

  // Set ESP8266 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP8266 as ESP-NOW slave
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  // Register the OnDataRecv callback function for ESP-NOW
  esp_now_register_recv_cb(OnDataRecv);
}


// Move the robot backward
void goBack() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

// Turn the robot to the right
void goRight() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

// Turn the robot to the left
void goLeft() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

// Stop the robot
void stopRobot() {  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void loop() {
  // Get sensor data from the received packet
  rdata1 = myData.a;
  rdata2 = myData.b;
  rdata3 = myData.c;
  rdata4 = myData.d;
  rdata5 = myData.e;
  rdata6 = myData.f;

  // Convert sensor data from strings to integers
  Sen1 = rdata1.toInt();
  Sen2 = rdata2.toInt();
  Sen3 = rdata3.toInt();
  Sen4 = rdata4.toInt();
  Sen5 = rdata5.toInt();
  Sen6 = rdata6.toInt();

  // Use sensor data to determine robot movement
  if (Sen1 < 800 && Sen2 > 850 && Sen3 > 850 && Sen4 > 850 && Sen5 > 850) {
    // If the robot is close to an obstacle in front, but far from obstacles on the sides,
    // move forward
    goAhead();
  } else if (Sen1 < 800 && Sen2 < 800 && Sen3 > 850 && Sen4 > 850 && Sen5 > 850) {
    // If the robot is close to an obstacle behind, but far from obstacles on the sides,
    // move backward
    goBack();
  } else if (Sen1 < 800 && Sen2 < 800 && Sen3 < 800 && Sen4 > 850 && Sen5 > 850) {
    // If the robot is close to an obstacle on the left side,
    // turn left
    goLeft();
  } else if (Sen1 < 800 && Sen2 < 800 && Sen3 < 800 && Sen4 < 800 && Sen5 > 850) {
    // If the robot is close to an obstacle on the right side,
    // turn right
    goRight();
  } else if (Sen1 < 800 && Sen2 < 800 && Sen3 < 800 && Sen4 < 800 && Sen5 < 800) {
    // If the robot is close to obstacles on all sides,
    // stop moving
    stopRobot();
  }
}
