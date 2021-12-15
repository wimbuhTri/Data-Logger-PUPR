void setup()
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}
void loop()
{
      int val;
      val=map(analogRead(0),0,1023,0,6000);   //connect grayscale sensor to Analog 0
      Serial.println(val,DEC);//print the value to serial
      delay(100);
}
