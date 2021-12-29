#include "SparkFun_SGP30_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_SGP30
#include <Wire.h>
SGP30 mySensor; //create an object of the SGP30 class
#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

//thermocouple
// this example is public domain. enjoy! https://learn.adafruit.com/thermocouple/
#include "max6675.h"
int thermoDO = 11;
int thermoCS = 9;
int thermoCLK = 13;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
float thermocouple_read;

/*
//SD card
#include <SD.h>
#include <SPI.h>
File myFile;
int pinCS = 10;
*/
//RTC
#include "RTClib.h"
RTC_DS3231 rtc;
char dataHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
String hari;
int tanggal, bulan, tahun, jam, menit, detik;
String nama_file;
String RTC_loctime;

//dust sensor
float concentration1 = 0;
float concentration2 = 0;
 
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
void sendToESP(){
  String param[11] = {"humid","tempC","heatIdxC","CO2","TVOOC","Vi","formalDehCons","lux","therCap","dust1","dust2"};
  float readings[11] = {h,t,hic,r_CO2,r_TVOC,r_Vi,r_FoCom,thermocouple_read,concentration1,concentration2};
  String telemetry;
  for(int i=0; i<sizeof(readings) ; i++){
    if(i!=sizeof(readings)-1){
      //telemetry = telemetry + parameter[i] + ":" + String(value[i]) + "," ;
      telemetry = telemetry + String(readings[i]) + "," ;
      }
    else{
      telemetry = telemetry + String(readings[i])+","+RTC_loctime;
      }

   //read to SD
   /*
    if (myFile) {
      Serial.println("Writing to file...");
      // Write to file
      String to_write = telemetry + RTC_loctime;
      myFile.println(to_write);
      myFile.close();
      
    }
    // if the file didn't open, print an error:
    else {
      Serial.println("error opening file");
    }
   */
    
  }
  //String(parameter+value).toCharArray(mystr, sizeof(mystr));   
  Serial.println(telemetry);
  String(telemetry).toCharArray(mystr, sizeof(mystr));   
  Serial1.write(mystr,100); //Write the serial data
}


float do_dust(){
  int pin2 = 2;
  int pin1 = 3;
  unsigned long duration1;
  unsigned long duration2;
  unsigned long starttime;
  unsigned long sampletime_ms = 1000;//sampe 1s ;
  unsigned long lowpulseoccupancy1 = 0;
  unsigned long lowpulseoccupancy2 = 0;
  float ratio1 = 0;
  float ratio2 = 0;
  
  duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;
  //starttime = millis();//get the current time;

  //if (true)
  //if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  //  {
      ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
      concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve    
      ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
      concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 
      
      //Serial.print("concentration1 = ");
      Serial.print(concentration1);
      Serial.println(" pcs/0.01cf  -  ");
      //Serial.print("concentration2 = ");
      Serial.print(concentration2);
      Serial.println(" pcs/0.01cf  -  ");
      
      lowpulseoccupancy1 = 0;
      lowpulseoccupancy2 = 0;
      //starttime = millis();
  //  }
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
  }

// Formaldehde concentration formula
float concentrationPPMform(float V, float Vstart){ 
  float con = 212.32619543773774 * exp( -( 7.653015806367451 * (4.95 - V) * Vstart ) / ( V * (4.95 - Vstart) ) );
  return con;
}

void setup() {  
  //RTC
  if (! rtc.begin()) {
    Serial.println("RTC Tidak Ditemukan");
    Serial.flush();
    abort();
  }
  
  //SD Card
  /*
  pinMode(pinCS, OUTPUT);
  if (SD.begin()){
    DateTime now = rtc.now();
    Serial.println("SD card is ready to use.");
    tanggal = now.day(), DEC;
    bulan   = now.month(), DEC;
    tahun   = now.year(), DEC;
    nama_file = String() + now.day() + now.month() + now.year() + ".txt";
    myFile = SD.open("test.txt", FILE_WRITE);
    Serial.println("SD begin");
  } 
  else{
    Serial.println("SD card initialization failed");
    return;
  }
  */

  //SGP
  Serial.begin(115200);
  Serial1.begin(115200);
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
  
  //DHT
  dht.begin();

  //Dust sensor
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  Serial.println("Dimulai...");
}


long SAMPLING_RATE= 1000;//dalam milis

unsigned long prev = 99999;
unsigned long start = 0;
unsigned long job_duration = 0;
unsigned long fluid_scale = SAMPLING_RATE;
void loop() {
  //if(millis()-prev >= SAMPLING_RATE){
    digitalWrite(LED_BUILTIN, HIGH); 
    start = millis();

    //RTC
    DateTime now = rtc.now();
    //hari    = dataHari[now.dayOfTheWeek()];
    tanggal = now.day(), DEC;
    bulan   = now.month(), DEC;
    tahun   = now.year(), DEC;
    jam     = now.hour(), DEC;
    menit   = now.minute(), DEC;
    detik   = now.second(), DEC;
    RTC_loctime = String() +tanggal + "/" + bulan + "/" + tahun + "," + jam + ":" + menit + ":" + detik;

   //Thermocouple
   thermocouple_read = thermocouple.readCelsius();
   Serial.println(thermocouple_read);
   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   
    
    //DHT
    do_dht();

    //Dust
    do_dust(); 
    
    
    //read lux
    lux = map(analogRead(A0),0,1023,6000,0);

    //Air quality
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
     
    sendToESP();
  
    
        
    digitalWrite(LED_BUILTIN, LOW); 
    prev = millis();
    /*
    Serial.println(millis()-prev);
    job_duration = millis()-start;
    fluid_scale = SAMPLING_RATE - job_duration;
    
    Serial.print("fluid scale: ");
    Serial.println(fluid_scale);
  }
 Serial.println("skipss...");
 Serial.println(millis()-start);
 */
  
 delay(1000);
}
