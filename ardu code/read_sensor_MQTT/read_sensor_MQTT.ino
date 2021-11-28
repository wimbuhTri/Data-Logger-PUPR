#include "SparkFun_SGP30_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_SGP30
#include <Wire.h>
SGP30 mySensor; //create an object of the SGP30 class

#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


float H[10];
float T[10];
float F[10];
float HIF[10];
float HIC[10];

float r_lux;
float lux[10];

int g_CO2[10];
int g_TVOC[10];
char mystr[50]; //String data
void sendToESP(String parameter, String value){
  //send to ESP8266
  //String(parameter+value).toCharArray(mystr, sizeof(mystr));   
  digitalWrite(LED_BUILTIN, HIGH); 
  String(parameter+value).toCharArray(mystr, sizeof(mystr));   
  Serial1.write(mystr,50); //Write the serial data
  delay(500);                      
  digitalWrite(LED_BUILTIN, LOW); 
  
 
  }

float do_dht(int loop_index){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  H[loop_index]=h,T[loop_index]=t,F[loop_index]=f,HIF[loop_index]=hif,HIC[loop_index]=hic;
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }  
  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  return(h,t,f,hif,hic);
  }

float concentrationPPMform(float V, float Vstart) // Formaldehde concentration formula
  {
  float con = 212.32619543773774 * exp( -( 7.653015806367451 * (4.95 - V) * Vstart ) / ( V * (4.95 - Vstart) ) );
  return con;
  }

void setup() {
  //SGP
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  if (mySensor.begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }
  mySensor.initAirQuality();

  //lux sensor
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  Serial.begin(9600);
  dht.begin();
}



int loop_index = 0;
void loop() {
  //DHT
  do_dht(loop_index);

  
  //read lux
  r_lux = map(analogRead(A0),0,1023,6000,0);
  lux[loop_index] = r_lux;
  Serial.print("lux: ");
  Serial.println(r_lux);
  
  

  //delay(1000); //Wait 1 second
  mySensor.measureAirQuality();
  int r_CO2 = mySensor.CO2;
  int r_TVOC = mySensor.TVOC;
  g_CO2[loop_index];
  g_TVOC[loop_index];
  Serial.print("CO2: ");
  Serial.print(r_CO2);
  Serial.print(" ppm\tTVOC: ");
  Serial.print(r_TVOC);
  Serial.println(" ppb");
  

  //HCHO
  float Vi = 0.1;
  float Vc = 4.95; // Constant voltage from pin
  int sensorValue = analogRead(A1); // Read analog pin for voltage info in 10 bits
  float Vf = sensorValue * 4.95 / 1023; // Voltage through pin when in presence of VOCs (formaldehyde)
  float r_Vi = sensorValue * 4.95 / 102;
  float r_FoCom = concentrationPPMform(Vf, Vi);
  int g_CO2[loop_index];
  int g_TVOC[loop_index];

  Serial.print("Vi: ");
  Serial.println(r_Vi);
  Serial.print("Formaldehyde Concentration: ");
  Serial.print(r_FoCom);
  Serial.print("ppm\n"); // Print concentration data
  //delay(500); // Delay 0.5 seconds
  
 Serial.print("INDEX::");
 Serial.print(loop_index);
 Serial.print("  millis: ");
 Serial.println(millis()); 
 if(loop_index > 9){
  loop_index = 0;
  Serial.println("######################");
  String to_send = "";
  for(int i=0; i<=9; i++){
    if(i != 9){
      //Serial.print(H[i],2);
      to_send = to_send + String(H[i],2)+",";
      //to_send = to_send + ",";
      }
    else{
      to_send = to_send + String(H[i],2);
      Serial.println("\n\n\n");
      sendToESP("Humid:",to_send);
      }
    }
/*
t[10];
f[10];
hif[10];
hic[10];

r_lux;
lux[10];

g_CO2[10];
g_TVOC;
*/
  
  Serial.println("######################");
  
  }
 else{  
  loop_index = loop_index + 1;
 }
 //wait for sensor to refresh
  delay(1000);
}
