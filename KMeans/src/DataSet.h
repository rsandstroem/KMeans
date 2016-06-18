/*
 * DataSet.h
 *
 *  Created on: Jun 17, 2016
 *      Author: rikard
 */

#ifndef DATASET_H_
#define DATASET_H_

#include <vector>
#include <string>

using namespace std;

class DataSet
{
public:
	DataSet ();
	DataSet (unsigned int K);
	DataSet (vector<string>& input);
	virtual ~DataSet();

	void parseData(vector<string>& input);
	void print();

	inline vector<double> const getX() const {return _x;};
	inline vector<double> const getY() const {return _y;};
	inline vector<double> const getZ() const {return _z;};
	inline vector<int> const getK() const {return _k;};
	inline void setX(const vector<double> x) {_x = x;};
	inline void setY(const vector<double> y) {_y = y;};
	inline void setZ(const vector<double> z) {_z = z;};
	inline void setK(const vector<int> k) {_k = k;};
	inline void set3DPoint(double x, double y, double z, int k) {_x[k]=x; _y[k]=y; _z[k]=z;};

private:
	vector<double> _x;
	vector<double> _y;
	vector<double> _z;
	vector<int> _k;
};

#endif /* DATASET_H_ */

