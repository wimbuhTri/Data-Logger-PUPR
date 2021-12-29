char mystr[400]; //String data
void sendToESP(){
  /*
  String param[11] = {"humid","tempC","heatIdxC","CO2","TVOOC","Vi","formalDehCons","lux","therCap","dust1","dust2"};
  float readings[11] = {h,t,hic,r_CO2,r_TVOC,r_Vi,r_FoCom,thermocouple_read,concentration1,concentration2};
  */
  //String telemetry;
  String telemetry = "11.11,22.22,33.33,44.44,55.55,66.66,77.77,88.88,99.99,10.10,11.11,12.12,13.13";
  /*
  for(int i=0; i<sizeof(readings) ; i++){
    if(i!=sizeof(readings)-1){
      //telemetry = telemetry + parameter[i] + ":" + String(value[i]) + "," ;
      telemetry = telemetry + String(readings[i]) + "," ;
      }
    else{
      telemetry = telemetry + String(readings[i])+","+RTC_loctime;
      }
  }
  */
  //String(parameter+value).toCharArray(mystr, sizeof(mystr));   
  Serial.println(telemetry);
  String(telemetry).toCharArray(mystr, sizeof(mystr));   
  Serial1.write(mystr,100); //Write the serial data
  //Serial1.write(mystr); //write str to serial
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

}

void loop() {
  sendToESP();
  delay(1000);
}
