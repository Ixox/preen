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

#ifdef LCDDEBUG
#include "LiquidCrystal.h"
extern LiquidCrystal lcd;
int pos = 0;


void eraseNext(int pos) {
	int x = (pos % 2) * 10;
	int y = pos / 2;
	y = y % 4;
	lcd.setCursor(x,y);
	lcd.print("- --- --- ");
}


void debug(char *l, int i1, int i2) {
	int x = (pos % 2) * 10;
	int y = pos / 2;
	y = y % 4;
	lcd.setCursor(x,y);
	lcd.print("          ");
	lcd.setCursor(x,y);
	lcd.print(l);
	lcd.setCursor(x+2, y);
	lcd.print(i1);
	lcd.setCursor(x+6, y);
	lcd.print(i2);
	pos++;
	eraseNext(pos);
}
#endif

MidiDecoder::MidiDecoder() {
	currentEventState.eventState = MIDI_EVENT_WAITING;
	currentEventState.index = 0;
	this->isSequencerPlaying = false;
	this->midiClockCpt = 0;
	this->runningStatus = 0;
}

MidiDecoder::~MidiDecoder() {
}

void MidiDecoder::setSynth(Synth* synth) {
	this->synth = synth;
}

void MidiDecoder::setVisualInfo(VisualInfo *visualInfo) {
	this->visualInfo = visualInfo;
}

void MidiDecoder::newByte(unsigned char byte) {

	if (this->synthState->fullState.midiConfigValue[MIDICONFIG_THROUGH] == 1) {
		Serial3.print((unsigned char) byte);
	}

	// Realtime first !
	if (byte >= 0xF8) {
		switch (byte) {
		case MIDI_CONTINUE:
			this->isSequencerPlaying = true;
			this->midiClockCpt = 0;
			this->synth->midiClockContinue(this->songPosition);
			if ((this->songPosition & 0x3) <= 1) {
				this->visualInfo->midiClock(true);
			}
			break;
		case MIDI_CLOCK:
			this->midiClockCpt++;
			if (this->midiClockCpt == 6) {
				if (this->isSequencerPlaying) {
					this->songPosition++;
				}
				this->midiClockCpt = 0;
				this->synth->midiClockSongPositionStep(this->songPosition);

				if ((this->songPosition & 0x3) == 0) {
					this->visualInfo->midiClock(true);
				}
				if ((this->songPosition & 0x3) == 0x2) {
					this->visualInfo->midiClock(false);
				}
			}
			break;
		case MIDI_START:
			this->isSequencerPlaying = true;
			this->songPosition = 0;
			this->midiClockCpt = 0;
			this->visualInfo->midiClock(true);
			this->synth->midiClockStart();
			break;
		case MIDI_STOP:
			this->isSequencerPlaying = false;
			this->synth->midiClockStop();
			this->visualInfo->midiClock(false);
			break;
		}
	} else {
		if (currentEventState.eventState == MIDI_EVENT_WAITING && byte >= 0x80) {
			newMessageType(byte);
			this->runningStatus = byte;
		} else if (currentEventState.eventState == MIDI_EVENT_WAITING && byte < 0x80) {
			// midi source use running status...
			if (this->runningStatus>0) {
				newMessageType(this->runningStatus);
				newMessageData(byte);
			}
		} else if (currentEventState.eventState == MIDI_EVENT_IN_PROGRESS) {
			newMessageData(byte);
		} else if (currentEventState.eventState == MIDI_EVENT_SYSEX) {
			if (byte == MIDI_SYSEX_END) {
				currentEventState.eventState = MIDI_EVENT_WAITING;
				currentEventState.index = 0;
			} else if (currentEventState.index == 0) {

				currentEventState.index = 1;
				// Only do something for non commercial sysex
				// We assume it's for us !
				if (byte == 0x7d) {
					// System exclusive
					// Allow patch if real time allowed OR if currenly waiting for sysex
					// Allow bank if currently waiting for sysex only
					bool waitingForSysex = this->synthState->fullState.currentMenuItem->menuState == MENU_MIDI_SYSEX_GET;
					bool realTimeSysexAllowed = this->synthState->fullState.midiConfigValue[MIDICONFIG_REALTIME_SYSEX] == 1;
					int r = PresetUtil::readSysex(realTimeSysexAllowed || waitingForSysex, waitingForSysex);
					currentEventState.eventState = MIDI_EVENT_WAITING;
					currentEventState.index = 0;
					if (r == 2) {
						this->synthState->newBankReady();
					}
				}
			} else {
				// We do nothing !
				// consume sysex message till the end !
			}
		}
	}
}

