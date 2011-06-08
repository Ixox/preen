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

#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <math.h>
#include "Synth.h"
#include "RingBuffer.h"
#include "MidiDecoder.h"
#include "Osc.h"

extern int frequencies[];

Synth synth;
MidiDecoder midiDecoder;
RingBuffer<short, 64> rb;
int audioGlitches=1;
short soundData[300000];
int soundDataIndex = 0;

void SendSamples() {
	while (rb.getCount()>0) {
		soundData[soundDataIndex] = rb.remove();
//		printf("remove  %i : %i\n", soundDataIndex, soundData[soundDataIndex] );
		soundDataIndex++;
	}
}

void setup()
{
	midiDecoder.setSynth(&synth);
	synth.noteOn(40,127);
}

int mainCpt = 0;
bool finishedShown = false;
int toggle = 0;


void loop() {
	int size = rand() % 60;
	int cpt = 0;
	while (cpt++<size && synth.isPlaying()) {

		synth.nextSample();
		int sample = synth.getSample();
/*
		if (sample>32768 || sample<-32768) {
			printf("Aie... %i : %i\n", mainCpt, sample);
		}
		*/
		//rb.insert((short)sample);
		 soundData[mainCpt] = (short)sample;
		mainCpt++;
		if (mainCpt==50000) {
			synth.noteOn(60,100);
		}
		if (mainCpt==100000) {
			synth.noteOn(80,70);
		}
		if (mainCpt==150000) {
			printf("XH: note OFF !!\n");
			synth.allNoteOff();
		}
	}
	if (!synth.isPlaying()) {
		printf("Stop...\n");
	}
}


int main(void)
{

    setup();
    while (synth.isPlaying()) {
    	loop();
    	//SendSamples();
    }



    printf("soundDataIndex : %i \n", soundDataIndex);
    printf("mainCpt : %i \n", mainCpt);
    SF_INFO sfinfo;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
    sfinfo.frames = mainCpt;
    sfinfo.samplerate = SAMPLE_RATE_x_8 / 8;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 ;


    SNDFILE* file = sf_open("samples.wav", SFM_WRITE, &sfinfo);
    sf_write_short(file, soundData, mainCpt);
/*
    int test1 = 32768;
    int test2 = -32768;
    int test3 = -7;
    printf("test1 : %i\n", test1>>8);
    printf("test2 : %i\n", test2>>8);
    printf("test3 : %i\n", (test3 & 0xffff));


    double step = 2*M_PI / 2048;
    int cpt = 0;
    for (double i= 0; i<2*M_PI; i+= step) {
    	printf("%i, ", (short)(sin(i)*32767));
    	cpt++;
    	if ((cpt%10) == 0) {
    		printf("\n");
    	}

    }

	printf("\n");
    printf("cpt : %i\n", cpt);

    unsigned int i1 = 65535;
    int i2 = 32767;
    int i3 = i1*i2;
    printf("Mul 1 : %i\n", i3);
    i2 = -32767;
    i3 = i1*i2;

    printf("Mul 2 : %i\n", i3);
*/
    return 0;
}




