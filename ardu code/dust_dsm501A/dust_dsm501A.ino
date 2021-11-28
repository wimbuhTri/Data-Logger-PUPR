/* Connect the DSM501 sensor as follow 
 * https://www.elektronik.ropla.eu/pdf/stock/smy/dsm501.pdf
 * 1 green vert - Not used
 * 2 yellow jaune - Vout2 - 1 microns (PM1.0)
 * 3 white blanc - Vcc
 * 4 red rouge - Vout1 - 2.5 microns (PM2.5)
 * 5 black noir - GND
*/
#define DUST_SENSOR_DIGITAL_PIN_PM10  2        // DSM501 Pin 2 of DSM501 (jaune / Yellow)
#define DUST_SENSOR_DIGITAL_PIN_PM25  3        // DSM501 Pin 4 (rouge / red) 


//#include <LiquidCrystal.h>
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int pin2 = 2;
int pin1 = 3;
unsigned long duration1;
unsigned long duration2;

unsigned long starttime;
unsigned long sampletime_ms = 3000;//sampe 1s ;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;
int wLed = A1;
int gLed = A2;
int yLed = A3;
int rLed = A4;
int bLed = A5;

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  /*
  pinMode(wLed,OUTPUT);
  pinMode(gLed,OUTPUT);
  pinMode(yLed,OUTPUT);
  pinMode(rLed,OUTPUT);
  pinMode(bLed,OUTPUT);
  */
  starttime = millis();//get the current time;
  //lcd.begin(16, 2);
}

void loop() {
  duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;


  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve

    ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

    /*
    lcd.setCursor(0, 0);
    lcd.print("PM10 ");
    lcd.setCursor(6, 0);
    lcd.print(concentration1,3);
    */   
    Serial.print("concentration1 = ");
    Serial.print(concentration1);
    Serial.println(" pcs/0.01cf  -  ");

    Serial.print("concentration2 = ");
    Serial.print(concentration2);
    Serial.println(" pcs/0.01cf  -  ");
    Serial.println();
    /*
    if (concentration1 < 1000) {
     lcd.setCursor (0, 1);
     for (int i = 0; i < 16; ++i)
   {
     lcd.write(' ');
   }
      
     lcd.setCursor(4, 1);
     lcd.print("CLEAN");
     
     digitalWrite(wLed, HIGH);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, LOW);
  }
    if (concentration1 > 1000 && concentration1 < 10000) {

     lcd.setCursor (0, 1);
     for (int i = 0; i < 16; ++i)
  {
     lcd.write(' ');
  }
      
     lcd.setCursor(4, 1);
     lcd.print("GOOD");
     
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, HIGH);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, LOW);
    }
    
    if (concentration1 > 10000 && concentration1 < 20000) {

    lcd.setCursor (0, 1);
    for (int i = 0; i < 16; ++i)
  {
    lcd.write(' ');
  }
      
      lcd.setCursor(4, 1);
     lcd.print("ACCEPTABLE");
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, HIGH);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, LOW);
    }
      if (concentration1 > 20000 && concentration1 < 50000) {
     lcd.setCursor (0, 1);
     for (int i = 0; i < 16; ++i)
  {
     lcd.write(' ');
  }   
     lcd.setCursor(4, 1);
     lcd.print("HEAVY");
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, HIGH);
     digitalWrite(bLed, LOW);
  }

    if (concentration1 > 50000 ) {
lcd.setCursor (0, 1);
for (int i = 0; i < 16; ++i)
{
  lcd.write(' ');
}
      
     lcd.setCursor(4, 1);
     lcd.print("HAZARD");
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, HIGH);
     
    } 
    */
      
    lowpulseoccupancy1 = 0;
    lowpulseoccupancy2 = 0;
    starttime = millis();
  }
}
