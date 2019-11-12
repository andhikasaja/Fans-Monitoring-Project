#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <PubSubClient.h>


#define LED 2
 
//Enter your wifi credentials
const char* ssid = "DTEO-VOKASI";  
const char* password = "TEO123456";

//Enter your mqtt server configurations
const char* mqttServer = "54.159.149.100";    //Enter Your mqttServer address
const int mqttPort = 1883;       //Port number
const char* mqttUser = ""; //User
const char* mqttPassword = ""; //Password

//variabel sensor kecepatan
float value=0;
float rev=0;
int rpm;
int oldtime=0;
//int time;
 
WiFiClient espClient;
PubSubClient client(espClient);


void isr() //interrupt service routine sensor
{
rev++;
}

 
void setup() {
  delay(1000);
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
 
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
 
    if (client.connect("ESP8266", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());  //If you get state 5: mismatch in configuration
      delay(2000);
 
    }
  }
 
  client.publish("FANS/mesin/speed", "Connected gan!");
 // client.subscribe("sensosr/rpm");

 attachInterrupt(digitalPinToInterrupt(14),isr,RISING);  //attaching the interrupt sensor
 
}
 
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
 
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
   Serial.print(message);
  if(message == "#on") {digitalWrite(LED,LOW);}   //LED on  
  if(message == "#off") {digitalWrite(LED,HIGH);} //LED off
 
  Serial.println();
  Serial.println("-----------------------");  
}
 
void loop() {
  client.loop();

delay(1000);
detachInterrupt(0);           //detaches the interrupt
int time=millis()-oldtime;       //finds the time 
rpm=(rev/time)*60000;        //calculates rpm
oldtime=millis();             //saves the current time
rev=0;
attachInterrupt(digitalPinToInterrupt(14),isr,RISING);

int r = rpm;

delay(1000);
String rpm = String(r);
char cstr[16];
int numr = r;
itoa(numr, cstr, 10);

Serial.print("rpm");
Serial.println(cstr);

client.publish("FANS/mesin/speed",cstr);
  
}
