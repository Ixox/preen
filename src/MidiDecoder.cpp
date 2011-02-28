/*
 * Copyright 2011 Xavier Hosxe
 *
 * Author: Xavier Hosxe (xavier.hosxe@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MidiDecoder.h"
#ifndef linux
#include "wirish.h"
#endif



MidiDecoder::MidiDecoder() {
	newEvent = true;
	index=0;
}

MidiDecoder::~MidiDecoder() {
}

void MidiDecoder::setSynth(Synth* synth) {
	this->synth = synth;
}

void MidiDecoder::newByte(unsigned char byte) {
	bool eventComplete = false;
	if (newEvent) {
		unsigned char hi = byte & 0xf0;
		//unsigned char lo = byte & 0x0f;
		switch (hi) {
		case 0x80:
		case 0x90:
		case 0xb0:
			currentEvent[0] = hi;
			newEvent = false;
			index = 0;
			numberOfBytes = 3;
			index ++;
			break;
	    case 0xd0:
			currentEvent[0] = hi;
			newEvent = false;
			index = 0;
	    	numberOfBytes = 2;
			index ++;
	    	break;
	    case 0xe0:
			currentEvent[0] = hi;
			newEvent = false;
			index = 0;
			numberOfBytes = 3;
			index ++;
			break;
		}
	} else {
		currentEvent[index] = byte;
		index++;
		if (index == numberOfBytes) {
			eventComplete = true;
			newEvent = true;
			index = 0;
		}
	}
	if (eventComplete) {
		sendMidiEvent();
	}
}


void MidiDecoder::sendMidiEvent() {
	switch (currentEvent[0]) {
	case 0x80:
		this->synth->noteOff(currentEvent[1]);
		break;
	case 0x90:
		this->synth->noteOn(currentEvent[1], currentEvent[2]);
		break;
	case 0xb0:
		/*
    	SerialUSB.print("control change  : ");
    	SerialUSB.print( (short)currentEvent[1]);
    	SerialUSB.print(", ");
    	SerialUSB.println((short) currentEvent[2]);
    	*/
		if (currentEvent[1] == 29) {
			this->synth->getMatrix()->setSource(MODWHEEL, currentEvent[2]);
/*
			lcd.setCursor(0,0);
			lcd.print("   ");
			lcd.setCursor(0,0);
			lcd.print((short)currentEvent[2]);
*/
		}
		break;
    case 0xd0:
    	this->synth->getMatrix()->setSource(AFTERTOUCH, currentEvent[1]);



    	/*
    	SerialUSB.print("AfterTouch  : ");
    	SerialUSB.println( currentEvent[1]<<8);
    	*/
    	break;
    case 0xe0:
    	this->synth->getMatrix()->setSource(PITCHBEND, (int)    ((((int)currentEvent[2] << 7) + (int)currentEvent[1] -8192) >>6)) ;
 /*
    	SerialUSB.print("Pitch bend : ");
    	SerialUSB.println((int)(((int)currentEvent[2]) << 7) + (int)currentEvent[1] -8192);
    	*/
    	break;
	}
}