void MidiDecoder::newMessageData(uint8 byte) {
	currentEvent.value[currentEventState.index++] = byte;
	if (currentEventState.index == currentEventState.numberOfBytes) {
		midiEventReceived(currentEvent);
		currentEventState.eventState = MIDI_EVENT_WAITING;
		currentEventState.index = 0;
	}
}


void MidiDecoder::newMessageType(uint8 byte) {
	currentEvent.eventType = (EventType)(byte & 0xf0);
	currentEvent.channel = byte & 0x0f;

	switch (currentEvent.eventType) {
	case MIDI_NOTE_OFF:
	case MIDI_NOTE_ON:
	case MIDI_CONTROL_CHANGE:
	case MIDI_PITCH_BEND:
	case MIDI_POLY_AFTER_TOUCH:
		currentEventState.numberOfBytes = 2;
		currentEventState.eventState = MIDI_EVENT_IN_PROGRESS;
		break;
	case MIDI_AFTER_TOUCH:
	case MIDI_PROGRAM_CHANGE:
		currentEventState.numberOfBytes = 1;
		currentEventState.eventState = MIDI_EVENT_IN_PROGRESS;
		break;
	case MIDI_REAL_TIME_EVENT:
		// We must be sure it's 0xF0 and not 0xF1, 0xF8....
		this->runningStatus = 0;
		switch (byte)  {
		case MIDI_SYSEX:
			currentEventState.eventState = MIDI_EVENT_SYSEX;
			break;
		case MIDI_SONG_POSITION:
			currentEvent.eventType = MIDI_SONG_POSITION;
			// Channel hack to make it accepted
			currentEvent.channel = 	this->synthState->fullState.midiConfigValue[MIDICONFIG_CHANNEL] -1;
			currentEventState.numberOfBytes = 2;
			currentEventState.eventState = MIDI_EVENT_IN_PROGRESS;
			break;
		}
		break;
	default :
#ifdef LCDDEBUG
		debug("W", currentEvent.eventType, 0);
#endif
		// Nothing to do...
		break;
	}
}

void MidiDecoder::midiEventReceived(MidiEvent midiEvent) {
	int acceptedChannel = this->synthState->fullState.midiConfigValue[MIDICONFIG_CHANNEL] -1;
	if (acceptedChannel >= 0 && midiEvent.channel != acceptedChannel) {
		// Not the right channel !
		return;
	}

	switch (midiEvent.eventType) {
	case MIDI_NOTE_OFF:
#ifdef LCDDEBUG
		debug("F", midiEvent.value[0], midiEvent.value[1]);
#endif
		this->synth->noteOff(midiEvent.value[0]);
		break;
	case MIDI_NOTE_ON:
		if (midiEvent.value[1] == 0) {
			// Some keyboards send note-off this way
#ifdef LCDDEBUG
			debug("F", midiEvent.value[0], midiEvent.value[1]);
#endif
			this->synth->noteOff(midiEvent.value[0]);
		} else {
#ifdef LCDDEBUG
			debug("O", midiEvent.value[0], midiEvent.value[1]);
#endif
			this->synth->noteOn(midiEvent.value[0], midiEvent.value[1]);
			this->synth->getMatrix()->setSource(MATRIX_SOURCE_KEY, 127-midiEvent.value[0]);
			this->synth->getMatrix()->setSource(MATRIX_SOURCE_VELOCITY, midiEvent.value[1]);
		}
		break;
	case MIDI_CONTROL_CHANGE:
		controlChange(midiEvent);
		break;
	case MIDI_POLY_AFTER_TOUCH:
		// We don't do anything
		// this->synth->getMatrix()->setSource(MATRIX_SOURCE_AFTERTOUCH, midiEvent.value[1]);
		break;
	case MIDI_AFTER_TOUCH:
		this->synth->getMatrix()->setSource(MATRIX_SOURCE_AFTERTOUCH, midiEvent.value[0]);
		break;
	case MIDI_PITCH_BEND:
		this->synth->getMatrix()->setSource(MATRIX_SOURCE_PITCHBEND,
				(int) ((((int) midiEvent.value[1] << 7) + (int) midiEvent.value[0]- 8192) >> 6));
		break;
	case MIDI_PROGRAM_CHANGE:
		// Programm change
		this->synth->allSoundOff();
		PresetUtil::readSynthParamFromEEPROM(this->synthState->fullState.bankNumber, midiEvent.value[0], &this->synthState->params);
		this->synthState->propagateAfterNewParamsLoad();
		this->synthState->resetDisplay();
		break;
	case MIDI_SONG_POSITION:
		this->songPosition = ((int) midiEvent.value[1] << 7) + midiEvent.value[0];
		this->synth->midiClockSetSongPosition(this->songPosition);
		break;
	}
}

