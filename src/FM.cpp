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

#include "wirish.h"
#include "libmaple.h"

#include "FMDisplay.h"
#include "Synth.h"
#include "RingBuffer.h"
#include "MidiDecoder.h"
#include "Encoders.h"


// Main timer define
#define TIME_NUMBER       2
#define CHANNEL_PWM       TIMER_CH1
#define CHANNEL_INTERUPT  TIMER_CH2
#define AUDIO_PIN    11

/*
#define TIME_NUMBER       1
#define CHANNEL_PWM       TIMER_CH3
#define CHANNEL_INTERUPT  TIMER_CH2
#define AUDIO_PIN    25
*/

SynthState		   synthState;
Synth              synth;
MidiDecoder        midiDecoder;
Encoders		   encoders;
RingBuffer<uint16, 50> rb;
FMDisplay          fmDisplay;

#ifdef PCB_R1
LiquidCrystal      lcd(2, 8, 3, 4, 5, 6, 7, 27, 26, 25, 22);
#endif

 #ifdef PCB_R2
LiquidCrystal      lcd(2, 8, 3, 4, 5, 6, 7, 31, 30, 29, 28);
#endif

#ifdef PCB_R3
LiquidCrystal      lcd(31, 30, 29, 28, 2, 3, 4, 5, 6, 7);
#endif

HardwareTimer mainTimer(TIME_NUMBER);

int mainCpt = 0;

void IRQSendSample() {
    if (rb.getCount()>0) {
        pwmWrite(AUDIO_PIN , rb.remove());
    }
}


inline void fillSoundBuffer() {
    for (int cpt=0; cpt<12 && !rb.isFull(); cpt++) {
        synth.nextSample();
        rb.insert((uint16)(synth.getSample()>>5)+1024);
    }
}

inline void fillSoundBufferFull() {
    for (int cpt=0; cpt<1000 && !rb.isFull(); cpt++) {
        synth.nextSample();
        rb.insert((uint16)(synth.getSample()>>5)+1024);
    }
}


void setup()
{
	SerialUSB.end();
	nvic_irq_disable(NVIC_USB_LP_CAN_RX0);

    byte midiIn[8] = {
            B01100,
            B10010,
            B10010,
            B01100,
            B00000,
            B00000,
            B00000,
            B00000
    };

    byte midiOut[8] = {
            B01100,
            B11110,
            B11110,
            B01100,
            B00000,
            B00000,
            B00000,
            B00000
    };

    byte modified[8] = {
            B00101,
            B00010,
            B00101,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000
    };

    byte stepCursor[8] = {
            B00000,
            B00000,
            B00000,
            B00000,
            B10001,
            B01110,
            B00100,
            B00100,
    };

    byte stepPos[8] = {
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00100,
            B00100,
            B00000,
    };

    byte firstSteps[8] = {
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B10000,
            B10000,
    };


    byte thirdStep[8] = {
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B10100,
            B10100,
    };

    Serial1.end();
    Serial2.end();
    Serial3.begin(31250);

    lcd.begin(20, 4);
    lcd.setCursor(0,0);
    lcd.print("        PreenFM "PREENFM_VERSION);
    lcd.setCursor(0,1);
    lcd.print("     By Xavier Hosxe");
    lcd.setCursor(0,3);
    lcd.print(" Powered by LeafLabs");

    // ---------------------------------------
    // Dependencies Injection

    // to SynthStateAware Class
    // MidiDecoder, Synth (Env,Osc, Lfo, Matrix, Voice ), FMDisplay, PresetUtil...

    synth.setSynthState(&synthState);
    midiDecoder.setSynthState(&synthState);
    fmDisplay.setSynthState(&synthState);
    PresetUtil::setSynthState(&synthState);

    midiDecoder.setSynth(&synth);

    // ---------------------------------------
    // Register listener

    // synthstate is updated by encoder change
    encoders.insertListener(&synthState);

    // fmDisplay and synth needs to be aware of synthState changes
    synthState.insertParamListener(&fmDisplay);
    synthState.insertParamListener(&synth);
    synthState.insertParamListener(&midiDecoder);
    synthState.insertMenuListener(&fmDisplay);

    // Timer init
    mainTimer.pause();
    mainTimer.setOverflow(2197);
    mainTimer.setPrescaleFactor(1);

    // PWM for the SOUND !
    mainTimer.setMode(CHANNEL_PWM, TIMER_PWM);
    pinMode(AUDIO_PIN, PWM);

    //
    mainTimer.refresh();
    mainTimer.resume();

    lcd.createChar(0, midiIn);
    lcd.createChar(1, midiOut);
    lcd.createChar(2, modified);
    lcd.createChar(3, stepCursor);
    lcd.createChar(4, stepPos);
    lcd.createChar(5, firstSteps);
    lcd.createChar(6, thirdStep);


    // Load config from EEPROM if has been saved...
    PresetUtil::loadConfigFromEEPROM();

    // init the audio interupt
    mainTimer.setCompare(CHANNEL_INTERUPT, 2048);
    mainTimer.setMode(CHANNEL_INTERUPT, TIMER_OUTPUTCOMPARE);
    mainTimer.attachInterrupt(CHANNEL_INTERUPT, IRQSendSample);

    pwmWrite(AUDIO_PIN , 1024);
    delay(500);


    fillSoundBuffer();
    for (int k=0; k<13; k++) {
        synth.noteOn(60+k, 60);
        for (int cpt=0; cpt<1500; cpt++) {
            fillSoundBuffer();
            delayMicroseconds(30);
        }
        synth.noteOff(60+k);
        for (int cpt=0; cpt<100; cpt++) {
            fillSoundBuffer();
            delayMicroseconds(30);
        }
    }

    for (int cpt=0; cpt<10000; cpt++) {
        fillSoundBuffer();
        delayMicroseconds(30);
    }

    // Load default patch
    synthState.propagateBeforeNewParamsLoad();
    PresetUtil::loadDefaultPatchIfAny();
    synthState.propagateAfterNewParamsLoad();

    fillSoundBuffer();
    fmDisplay.init(&lcd);

}

