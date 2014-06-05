/*
 Arduino Library for Sonmicro SM125 RFID reader (over I2C protocol)
 go to http://www.sonmicro.com/en/index.php?option=com_content&view=article&id=48&Itemid=64 for more information
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created July 2013
 by Jonathan Ruiz de Garibay

 */ 

#ifndef WIFLY_H
#define WIFLY_H

#include "Arduino.h"


class Wifly
{
	public:
		boolean begin(Stream * serialPort);
		void setNetwork(String name, String password);
		void isConnected();

	private:
        Stream * _serialPort;
		
		void enterToCommandMode();
		void releaseCommandMode();
		void reboot();
		void save();
		
};

#endif