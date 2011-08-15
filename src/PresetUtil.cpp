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

#include "PresetUtil.h"
#include "SynthState.h"

#include "LiquidCrystal.h"

extern LiquidCrystal lcd;


char PresetUtil::readName[13];
SynthState * PresetUtil::synthState;

PresetUtil::PresetUtil() {
}

PresetUtil::~PresetUtil() {
}

void PresetUtil::setSynthState(SynthState* synthState) {
	// init
	PresetUtil::synthState = synthState;
	// enable the i2c bus
	i2c_master_enable(I2C1, 0);
}

uint8 PresetUtil::getDeviceId(int bankNumber) {
	if (bankNumber <4) {
		return EEPROM1_ID;
	} else {
		// Bank number 5 (tmp bank) is at the end of EEPROM2
		return EEPROM2_ID;
	}
}

int PresetUtil::getAddress(int bankNumber, int preset) {
	if (bankNumber <4) {
		return preset * 128 + bankNumber * 128 * 128;
	} else {
		// Bank number 5 (tmp bank) is at the end of EEPROM2
		return preset * 128 + 3 * 128 * 128;
	}
}


void PresetUtil::dumpLine(int a, int b, int c, int d) {
	SerialUSB.print("{ ");
	SerialUSB.print(a);
	SerialUSB.print(", ");
	SerialUSB.print(b);
	SerialUSB.print(", ");
	SerialUSB.print(c);
	SerialUSB.print(", ");
	SerialUSB.print(d);
	SerialUSB.print("} ");
	SerialUSB.println(", ");
}

void PresetUtil::dumpPatch() {
	dumpLine(PresetUtil::synthState->params.engine1.algo,
			PresetUtil::synthState->params.engine1.velocity,
			PresetUtil::synthState->params.engine1.numberOfVoice,
			PresetUtil::synthState->params.engine1.glide);
	dumpLine(PresetUtil::synthState->params.engine2.modulationIndex1,
			PresetUtil::synthState->params.engine2.modulationIndex2,
			PresetUtil::synthState->params.engine2.modulationIndex3,
			PresetUtil::synthState->params.engine2.modulationIndex4);
	dumpLine(PresetUtil::synthState->params.engine3.mixOsc1,
			PresetUtil::synthState->params.engine3.mixOsc2,
			PresetUtil::synthState->params.engine3.mixOsc3,
			PresetUtil::synthState->params.engine3.mixOsc4);
	OscillatorParams * o =
			(OscillatorParams *) (&(PresetUtil::synthState->params.osc1));
	for (int k = 0; k < 6; k++) {
		dumpLine(o[k].shape, o[k].frequencyType, o[k].frequencyMul, o[k].detune);
	}
	EnvelopeParams * e =
			(EnvelopeParams*) (&(PresetUtil::synthState->params.env1));
	for (int k = 0; k < 6; k++) {
		dumpLine(e[k].attack, e[k].decay, e[k].sustain, e[k].release);
	}
	MatrixRowParams
			* m =
					(MatrixRowParams*) (&(PresetUtil::synthState->params.matrixRowState1));
	for (int k = 0; k < 8; k++) {
		dumpLine(m[k].source, m[k].mul, m[k].destination, 0);
	}
	LfoParams* l = (LfoParams*) (&(PresetUtil::synthState->params.lfo1));
	for (int k = 0; k < 4; k++) {
		dumpLine(l[k].shape, l[k].freq, l[k].bias, l[k].keybRamp);
	}
	SerialUSB.println(PresetUtil::synthState->params.presetName);
}

void PresetUtil::readFromEEPROM(uint8 bankNumber, uint8 preset, char* params) {
	uint8 deviceaddress = PresetUtil::getDeviceId(bankNumber);
	int address = PresetUtil::getAddress(bankNumber, preset);
	uint8 bufReadAddress[2];
	i2c_msg msgsRead[2];
	int block1Size = 64;

	bufReadAddress[0] = (uint8) ((int) address >> 8);
	bufReadAddress[1] = (uint8) ((int) address & 0xff);

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = block1Size;
	msgsRead[1].data = (uint8*)params; // (uint8*)&PresetUtil::synthState->params;

	i2c_master_xfer(I2C1, msgsRead, 2, 500);

	delay(100);

	int block2Size = sizeof(struct AllSynthParams) - block1Size;

	address = address + block1Size;
	bufReadAddress[0] = (uint8) ((int) address >> 8);
	bufReadAddress[1] = (uint8) ((int) address & 0xff);

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = block2Size;
	msgsRead[1].data = (uint8 *)&params[block1Size];
	i2c_master_xfer(I2C1, msgsRead, 2, 500);

	delay(20);
}

