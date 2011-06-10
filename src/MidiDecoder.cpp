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
#include "SynthState.h"




MidiDecoder::MidiDecoder() {
    newEvent = true;
    index=0;
    maxInBuffer=0;
    maxInARow=0;
}

MidiDecoder::~MidiDecoder() {
}

void MidiDecoder::setSynth(Synth* synth) {
    this->synth = synth;
}


void MidiDecoder::readSysex() {
    unsigned int HEADER_SIZE = 2;
    // 0x7d : non commercial
    // 01 : Device ID
    // 01 : model ID... Version
    // 12 : Information sent
    // 0x000000 : load patch....
    // .. The patch

    /*
    SerialUSB.println("readSysex....");
    SerialUSB.print("index up to  :");
    SerialUSB.println(sizeof(struct AllSynthParams)+7);
     */
    static int newPatch[] = {0x7d, 0x01};

    unsigned int index = 0;
    uint8 value = 0;
    int checksum = 0, sentChecksum = 0;

    while (true) {
        int timeout = 0;
        while (!Serial3.available() && timeout<5000) {
            timeout ++;
        }
        if (timeout>=5000) {
            break;
        }

        uint8 byte = Serial3.read();
        if (byte == 0xF7) {
            break;
        }
        if (index < HEADER_SIZE) {
            if (byte == newPatch[index]) {
                index ++;
            }
        } else if (index < sizeof(struct AllSynthParams)+HEADER_SIZE) {
            value+= byte;

            if (byte<127) {
                ((uint8*) &synthState.backupParams)[index-HEADER_SIZE] = value;
                index++;
                checksum += value;
                value = 0;
            }
        } else {
            sentChecksum = byte;
            checksum = checksum % 128;
        }
    }

    /*
    SerialUSB.print("Received checksum : ");
    SerialUSB.println(sentChecksum);

    SerialUSB.print("Computed checksum : ");
    SerialUSB.println(checksum);

    for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
        SerialUSB.print(k);
        SerialUSB.print(" : ");
        SerialUSB.print((int)((uint8*) &synthState.backupParams)[k]);
        SerialUSB.print(" /  ");
    }
    SerialUSB.println();
     */

    if (checksum == sentChecksum) {
        synthState.copyPatch((char*)&synthState.backupParams, (char*)&synthState.params, true);
        synthState.resetDisplay();
    }

}


