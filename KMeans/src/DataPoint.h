/*
 * DataPoint.h
 *
 *  Created on: Jun 18, 2016
 *      Author: rikard
 */

#ifndef DATAPOINT_H_
#define DATAPOINT_H_

#include <iostream>
#include <iomanip>

using namespace std;

class DataPoint {
public:
	DataPoint();
	DataPoint(double x, double y, double z, int k);
	virtual ~DataPoint();

	inline void print() const {cout << setw(12) << _x << setw(12) << _y << setw(12) << _z << setw(8) << _k << endl;};
	inline double const & getX() const {return _x;};
	inline double const & getY() const {return _y;};
	inline double const & getZ() const {return _z;};
	inline int const & getK() const {return _k;};
	inline void setX(const double x) {_x = x;};
	inline void setY(const double y) {_y = y;};
	inline void setZ(const double z) {_z = z;};
	inline void setK(const int k) {_k = k;};

private:
	double _x;
	double _y;
	double _z;
	int _k;
};

#endif /* DATAPOINT_H_ */
