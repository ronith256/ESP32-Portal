#include <SPI.h>
#include <LoRa.h>

// Define the LoRa pins on Heltec/LilyGo ESP32 Boards
// Built in pins

#define ss 18
#define rst 14
#define dio0 26


int counter = 0;
int currentTime = millis();
// void sender(int time);

void setup() {

  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  // Set LoRa frequency to 866MHz 
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
   // Sync word: (0xF3) 
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  sender(currentTime);
}

void loop() {
  int gotPacketTime = 0;
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    int gotPacketTime = millis() - currentTime;
    Serial.println(gotPacketTime);
    sender(gotPacketTime);
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void sender(int time){
  currentTime = millis();
  Serial.println("Sending Packet: ");
  Serial.println(counter);
  LoRa.beginPacket();
  LoRa.print("Time is ");
  LoRa.print(time);
  LoRa.endPacket();
  counter++;
}
