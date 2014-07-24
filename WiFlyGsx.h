/*
 Arduino Library for WiFly GSX transceiver
 go to https://www.sparkfun.com/datasheets/Wireless/WiFi/WiFlyGSX-um.pdf for more information
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created June 2014
 by Jonathan Ruiz de Garibay

 */ 

#ifndef WIFLY_H
#define WIFLY_H

#include "Arduino.h"

#define IDLE			0x00
#define CONNECTED		0x01
#define NOIP			0x03
#define CONNECTING		0x04
#define ERROR			0xFF

class WiFlyGsx
{
	public:
		void begin(Stream * serialPort);
		boolean setWlan(String name, String password);
		int status();
		boolean openTcp(String ip, uint16_t port);
		void closeTcp();
		boolean available();
		void print(String text);
		void write(uint8_t value);
		uint8_t read();
		
	private:
        Stream * _serialPort;

		boolean enterToCommandMode();
		boolean releaseCommandMode();
		boolean save();
		boolean reboot();
		void sendCommand(String command);
		String getReply(int timeout);
		boolean waitReply(String reply, int timeout);
};

#endif