void MidiDecoder::controlChange(MidiEvent& midiEvent) {
	int receives = this->synthState->fullState.midiConfigValue[MIDICONFIG_RECEIVES] ;

	// the following one should always been received...
	switch (midiEvent.value[0]) {
		case CC_BANK_SELECT:
			if (midiEvent.value[1] >= 1 and midiEvent.value[1] <= 3) {
				this->synthState->fullState.bankNumber = midiEvent.value[1];
			}
			break;
		case CC_MODWHEEL:
			this->synth->getMatrix()->setSource(MATRIX_SOURCE_MODWHEEL, midiEvent.value[1]);
			break;
		case CC_ALL_NOTES_OFF:
			this->synth->allNoteOff();
			break;
		case CC_ALL_SOUND_OFF:
			this->synth->allSoundOff();
			break;
	}


	if (receives == 1 || receives ==3) {
		switch (midiEvent.value[0]) {
		case CC_VOICE:
			if (midiEvent.value[1] >= 1 and midiEvent.value[1] <= 4) {
				this->synthState->setNewValue(ROW_ENGINE, ENCODER_ENGINE_VOICE,
						midiEvent.value[1]);
			}
			break;
		case CC_GLIDE:
			this->synthState->setNewValue(ROW_ENGINE, ENCODER_ENGINE_GLIDE,
					midiEvent.value[1]);
			break;
		case CC_IM1:
			this->synthState->setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM1,
					midiEvent.value[1] * 2);
			break;
		case CC_IM2:
			this->synthState->setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM2,
					midiEvent.value[1] * 2);
			break;
		case CC_IM3:
			this->synthState->setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM3,
					midiEvent.value[1] * 2);
			break;
		case CC_IM4:
			this->synthState->setNewValue(ROW_MODULATION, ENCODER_ENGINE_IM4,
					midiEvent.value[1] * 2);
			break;
		case CC_MIX1:
			this->synthState->setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX1,
					midiEvent.value[1]);
			break;
		case CC_MIX2:
			this->synthState->setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX2,
					midiEvent.value[1]);
			break;
		case CC_MIX3:
			this->synthState->setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX3,
					midiEvent.value[1]);
			break;
		case CC_MIX4:
			this->synthState->setNewValue(ROW_OSC_MIX, ENCODER_ENGINE_MIX4,
					midiEvent.value[1]);
			break;
		case CC_OSC1_FREQ:
			this->synthState->setNewValue(ROW_OSC1, ENCODER_OSC_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC2_FREQ:
			this->synthState->setNewValue(ROW_OSC2, ENCODER_OSC_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC3_FREQ:
			this->synthState->setNewValue(ROW_OSC3, ENCODER_OSC_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC4_FREQ:
			this->synthState->setNewValue(ROW_OSC4, ENCODER_OSC_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC5_FREQ:
			this->synthState->setNewValue(ROW_OSC5, ENCODER_OSC_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC6_FREQ:
			this->synthState->setNewValue(ROW_OSC6, ENCODER_OSC_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC1_DETUNE:
			this->synthState->setNewValue(ROW_OSC1, ENCODER_OSC_FTUNE,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC2_DETUNE:
			this->synthState->setNewValue(ROW_OSC2, ENCODER_OSC_FTUNE,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC3_DETUNE:
			this->synthState->setNewValue(ROW_OSC3, ENCODER_OSC_FTUNE,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC4_DETUNE:
			this->synthState->setNewValue(ROW_OSC4, ENCODER_OSC_FTUNE,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC5_DETUNE:
			this->synthState->setNewValue(ROW_OSC5, ENCODER_OSC_FTUNE,
					midiEvent.value[1] * 2);
			break;
		case CC_OSC6_DETUNE:
			this->synthState->setNewValue(ROW_OSC6, ENCODER_OSC_FTUNE,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV1_ATTACK:
			this->synthState->setNewValue(ROW_ENV1, ENCODER_ENV_A,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV1_RELEASE:
			this->synthState->setNewValue(ROW_ENV1, ENCODER_ENV_R,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV2_ATTACK:
			this->synthState->setNewValue(ROW_ENV2, ENCODER_ENV_A,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV2_RELEASE:
			this->synthState->setNewValue(ROW_ENV2, ENCODER_ENV_R,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV3_ATTACK:
			this->synthState->setNewValue(ROW_ENV3, ENCODER_ENV_A,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV3_RELEASE:
			this->synthState->setNewValue(ROW_ENV3, ENCODER_ENV_R,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV4_ATTACK:
			this->synthState->setNewValue(ROW_ENV4, ENCODER_ENV_A,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV4_RELEASE:
			this->synthState->setNewValue(ROW_ENV4, ENCODER_ENV_R,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV5_ATTACK:
			this->synthState->setNewValue(ROW_ENV5, ENCODER_ENV_A,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV5_RELEASE:
			this->synthState->setNewValue(ROW_ENV5, ENCODER_ENV_R,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV6_ATTACK:
			this->synthState->setNewValue(ROW_ENV6, ENCODER_ENV_A,
					midiEvent.value[1] * 2);
			break;
		case CC_ENV6_RELEASE:
			this->synthState->setNewValue(ROW_ENV6, ENCODER_ENV_R,
					midiEvent.value[1] * 2);
			break;
		case CC_MATRIXROW1_MUL:
			this->synthState->setNewValue(ROW_MATRIX1, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_MATRIXROW2_MUL:
			this->synthState->setNewValue(ROW_MATRIX2, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_MATRIXROW3_MUL:
			this->synthState->setNewValue(ROW_MATRIX3, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_MATRIXROW4_MUL:
			this->synthState->setNewValue(ROW_MATRIX4, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_MATRIXROW5_MUL:
			this->synthState->setNewValue(ROW_MATRIX5, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_MATRIXROW6_MUL:
			this->synthState->setNewValue(ROW_MATRIX6, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_MATRIXROW7_MUL:
			this->synthState->setNewValue(ROW_MATRIX7, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_MATRIXROW8_MUL:
			this->synthState->setNewValue(ROW_MATRIX8, ENCODER_MATRIX_MUL,
					midiEvent.value[1] * 2 - 128);
			break;
		case CC_LFO1_FREQ:
			this->synthState->setNewValue(ROW_LFO1, ENCODER_LFO_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_LFO2_FREQ:
			this->synthState->setNewValue(ROW_LFO2, ENCODER_LFO_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_LFO3_FREQ:
			this->synthState->setNewValue(ROW_LFO3, ENCODER_LFO_FREQ,
					midiEvent.value[1] * 2);
			break;
		case CC_STEPSEQ5_BPM:
			this->synthState->setNewValue(ROW_LFO5, ENCODER_STEPSEQ_BPM,
					midiEvent.value[1] * 2);
			break;
		case CC_STEOSEQ6_BPM:
			this->synthState->setNewValue(ROW_LFO6, ENCODER_STEPSEQ_BPM,
					midiEvent.value[1] * 2);
			break;
		case CC_STEPSEQ5_GATE:
			this->synthState->setNewValue(ROW_LFO5, ENCODER_STEPSEQ_GATE,
					midiEvent.value[1]);
			break;
		case CC_STEPSEQ6_GATE:
			this->synthState->setNewValue(ROW_LFO6, ENCODER_STEPSEQ_GATE,
					midiEvent.value[1]);
			break;
		case CC_MATRIX_SOURCE_CC1:
			this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC1, midiEvent.value[1]);
			break;
		case CC_MATRIX_SOURCE_CC2:
			this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC2, midiEvent.value[1]);
			break;
		case CC_MATRIX_SOURCE_CC3:
			this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC3, midiEvent.value[1]);
			break;
		case CC_MATRIX_SOURCE_CC4:
			this->synth->getMatrix()->setSource(MATRIX_SOURCE_CC4, midiEvent.value[1]);
			break;
		}
	}

	// Do we accept NRPN in the menu
	if (receives == 2 || receives ==3) {
		switch (midiEvent.value[0]) {
		case 99:
			this->currentNrpn.paramMSB = midiEvent.value[1];
			break;
		case 98:
			this->currentNrpn.paramLSB = midiEvent.value[1];
			break;
		case 6:
			this->currentNrpn.valueMSB = midiEvent.value[1];
			break;
		case 38:
			this->currentNrpn.valueLSB = midiEvent.value[1];
			this->currentNrpn.readyToSend = true;
			break;
		case 96:
			// nrpn increment
			if (this->currentNrpn.valueLSB == 127) {
				this->currentNrpn.valueLSB = 0;
				this->currentNrpn.valueMSB ++;
			} else {
				this->currentNrpn.valueLSB ++;
			}
			this->currentNrpn.readyToSend = true;
			break;
		case 97:
			// nrpn decremenet
			if (this->currentNrpn.valueLSB == 0) {
				this->currentNrpn.valueLSB = 127;
				this->currentNrpn.valueMSB --;
			} else {
				this->currentNrpn.valueLSB --;
			}
			this->currentNrpn.readyToSend = true;
			break;
		default:
			break;
		}

		if (this->currentNrpn.readyToSend) {
			decodeNrpn();
			this->currentNrpn.readyToSend = false;
		}
	}
}

void MidiDecoder::decodeNrpn() {
	if (this->currentNrpn.paramMSB < 2) {
		unsigned int index = (this->currentNrpn.paramMSB << 7) + this->currentNrpn.paramLSB;
		unsigned int value = (this->currentNrpn.valueMSB << 7) + this->currentNrpn.valueLSB;
		unsigned int row = index / NUMBER_OF_ENCODERS;
		unsigned int encoder = index % NUMBER_OF_ENCODERS;

		if (row < NUMBER_OF_ROWS) {
			// 0 of received value must be min value of the param...
			struct ParameterDisplay param =
					allParameterRows.row[row]->params[encoder];
			this->synthState->setNewValue(row, encoder, param.minValue + value);
		} else if (index >= 228 && index<240) {
			this->synthState->params.presetName[index - 228] = (char) value;
			this->synthState->propagateNewPresetName((value == 0));
		}
	} else if (this->currentNrpn.paramMSB < 4)  {
		unsigned int whichStepSeq = this->currentNrpn.paramMSB -2;
		unsigned int step = this->currentNrpn.paramLSB;
		unsigned int value = this->currentNrpn.valueLSB;

		this->synthState->setNewStepValue(whichStepSeq, step, value);
	} else if (this->currentNrpn.paramMSB == 127 && this->currentNrpn.paramLSB == 127)  {
		PresetUtil::sendCurrentPatchAsNrpns();
	}
}

void MidiDecoder::newParamValueFromExternal(SynthParamType type,
		int currentrow, int encoder, ParameterDisplay* param, int oldValue,
		int newValue) {
	// Do nothing here...
}

void MidiDecoder::newParamValue(SynthParamType type, int currentrow,
	int encoder, ParameterDisplay* param, int oldValue, int newValue) {

	int sendCCOrNRPN = this->synthState->fullState.midiConfigValue[MIDICONFIG_SENDS] ;
	int channel = this->synthState->fullState.midiConfigValue[MIDICONFIG_CHANNEL] -1;

	struct MidiEvent cc;
	cc.eventType = MIDI_CONTROL_CHANGE;
	// Si channel = ALL envoie sur 1
	if (channel == -1) {
		channel = 0;
	}
	cc.channel = channel;

	// Do we send NRPN ?
	if (sendCCOrNRPN == 2) {
		// Special case for Step sequencer
		if (currentrow >= ROW_LFO5 && encoder >=2) {
			if (encoder == 2) {
				return;
			}
			if (encoder == 3) {
				// We modify a step value, we want to send that
				int currentStepSeq = currentrow - ROW_LFO5;
				// NRPN is 4 control change
				cc.value[0] = 99;
				cc.value[1] = currentStepSeq + 2;
				midiToSend.insert(cc);
				cc.value[0] = 98;
				cc.value[1] = this->synthState->stepSelect[currentStepSeq];
				midiToSend.insert(cc);
				cc.value[0] = 6;
				cc.value[1] = 0;
				midiToSend.insert(cc);
				cc.value[0] = 38;
				cc.value[1] = (uint8) newValue;
				midiToSend.insert(cc);
			}
		} else {
			// MSB / LSB
			int paramNumber =  currentrow * NUMBER_OF_ENCODERS + encoder;
			// Value to send must be positive
			int valueToSend = newValue - param->minValue;
			// NRPN is 4 control change
			cc.value[0] = 99;
			cc.value[1] = (uint8)(paramNumber >> 7);
			midiToSend.insert(cc);
			cc.value[0] = 98;
			cc.value[1] = (uint8)(paramNumber & 127);
			midiToSend.insert(cc);
			cc.value[0] = 6;
			cc.value[1] = (uint8) (valueToSend >> 7);
			midiToSend.insert(cc);
			cc.value[0] = 38;
			cc.value[1] = (uint8) (valueToSend & 127);
			midiToSend.insert(cc);
		}
	}

	// Do we send control change
	if (sendCCOrNRPN == 1) {

		cc.value[0] = 0;

		switch (currentrow) {
		case ROW_ENGINE:
			if (encoder == ENCODER_ENGINE_VOICE) {
				cc.value[0] = CC_VOICE;
				cc.value[1] = newValue;
			} else if (encoder == ENCODER_ENGINE_GLIDE) {
				cc.value[0] = CC_GLIDE;
				cc.value[1] = newValue;
			}
			break;
		case ROW_MODULATION:
			// We only send even value so that we're not stick we receving the host midi loopback
			if ((newValue & 0x1) >0)
				break;
			cc.value[0] = CC_IM1 + encoder;
			cc.value[1] = newValue >> 1;
			break;
		case ROW_OSC_MIX:
			cc.value[0] = CC_MIX1 + encoder;
			cc.value[1] = newValue;
			break;
		case ROW_OSC_FIRST ... ROW_OSC_LAST:
			if ((newValue & 0x1) >0)
				break;
			if (encoder == ENCODER_OSC_FREQ) {
				cc.value[0] = CC_OSC1_FREQ + (currentrow - ROW_OSC_FIRST);
				cc.value[1] = newValue >> 1;
			} else if (encoder == ENCODER_OSC_FTUNE) {
				cc.value[0] = CC_OSC1_DETUNE + (currentrow - ROW_OSC_FIRST);
				cc.value[1] = newValue >> 1;
			}
			break;
		case ROW_ENV_FIRST ... ROW_ENV_LAST:
			if ((newValue & 0x1) >0)
				break;
			if (encoder == ENCODER_ENV_A) {
				cc.value[0] = CC_ENV1_ATTACK + (currentrow - ROW_ENV_FIRST);
				cc.value[1] = newValue >> 1;
			} else if (encoder == ENCODER_ENV_R) {
				cc.value[0] = CC_ENV1_RELEASE + (currentrow - ROW_ENV_FIRST);
				cc.value[1] = newValue >> 1;
			}
			break;
		case ROW_MATRIX_FIRST ... ROW_MATRIX_LAST:
			if ((newValue & 0x1) >0)
				break;
			if (encoder == ENCODER_MATRIX_MUL) {
				cc.value[0] = CC_MATRIXROW1_MUL + currentrow - ROW_MATRIX_FIRST;
				cc.value[1] = (newValue >> 1) + 64;
			}
			break;
		case ROW_LFO_FIRST ... ROW_LFO3:
			if ((newValue & 0x1) >0)
				break;
			if (encoder == ENCODER_LFO_FREQ) {
				cc.value[0] = CC_LFO1_FREQ + (currentrow - ROW_LFO_FIRST);
				cc.value[1] = (newValue >> 1);
			}
			break;
		case ROW_LFO5 ... ROW_LFO_LAST:
			if (encoder == ENCODER_STEPSEQ_BPM) {
				if ((newValue & 0x1) >0)
					break;
				cc.value[0] = CC_STEPSEQ5_BPM + (currentrow - ROW_LFO5);
				cc.value[1] = (newValue >> 1);
			}
			else if (encoder == ENCODER_STEPSEQ_GATE) {
				cc.value[0] = CC_STEPSEQ5_GATE + (currentrow - ROW_LFO5);
				cc.value[1] = newValue;
			}
			break;
		}

		if (cc.value[0] != 0) {
			midiToSend.insert(cc);
		}
	}
}

void MidiDecoder::sendMidiOut() {
	// 1.52 : from now only MIDI_CONTRIL_CHANGE should be sent...
	// Other events are directly copied when recevied...
	// Check before calling that there is something to send...

	MidiEvent toSend = midiToSend.remove();

	switch (toSend.eventType) {
	case MIDI_NOTE_OFF:
	case MIDI_NOTE_ON:
	case MIDI_CONTROL_CHANGE:
	case MIDI_PITCH_BEND:
		Serial3.print((unsigned char) (toSend.eventType + toSend.channel));
		Serial3.print((unsigned char) toSend.value[0]);
		Serial3.print((unsigned char) toSend.value[1]);
		break;
	case MIDI_AFTER_TOUCH:
	case MIDI_PROGRAM_CHANGE:
		Serial3.print((unsigned char) (toSend.eventType + toSend.channel));
		Serial3.print((unsigned char) toSend.value[0]);
		break;
	}
}

