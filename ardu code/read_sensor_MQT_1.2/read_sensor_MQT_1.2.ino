#include "SparkFun_SGP30_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_SGP30
#include <Wire.h>
SGP30 mySensor; //create an object of the SGP30 class
#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


float h;
float t;
float f;
float hif;
float hic;
float lux;
int r_CO2;
int r_TVOC;
float r_Vi;
float r_FoCom;

char mystr[100]; //String data
void sendToESP(String parameter[8], float value[8]){
  //send to ESP8266
  String telemetry;
  for(int i=0; i<8 ; i++){
    if(i!=7){
      //telemetry = telemetry + parameter[i] + ":" + String(value[i]) + "," ;
      telemetry = telemetry + String(value[i]) + "," ;
      }
    else{
      telemetry = telemetry + String(value[i]);
      }
    
  }
  //String(parameter+value).toCharArray(mystr, sizeof(mystr));   

  Serial.println(telemetry);
  String(telemetry).toCharArray(mystr, sizeof(mystr));   
  Serial1.write(mystr,100); //Write the serial data
  
  }

float do_dht(){
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  hif = dht.computeHeatIndex(f, h);
  hic = dht.computeHeatIndex(t, h, false);
  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  /*  
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
  */
  }

// Formaldehde concentration formula
float concentrationPPMform(float V, float Vstart){ 
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


long SAMPLING_RATE= 1000;//dalam milis

unsigned long prev = 0;
unsigned long start = 0;
unsigned long job_duration = 0;
unsigned long fluid_scale = SAMPLING_RATE;
void loop() {
  if(millis()-prev >= fluid_scale){
    digitalWrite(LED_BUILTIN, HIGH); 
    start = millis();
    //DHT
    do_dht();
  
    
    //read lux
    lux = map(analogRead(A0),0,1023,6000,0);
    /*
    Serial.print("lux: ");
    Serial.println(lux);
    */
    
    mySensor.measureAirQuality();
    r_CO2 = mySensor.CO2;
    r_TVOC = mySensor.TVOC;
    /*
    Serial.print("CO2: ");
    Serial.print(r_CO2);
    Serial.print(" ppm\tTVOC: ");
    Serial.print(r_TVOC);
    Serial.println(" ppb");
    */
    
    //HCHO
    float Vi = 0.1;
    float Vc = 4.95; // Constant voltage from pin
    int sensorValue = analogRead(A1); // Read analog pin for voltage info in 10 bits
    float Vf = sensorValue * 4.95 / 1023; // Voltage through pin when in presence of VOCs (formaldehyde)
    r_Vi = sensorValue * 4.95 / 102;
    r_FoCom = concentrationPPMform(Vf, Vi);
    /*
    Serial.print("Vi: ");
    Serial.println(r_Vi);
    Serial.print("Formaldehyde Concentration: ");
    Serial.print(r_FoCom);
    Serial.print("ppm\n"); // Print concentration data
    */

    String  param[8] = {"humid","tempC","heatIdxC","CO2","TVOOC","Vi","formalDehCons","lux"};
    float value[8] = {h,t,hic,r_CO2,r_TVOC,r_Vi,r_FoCom};
    sendToESP(param,value);
    
     
    digitalWrite(LED_BUILTIN, LOW); 
    Serial.println(millis()-prev);

    
    job_duration = millis()-start;
    fluid_scale = SAMPLING_RATE - job_duration;
    prev = millis();
  }
}
