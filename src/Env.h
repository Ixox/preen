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

#include "SynthStatus.h"

struct EnvState {
	// life cycle of the env
	int index;
	// Higher sample before release
	int releaseSample;
	// Is the note still playing
	bool noteon;
	// Is the env available.. the release is finished
	bool dead;
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
		Envelope * e = (Envelope *)(&(currentSynthState->env1));
		Envelope* envState = &e[number-1];

		attack = envState->attack * envState->attack;
		decay = envState->decay * envState->decay;
		sustain = envState->sustain << 8;
		release = envState->release * envState->release;
	}

	int getAmp(struct EnvState& env) {
		if (env.dead) {
			return 0;
		}
		if (env.noteon) {
			if (env.index<attack) {
				return (env.index << 15) / attack;
			} else if (env.index < (attack+decay)) {
				int index2 = env.index - attack;
				return (((decay - index2) << 15) +  index2 * sustain ) / decay;
			} else  {
				return sustain;
			}
		} else {
			if (env.index< release) {
				return env.releaseSample - env.index * env.releaseSample / release;
			} else {
				env.dead = true;
				return 0;
			}
		}
	}

	void noteOn(struct EnvState& env) {
		env.dead = false;
		env.index = 0;
		env.noteon = true;
	}

	void noteOff(struct EnvState& env) {
		env.releaseSample = this->getAmp(env);
		env.noteon = false;
		env.index = 0;
	}

	static void nextSample(struct EnvState& env) {
		env.index++;
	}
	static bool isDead(struct EnvState& env) {
		return env.dead;
	}

private:
	// ADSR
	int attack;
	int decay;
	int sustain;
	int release;
};

