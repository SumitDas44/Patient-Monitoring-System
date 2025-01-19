#include <Wire.h>
  #include <Adafruit_BMP085.h>
  #define seaLevelPressure_hPa 1013.25
  #include<LiquidCrystal_I2C.h>
  #include<SoftwareSerial.h>
  #include <OneWire.h>
  #include <DallasTemperature.h>

  #define ONE_WIRE_BUS 2

  OneWire oneWire(ONE_WIRE_BUS);	

  DallasTemperature sensors(&oneWire);

  SoftwareSerial BT(10,11);
  LiquidCrystal_I2C lcd(0x27,16,2);
  Adafruit_BMP085 bmp;
    
  void setup() {
    Serial.begin(9600);
    sensors.begin();	
    BT.begin(9600);
    if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
    }
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(4,OUTPUT);
    lcd.init();
    lcd.backlight();
    lcd.clear();
  }
    
  void loop() {
      // Serial.print("Temperature = ");
    sensors.requestTemperatures(); 
    int c=analogRead(A1);
    //print the temperature in Celsius
    // Serial.print("Temperature: ");
      Serial.print(sensors.getTempCByIndex(0));
      Serial.print(","); 
      // Serial.print(bmp.readTemperature());
      // Serial.print(","); 
      // Serial.println(" *C");
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print("C");
      // Serial.print("Pressure = ");
      // Serial.print(bmp.readPressure());
      // Serial.println(" Pa");

      float t = sensors.getTempCByIndex(0);

      int x=map(bmp.readPressure(),101350,104090,80,110);
      Serial.print(x);
          Serial.print(","); 

        int CO2 = map(c,1,1023,100,1);


          // if(t>32 || x>92 || ym>110){
          //   digitalWrite(4,HIGH);
          // }
  lcd.print(" P:");
  lcd.print(x);
  lcd.print("Hg");

      int y=analogRead(A0);
      int ym=map(y,200,800,60,100);
      Serial.print(ym);
      Serial.print(",");
Serial.println(CO2);       
      lcd.setCursor(0,1);
  lcd.print("H:");
  lcd.print(ym);
  lcd.print("bpm");

  lcd.print(" CO2:");
  lcd.print(CO2);
  lcd.print("ppm");


  BT.print(sensors.getTempCByIndex(0));
  BT.print(",");
  BT.print(x);
  BT.print(",");
  BT.print(ym);
  BT.print(",");
  BT.print(CO2);
  BT.print(";");

          if(t>33 || x>90 || ym>100){
            digitalWrite(4,HIGH);
          }else{
            digitalWrite(4,LOW);
          }

if(t>33){
  digitalWrite(5,HIGH);
}else{
  digitalWrite(5,LOW);  
}

if(x>90){
  digitalWrite(6,HIGH);
}else{
  digitalWrite(6,LOW);  
}

if(ym>100){
  digitalWrite(7,HIGH);
}else{
  digitalWrite(7,LOW);  
}
      // Serial.println();
    
     delay(300);
}