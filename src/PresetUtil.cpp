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
extern const struct MidiConfig midiConfig[];

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
	i2c_master_enable(I2C1, I2C_FAST_MODE & I2C_BUS_RESET);
}

uint8 PresetUtil::getDeviceId1(int bankNumber) {
	if (bankNumber <4) {
		return EEPROM1_ID;
	} else {
		// Bank number 5 (tmp bank) is at the end of EEPROM2
		return EEPROM2_ID;
	}
}

uint8 PresetUtil::getDeviceId2(int bankNumber) {
	return EEPROM2_ID;
}


// Part 2 is 128 bytes long
int PresetUtil::getAddress1(int bankNumber, int preset) {
	if (bankNumber <4) {
		return bankNumber * BANKSIZE + preset * PART1_SIZE ;
	} else if (bankNumber == 4) {
		// Bank number 4 where is stored default patch...
		// EEPROM 2 - bank 2 - bloack 1
		return 2 * BANKSIZE  + BLOCKSIZE * 1 + preset * PART1_SIZE ;
	} else if (bankNumber == 5) {
		// Bank number 5 (tmp bank) place 3 of EEPROM2
		return 3 * BANKSIZE + preset * PART1_SIZE ;
	}
}

// Part 2 is 64 bytes long
int PresetUtil::getAddress2(int bankNumber, int preset) {
	if (bankNumber <4) {
		// EEPROM 2 - Bank 0 - column bankNumber
		return bankNumber * BLOCKSIZE *2 + preset * PART2_SIZE ;
	} else if (bankNumber == 4) {
		// Bank number 4 where is stored default patch...
		return 2 * BANKSIZE  + BLOCKSIZE * 1 + BLOCKSIZE / 2 + preset * PART2_SIZE;
	} else if (bankNumber == 5) {
		// EEPROM 2 - bank 2 - column 3
		return 2 * BANKSIZE + BLOCKSIZE * 2 + preset * PART2_SIZE ;
	}
}


#ifdef DEBUG

const char* engineEnums [] = { "ALGO1", "ALGO2", "ALGO3", "ALGO4", "ALGO5", "ALGO6", "ALGO7", "ALGO8", "ALGO9" };
const char* oscTypeEnums [] = { "OSC_FT_KEYBOARD ", "OSC_FT_FIXE" };
const char* mixOscShapeEnums [] = { "OSC_SHAPE_SIN", "OSC_SHAPE_SIN2", "OSC_SHAPE_SIN3", "OSC_SHAPE_SIN4", "OSC_SHAPE_RAND", "OSC_SHAPE_SQUARE", "OSC_SHAPE_SAW", "OSC_SHAPE_OFF" };
const char* lfoShapeEnums [] = { "LFO_SAW", "LFO_RAMP", "LFO_SQUARE", "LFO_RANDOM", "LFO_TYPE_MAX" };
const char* matrixSourceEnums [] = { "MATRIX_SOURCE_NONE", "MATRIX_SOURCE_LFO1", "MATRIX_SOURCE_LFO2", "MATRIX_SOURCE_LFO3", "MATRIX_SOURCE_LFO4", "MATRIX_SOURCE_PITCHBEND", "MATRIX_SOURCE_AFTERTOUCH", "MATRIX_SOURCE_MODWHEEL", "MATRIX_SOURCE_VELOCITY", "MATRIX_SOURCE_CC1", "MATRIX_SOURCE_CC2", "MATRIX_SOURCE_CC3", "MATRIX_SOURCE_CC4", "MATRIX_SOURCE_MAX" };
const char* matrixDestEnums [] = { "DESTINATION_NONE", "OSC1_FREQ", "OSC2_FREQ", "OSC3_FREQ", "OSC4_FREQ", "OSC5_FREQ", "OSC6_FREQ", "INDEX_MODULATION1", "INDEX_MODULATION2", "INDEX_MODULATION3", "INDEX_MODULATION4", "MIX_OSC1", "MIX_OSC2", "MIX_OSC3", "MIX_OSC4", "LFO1_FREQ", "LFO2_FREQ", "LFO3_FREQ", "LFO4_FREQ", "MTX1_MUL", "MTX2_MUL", "MTX3_MUL", "MTX4_MUL", "MTX5_MUL", "MTX6_MUL", "MTX7_MUL", "MTX8_MUL", "DESTINATION_MAX" };