unsigned short midiReceive = 0;
unsigned short midiSent = 0;
uint32 encoderMicros = 0;
uint32 midiInMicros = 0;
uint32 midiOutMicros = 0;


void loop() {
    uint32 newMicros = micros();

    mainCpt++;
    fillSoundBufferFull();

    if ((newMicros - midiInMicros) > 200) {
        while (Serial3.available()) {
            midiDecoder.newByte(Serial3.read());
            if (midiReceive == 0 && synthState.fullState.synthMode == SYNTH_MODE_EDIT) {
                fillSoundBuffer();
                midiReceive = 2500;
                lcd.setCursor(0,0);
                lcd.print((char)0);
            }
        }

        if (midiReceive>0) {
            if (midiReceive == 1 && synthState.fullState.synthMode == SYNTH_MODE_EDIT) {
                fillSoundBuffer();
                lcd.setCursor(0,0);
                lcd.print(' ');
            }
            midiReceive--;
        }

        midiInMicros = newMicros;
    }

    if ((newMicros - midiOutMicros) > 240) {
        if (midiDecoder.hasMidiToSend()) {

            fillSoundBufferFull();
            midiDecoder.sendMidiOut();

            if (midiSent == 0 && synthState.fullState.synthMode == SYNTH_MODE_EDIT) {
                fillSoundBuffer();
                midiSent = 2500;
                lcd.setCursor(1,0);
                lcd.print((char)1);
            }
        }

        if (midiSent>0) {
            if (midiSent == 1  && synthState.fullState.synthMode == SYNTH_MODE_EDIT) {
                fillSoundBuffer();
                lcd.setCursor(1,0);
                lcd.print(' ');
            }
            midiSent--;
        }

        midiOutMicros = newMicros;
    }

    if (fmDisplay.needRefresh() && ((mainCpt & 0x7) == 0)) {
        fillSoundBufferFull();
        fmDisplay.refreshAllScreenByStep();
    }

    if ((newMicros - encoderMicros) > 2500) {
    	fillSoundBufferFull();
        encoders.checkStatus();
        encoderMicros = newMicros;
    }
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated object that need libmaple may fail.
__attribute__(( constructor )) void premain() {
    init();
}

int main(void)
{
    setup();

    while (1) {
        loop();
    }
    return 0;
}
