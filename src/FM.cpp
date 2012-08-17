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

#include <stdlib.h>
#include "wirish.h"
#include "libmaple.h"

#include "FMDisplay.h"
#include "Synth.h"
#include "RingBuffer.h"
#include "MidiDecoder.h"
#include "Encoders.h"
#include "Menu.h"


// Main timer define
#define TIME_NUMBER       2
#define CHANNEL_PWM_MSB   TIMER_CH1
#define CHANNEL_PWM_LSB   TIMER_CH2
#define CHANNEL_INTERUPT  TIMER_CH3
#define AUDIO_PIN_MSB    11
#define AUDIO_PIN_LSB    10

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
RingBuffer<int, 96> rb;
FMDisplay          fmDisplay;

#ifdef PCB_R1
LiquidCrystal      lcd(2, 3, 4, 5, 6, 7, 27, 26, 25, 22);
#define TIMER_OVERFLOW 2197
#define TIMER_IRQ_COMPARE 2048
#define TIMER_PRESCALE 1
#endif

#ifdef PCB_R2
LiquidCrystal      lcd(2, 3, 4, 5, 6, 7, 31, 30, 29, 28);
#define TIMER_OVERFLOW 2197
#define TIMER_PRESCALE 1
#define TIMER_IRQ_COMPARE 2048
#endif

#ifdef PCB_R3
LiquidCrystal      lcd(31, 30, 29, 28, 2, 3, 4, 5, 6, 7);
#define TIMER_OVERFLOW 2197
#define TIMER_IRQ_COMPARE 2048
#define TIMER_PRESCALE 1
#endif

#ifdef PCB_R4
LiquidCrystal      lcd(31, 30, 29, 28, 2, 3, 4, 5, 6, 7);
#define TIMER_OVERFLOW 274
#define TIMER_IRQ_COMPARE 240
#define TIMER_PRESCALE 8
#endif


HardwareTimer mainTimer(TIME_NUMBER);

int mainCpt = 0;

void IRQSendSample() {
    if (rb.getCount()>0) {
#ifndef PCB_R4
        pwmWrite(AUDIO_PIN_MSB , (rb.remove() >>5)+1024);
#else
        int sample = (rb.remove() + 32768);
        pwmWrite(AUDIO_PIN_LSB, sample & 0xff);
        pwmWrite(AUDIO_PIN_MSB, sample >> 8);
#endif
    }
}

inline void fillSoundBuffer() {
    if (rb.getCount() < 64) {
        synth.nextSampleBlock();
        rb.appendBlock(synth.getSampleBlock(), 32);
    }
}


