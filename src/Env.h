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

#pragma once

#include "SynthState.h"


enum EnvState {
	ENV_STATE_ON_A = 0,
	ENV_STATE_ON_D,
	ENV_STATE_ON_S,
	ENV_STATE_ON_R,
	ENV_STATE_QUICK_OFF,
	ENV_STATE_DEAD
};

struct EnvData {
	// life cycle of the env
	unsigned int index;
	// Higher sample before release
	int releaseSample;
	// State of the env
	EnvState envState;
};


template <int number>
class Env
{
public:
	Env() {
	}
	~Env(void) {
	}

	void loadADSR() {
		EnvelopeParams * e = (EnvelopeParams *)&(synthState.params.env1);
		EnvelopeParams* envState = &e[number-1];

		attack = envState->attack * envState->attack;
		decay = envState->decay * envState->decay;
		sustain = envState->sustain << 7;
		release = envState->release * envState->release;
	}

	int getAmp(struct EnvData& env) {
	    int rAmp = 0;
		switch (env.envState) {
		case ENV_STATE_DEAD:
			return 0;
			break;
		case ENV_STATE_QUICK_OFF:
			if (env.index < 32) {
				return env.releaseSample - ((env.index * env.releaseSample) >> 5);
			} else {
				env.envState = ENV_STATE_DEAD;
				return 0;
			}
			break;
		case ENV_STATE_ON_A:
			if (env.index<attack) {
				return (env.index << 15) / attack;
			}
			env.index = 0;
			env.envState = ENV_STATE_ON_D;
			// No break go to next state
		case ENV_STATE_ON_D:
			if (env.index <  decay) {
				return (((decay - env.index) << 15) +  env.index * sustain ) / decay;
			}
			// No break go to next state
			env.envState = ENV_STATE_ON_S;
		case ENV_STATE_ON_S:
			return sustain;
		case ENV_STATE_ON_R:
/*
            int tmp;
		    asm volatile("  cmp %4, %3\n\t"
		        "   beq 1\n\t"
		        "   mov %4, %2\n\t"
		        "   mul %4, %4, %2\n\t"
		        "   sdiv %4, %4, %3\n\t"
                "   mov %0, %2\n\t"
		        "   sub %0, %0, %4\n\t"
		        "   b 2\n\t"
		        "1:\n\t"
		        "   mov %1, #4\n\t"
		        "2:\n\t"
                "nop\n\t"
		            : "=r" (rAmp), "=r "(env.envState)
		            : "r" (&env), "r" (release), "r" (tmp));
*/


            if (env.index< release) {
				return env.releaseSample - env.index * env.releaseSample / release;
			}
			env.envState = ENV_STATE_DEAD;
		    break;
		}
//		asm volatile("mov %0, %0, ror #1" : "=r" (rAmp) );
		return rAmp;
	}

	void noteOn(struct EnvData& env) {
		env.envState = ENV_STATE_ON_A;
		env.index = 0;
	}

	void noteOffQuick(struct EnvData& env) {
		env.releaseSample = this->getAmp(env);
		env.envState = ENV_STATE_QUICK_OFF;
		env.index = 0;
	}


	void noteOff(struct EnvData& env) {
		env.releaseSample = this->getAmp(env);
		env.envState = ENV_STATE_ON_R;
		env.index = 0;
	}

	static void nextSample(struct EnvData& env) {
		env.index++;
	}
	static bool isDead(struct EnvData& env) {
		return env.envState == ENV_STATE_DEAD;
	}

private:
	// ADSR
	unsigned int attack;
	unsigned int decay;
	unsigned int sustain;
	unsigned int release;
};

