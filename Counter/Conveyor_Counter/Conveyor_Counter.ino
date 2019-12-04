
/*
  The circuit:
* 5V to Arduino 5V pin
* GND to Arduino GND pin
* CLK to Analog #5
* DAT to Analog #4
*/


#include <Wire.h>
#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <PubSubClient.h>

//Enter your wifi credentials
const char* ssid = "DTEO-VOKASI";  
const char* password = "TEO123456";

//Enter your mqtt server configurations
const char* mqttServer = "54.159.149.100";    //Enter Your mqttServer address
const int mqttPort = 1883;       //Port number
const char* mqttUser = ""; //User
const char* mqttPassword = ""; //Password

byte ldr= A0;
int  nilai;
int things;
int count=0;
int pushbutton = D0;


WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
Serial.begin(9600);

delay(1000);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(MQTTcallback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("Couter A", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());  //If you get state 5: mismatch in configuration
      delay(2000);
 
    }
  }
  
client.publish("FANS/mesin/counter", "Connected gan!");

}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
 
 String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
 //  Serial.print(message);
 // if(message == "#on") {digitalWrite(LED,LOW);}   //LED on  
//  if(message == "#off") {digitalWrite(LED,HIGH);} //LED off
 
  Serial.println();
  Serial.println("-----------------------");  
}



void loop(){
nilai= analogRead(ldr);
  
if (nilai < 400) {
      count++; 
      }

//lcd.setCursor(0,1);
Serial.print("Jumlah Barang: ");
//lcd.setCursor(0,2);
Serial.println(count);
Serial.print("intensitas cahaya: ");
Serial.println(nilai);
delay(1000);


int c = count;

delay(1000);
String rpm = String(c);
char cstr[16];
int numr = c;
itoa(numr, cstr, 10);

  client.publish("FANS/mesin/counter",cstr);

  if(digitalRead(pushbutton) == LOW) // When button pressed
  {
    count = 0; // Set counter to 0
  }
delay(200);

}
