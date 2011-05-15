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

#define AUDIO_PIN    8

SynthState		   synthState;
Synth              synth;
MidiDecoder        midiDecoder;
Encoders		   encoders;
//RingBuffer<uint16, 32> rb;
FMDisplay          fmDisplay;
LiquidCrystal      lcd(23,24, 25,26,27,28,29,30,31,32);

int mainCpt = 0;


uint16 currentSample = 1024;
/*
int max = 1024;
int min = 1024;
 */

//int glitches = 0;

int cpt;
void IRQSendSample() {
    /*
    if (rb.getCount()>0) {
        pwmWrite(AUDIO_PIN , rb.remove());
    } else {
        glitches++;
    }
     */

    pwmWrite(AUDIO_PIN , currentSample);
    synth.nextSample();
    currentSample = (uint16)(synth.getSample()>>5) + 1024;


    /*
	if (currentSample > max) {
        max = currentSample ;
    }
    if (currentSample < min) {
        min = currentSample ;
    }
     */

}

void IRQEncoders() {
    // 16*25 micros of dealy to remove hissing noise (????).....
    encoders.checkStatus();
}

unsigned int time = 0;
unsigned int previousTime = 0;
unsigned int fullDelay;

void setup()
{
    //	timer_disable_all();
    byte midiIn[8] = {
            B01100,
            B10010,
            B10010,
            B01100,
            B00000,
            B00000,
            B00000,
    };

    byte midiOut[8] = {
            B01100,
            B11110,
            B11110,
            B01100,
            B00000,
            B00000,
            B00000,
    };

    byte modified[8] = {
            B00101,
            B00010,
            B00101,
            B00000,
            B00000,
            B00000,
            B00000,
    };

    lcd.begin(20, 4);
    lcd.setCursor(0,0);
    lcd.print("PreenFM V0.1");
    lcd.setCursor(0,1);
    lcd.print("          By Ixox");
    lcd.setCursor(0,3);
    lcd.print("Powered by Leaflabs");

    lcd.createChar(0, midiIn);
    lcd.createChar(1, midiOut);
    lcd.createChar(2, modified);



    // Dependencies injection

    // synthstate is updated by encoder change
    encoders.insertListener(&synthState);

    // fmDisplay and synth needs to be aware of synthState changes
    synthState.insertParamListener(&fmDisplay);
    synthState.insertParamListener(&synth);
    synthState.insertParamListener(&midiDecoder);
    synthState.insertMenuListener(&fmDisplay);

    midiDecoder.setSynth(&synth);
    /*
	int cpt= 0;
	while (cpt<20) {
		synth.nextSample();
		rb.insert((uint16)(synth.getSample()>>5)+1024);
		cpt++;
	}
     */

    Serial2.begin(31250);

    Timer1.setOverflow(2197);
    Timer1.setPrescaleFactor(1);
    pwmWrite(AUDIO_PIN , 0);


    Timer1.setChannel3Mode(TIMER_PWM);
    pwmWrite(AUDIO_PIN , 0);
    pinMode(AUDIO_PIN, PWM);

    // At 2048 it should be ok to set the new one.
    Timer1.setCompare1(1);
    Timer1.setChannel1Mode(TIMER_OUTPUTCOMPARE);
    Timer1.attachCompare1Interrupt(IRQSendSample);

    for (int k=0; k<13; k++) {
        synth.noteOn(60+k,100);
        delay(50);
        synth.noteOff(60+k);
        delay(5);
    }

    fmDisplay.init(&lcd);

    /*
    Timer2.setOverflow(1000);
    Timer2.setPrescaleFactor(16);
    Timer2.setCompare1(500);
    Timer2.setChannel1Mode(TIMER_OUTPUTCOMPARE);
    Timer2.attachCompare1Interrupt(IRQEncoders);
     */
}


unsigned short midiReceive = 0;
unsigned short midiSent = 0;

void loop() {

    mainCpt++;

    while (Serial2.available()) {
        midiDecoder.newByte(Serial2.read());

        if (midiReceive == 0 && synthState.fullState.synthMode == SYNTH_MODE_EDIT) {
            midiReceive = 300;
            lcd.setCursor(0,0);
            lcd.print((char)0);
        }

    }

    if (midiDecoder.hasMidiToSend()) {
        if (midiSent == 0 && synthState.fullState.synthMode == SYNTH_MODE_EDIT) {
            midiSent = 300;
            lcd.setCursor(1,0);
            lcd.print((char)1);
        }
    }

    if ((mainCpt&0xf) == 1) {
        midiDecoder.sendOneMidiEvent();

    }

    if (midiSent>0) {
        if (midiSent == 1) {
            lcd.setCursor(1,0);
            lcd.print(' ');
        }
        midiSent--;
    }

    if (midiReceive>0) {
        if (midiReceive == 1) {
            lcd.setCursor(0,0);
            lcd.print(' ');
        }
        midiReceive--;
    }

    if (fmDisplay.needRefresh() && ((mainCpt & 0x3) == 0)) {
        fmDisplay.refreshAllScreenByStep();
    }


    encoders.checkStatus();


    /*
	if ((mainCpt & 0xff) == 0) {
        lcd.setCursor(0,0);
        lcd.print(max);
        lcd.setCursor(6,0);
        lcd.print(min);
        lcd.print("  ");
    }
     */
    //    delayMicroseconds(300);
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