void MidiDecoder::newByte(unsigned char byte) {
    bool eventComplete = false;
    if (newEvent) {
        unsigned char hi = byte & 0xf0;
        channel = byte & 0x0f;
        /*
        SerialUSB.print("hi : ");
        SerialUSB.println((int)hi);
         */
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
        case 0xc0:
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
        case 0xf0:
            readSysex();
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

//extern LiquidCrystal lcd;

void MidiDecoder::sendMidiEvent() {
    if (channel != synthState.fullState.midiChannel) {
        Serial3.print((unsigned char)(currentEvent[0] + channel));
        for (int k=0; k<numberOfBytes-1; k++) {
            Serial3.print((unsigned char)(currentEvent[k+1]));
        }
        return;
    }

    switch (currentEvent[0]) {
    case 0x80:
        this->synth->noteOff(currentEvent[1]);
        break;
    case 0x90:
        this->synth->noteOn(currentEvent[1], currentEvent[2]);
        this->synth->getMatrix()->setSource(VELOCITY, currentEvent[2]);
        break;
    case 0xb0:
        /*
    	SerialUSB.print("control change  : ");
    	SerialUSB.print( (short)currentEvent[1]);
    	SerialUSB.print(", ");
    	SerialUSB.println((short) currentEvent[2]);
         */
        controlChange(currentEvent);
        break;
    case 0xd0:
        this->synth->getMatrix()->setSource(AFTERTOUCH, currentEvent[1]);
        break;
    case 0xe0:
        this->synth->getMatrix()->setSource(PITCHBEND, (int)    ((((int)currentEvent[2] << 7) + (int)currentEvent[1] -8192) >>6)) ;
        break;
    case 0xc0:
        // Programm change
        this->synth->allSoundOff();
        // load curentEvent[1]
        PresetUtil::readFromEEPROM(synthState.fullState.bankNumber, currentEvent[1]);
        break;
    }
}




void MidiDecoder::controlChange(unsigned char *currentEvent) {
    switch (currentEvent[1]) {
    case CC_BANK_SELECT:
        if (currentEvent[2]>=1 and currentEvent[2]<=3) {
            synthState.fullState.bankNumber = currentEvent[2];
        }
        break;
    case CC_MODWHEEL:
        this->synth->getMatrix()->setSource(MODWHEEL, currentEvent[2]);
        break;
    case CC_VOICE:
        synthState.setNewValue(ROW_ENGINE, ENCODER_ENGINE_VOICE, currentEvent[2]);
        break;
    case CC_GLIDE:
        synthState.setNewValue(ROW_ENGINE, ENCODER_ENGINE_GLIDE, currentEvent[2]);
        break;
    case CC_IM1:
        synthState.setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM1, currentEvent[2]*2);
        break;
    case CC_IM2:
        synthState.setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM2, currentEvent[2]*2);
        break;
    case CC_IM3:
        synthState.setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM3, currentEvent[2]*2);
        break;
    case CC_IM4:
        synthState.setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM4, currentEvent[2]*2);
        break;
    case CC_MIX1:
        synthState.setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX1, currentEvent[2]);
        break;
    case CC_MIX2:
        synthState.setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX2, currentEvent[2]);
        break;
    case CC_MIX3:
        synthState.setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX3, currentEvent[2]);
        break;
    case CC_MIX4:
        synthState.setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX4, currentEvent[2]);
        break;
    case CC_OSC1_FREQ:
        synthState.setNewValue(ROW_OSC1, ENCODER_OSC_FREQ, currentEvent[2]*2);
        break;
    case CC_OSC2_FREQ:
        synthState.setNewValue(ROW_OSC2, ENCODER_OSC_FREQ, currentEvent[2]*2);
        break;
    case CC_OSC3_FREQ:
        synthState.setNewValue(ROW_OSC3, ENCODER_OSC_FREQ, currentEvent[2]*2);
        break;
    case CC_OSC4_FREQ:
        synthState.setNewValue(ROW_OSC4, ENCODER_OSC_FREQ, currentEvent[2]*2);
        break;
    case CC_OSC5_FREQ:
        synthState.setNewValue(ROW_OSC5, ENCODER_OSC_FREQ, currentEvent[2]*2);
        break;
    case CC_OSC6_FREQ:
        synthState.setNewValue(ROW_OSC6, ENCODER_OSC_FREQ, currentEvent[2]*2);
        break;
    case CC_OSC1_DETUNE:
        synthState.setNewValue(ROW_OSC1, ENCODER_OSC_FTUNE, currentEvent[2]*2);
        break;
    case CC_OSC2_DETUNE:
        synthState.setNewValue(ROW_OSC2, ENCODER_OSC_FTUNE, currentEvent[2]*2);
        break;
    case CC_OSC3_DETUNE:
        synthState.setNewValue(ROW_OSC3, ENCODER_OSC_FTUNE, currentEvent[2]*2);
        break;
    case CC_OSC4_DETUNE:
        synthState.setNewValue(ROW_OSC4, ENCODER_OSC_FTUNE, currentEvent[2]*2);
        break;
    case CC_OSC5_DETUNE:
        synthState.setNewValue(ROW_OSC5, ENCODER_OSC_FTUNE, currentEvent[2]*2);
        break;
    case CC_OSC6_DETUNE:
        synthState.setNewValue(ROW_OSC6, ENCODER_OSC_FTUNE, currentEvent[2]*2);
        break;
    case CC_ENV1_ATTACK :
        synthState.setNewValue(ROW_ENV1, ENCODER_ENV_A, currentEvent[2]*2);
        break;
    case CC_ENV1_RELEASE:
        synthState.setNewValue(ROW_ENV1, ENCODER_ENV_R, currentEvent[2]*2);
        break;
    case CC_ENV2_ATTACK :
        synthState.setNewValue(ROW_ENV2, ENCODER_ENV_A, currentEvent[2]*2);
        break;
    case CC_ENV2_RELEASE:
        synthState.setNewValue(ROW_ENV2, ENCODER_ENV_R, currentEvent[2]*2);
        break;
    case CC_ENV3_ATTACK :
        synthState.setNewValue(ROW_ENV3, ENCODER_ENV_A, currentEvent[2]*2);
        break;
    case CC_ENV3_RELEASE:
        synthState.setNewValue(ROW_ENV3, ENCODER_ENV_R, currentEvent[2]*2);
        break;
    case CC_ENV4_ATTACK :
        synthState.setNewValue(ROW_ENV4, ENCODER_ENV_A, currentEvent[2]*2);
        break;
    case CC_ENV4_RELEASE:
        synthState.setNewValue(ROW_ENV4, ENCODER_ENV_R, currentEvent[2]*2);
        break;
    case CC_ENV5_ATTACK :
        synthState.setNewValue(ROW_ENV5, ENCODER_ENV_A, currentEvent[2]*2);
        break;
    case CC_ENV5_RELEASE:
        synthState.setNewValue(ROW_ENV5, ENCODER_ENV_R, currentEvent[2]*2);
        break;
    case CC_ENV6_ATTACK :
        synthState.setNewValue(ROW_ENV6, ENCODER_ENV_A, currentEvent[2]*2);
        break;
    case CC_ENV6_RELEASE:
        synthState.setNewValue(ROW_ENV6, ENCODER_ENV_R, currentEvent[2]*2);
        break;
    case CC_MATRIXROW1_MUL:
        synthState.setNewValue(ROW_MATRIX1, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_MATRIXROW2_MUL:
        synthState.setNewValue(ROW_MATRIX2, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_MATRIXROW3_MUL:
        synthState.setNewValue(ROW_MATRIX3, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_MATRIXROW4_MUL:
        synthState.setNewValue(ROW_MATRIX4, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_MATRIXROW5_MUL:
        synthState.setNewValue(ROW_MATRIX5, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_MATRIXROW6_MUL:
        synthState.setNewValue(ROW_MATRIX6, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_MATRIXROW7_MUL:
        synthState.setNewValue(ROW_MATRIX7, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_MATRIXROW8_MUL:
        synthState.setNewValue(ROW_MATRIX8, ENCODER_MATRIX_MUL, currentEvent[2]*2 - 128);
        break;
    case CC_LFO1_FREQ:
        synthState.setNewValue(ROW_LFO1, CC_LFO1_FREQ, currentEvent[2]*2);
        break;
    case CC_LFO2_FREQ:
        synthState.setNewValue(ROW_LFO2, CC_LFO1_FREQ, currentEvent[2]*2);
        break;
    case CC_LFO3_FREQ:
        synthState.setNewValue(ROW_LFO3, CC_LFO1_FREQ, currentEvent[2]*2);
        break;
    case CC_LFO4_FREQ:
        synthState.setNewValue(ROW_LFO4, CC_LFO1_FREQ, currentEvent[2]*2);
        break;
    case CC_ALL_NOTES_OFF:
        this->synth->allNoteOff();
        break;
    case CC_ALL_SOUND_OFF:
        this->synth->allSoundOff();
        break;
    case 99:
        currentNrpn.paramMSB = currentEvent[2];
        break;
    case 98:
        currentNrpn.paramLSB = currentEvent[2];
        break;
    case 38 :
        currentNrpn.valueMSB = currentEvent[2];
        break;
    case 6 :
        currentNrpn.valueLSB = currentEvent[2];
        decodeNrpn();
        break;
    case 96 :
        // nrpn increment
        // incNrpn();
        break;
    case 97 :
        // nrpn decremenet
        //decNrpn();
        break;


    }
}

void MidiDecoder::decodeNrpn() {
    unsigned int index = (currentNrpn.paramMSB<<7) +  currentNrpn.paramLSB;
    unsigned int value = (currentNrpn.valueMSB<<7) +  currentNrpn.valueLSB;
    unsigned int row = index / NUMBER_OF_ENCODERS;
    unsigned int encoder = index % NUMBER_OF_ENCODERS;


    if (row<NUMBER_OF_ROWS) {
        struct ParameterDisplay param = allParameterRows.row[row]->params[encoder];
        synthState.setNewValue(row, encoder, param.minValue + value);
    }
}


void MidiDecoder::newParamValueFromExternal(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
    // Do nothing here...
}

void MidiDecoder::newParamValue(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {

    if (false) {
        // NRPN SEND... IN PROGRESS....
        struct ControlChange cc1, cc2, cc3;
        cc1.control = 99;
        cc1.value = 0;
        midiToSend.insert(cc1);
        cc2.control = 98;
        cc2.value = (uint8)currentrow*NUMBER_OF_ENCODERS + encoder;
        midiToSend.insert(cc2);
        /*    cc.control = 38;
        cc.value = 0;
        midiToSend.insert(cc);
         */
        cc3.control = 6;
        cc3.value = (uint8)newValue;
        midiToSend.insert(cc3);
    }

    // Send control change
    struct ControlChange cc;
    cc.control = 0;
    cc.value = 0;

    switch (currentrow) {
    case ROW_ENGINE:
        if (encoder == ENCODER_ENGINE_VOICE) {
            cc.control = CC_VOICE;
            cc.value =  newValue;
        } else if (encoder == ENCODER_ENGINE_GLIDE) {
            cc.control = CC_GLIDE;
            cc.value =  newValue;
        }
        break;
    case ROW_MODULATION:
        cc.control = CC_IM1 + encoder;
        cc.value =  newValue>>1;
        break;
    case ROW_OSC_MIX:
        cc.control = CC_MIX1 + encoder;
        cc.value =  newValue;
        break;
    case ROW_OSC_FIRST...ROW_OSC_LAST:
    if (encoder == ENCODER_OSC_FREQ) {
        cc.control = CC_OSC1_FREQ + (currentrow - ROW_OSC_FIRST);
        cc.value = newValue>>1;
    } else if (encoder == ENCODER_OSC_FTUNE) {
        cc.control = CC_OSC1_DETUNE + (currentrow - ROW_OSC_FIRST);
        cc.value = newValue>>1;
    }
    break;
    case ROW_ENV_FIRST...ROW_ENV_LAST:
    if (encoder == ENCODER_ENV_A) {
        cc.control = CC_ENV1_ATTACK + (currentrow - ROW_ENV_FIRST);
        cc.value = newValue>>1;
    } else if (encoder==ENCODER_ENV_R) {
        cc.control = CC_ENV1_RELEASE + (currentrow - ROW_ENV_FIRST);
        cc.value = newValue>>1;
    }
    break;
    case ROW_MATRIX_FIRST...ROW_MATRIX_LAST:
    if (encoder==ENCODER_MATRIX_MUL) {
        cc.control = CC_MATRIXROW1_MUL + currentrow - ROW_MATRIX_FIRST;
        cc.value = (newValue>>1)+64;
    }
    break;
    case ROW_LFO_FIRST...ROW_LFO_LAST:
    if (encoder==ENCODER_LFO_FREQ) {
        cc.control = CC_MATRIXROW1_MUL + currentrow - ROW_LFO_FIRST;
        cc.value = (newValue>>1);
    }
    break;
    }

    if (cc.control!=0) {
        midiToSend.insert(cc);
    }
}


void MidiDecoder::sendOneMidiEvent() {
    int howManyToSend =midiToSend.getCount();
    if (howManyToSend>0) {
        ControlChange toSend = midiToSend.remove();
        while (midiToSend.getOneAfter().control == toSend.control) {
            toSend = midiToSend.remove();
        }
        Serial3.print((unsigned char)(0xb0 + synthState.fullState.midiChannel));
        Serial3.print((unsigned char)toSend.control);
        Serial3.print((unsigned char)toSend.value);
    }
}
