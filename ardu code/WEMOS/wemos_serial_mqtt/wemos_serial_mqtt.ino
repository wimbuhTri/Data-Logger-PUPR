#include <ESP8266WiFi.h>
#include <PubSubClient.h>
/*
const char* ssid = "Kontrakan";
const char* password = "EngkotakonAdi";
const char* mqtt_server = "192.168.100.33";
*/
const char* ssid = "UGM-Hotspot";
const char* password = NULL;
const char* mqtt_server = "10.33.109.248";
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
unsigned long lastMsg = 0;


void setup_wifi() {
  int status = 13;
  pinMode(status, INPUT);
  pinMode(15, OUTPUT);
  Serial.println();
  Serial.print("Attempting connection to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived ["); Serial.print(topic); Serial.print("] ");
  char container[54];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    container[i] = payload[i], HEX;
  }
  Serial.println();

  //convert message to ASCII
  String ASCIIValue ; String msgValue;
  Serial.print("\n\n");
  for (int n = 0; n < length; n++) {
    ASCIIValue = container[n], (HEX);
    msgValue = msgValue + ASCIIValue;
  }
  Serial.println("ISI PESAN : " + msgValue); 
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/status", "{\"deviceA\":\"just wakeup\"}");
      // ... and resubscribe
      client.subscribe("/hoes/deviceA");
      client.subscribe("/room1");
      client.subscribe("/process");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


char mystr[400]; //String data
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
  //Serial1.begin(115200);
  Serial.setTimeout(10);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}



String a;
void loop() {
  //network connectivity
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  

  
  if(Serial.available()) {  
    digitalWrite(LED_BUILTIN, LOW);
    //a= Serial.readString();// read the incoming data as string    
    Serial.readBytes(mystr, 1000); //Read the serial data and store in var
    Serial.println(mystr);
    client.publish("/status", mystr);    
    digitalWrite(LED_BUILTIN, HIGH);  
    }
  
  if(Serial1.available()) {  
    a= Serial1.readString();// read the incoming data as string    
    Serial.println("::"+a);

    }
  delay(500);
}
