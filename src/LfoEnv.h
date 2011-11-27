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

#ifndef LFOENV_H_
#define LFOENV_H_

#include "Lfo.h"
#include "Env.h"

// Cannot use EnvData because currentAmp can be negative during one sample... which caused an audio noise !
// Don't want to add a test that will slow down the algo, so i create a new struc.
struct LfoEnvData {
    // life cycle of the env
    unsigned int index;
    // Current sample
    int currentAmp;
    // State of the env
    unsigned int envState;
    // Release speed (dependent on the voice when RELEASE)
    int currentAmpSpeed;
};



class LfoEnv: public Lfo {
public:

	void init(int number, Matrix* matrix, SourceEnum source, DestinationEnum dest);

	void valueChanged(int encoder) {
        // for 0, 1, 3 -> 255 must be 2 seconds so 2048
        // / 32 = >>5
        adsr[0] = ((this->envParams->attack * this->envParams->attack) >> 5) +1;
        adsr[1] = ((this->envParams->decay * this->envParams->decay) >> 5)+1;
        adsr[2] = this->envParams->sustain;
        adsr[3] = ((this->envParams->release * this->envParams->release) >> 5)+1;

        // Should go up to 127 in adsr[0] calls
        // Step = 127 / adsr[0]
        // << 16 for fixed point
        incA =  (127 << 16) / adsr[0] ;
        // Should go down from 127 to adsr[2] in adsr [1] calls
        // adsr[2] is twice too big which is why we << 15
        incD = ((255 - adsr[2]) << 15) /  adsr[1];
	}

	void nextValueInMatrix() {
		envData.index --;

		switch (envData.envState) {
		case ENV_STATE_ON_A:
			this->envData.currentAmp += incA;
			if (envData.index == 0) {
				envData.envState = ENV_STATE_ON_D;
				envData.index = adsr[1];
			}
			break;
		case ENV_STATE_ON_D:
			this->envData.currentAmp -= incD;
			if (envData.index == 0) {
				envData.envState = ENV_STATE_ON_S;
			}
			break;
		case ENV_STATE_ON_S:
			this->envData.currentAmp = adsr[2] << 15;
			break;
		case ENV_STATE_ON_R:
			this->envData.currentAmp -= this->envData.currentAmpSpeed;
			if (envData.index == 0) {
				envData.envState = ENV_STATE_DEAD;
			}
			break;
		case ENV_STATE_DEAD:
			this->envData.currentAmp = 0;
			break;
		}

		matrix->setSource(source, this->envData.currentAmp >> 16);
	}

	void noteOn() {
		this->envData.currentAmp= 0;
		this->envData.index = adsr[0];
        this->envData.envState = ENV_STATE_ON_A;
	}

	void noteOff() {
		if (this->envParams->release <255) {
			this->envData.index = adsr[3] ;
			this->envData.envState = ENV_STATE_ON_R;
			this->envData.currentAmpSpeed =  this->envData.currentAmp  / adsr[3] ;
		}
	}


private:
	EnvelopeParams* envParams;
	LfoEnvData envData;
	// ADSR
	unsigned int adsr[4];
	// Ramp speed Of attack and Decay
    unsigned int incA, incD;
};

#endif /* LFOENV_H_ */
