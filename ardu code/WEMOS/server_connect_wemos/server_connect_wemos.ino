#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

const char* ssid = "Kontrakan";
const char* password = "EngkotakonAdi";
//const char* ssid = "UGM-Hotspot";
//const char* password = NULL;
//const char* mqtt_server = "10.33.88.59";
const char* mqtt_server = "192.168.100.33";
 
WiFiClient espClient;
PubSubClient client(espClient);
/*
char msg[50];
unsigned long lastMsg = 0;
unsigned long lastSensorSend = 0;
unsigned long lastSensorRead = 0;
unsigned long lastAssignCommand = 0;
unsigned long lastSensorLow = 0;
int deg = 30;
*/
int value = 0;

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



  //Handle incoming msg
  //#####################################################################################
  /*
    //if(topic == "/process" && msgValue == "\"PURGE Stopped\""){
    if(msgValue == "PURGE Stopped"){
    digitalWrite(relayPin,LOW);
    Serial.println("-20");
    }

    if (msgValue == "buka") {
    Serial.println("Disutuh MQTT : BUKA");
    if(isOpened == false && isClosed == true || justStared == true){
        lastAssignCommand = millis();
        digitalWrite(relayPin,HIGH);
        isOpening = true;
        isClosing = false;
        justStared = false;
        Serial.println("NGE BUKA");
        delay(2000);
        client.publish("/hoes/deviceA","YASS");
        }
    client.publish("/process","{\"deviceA\":\"opening\"}");}

    else if (msgValue == "tutup"){
    Serial.println("Disutuh MQTT : TUTUP");
    if(isOpened == true && isClosed == false){
        digitalWrite(relayPin,LOW);
        isOpening = false;
        isClosing = true;
        Serial.println("NGE TUTUP");
        delay(2000);
        client.publish("/hoes/deviceA","YASS");
        }
    client.publish("/process","{\"deviceA\":\"closing\"}");}
    else if (msgValue == "hallo"){
    client.publish("/status","closing deviceA");
    }
    else if (msgValue == "start purge"){
    client.publish("/process","{\"deviceA\":\"starting purg\"}");
    digitalWrite(relayPin,HIGH);
    isPurgeing = true;
    }
    else if (msgValue == "stop purge"){
    client.publish("/process","{\"deviceA\":\"stopping purg\"}");
    digitalWrite(relayPin,LOW);
    }
  */
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



//Runtime
//===========================================================================

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


char mystr[200]; //Initialized variable to store recieved data
String incomingString;
unsigned long now;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  now = millis();

  incomingByte
  if (Serial1.available() > 0) {
    incomingByte = Serial1.read(); // read the incoming byte:
    Serial.print(" I received:");
    Serial.println(incomingByte);
    /*
    //digitalWrite(LED_BUILTIN, HIGH);
    Serial1.readBytes(mystr, 200); //Read the serial data and store in var
    //Print data on Serial Monitor
    Serial.print("incoming msg: ");
    Serial.println(mystr);
    */
    

    /*
      char container[5];
      for (int i = 0; i <= 5; i++) {
      //Serial.print((char)mystr[i]);
      container[i] = mystr[i];


      /*
      //convert message to ASCII
      String ASCIIValue ; String msgValue;
      Serial.print("\n\n");
      for (int n = 0; n < length; n++){
      ASCIIValue = container[n]; //,(HEX);
      msgValue = msgValue+ASCIIValue; }
      //Serial.println("ISI PESAN : " + msgValue);
    */
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    /*
    digitalWrite(LED_BUILTIN, LOW);
    client.publish("/status", mystr);
    Serial.println(millis()-now);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    //Serial.println(" MQTT OK");
    //digitalWrite(LED_BUILTIN, LOW);
    */
  }
 
}