void PresetUtil::dumpLine(const char *enums1[], int a, const char *enums2[], int b, const char *enums3[], int c, const char *enums4[], int d) {

	SerialUSB.print("{ ");
	if (enums1 == NULL) {
		SerialUSB.print(a);
	} else {
		SerialUSB.print(enums1[a]);
	}
	SerialUSB.print(", ");
	if (enums2 == NULL) {
		SerialUSB.print(b);
	} else {
		SerialUSB.print(enums2[b]);
	}
	SerialUSB.print(", ");
	if (enums3 == NULL) {
		SerialUSB.print(c);
	} else {
		SerialUSB.print(enums3[c]);
	}
	SerialUSB.print(", ");
	if (enums4 == NULL) {
		SerialUSB.print(d);
	} else {
		SerialUSB.print(enums4[d]);
	}
	SerialUSB.print("} ");
	SerialUSB.println(", ");
}

void PresetUtil::dumpPatch() {
	SerialUSB.print("// patch name : '");
	SerialUSB.print(PresetUtil::synthState->params.presetName);
	SerialUSB.println("'");
	SerialUSB.println("// Engine ");
	dumpLine(engineEnums,
			PresetUtil::synthState->params.engine1.algo,
			NULL,
			PresetUtil::synthState->params.engine1.velocity,
			NULL,
			PresetUtil::synthState->params.engine1.numberOfVoice,
			NULL,
			PresetUtil::synthState->params.engine1.glide);
	dumpLine(NULL,
			PresetUtil::synthState->params.engine2.modulationIndex1,
			NULL,
			PresetUtil::synthState->params.engine2.modulationIndex2,
			NULL,
			PresetUtil::synthState->params.engine2.modulationIndex3,
			NULL,
			PresetUtil::synthState->params.engine2.modulationIndex4);
	dumpLine(NULL,
			PresetUtil::synthState->params.engine3.mixOsc1,
			NULL,
			PresetUtil::synthState->params.engine3.mixOsc2,
			NULL,
			PresetUtil::synthState->params.engine3.mixOsc3,
			NULL,
			PresetUtil::synthState->params.engine3.mixOsc4);
	SerialUSB.println("// Oscillator");
	OscillatorParams * o =
			(OscillatorParams *) (&(PresetUtil::synthState->params.osc1));
	for (int k = 0; k < 6; k++) {
		dumpLine(mixOscShapeEnums,
				o[k].shape,
				oscTypeEnums,
				o[k].frequencyType,
				NULL,
				o[k].frequencyMul,
				NULL,
				o[k].detune);
	}
	SerialUSB.println("// Enveloppe");
	EnvelopeParams * e =
			(EnvelopeParams*) (&(PresetUtil::synthState->params.env1));
	for (int k = 0; k < 6; k++) {
		dumpLine(
				NULL,
				e[k].attack,
				NULL,
				e[k].decay,
				NULL,
				e[k].sustain,
				NULL,
				e[k].release);
	}
	SerialUSB.println("// Modulation matrix");
	MatrixRowParams	* m = (MatrixRowParams*) (&(PresetUtil::synthState->params.matrixRowState1));
	for (int k = 0; k < 8; k++) {
		dumpLine(
				matrixSourceEnums,
				m[k].source,
				NULL,
				m[k].mul,
				matrixDestEnums,
				m[k].destination,
				NULL,
				0);
	}
	SerialUSB.println("// LFOs");
	LfoParams* l = (LfoParams*) (&(PresetUtil::synthState->params.lfo1));
	for (int k = 0; k < 3; k++) {
		dumpLine(
				lfoShapeEnums,
				l[k].shape,
				NULL,
				l[k].freq,
				NULL,
				l[k].bias,
				NULL,
				l[k].keybRamp);
	}
	dumpLine(
			NULL,
			l[3].shape,
			NULL,
			l[3].freq,
			NULL,
			l[3].bias,
			NULL,
			l[3].keybRamp);
	SerialUSB.print("\"");
	SerialUSB.print(PresetUtil::synthState->params.presetName);
	SerialUSB.println("\"");
}

#endif


void PresetUtil::readSynthParamFromEEPROM(int bankNumber, int preset, AllSynthParams* params) {
	uint8 paramChars[PATCH_SIZE];
	PresetUtil::readCharsFromEEPROM(bankNumber, preset, paramChars);
	convertCharArrayToSynthState(paramChars, params);
}

