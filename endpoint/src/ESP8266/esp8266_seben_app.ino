//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
  
// Replace the next variables with your SSID/Password combination

#ifndef STASSID
#define STASSID "JMJ3"
#define STAPSK  "BARBETRivera38"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;


// Add your MQTT Broker IP address, example:
//char* mqtt_server = "192.168.56.1";
char* mqtt_server = "192.168.137.1";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int ledPin = 2;
boolean toggle = false;



void setup() {
  Serial.begin(115200);

  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
  
  if (mqttClient.connect("espRadiateur"))
  {
    // Connection succeeded
    Serial.println("Connected ");
    // Subscribe
    boolean res = mqttClient.subscribe("home/doors/1");
    mqttClient.publish("home/id/NodeMcu Esp8266", "connect");
    Serial.println("subscribe");
    Serial.println(res);
  }
  else {
    // connection failed
    mqttClient.state(); //will provide more information
    // on why it failed.
    Serial.println("Connection failed ");
  }
      
  pinMode(ledPin, OUTPUT);
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* message, unsigned int length) {

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  byte messageDoor = 1;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();

  if (String(topic) == "home/doors/1") 
  {
    if (length > 0)
    {
      messageDoor = message[0];
      messageDoor = atoi((const char *)&messageDoor);
    }
    
    if (messageDoor == 1)
    {
        Serial.println("Porte Ouverte");
        digitalWrite(ledPin, LOW);
    }
    else
    {
        Serial.println("Porte Fermee");
        digitalWrite(ledPin, HIGH);      
    }
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    // Subscribe
    if (mqttClient.connect("espRadiateur"))
    {
      // Connection succeeded
      Serial.println("Connected ");
      // Subscribe
      boolean res = mqttClient.subscribe("home/doors/1");  
      Serial.print("subscribe ");
      Serial.println(res);
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!mqttClient.connected()) {
    Serial.println("\nReconnect...");
    reconnect();
  }
  //Serial.print(".");
  
  mqttClient.loop();
  delay(100);
}
