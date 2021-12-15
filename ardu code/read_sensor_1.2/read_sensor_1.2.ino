/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/

//Libraries
#include <DHT.h>;

//HCHO


//SGP30
#include "SparkFun_SGP30_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_SGP30
#include <Wire.h>
SGP30 mySensor; //create an object of the SGP30 class
float r_CO2;
float r_TVOC;


//Constants
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//lux
float lux;

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

//RTC
#include "RTClib.h"
RTC_DS3231 rtc;
char dataHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
String hari;
int tanggal, bulan, tahun, jam, menit, detik;
String nama_file;
String RTC_loctime;

// Formaldehde concentration formula - HCHO WSP2110
float concentrationPPMform(float V, float Vstart){ 
  float con = 212.32619543773774 * exp( -( 7.653015806367451 * (4.95 - V) * Vstart ) / ( V * (4.95 - Vstart) ) );
  return con;
}


void setup()
{
  //RTC
  if (! rtc.begin()) {
    Serial.println("RTC Tidak Ditemukan");
    Serial.flush();
    abort();
  }  
  Serial.begin(115200);
  dht.begin();

  //SGP30 air quality
  Wire.begin();
  if (mySensor.begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }
  mySensor.initAirQuality();
  
}

void loop()
{
    delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

    //LUX
    lux = map(analogRead(A0),0,1023,0,6000);
    Serial.println(lux );
    delay(1000); //Delay 2 sec.

     //RTC
    DateTime now = rtc.now();
    /*
    //hari    = dataHari[now.dayOfTheWeek()];
    tanggal = now.day(), DEC;
    bulan   = now.month(), DEC;
    tahun   = now.year(), DEC;
    jam     = now.hour(), DEC;
    menit   = now.minute(), DEC;
    detik   = now.second(), DEC;
    */
    RTC_loctime = String() + (now.day(),DEC) + "/" + (now.month(),DEC) + "/" + (now.year(),DEC) + "," + (now.hour(),DEC) + ":" + (now.minute(),DEC) + ":" + (now.second(),DEC);
    Serial.println(RTC_loctime);

    //SGP30 air quality
    mySensor.measureAirQuality();
    r_CO2 = mySensor.CO2;
    r_TVOC = mySensor.TVOC;
    Serial.print("SGP30 :");
    Serial.print(r_CO2);
    Serial.print(" , ");
    Serial.println(r_TVOC);
    
    //HCHO
    float Vi = 0.1;
    float Vc = 4.95; // Constant voltage from pin
    int sensorValue = analogRead(A1); // Read analog pin for voltage info in 10 bits
    float Vf = sensorValue * 4.95 / 1023; // Voltage through pin when in presence of VOCs (formaldehyde)
    r_Vi = sensorValue * 4.95 / 102;
    r_FoCom = concentrationPPMform(Vf, Vi); 
}

   