void PresetUtil::readCharsFromEEPROM(int bankNumber, int preset, uint8* chars) {
	uint8 deviceaddress = PresetUtil::getDeviceId1(bankNumber);
	int address = PresetUtil::getAddress1(bankNumber, preset);

	uint8 bufReadAddress[2];
	i2c_msg msgsRead[2];
	int blockSize = 64;

	bufReadAddress[0] = (uint8) ((int) address >> 8);
	bufReadAddress[1] = (uint8) ((int) address & 0xff);

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = blockSize;
	msgsRead[1].data = chars;

	i2c_master_xfer(I2C1, msgsRead, 2, 500);
	delay(1);

	// Part 1 second block
	address = address + blockSize;
	bufReadAddress[0] = (uint8) ((int) address >> 8);
	bufReadAddress[1] = (uint8) ((int) address & 0xff);

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = blockSize;
	msgsRead[1].data = chars+ 64;
	i2c_master_xfer(I2C1, msgsRead, 2, 500);

	delay(1);

	// Now let's read part 2
	deviceaddress = PresetUtil::getDeviceId2(bankNumber);
	address = PresetUtil::getAddress2(bankNumber, preset);

	bufReadAddress[0] = (uint8) ((int) address >> 8);
	bufReadAddress[1] = (uint8) ((int) address & 0xff);

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = blockSize;
	msgsRead[1].data = chars+ blockSize *2;
	i2c_master_xfer(I2C1, msgsRead, 2, 500);

	delay(1);

}

char* PresetUtil::readPresetNameFromEEPROM(int bankNumber, int preset) {
	uint8 deviceaddress = PresetUtil::getDeviceId1(bankNumber);
	int address = PresetUtil::getAddress1(bankNumber, preset) + 27*4;
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
	delay(1);

	return readName;
}


void PresetUtil::savePatchToEEPROM(AllSynthParams* synthParams, int bankNumber, int preset) {
	uint8 paramChars[PATCH_SIZE];
	PresetUtil::convertSynthStateToCharArray(synthParams, paramChars );
	PresetUtil::saveCharParamsToEEPROM(paramChars, bankNumber, preset);
}


void PresetUtil::saveCharParamsToEEPROM(uint8* chars, int bankNumber, int preset) {
	const int blockSize = 64;
	uint8 deviceaddress = PresetUtil::getDeviceId1(bankNumber);
	int address = PresetUtil::getAddress1(bankNumber, preset);

	i2c_msg msgWrite1, msgWrite2;

	uint8 bufWrite[blockSize];

	bufWrite[0] = (uint8) ((int) address >> 8);
	bufWrite[1] = (uint8) ((int) address & 0xff);
	for (int k = 0; k < blockSize; k++) {
		bufWrite[k + 2] = chars[k];
	}

	/* Write test pattern  */
	msgWrite1.addr = deviceaddress;
	msgWrite1.flags = 0;
	msgWrite1.length = blockSize + 2;
	msgWrite1.data = bufWrite;
	i2c_master_xfer(I2C1, &msgWrite1, 1, 500);
	delay(5);

	address = address + blockSize;
	bufWrite[0] = (uint8) ((int) address >> 8);
	bufWrite[1] = (uint8) ((int) address & 0xff);
	for (int k = 0; k < blockSize; k++) {
		bufWrite[k + 2] = chars[k + blockSize];
	}
	msgWrite2.addr = deviceaddress;
	msgWrite2.flags = 0;
	msgWrite2.length = blockSize + 2;
	msgWrite2.data = bufWrite;
	i2c_master_xfer(I2C1, &msgWrite2, 1, 500);
	delay(5);

	// Let's save part 2 !!!
	deviceaddress = PresetUtil::getDeviceId2(bankNumber);
	address = PresetUtil::getAddress2(bankNumber, preset);

	bufWrite[0] = (uint8) ((int) address >> 8);
	bufWrite[1] = (uint8) ((int) address & 0xff);
	for (int k = 0; k < blockSize; k++) {
		bufWrite[k + 2] = chars[k + blockSize * 2];
	}
	msgWrite2.addr = deviceaddress;
	msgWrite2.flags = 0;
	msgWrite2.length = blockSize + 2;
	msgWrite2.data = bufWrite;
	i2c_master_xfer(I2C1, &msgWrite2, 1, 500);
	delay(5);

}

void PresetUtil::saveCurrentPatchToEEPROM(int bankNumber, int preset) {
	PresetUtil::savePatchToEEPROM(&PresetUtil::synthState->params, bankNumber, preset);
}


void PresetUtil::loadConfigFromEEPROM() {
	// Config EEPROM 2 - bank 2 - block 0
	uint8 deviceaddress = EEPROM2_ID;
	int address = 2 * BANKSIZE;

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
	delay(1);

	if (eeprom[0] == EEPROM_CONFIG_CHECK) {
		for (int k = 0; k < MIDICONFIG_SIZE; k++) {
			uint8 value = eeprom[k + 1];
			if (value >=0 && value < midiConfig[k].maxValue ) {
				PresetUtil::synthState->fullState.midiConfigValue[k] = value;
			}
		}
	}

}

