/*
 * DataSet.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: rikard
 */

#include "DataSet.h"

#include <iostream>
#include <sstream>
#include <iomanip>

DataSet::DataSet() {
}

DataSet::DataSet(unsigned int K) {
	_x.resize(K);
	_y.resize(K);
	_z.resize(K);
	_k.resize(K);
}

DataSet::DataSet(vector<string>& input) {
	parseData(input);
}

DataSet::~DataSet() {
	// TODO Auto-generated destructor stub
}

void DataSet::print() {
	for (unsigned int irow = 0; irow < _x.size(); ++irow ) {
		cout << irow << ": " << setw(12)
			 << _x[irow] << setw(12)
			 << _y[irow] << setw(12)
			 << _z[irow] << setw(8)
			 << _k[irow] << endl;
	}
}

void DataSet::parseData(std::vector<std::string>& input) {
	unsigned int n_rows = input.size();
	_x.resize(n_rows);
	_y.resize(n_rows);
	_z.resize(n_rows);
	_k.resize(n_rows);
	for (unsigned int irow=0; irow<n_rows; ++irow){
		stringstream iss(input[irow]);
		iss >> _x[irow] >> _y[irow] >> _z[irow];
		_k[irow] = -1;
	}
}


