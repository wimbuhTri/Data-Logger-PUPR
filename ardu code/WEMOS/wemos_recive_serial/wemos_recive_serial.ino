char mystr[400]; //String data

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
  //Serial1.begin(115200);
  Serial.setTimeout(10);  // set serial timeout for each reading from Nano  
} 

String a;
void loop() {
  if(Serial.available()) {  
    digitalWrite(LED_BUILTIN, LOW);
    //a= Serial.readString();// read the incoming data as string    
    Serial.readBytes(mystr, 1000); //Read the serial data and store in var
    Serial.println(mystr);
    digitalWrite(LED_BUILTIN, HIGH);  
    }
  
  if(Serial1.available()) {  
    a= Serial1.readString();// read the incoming data as string    
    Serial.println("::"+a);

    }
  delay(500);
}