void PresetUtil::saveConfigToEEPROM() {
	// Config EEPROM 2 - bank 2 - block 0
	uint8 deviceaddress = EEPROM2_ID;
	int address = 2 * BANKSIZE;

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

	delay(1);
}

void PresetUtil::formatEEPROM() {

	AllSynthParams synthParams  =  {
	        		// patch name : 'Preen'
	        		// Engine
	        		{ ALGO1, 6, 4, 6} ,
	        		{ 16, 24, 0, 0 } ,
	        		{ 128, 128, 128, 128} ,
	        		// Oscillator
	        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
	        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
	        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
	        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
	        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
	        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
	        		// Enveloppe
	        		{ 5, 0, 255, 50} ,
	        		{ 0, 65, 50, 50} ,
	        		{ 100, 65, 150, 255} ,
	        		{ 100, 65, 150, 100} ,
	        		{ 100, 65, 150, 100} ,
	        		{ 100, 65, 150, 100} ,
	        		// Modulation matrix
	        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION1, 0} ,
	        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
	        		{ MATRIX_SOURCE_LFO1, 0, INDEX_MODULATION2, 0} ,
	        		{ MATRIX_SOURCE_LFO6, 0, INDEX_MODULATION1, 0} ,
	        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
	        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
	        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
	        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
	        		// LFOs
	        		{ LFO_SAW, 18, 127, 150} ,
	        		{ LFO_SAW, 20, 0, 0} ,
	        		{ LFO_SAW, 3, 0, 0} ,
	        		{ 100, 65, 150, 100} ,
	        		{ 70, 16,  0, 0}  ,
	        		{ 140, 16, 0, 0},
	        		{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 , 0} ,
	        		{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 , 0} ,
	        		"<Empty>"
	        };

	for (int bankNumber = 0; bankNumber < 4; bankNumber++) {
		lcd.setCursor(3,2);
		lcd.print("Bank ");
		lcd.print((char)('A'+bankNumber));
		for (int preset = 0; preset < 128; preset++) {

			lcd.setCursor(11,2);
			lcd.print(preset);
			lcd.print("  ");

			PresetUtil::savePatchToEEPROM(&synthParams, bankNumber, preset);

		}
	}
}

void PresetUtil::sendBankToSysex(int bankNumber) {
	uint8 paramChars[PATCH_SIZE];

	uint8 newPatch[] = { 0xf0, 0x7d, 0x02 };
	for (int k = 0; k <= 2; k++) {
		Serial3.print(newPatch[k]);
	}

	for (int preset = 0; preset < 128; preset++) {
		lcd.setCursor(3,2);
		lcd.print(preset);
		lcd.print(" / 128");
		PresetUtil::readCharsFromEEPROM(bankNumber, preset, paramChars);
		PresetUtil::sendParamsToSysex(paramChars);
	}

	Serial3.print((uint8) 0xf7);

}


void PresetUtil::checkReadEEPROM() {
	struct AllSynthParams synthParams;

	lcd.clear();
	lcd.setCursor(3,0);
	lcd.print("Check EEPROM");

	for (int bank = 0; bank<4; bank++) {
		lcd.setCursor(3,1);
		lcd.print("Bank ");
		lcd.print((char)('A'+bank));

		for (int test = 0; test< 256; test++) {
			int preset = test%128;
			lcd.setCursor(3,2);
			lcd.print(preset);
			lcd.print(" ");
			PresetUtil::readSynthParamFromEEPROM(bank, preset, &synthParams);
		}
	}
}


void PresetUtil::sendCurrentPatchToSysex() {
	uint8 paramChars[PATCH_SIZE];
	uint8 newPatch[] = { 0xf0, 0x7d, 0x01 };
	for (int k = 0; k <= 2; k++) {
		Serial3.print(newPatch[k]);
	}

	PresetUtil::convertSynthStateToCharArray(&PresetUtil::synthState->params, paramChars);
	PresetUtil::sendParamsToSysex(paramChars);

	Serial3.print((uint8) 0xf7);
}

