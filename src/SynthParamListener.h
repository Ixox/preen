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


#ifndef SYNTHPARAMLISTENER_H_
#define SYNTHPARAMLISTENER_H_

enum SynthParamListenerType {
	SYNTH_PARAM_ENGINE_LISTENER = 0,
	SYNTH_PARAM_OSCILLATOR_LISTENER,
	SYNTH_PARAM_ENVELOPE_LISTENER,
	SYNTH_PARAM_MATRIX_LISTENER,
	SYNTH_PARAM_LFO_LISTENER,
	SYNTH_PARAM_INVALID_LISTENER
};


class SynthParamListener {
public:
	SynthParamListener();
	~SynthParamListener();

    virtual void newParamValue(SynthParamListenerType type, int param, int oldValue, int newValue) = 0;

    SynthParamListener* nextListener;

	SynthParamListenerType getListenerType() {
		return listenerType;
	}

	void setListenerType(SynthParamListenerType listenerType) {
    	this->listenerType = listenerType;
    }

private:
	SynthParamListenerType listenerType;
};

#endif /* SYNTHPARAMLISTENER_H_ */