char* PresetUtil::readPresetNameFromEEPROM(int bankNumber, int preset) {

	uint8 deviceaddress = PresetUtil::getDeviceId(bankNumber);
	int address = PresetUtil::getAddress(bankNumber, preset);
	uint8 bufReadAddress[2];
	i2c_msg msgsRead[2];

	bufReadAddress[0] = (uint8) ((int) address >> 8);
	bufReadAddress[1] = (uint8) ((int) address & 0xff);

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = 13;
	msgsRead[1].data = (uint8*) readName;

	i2c_master_xfer(I2C1, msgsRead, 2, 500);

	delay(20);

	return readName;
}

void PresetUtil::savePatchToEEPROM(uint8* params, int bankNumber, int preset) {
	uint8 deviceaddress = PresetUtil::getDeviceId(bankNumber);
	int address = PresetUtil::getAddress(bankNumber, preset);

	i2c_msg msgWrite1, msgWrite2;
	int block1Size = 64;
	uint8 bufWrite1[block1Size + 2];

	bufWrite1[0] = (uint8) ((int) address >> 8);
	bufWrite1[1] = (uint8) ((int) address & 0xff);
	for (int k = 0; k < block1Size; k++) {
		bufWrite1[k + 2] = params[k];
	}

	/* Write test pattern  */
	msgWrite1.addr = deviceaddress;
	msgWrite1.flags = 0;
	msgWrite1.length = block1Size + 2;
	msgWrite1.data = bufWrite1;
	i2c_master_xfer(I2C1, &msgWrite1, 1, 500);
	delay(5);

	int block2Size = sizeof(struct AllSynthParams) - block1Size;
	uint8 bufWrite2[block2Size + 2];
	address = address + block1Size;
	bufWrite2[0] = (uint8) ((int) address >> 8);
	bufWrite2[1] = (uint8) ((int) address & 0xff);
	for (int k = 0; k < block2Size; k++) {
		bufWrite2[k + 2] = params[k + block1Size];
	}
	msgWrite2.addr = deviceaddress;
	msgWrite2.flags = 0;
	msgWrite2.length = block2Size + 2;
	msgWrite2.data = bufWrite2;
	i2c_master_xfer(I2C1, &msgWrite2, 1, 500);
	delay(5);
}

void PresetUtil::saveCurrentPatchToEEPROM(int bankNumber, int preset) {
	PresetUtil::savePatchToEEPROM((uint8*)&PresetUtil::synthState->params, bankNumber, preset);
}


void PresetUtil::loadConfigFromEEPROM() {
	// Config at the begining of EEPROM2
	uint8 deviceaddress = EEPROM2_ID;
	int address = 0;

	uint8 bufReadAddress[2];
	i2c_msg msgsRead[2];
	uint8 eeprom[MIDICONFIG_SIZE + 1];

	bufReadAddress[0] = (uint8) ((int) address >> 8);
	bufReadAddress[1] = (uint8) ((int) address & 0xff);

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = MIDICONFIG_SIZE + 1;
	msgsRead[1].data = (uint8*) eeprom;

	i2c_master_xfer(I2C1, msgsRead, 2, 500);

	if (eeprom[0] == EEPROM_CONFIG_CHECK) {
		for (int k = 0; k < MIDICONFIG_SIZE; k++) {
			PresetUtil::synthState->fullState.midiConfigValue[k] = eeprom[k + 1];
		}
	}

	delay(20);
}