void PresetUtil::sendParamsToSysex(uint8* params) {
	int checksum = 0;

	for (int k = 0; k < PATCH_SIZE; k++) {
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

int PresetUtil::readSysexPatch(uint8* params) {
	int checkSum = 0;
	unsigned int index = 0;
	int value = 0;

	while (index < PATCH_SIZE) {

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
	uint8 paramChars[PATCH_SIZE];

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
				if ((errorCode = PresetUtil::readSysexPatch(paramChars)) <0) {
					index = -errorCode;
					bError = true;
				} else {
					PresetUtil::synthState->propagateBeforeNewParamsLoad();
					PresetUtil::convertCharArrayToSynthState(paramChars, &PresetUtil::synthState->params);
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


void PresetUtil::copySynthParams(char* source, char* dest) {
    for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
        dest[k] = source[k];
    }
}


int PresetUtil::readSysexBank() {
	uint8 paramChars[PATCH_SIZE];
	int errorCode = 0;

	lcd.setCursor(1,3);
	lcd.print("Bank:");

	for (int preset = 0; preset<128 && errorCode>=0; preset++) {
		if ((errorCode = PresetUtil::readSysexPatch(paramChars)) <0) {
			errorCode = -500 - preset;
			break;
		}

		lcd.setCursor(7,3);
		lcd.print(preset);

		PresetUtil::saveCharParamsToEEPROM(paramChars, 4, preset);
	}

	return errorCode;
}



void PresetUtil::copyBank(int source, int dest) {
	uint8 paramChars[PATCH_SIZE];
	lcd.setCursor(1,3);
	lcd.print("Save:");
	for (int preset=0; preset<127; preset++) {
		PresetUtil::readCharsFromEEPROM(source, preset, paramChars);
		lcd.setCursor(7,3);
		lcd.print(preset);
		PresetUtil::saveCharParamsToEEPROM(paramChars, dest, preset);
	}
}


void PresetUtil::loadDefaultPatchIfAny() {
	uint8 paramChars[PATCH_SIZE];
	PresetUtil::readCharsFromEEPROM(5, 0, paramChars);

	if (((char*)&paramChars)[0] == EEPROM_CONFIG_CHECK) {
        PresetUtil::readSynthParamFromEEPROM(5, 1, &PresetUtil::synthState->params);
	}
}


void PresetUtil::saveCurrentPatchAsDefault() {
	uint8 paramChars[PATCH_SIZE];
	for (unsigned int k=0; k<PATCH_SIZE; k++) {
		paramChars[k] = EEPROM_CONFIG_CHECK;
	}
	PresetUtil::saveCharParamsToEEPROM(paramChars, 5, 0);
	PresetUtil::saveCurrentPatchToEEPROM(5,1);
}

void PresetUtil::convertSynthStateToCharArray(AllSynthParams* params, uint8* chars) {
	// Clean
	for (unsigned int k=0; k<PATCH_SIZE; k++) {
		chars[k] = 0;
	}

	// 1.00 compatibility
	int firstPartSize = 27*4;
	for (unsigned int k=0; k<firstPartSize; k++) {
		chars[k] = ((char*) params)[k];
	}

	for (unsigned int k=0; k<12; k++) {
		chars[firstPartSize + k] = params->presetName[k];
	}

	// 120 - 128 : step seqs
	for (unsigned int k=0; k<8; k++) {
		chars[120 + k] = ((char*) &params->lfo5)[k];
	}

	// 128 : 16 step of seq 1 on 8 bits
	// 4 left bits + 4 right bits
	for (unsigned int k=0; k<8; k++) {
		chars[128+k] = (params->steps5.steps[k*2]<<4) + params->steps5.steps[k*2+1] ;
	}
	// 136 : 16 step of seq 2 on 8 bits
	for (unsigned int k=0; k<8; k++) {
		chars[136+k] = (params->steps6.steps[k*2]<<4) + params->steps6.steps[k*2+1] ;
	}
}

void PresetUtil::convertCharArrayToSynthState(uint8* chars, AllSynthParams* params) {

	// Copy first part
	int firstPartSize = 27*4;
	for (unsigned int k=0; k<firstPartSize; k++) {
		((char*) params)[k] = chars[k];
	}
	// Title
	for (unsigned int k=0; k<12; k++) {
		params->presetName[k] = chars[firstPartSize+k];
	}
	params->presetName[12] = '\0';


	// 120 - 128 : step seqs
	for (unsigned int k=0; k<8; k++) {
		((char*) &params->lfo5)[k] = chars[120 + k];
	}

	// 128 : 16 step of seq 1 on 8 bits
	// 4 left bits + 4 right bits
	for (unsigned int k=0; k<8; k++) {
		params->steps5.steps[k*2]   = chars[128+k] >> 4;
		params->steps5.steps[k*2+1] = chars[128+k] & 0xf;
	}
	// 136 : 16 step of seq 2 on 8 bits
	for (unsigned int k=0; k<8; k++) {
		params->steps6.steps[k*2]   = chars[136+k] >> 4;
		params->steps6.steps[k*2+1] = chars[136+k] & 0xf;
	}
}

