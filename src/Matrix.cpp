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

#include "Matrix.h"


Matrix::Matrix() {
	rows = &(synthStatus.state->matrixRowState1);
	reinitUsage();
}

Matrix::~Matrix() {
}

void Matrix::reinitUsage(int k, int oldValue, int value) {
	// Source
	if (k==0) {
		this->sourceUsed[oldValue] = false;
		this->sourceUsed[value] = true;
		this->sources[oldValue] = 0;
	}
	// Destination
	if (k==2) {
		this->destinationUsed[oldValue] = false;
		this->destinationUsed[value] = true;
		this->destinations[oldValue] = 0;
	}
}


void Matrix::reinitUsage() {

	for (int k=0; k<DESTINATION_MAX; k++) {
		this->destinationUsed[k] = false;
	}
	for (int k=0; k<SOURCE_MAX; k++) {
		this->sourceUsed[k] = false;
	}

	for (int k=0; k<MATRIX_SIZE; k++) {
		if (rows[k].mul != 0) {
			if (rows[k].source != SOURCE_NONE) {
				destinationUsed[(int)rows[k].destination] = true;
			}
			if (rows[k].destination != DESTINATION_NONE) {
				sourceUsed[(int)rows[k].source] = true;
			}
		}
	}
}

void Matrix::setRowSource(int index, SourceEnum source) {
	// free previous source
	this->sourceUsed[(int)rows[index].source] = SOURCE_NONE;
	// Assign new one
	this->sourceUsed[source] = true;
}

void Matrix::setRowDestination(int index, DestinationEnum destination) {
	// free previous destination
	this->destinationUsed[(int)rows[index].destination] = DESTINATION_NONE;
	// Assign new one
	this->destinationUsed[destination] = true;
}