void setup()
{
#ifndef DEBUG
    SerialUSB.end();
    nvic_irq_disable(NVIC_USB_LP_CAN_RX0);
#endif

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

    byte note[8] = {
            B00100,
            B00110,
            B00101,
            B00101,
            B00100,
            B11100,
            B11100,
            B00000,
    };

    Serial1.end();
    Serial2.end();
    Serial3.begin(31250);

    lcd.begin(20, 4);
    lcd.setCursor(12-FMDisplay::getLength(PREENFM_VERSION),0);
    lcd.print("PreenFM "PREENFM_VERSION);
    lcd.setCursor(0,1);
    lcd.print("     By Xavier Hosxe");
    lcd.setCursor(0,3);
    lcd.print(" Powered by LeafLabs");

    // ---------------------------------------
    // Dependencies Injection

    // to SynthStateAware Class
    // MidiDecoder, Synth (Env,Osc, Lfo, Matrix, Voice ), FMDisplay, PresetUtil...

    synth.setSynthState(&synthState);
    fmDisplay.setSynthState(&synthState);
    midiDecoder.setSynthState(&synthState);
    midiDecoder.setVisualInfo(&fmDisplay);
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
    mainTimer.setOverflow(TIMER_OVERFLOW);
    mainTimer.setPrescaleFactor(TIMER_PRESCALE);

    // PWM for the SOUND !
#ifdef PCB_R4
    mainTimer.setMode(CHANNEL_PWM_LSB, TIMER_PWM);
    pinMode(AUDIO_PIN_LSB, PWM);
#endif

    mainTimer.setMode(CHANNEL_PWM_MSB, TIMER_PWM);
    pinMode(AUDIO_PIN_MSB, PWM);

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
    lcd.createChar(7, note);


    // Load config from EEPROM if has been saved...
    PresetUtil::loadConfigFromEEPROM();

    // init the audio interupt
    mainTimer.setCompare(CHANNEL_INTERUPT, TIMER_IRQ_COMPARE);
    mainTimer.setMode(CHANNEL_INTERUPT, TIMER_OUTPUTCOMPARE);
    mainTimer.attachInterrupt(CHANNEL_INTERUPT, IRQSendSample);

#ifndef PCB_R4
    pwmWrite(AUDIO_PIN_MSB , 1024);
#else
    pwmWrite(AUDIO_PIN_LSB , 0);
    pwmWrite(AUDIO_PIN_MSB , 64);
#endif
    delay(500);


    fillSoundBuffer();
    synth.noteOn(48, 60);
    fillSoundBuffer();
    int notes[] = { 60, 64, 60, 67, 60, 72};
    for (int k=0; k<6; k++) {
        synth.noteOn(notes[k], 60);
        for (int cpt=0; cpt<1500; cpt++) {
            fillSoundBuffer();
            delayMicroseconds(35 - k*2);
        }
        synth.noteOff(notes[k]);
        for (int cpt=0; cpt<100; cpt++) {
            fillSoundBuffer();
            delayMicroseconds(35 - k*2);
        }
    }
    synth.noteOff(48);
    for (int cpt=0; cpt<7000; cpt++) {
        delayMicroseconds(30);
        fillSoundBuffer();
    }

    // Load default patch
    synthState.propagateBeforeNewParamsLoad();
    PresetUtil::loadDefaultPatchIfAny();
    synthState.propagateAfterNewParamsLoad();

    fmDisplay.init(&lcd);



    int bootOption = synthState.fullState.midiConfigValue[MIDICONFIG_BOOT_START];

    if (bootOption == 0) {
        fmDisplay.displayPreset();
        fmDisplay.setRefreshStatus(12);
    } else {
        // Menu
        synthState.buttonPressed(BUTTON_MENUSELECT);
        // Load
        synthState.buttonPressed(BUTTON_MENUSELECT);
        if (bootOption == 5) {
            // Internal bank
            synthState.encoderTurned(0, 1);
            synthState.buttonPressed(BUTTON_MENUSELECT);
        } else {
            // User
            synthState.buttonPressed(BUTTON_MENUSELECT);
            // Bank
            for (int k = 0; k < bootOption - 1; k++) {
                synthState.encoderTurned(0, 1);
            }
            synthState.buttonPressed(BUTTON_MENUSELECT);
        }
    }
    srand(micros());
}

unsigned short midiReceive = 0;
unsigned short midiSent = 0;
uint32 encoderMicros = 0;
uint32 midiInMicros = 0;
uint32 midiOutMicros = 0;
uint32 synthStateMicros = 0;
uint32 externGearMidiMicros = 0;
int ECCnumber = 0;



void loop() {
    uint32 newMicros = micros();

    mainCpt++;

    fillSoundBuffer();

    while (Serial3.available()) {
        midiDecoder.newByte(Serial3.read());
        if (midiReceive == 0) {
            fillSoundBuffer();
            fmDisplay.midiIn(true);
        }
        if (synthState.fullState.synthMode == SYNTH_MODE_MENU) {
            midiReceive = 0;
        } else {
            midiReceive = 2500;
        }
    }

    if ((newMicros - midiInMicros) > 200) {
        if (midiReceive>0) {
            if (midiReceive == 1) {
                fillSoundBuffer();
                fmDisplay.midiIn(false);
            }
            midiReceive--;
        }

        midiInMicros = newMicros;
    }

    if ((newMicros - midiOutMicros) > 240) {
        if (midiDecoder.hasMidiToSend()) {

            while (midiDecoder.hasMidiToSend()) {
                fillSoundBuffer();
                midiDecoder.sendMidiOut();
            }

            if (midiSent == 0) {
                fillSoundBuffer();
                fmDisplay.midiOut(true);
            }
            if (synthState.fullState.synthMode == SYNTH_MODE_MENU) {
                midiSent = 0;
            } else {
                midiSent = 2500;
            }
        }

        if (midiSent>0) {
            if (midiSent == 1) {
                fillSoundBuffer();
                fmDisplay.midiOut(false);
            }
            midiSent--;
        }

        midiOutMicros = newMicros;
    }

    if ((newMicros - externGearMidiMicros) > 2500) {
        fillSoundBuffer();
        midiDecoder.sendToExternalGear(ECCnumber);
        externGearMidiMicros = newMicros;
        ECCnumber++;
        ECCnumber &= 0x3;
    }

    if (fmDisplay.needRefresh() && ((mainCpt & 0x7) == 0)) {
        fillSoundBuffer();
        fmDisplay.refreshAllScreenByStep();
    }

    if ((newMicros - encoderMicros) > 1500) {
        fillSoundBuffer();
        encoders.checkStatus();
        encoderMicros = newMicros;
    }

    if ((newMicros - synthStateMicros) > 200000) {
        fillSoundBuffer();
        synthState.tempoClick();
        synthStateMicros = newMicros;
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
