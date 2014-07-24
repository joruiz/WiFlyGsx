#include "WiFlyGsx.h"

#define WifiSerial Serial1

WiFlyGsx transceiver(&WifiSerial);
int i = 0;

void setup() {
 
  Serial.begin(9600);
  
  WifiSerial.begin(9600);
  
  delay(2000);
//  transceiver.setWlan("SmartLab-WiFi", "l4bl1sto");
  delay(1000);
}

void loop() {

  Serial.println(i++);
  transceiver.status();
  delay(1000);
}
