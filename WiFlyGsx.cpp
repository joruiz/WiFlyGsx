/*
 Arduino Library for WiFly GSX transceiver
 go to https://www.sparkfun.com/datasheets/Wireless/WiFi/WiFlyGSX-um.pdf for more information
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created June 2014
 by Jonathan Ruiz de Garibay

 */ 

#include "Arduino.h"
#include "WiFlyGsx.h"

//#define DEBUG_WIFLYGSX

void WiFlyGsx::begin(Stream * serialPort) {

	_serialPort = serialPort;
	_serialPort->flush();
}

boolean WiFlyGsx::setWlan(String name, String password) {

	if (this->enterToCommandMode()) {
		this->sendCommand("set wlan ssid " + name);
		if (this->waitReply("AOK", 2000)) {
			delay(50);
			Serial.println("  - Llego a SSID");
			this->sendCommand("set wlan phrase " + password);
			if (this->waitReply("AOK", 4000)){
				Serial.println("  - Llego a PASS");
				this->save();
				Serial.println("  - Llego a SAVE");
				this->reboot();
				Serial.println("  - Llego a REBOOT");
				this->releaseCommandMode();
#ifdef DEBUG_WIFLYGSX
				Serial.println("Wlan configuration updated");
#endif
				return true;
			}
		}
	}
	this->releaseCommandMode();
	return false;
}

int WiFlyGsx::status() {

	int state = ERROR;
	if (this->enterToCommandMode()) {
		this->sendCommand("show connection");
		String reply = this->getReply(200);
		int index = reply.indexOf('8');
		if ((index != -1) && (reply.length() >= (index + 3))) {
#ifdef DEBUG_WIFLYGSX
			switch (reply[index + 3])
			{
				case '0':
					Serial.println("State: IDLE");
					break;
				case '1':
					Serial.println("State: CONNECTED");
					break;
				case '3':
					Serial.println("State: NOIP");
					break;
				case '4':
					Serial.println("State: CONNECTING");
					break;
			}
#endif
			state = reply[index + 3] - 48;
		}
		this->releaseCommandMode();
	}
	return state;
}

boolean WiFlyGsx::openTcp(String ip, uint16_t port) {

	if (this->enterToCommandMode()) {
		String command = "open " + ip + " " + String(port);
		this->sendCommand(command);
		boolean correct = this->waitReply("*OPEN*", 5000);
		if (!correct){
			this->releaseCommandMode();
#ifdef DEBUG_WIFLYGSX
			Serial.println("Socket TCP open error");
#endif
			}
#ifdef DEBUG_WIFLYGSX
		else
			Serial.println("Socket TCP opened");
#endif
		return correct;
	}
	return false;
}

void WiFlyGsx::closeTcp() {

	if (this->enterToCommandMode()) {
		this->sendCommand("close");
		boolean correct = this->waitReply("CLOS", 2000);
#ifdef DEBUG_WIFLYGSX
		if (correct)
			Serial.println("Socket TCP closed");
#endif
		this->releaseCommandMode();
	}
}

boolean WiFlyGsx::available() {

	_serialPort->available();
}

void WiFlyGsx::print(String text) {

	_serialPort->print(text);
}

void WiFlyGsx::write(uint8_t value) {

	_serialPort->write(value);
}

uint8_t WiFlyGsx::read() {

	_serialPort->read();
}

/*********************************************************************************************/
/* Private methods                                                                           */
/*********************************************************************************************/

//
// enterToCommandMode
//
//
boolean WiFlyGsx::enterToCommandMode() {

	_serialPort->print("$$$");
	String reply = this->getReply(500);
	boolean correct = false;
	if (reply.indexOf("CMD") != -1)
		correct = true;
	else if (reply.indexOf("$$$") != -1) {
		_serialPort->write(0x0D);
		correct = true;
		reply = getReply(1000);
	}	
#ifdef DEBUG_WIFLYGSX
	if (correct)
		Serial.println("Enter to command mode...");
#endif
	return correct;
}

//
// releaseCommandMode
//
//
boolean WiFlyGsx::releaseCommandMode() {

	this->sendCommand("exit");
	boolean correct = this->waitReply("EXIT", 500);
#ifdef DEBUG_WIFLYGSX
	if (correct) 
		Serial.println("End of command mode");
#endif
	return correct;
}

//
// save
//
// save configuration (requires enter to command mode)
boolean WiFlyGsx::save() {

	this->sendCommand("save");
	boolean correct = this->waitReply("Storing in config", 500);
#ifdef DEBUG_WIFLYGSX
	if (correct) 
		Serial.println("Configuration saved");
#endif
	return correct;
}

//
// reboot
//
// reboot WiFly module (requires enter to command mode)
boolean WiFlyGsx::reboot() {

	this->sendCommand("reboot");
	boolean correct = this->waitReply("*Reboot*", 500);
#ifdef DEBUG_WIFLYGSX
	if (correct) 
		Serial.println("Reboot now");
#endif
	return correct;
}

void WiFlyGsx::sendCommand(String command) {

	_serialPort->print(command);
	_serialPort->write(0x0D);
#ifdef DEBUG_WIFLYGSX
	Serial.println("Command sent: " + command);
#endif
}

String WiFlyGsx::getReply(int timeout) {

	long initTime = millis();
	String dataReceived = "";
	do {
		while (_serialPort->available()) {
			char data = _serialPort->read();
			dataReceived += data;
		}
		if (dataReceived.indexOf('>') != -1)
			break;
	}
	while ((millis() - initTime) < timeout);
	return dataReceived;
}

boolean WiFlyGsx::waitReply(String reply, int timeout) {

	long initTime = millis();
	String dataReceived = "";
	do {
		delay(1);
		while (_serialPort->available()) {
			char data = _serialPort->read();
			dataReceived += data;
		}
		if (dataReceived.length() >= reply.length())
			if (dataReceived.indexOf(reply) != -1) {
				return true;
			}
	}
	while ((millis() - initTime) < timeout);
	return false;
}