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

#define AUDIO_PIN    7

Synth              synth;
MidiDecoder        midiDecoder;
Encoders		   encoders;
RingBuffer<uint16, 64> rb;
FMDisplay          fmDisplay;
LiquidCrystal      lcd(12, 11, 5, 4, 3, 2);

volatile int audioGlitches = 0;
int          ag            = 0;
bool         finish        = false;
bool         finishedShown = false;


void IRQSendSample() {
	if (rb.getCount()==0) {
		pwmWrite(AUDIO_PIN , rb.remove());
	} else {
		audioGlitches++;
		synth.nextSample();
		pwmWrite(AUDIO_PIN , (uint16)(synth.getSample()>>5)+1024);
	}
}


void setup() 
{
	timer_disable_all();

	lcd.begin(20, 2);
    lcd.print("IxOx FM Synth V0.1");

 	encoders.setSynth(&synth);
	midiDecoder.setSynth(&synth);
	int cpt= 0;
	while (cpt<20) {
		synth.nextSample();
		rb.insert((uint16)(synth.getSample()>>5)+1024);
		cpt++;
	}

	Serial3.begin(31250);

	Timer1.setOverflow(2197);
	Timer1.setPrescaleFactor(1);
	Timer1.setCompare1(2100);
	Timer1.setChannel1Mode(TIMER_OUTPUTCOMPARE);
	Timer1.attachCompare1Interrupt(IRQSendSample);

	Timer1.setChannel2Mode(TIMER_PWM);
	pinMode(AUDIO_PIN, PWM);

	audioGlitches = 0;
	Timer1.resume();
	delay(1000);

	fmDisplay.init(&encoders, &lcd);
}

int lcdMod = 0;

void loop() {
	int samples = 0;
	while (!rb.isFull() && (samples++<50)) {
		synth.nextSample();
		rb.insert((uint16)(synth.getSample()>>5)+1024);
	}


	unsigned int numberOfEvents = Serial3.available();
	while (numberOfEvents>0) {
		midiDecoder.newByte(Serial3.read());
		numberOfEvents--;
	}

	encoders.checkStatus();

	lcdMod++;
	if (fmDisplay.needRefresh() && ((lcdMod & 0xf) == 0)) {
		while (!rb.isFull()) {
			synth.nextSample();
			rb.insert((uint16)(synth.getSample()>>5)+1024);
		}
		fmDisplay.refreshAllScreenByStep();
	}

	if ((encoders.rowChanged() || encoders.valueHasChanged()>=0) &&((lcdMod & 0x7f) == 0)) {
		while (!rb.isFull()) {
			synth.nextSample();
			rb.insert((uint16)(synth.getSample()>>5)+1024);
		}
		fmDisplay.update();
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




