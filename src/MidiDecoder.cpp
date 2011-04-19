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

void MidiDecoder::newByte(unsigned char byte) {
    bool eventComplete = false;
    if (newEvent) {
        unsigned char hi = byte & 0xf0;
        channel = byte & 0x0f;
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

extern LiquidCrystal lcd;

void MidiDecoder::sendMidiEvent() {
    if (channel != synthState.fullState.midiChannel) {
        Serial2.print((unsigned char)(currentEvent[0] + channel));
        for (int k=0; k<numberOfBytes-1; k++) {
            Serial2.print((unsigned char)(currentEvent[k+1]));
        }
        return;
    }

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
        switch (currentEvent[1]) {
        case CC_MODWHEEL:
            this->synth->getMatrix()->setSource(MODWHEEL, currentEvent[2]);
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
        case CC_ENV1_ATTACK :
            synthState.setNewValue(ROW_ENV1, ENCODER_ENV_A, currentEvent[2]*2);
            break;
        case CC_ENV1_SUSTAIN:
            synthState.setNewValue(ROW_ENV1, ENCODER_ENV_S, currentEvent[2]*2);
            break;
        case CC_ENV1_DECAY:
            synthState.setNewValue(ROW_ENV1, ENCODER_ENV_D, currentEvent[2]*2);
            break;
        case CC_ENV2_ATTACK :
            synthState.setNewValue(ROW_ENV2, ENCODER_ENV_A, currentEvent[2]*2);
            break;
        case CC_ENV2_SUSTAIN:
            synthState.setNewValue(ROW_ENV2, ENCODER_ENV_S, currentEvent[2]*2);
            break;
        case CC_ENV2_DECAY:
            synthState.setNewValue(ROW_ENV2, ENCODER_ENV_D, currentEvent[2]*2);
            break;
        case CC_ENV3_ATTACK :
            synthState.setNewValue(ROW_ENV3, ENCODER_ENV_A, currentEvent[2]*2);
            break;
        case CC_ENV3_SUSTAIN:
            synthState.setNewValue(ROW_ENV3, ENCODER_ENV_S, currentEvent[2]*2);
            break;
        case CC_ENV3_DECAY:
            synthState.setNewValue(ROW_ENV3, ENCODER_ENV_D, currentEvent[2]*2);
            break;
        case CC_ENV4_ATTACK :
            synthState.setNewValue(ROW_ENV4, ENCODER_ENV_A, currentEvent[2]*2);
            break;
        case CC_ENV4_SUSTAIN:
            synthState.setNewValue(ROW_ENV4, ENCODER_ENV_S, currentEvent[2]*2);
            break;
        case CC_ENV4_DECAY:
            synthState.setNewValue(ROW_ENV4, ENCODER_ENV_D, currentEvent[2]*2);
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
        case CC_MATRIX_SOURCE1:
            this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC1, currentEvent[2]);
            break;
        case CC_MATRIX_SOURCE2:
            this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC2, currentEvent[2]);
            break;
        case CC_MATRIX_SOURCE3:
            this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC3, currentEvent[2]);
            break;
        case CC_MATRIX_SOURCE4:
            this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC4, currentEvent[2]);
            break;
        }
        case 0xd0:
            this->synth->getMatrix()->setSource(AFTERTOUCH, currentEvent[1]);
            break;
        case 0xe0:
            this->synth->getMatrix()->setSource(PITCHBEND, (int)    ((((int)currentEvent[2] << 7) + (int)currentEvent[1] -8192) >>6)) ;
            break;
    }
}

void MidiDecoder::newParamValueFromExternal(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
    // Do nothing here...
}

void MidiDecoder::newParamValue(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
    // Send control change
    struct ControlChange cc;
    cc.control = 0;
    cc.value = 0;

    // Engine
    if (currentrow==0) {
        cc.control = CC_IM1 + encoder -1;
        cc.value =  newValue>>1;
    }

    // OSCILLATOR
    if (currentrow>=1 && currentrow<=4) {
        if (encoder == 2) {
            cc.control = CC_OSC1_FREQ + (currentrow - 1);
            cc.value = newValue>>1;
        } else if (encoder == 3) {
            cc.control = CC_OSC1_DETUNE + (currentrow - 1);
            cc.value = newValue>>1;
        }
    }

    // Enveloppe
    if (currentrow>=5 and currentrow<=8) {
        if (encoder == 0) {
            cc.control = CC_ENV1_ATTACK + (currentrow - 5);
            cc.value = newValue>>1;
        } else if (encoder==2) {
            cc.control = CC_ENV1_SUSTAIN + (currentrow - 5);
            cc.value = newValue>>1;
        } else if (encoder==3) {
            cc.control = CC_ENV1_DECAY + (currentrow - 5);
            cc.value = newValue>>1;
        }
    }
    // Matrix mul
    if (currentrow>=9 and currentrow<=14) {
        if (encoder==1) {
            cc.control = CC_MATRIXROW1_MUL + currentrow - 9;
            cc.value = (newValue>>1)+64;
        }
    }
    // LFO Freq
    if (currentrow>=15 && currentrow<=18) {
        if (encoder==1) {
            cc.control = CC_MATRIXROW1_MUL + currentrow - 15;
            cc.value = (newValue>>1);
        }
    }


    if (cc.control!=0) {
        midiToSend.insert(cc);
    }
}


void MidiDecoder::sendOneMidiEvent() {
    int howManyToSend =midiToSend.getCount();
    if (howManyToSend>0) {
        /* debug....
		if (howManyToSend>maxInBuffer) {
			maxInBuffer = howManyToSend;
			lcd.setCursor(12,0);
			lcd.print(maxInBuffer);
		}
         */
        ControlChange toSend = midiToSend.remove();
        // int cpt = 1;
        while (midiToSend.getOneAfter().control == toSend.control) {
            toSend = midiToSend.remove();
            //cpt++;
        }
        /* debug
		if (cpt>maxInARow) {
			maxInARow = cpt;
			lcd.setCursor(9,0);
			lcd.print(maxInARow);
		}
         */
        Serial2.print((unsigned char)(0xb0 + synthState.fullState.midiChannel));
        Serial2.print((unsigned char)toSend.control);
        Serial2.print((unsigned char)toSend.value);
        /* debug
		lcd.setCursor(0,0);
		lcd.print("    ");
		lcd.setCursor(0,0);
		lcd.print((short)toSend.control);
		lcd.setCursor(4,0);
		lcd.print("    ");
		lcd.setCursor(4,0);
		lcd.print((short)toSend.value);
         */
    }
}
