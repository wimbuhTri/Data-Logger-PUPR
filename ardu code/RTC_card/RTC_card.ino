#include <SD.h>
#include <SPI.h>
File myFile;
int pinCS = 10;

#include "RTClib.h"
RTC_DS3231 rtc;
char dataHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
String hari;
int tanggal, bulan, tahun, jam, menit, detik;
String nama_file;

void setup() {
  Serial.begin(115200);
  if (! rtc.begin()) {
    Serial.println("RTC Tidak Ditemukan");
    Serial.flush();
    abort();
  }

  //SD card
  pinMode(pinCS, OUTPUT);
  if (SD.begin())
  {
    DateTime now = rtc.now();
    Serial.println("SD card is ready to use.");
    tanggal = now.day(), DEC;
    bulan   = now.month(), DEC;
    tahun   = now.year(), DEC;
    //nama_file = String() + now.day() + "_" + now.month() + "_" + now.year() + ".txt";
    nama_file = String() + now.day() + now.month() + now.year() + ".txt";
    Serial.println(nama_file);

  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}
 

String RTC_loctime;
void loop() {
  DateTime now = rtc.now();
  //hari    = dataHari[now.dayOfTheWeek()];
  tanggal = now.day(), DEC;
  bulan   = now.month(), DEC;
  tahun   = now.year(), DEC;
  jam     = now.hour(), DEC;
  menit   = now.minute(), DEC;
  detik   = now.second(), DEC;
  //suhu    = rtc.getTemperature();

  RTC_loctime = String() + tanggal + "/" + bulan + "/" + tahun + "," + jam + ":" + menit + ":" + detik;
  Serial.println(RTC_loctime);

  // Create/Open file

  myFile = SD.open(nama_file, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing to file...");
    // Write to file
    myFile.println(RTC_loctime);
    myFile.close();
    Serial.println(RTC_loctime);
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening file");
  }
  delay(1000);
}
