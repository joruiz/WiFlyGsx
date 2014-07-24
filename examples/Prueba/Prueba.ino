#include "WiFlyGsx.h"

#define WifiSerial Serial1

WiFlyGsx transceiver(&WifiSerial);

void setup() {
 
  Serial.begin(9600);
  
}

void loop() {

  Serial.print(transceiver.status(), DEC);
  delay(1000);
}
