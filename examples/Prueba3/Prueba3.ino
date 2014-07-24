#include "WiFlyGsx.h"

#define WifiSerial Serial1

WiFlyGsx transceiver;
int i = 0;

void setup() {
 
  Serial.begin(9600);
  WifiSerial.begin(9600);
  transceiver.begin(&WifiSerial);
  
  transceiver.openTcp("192.168.2.101", 9999);
}

void loop() {

  transceiver.print(String(i++));
  if (i == 8)
    transceiver.closeTcp();
  delay(1000);
}
