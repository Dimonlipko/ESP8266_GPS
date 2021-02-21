#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
//Pinos utilizados para conexao do modulo GPS
static const int RXPin = 4, TXPin = 3;
//Objeto TinyGPS++
TinyGPSPlus gps;
//Conexao serial do modulo GPS
SoftwareSerial Serial_GPS(RXPin, TXPin);
//Inicializa o display no endereco 0x3B
//e define o numero de colunas e linhas do display
LiquidCrystal_I2C lcd(0x3B, 16, 2);
void setup()
{
  //Baud rate Arduino
  Serial.begin(115200);
  //Baud rate Modulo GPS
  Serial_GPS.begin(9600);
  //Mostra informacoes iniciais no serial monitor
  Serial.println(F("Modulo GPS VK2828U7G5LF "));
  Serial.print(F("Biblioteca TinyGPS++ v. "));
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
  //Inicializa o display LCD I2C
  lcd.init();
  lcd.backlight();
  //Informacoes iniciais no display
  lcd.setCursor(0, 0);
  lcd.print("Lat: ");
  lcd.setCursor(0, 1);
  lcd.print("Lon: ");
}
void loop()
{
  //Conexao com modulo GPS
  while (Serial_GPS.available() > 0)
    if (gps.encode(Serial_GPS.read()))
    {
      displayInfo();
    }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("GPS nao detectado! Verifique as conexoes..."));
    while (true);
  }
}
void displayInfo()
{
  //Mostra informacoes no Serial Monitor e no display LCD
  Serial.print(F("Coordenadas (lat/long): "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6); //latitude
    lcd.setCursor(5, 0);
    lcd.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6); //longitude
    lcd.setCursor(5, 1);
    lcd.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.print(F("  Data/Hora: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.day()); //dia
    Serial.print(F("/"));
    Serial.print(gps.date.month()); //mes
    Serial.print(F("/"));
    Serial.print(gps.date.year()); //ano
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour()); //hora
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute()); //minuto
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second()); //segundo
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}
