#include <Wire.h>
#include <TinyMPU6050.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
const char* ssid = "Sumits Pixel";        // Replace with your WiFi SSID
const char* password = "01775919428";  // Replace with your WiFi password
MPU6050 mpu(Wire);
const char* BOTtoken = "7554417437:AAFG4bVcJmTMAOx6o0deJuAvHl_oAK0PlRw";  // Replace with your bot token
const char* CHAT_ID = "5232967288"; 

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(9600); // Use a higher baud rate for debugging
  Wire.begin(D2, D1);   // SDA -> D2, SCL -> D1 on ESP8266
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  mpu.Initialize();
  mpu.Calibrate();
  Serial.println("Calibration done!");

  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);
  bot.sendMessage(CHAT_ID, "Hello,This is your health assitant", "");

  pinMode(12,OUTPUT);
}

void loop() {
  mpu.Execute();

  int16_t accZ = mpu.GetRawAccZ();
  int16_t accX = mpu.GetRawAccX();
  int16_t accY = mpu.GetRawAccY();


Serial.print(accX);
Serial.print(",");
Serial.print(accY);
Serial.print(",");
Serial.println(accZ);

  //Detect movements and log
  if (accZ <= -8000) {
    Serial.println("-Z");
    sendTelegramMessage("Fall Detected");
    digitalWrite(12,HIGH);
    // delay(4000);
  } else{} 
  if (accZ >= 8000) {
    Serial.println("Z");
    sendTelegramMessage("Fall Detected");    
    digitalWrite(12,HIGH);
    // delay(4000);
  } else{} 
  if (accY <= -8000) {
    Serial.println("-Y");
    sendTelegramMessage("Fall Detected");
    digitalWrite(12,HIGH);
    // delay(4000);
  } else{} 
  if (accY >= 8000) {
    Serial.println("Y");
    sendTelegramMessage("Fall Detected");
    digitalWrite(12,HIGH);
    // delay(4000);
  } else{
        digitalWrite(12,LOW);
  } 

  // Small delay to allow system tasks
  // Adjust delay based on responsiveness needed
}

void sendTelegramMessage(String data) {
  // Log received data
  Serial.println("Received Data: " + data);

  // Send data to Telegram bot
  if (bot.sendMessage(CHAT_ID, "Received Data: " + data, "")) {
    Serial.println("Message sent to Telegram successfully.");
  } else {
    Serial.println("Failed to send message to Telegram.");
}
}