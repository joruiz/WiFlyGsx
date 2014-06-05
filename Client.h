/*
 Arduino Library for Sonmicro SM125 RFID reader (over I2C protocol)
 go to http://www.sonmicro.com/en/index.php?option=com_content&view=article&id=48&Itemid=64 for more information
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created July 2013
 by Jonathan Ruiz de Garibay

 */ 

#ifndef CLIENT_H
#define CLIENT_H

#include "Arduino.h"


class Client
{
	public:
		Client(uint8_t *ip, uint16_t port);
		boolean connect();
		boolean isConnected();
		void close();
		void write(byte value);
		void write(const char *str);
		void writeln(const char *str);
		
		int available();
		byte readByte();
		String readln();
		void flush();

	private:
		uint8_t *_ip;
		uint16_t _port;
		
		bool isOpen;

};

#endif