void PresetUtil::saveConfigToEEPROM() {
	// Config at the begining of EEPROM2
	uint8 deviceaddress = EEPROM2_ID;
	int address = 0;

	i2c_msg msgWrite1;
	int block1Size = MIDICONFIG_SIZE + 1;
	uint8 bufWrite1[block1Size + 2];

	bufWrite1[0] = (uint8) ((int) address >> 8);
	bufWrite1[1] = (uint8) ((int) address & 0xff);

	bufWrite1[2] = (uint8) EEPROM_CONFIG_CHECK;

	for (int k = 0; k < MIDICONFIG_SIZE; k++) {
		bufWrite1[k + 3] = PresetUtil::synthState->fullState.midiConfigValue[k];
	}

	/* Write test pattern  */
	msgWrite1.addr = deviceaddress;
	msgWrite1.flags = 0;
	msgWrite1.length = block1Size + 2;
	msgWrite1.data = bufWrite1;
	i2c_master_xfer(I2C1, &msgWrite1, 1, 500);
	delay(50);

}

void PresetUtil::formatEEPROM() {
	for (int bankNumber = 0; bankNumber < 4; bankNumber++) {
		for (int preset = 0; preset < 128; preset++) {
			uint8 deviceaddress = PresetUtil::getDeviceId(bankNumber);
			int address = PresetUtil::getAddress(bankNumber, preset);

			i2c_msg msgWrite1, msgWrite2;
			int block1Size = 64;
			uint8 bufWrite1[block1Size + 2];

			bufWrite1[0] = (uint8) ((int) address >> 8);
			bufWrite1[1] = (uint8) ((int) address & 0xff);
			for (int k = 0; k < block1Size; k++) {
				bufWrite1[k + 2] = ((uint8*) &presets[bankNumber])[k];
			}
			/* Write test pattern  */
			msgWrite1.addr = deviceaddress;
			msgWrite1.flags = 0;
			msgWrite1.length = block1Size + 2;
			msgWrite1.data = bufWrite1;
			i2c_master_xfer(I2C1, &msgWrite1, 1, 500);
			delay(5);

			int block2Size = sizeof(struct AllSynthParams) - block1Size;
			uint8 bufWrite2[block2Size + 2];
			address = address + block1Size;
			bufWrite2[0] = (uint8) ((int) address >> 8);
			bufWrite2[1] = (uint8) ((int) address & 0xff);
			for (int k = 0; k < block2Size; k++) {
				bufWrite2[k + 2] = ((uint8*) &presets[bankNumber])[k
						+ block1Size];
			}
			msgWrite2.addr = deviceaddress;
			msgWrite2.flags = 0;
			msgWrite2.length = block2Size + 2;
			msgWrite2.data = bufWrite2;
			i2c_master_xfer(I2C1, &msgWrite2, 1, 500);
			delay(5);
		}
	}
}

void PresetUtil::sendBankToSysex(int bankNumber) {
	char params[sizeof(struct AllSynthParams)];

	uint8 newPatch[] = { 0xf0, 0x7d, 0x02 };
	for (int k = 0; k <= 2; k++) {
		Serial3.print(newPatch[k]);
	}

	for (int preset = 0; preset < 128; preset++) {
		lcd.setCursor(3,2);
		lcd.print(preset);
		lcd.setCursor(10,2);
		lcd.print(0);
		PresetUtil::readFromEEPROM(bankNumber, preset, params);
		lcd.setCursor(10,2);
		lcd.print(1);
		PresetUtil::sendParamsToSysex(params, sizeof(struct AllSynthParams));
		lcd.setCursor(10,2);
		lcd.print(2);
	}

	Serial3.print((uint8) 0xf7);
	lcd.setCursor(3,3);
	lcd.print("Fin...");
}

void PresetUtil::sendCurrentPatchToSysex() {
	uint8 newPatch[] = { 0xf0, 0x7d, 0x01 };
	for (int k = 0; k <= 2; k++) {
		Serial3.print(newPatch[k]);
	}

	PresetUtil::sendParamsToSysex((char*)&PresetUtil::synthState->params, sizeof(struct AllSynthParams));

	Serial3.print((uint8) 0xf7);
}

