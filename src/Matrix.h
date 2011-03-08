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


#ifndef MATRIX_H_
#define MATRIX_H_

#define MATRIX_SIZE 6

#include "SynthState.h"
#include "SynthParamListener.h"




class Matrix  {
public:
	Matrix();
	~Matrix();
	void reinitUsage();
	void reinitUsage(int k, int oldValue, int value);

	void resetDestination() {
		for (int k=0; k< MATRIX_SIZE; k++) {
			destinations[(int)rows[k].destination] = 0;
		}
	}

	void computeDestintation() {
		for (int k=0; k< MATRIX_SIZE; k++) {
			destinations[(int)rows[k].destination] += sources[(int)rows[k].source] * rows[k].mul;
		}
	}

	bool isDestinationUsed(DestinationEnum index) {
		return destinationUsed[index];
	}
	bool isSourceUsed(SourceEnum index) {
		return sourceUsed[index];
	}
	void setRowSource(int index, SourceEnum source);
	void setRowDestination(int index, DestinationEnum destination) ;
	void setRowMul(int index, short mul);
	void checkRow(int index);
	void setDestination(DestinationEnum destination, int value) {
		destinations[destination] = value;
	}
	void setSource(SourceEnum source, int value) {
		this->sources[source] = value;
	}
	int getDestination(DestinationEnum destination) {
		return this->destinations[destination];
	}
	int getSource(SourceEnum source) {
		return this->sources[source];
	}

    void newParamValue(int param, int oldValue, int newValue);;


private:
	bool sourceUsed[SOURCE_MAX];
	bool destinationUsed[DESTINATION_MAX];
	int sources[SOURCE_MAX];
	int destinations[DESTINATION_MAX];
	MatrixRowParams* rows;
};

#endif /* MATRIX_H_ */
