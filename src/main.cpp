#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Oppo F7";
const char* password = "12345678";
const char* mqttuser = "uuknfjep";
const char* mqttpassword = "t0wCgMnr8KEt";
const char* mqttserver = "soldier.cloudmqtt.com";
const char* device_id = "esp8266";

WiFiClient espClient;
PubSubClient client(espClient);

const byte ledPin5 = 12;
char message_buff[100];

void callback(char* led_control, byte* payload, unsigned int length)
{
  Serial.println("Message arrived [");
  Serial.println(led_control);
  Serial.println("]");
  int i;
  for (i = 0; i < length; i++)
  {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff);
  Serial.println(msgString);

  if(strcmp(led_control,"led_control")==0)
  {
    if(msgString == "1")
    {
      digitalWrite(ledPin5,LOW);
    }
    if (msgString == "0")
    {
      digitalWrite(ledPin5,HIGH);
    }
  }
}

void reconnect()
{
  while(!client.connected())
  {
    Serial.println("Attempting to connect MQTT");
    if(client.connect(device_id, mqttuser, mqttpassword))
    {
      Serial.println("connected");
      client.subscribe("led_control");
    }
    else
    {
      Serial.println("Connection failed");
      Serial.println(client.state());
      Serial.println("Attempting connection in 5 sec");
      delay(5000);
    }
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  client.setServer(mqttserver, 16923);
  client.setCallback(callback);

  pinMode(ledPin5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected())
  {
    reconnect();
  }
  client.loop();
}