void PresetUtil::sendParamsToSysex(char* params, int size) {
	int checksum = 0;

	for (int k = 0; k < size; k++) {
		uint8 byte = params[k];
		checksum += byte;
		while (byte >= 127) {
			Serial3.print((uint8) 127);
			byte -= 127;
		}
		Serial3.print(byte);
	}

	Serial3.print((uint8) (checksum % 128));
}

int PresetUtil::readSysexPatch(char* params) {
	int checkSum = 0;
	unsigned int index = 0;
	int value = 0;

	while (index < sizeof(struct AllSynthParams)) {

		int byte = PresetUtil::getNextMidiByte();
		if (byte < 0) {
			return -index - 100;
		}
		value += byte;

		if (byte < 127) {
			params[index]	= value;
			index++;
			checkSum += value;
			value = 0;
		}
	}

	int sentChecksum = PresetUtil::getNextMidiByte();
	if (sentChecksum <0) {
		return -198;
	} else {
		checkSum = checkSum % 128;

		if (checkSum != sentChecksum) {
			return -200 - sentChecksum;
		}
	}
	return sentChecksum;
}

int PresetUtil::getNextMidiByte() {
	int timeout = 0;
	while (!Serial3.available()) {
		if (timeout++ >= 100000) {
			return -1;
		}
	}
	return (int)Serial3.read();
}

/*
 * 0xf0 is already read.
 * 0x7d : non commercial
 * 01 = patch, 02 = bank
 *
 * return value : -1 : error
 *                1 : patch read
 *                2 : bank read (and store to temporary eeprom)
 */

int PresetUtil::readSysex(bool patchAllowed, bool bankAllowed) {
	char params[sizeof(struct AllSynthParams)];
	boolean isPatch = true;
	boolean bError = false;
	boolean bSysexRead = false;
	int index = 0;

	while (true) {
		int byte = PresetUtil::getNextMidiByte();
		if (byte<0) {
			bError = true;
			break;
		}

		if (byte == 0xF7) {
			// Should be found after patch or full bank is read....
			break;
		}

		if (index  == 0) {
			if (byte == 0x7d) {
				index++;
			} else {
				bError = true;
				break;
			}
		} else if (index  == 1) {
			// Batch or bank
			index++;
			if (byte == 1) {
				isPatch = true;
			} else if (byte == 2) {
				isPatch = false;
			} else {
				bError = true;
				break;
			}
			if ((isPatch && !patchAllowed) || (!isPatch && !bankAllowed)) {
				// Will wait untill F7 is received (end of sysex)
				bSysexRead = true;
			}
		}

		if (index >1 && !bSysexRead) {
			bSysexRead = true;
			if (isPatch) {
				int errorCode = 0;
				if ((errorCode = PresetUtil::readSysexPatch(params)) <0) {
					index = -errorCode;
					bError = true;
				} else {
					PresetUtil::synthState->propagateBeforeNewParamsLoad();
					PresetUtil::copyPatch(params, (char*) &PresetUtil::synthState->params);
					PresetUtil::synthState->propagateAfterNewParamsLoad();
					PresetUtil::synthState->resetDisplay();
				}
			} else {
				// Bank !!
				int errorCode = 0;
				if ((errorCode = PresetUtil::readSysexBank()) <0) {
					index = -errorCode;
					bError = true;
				}
			}
		}
	}

	return bError ? -index : (isPatch ? 1 : 2);
}


void PresetUtil::copyPatch(char* source, char* dest) {
    for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
        dest[k] = source[k];
    }
}


int PresetUtil::readSysexBank() {
	char params[sizeof(struct AllSynthParams)];
	int errorCode = 0;
	for (int preset = 0; preset<128; preset++) {
		if ((errorCode = PresetUtil::readSysexPatch(params)) <0) {
			errorCode = -500 - preset;
			break;
		}
		PresetUtil::savePatchToEEPROM((uint8*)params, 4, preset);
	}

	return errorCode;
}



void PresetUtil::copyBank(int source, int dest) {
	char params[sizeof(struct AllSynthParams)];
	for (int preset=0; preset<127; preset++) {
		lcd.setCursor(3,2);
		lcd.print(preset);
		PresetUtil::readFromEEPROM(source, preset, params);
		PresetUtil::savePatchToEEPROM((uint8 *)params, dest, preset);
	}
}
