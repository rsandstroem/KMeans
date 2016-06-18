/*
 * BasicAlgorithm.h
 *
 *  Created on: Jun 17, 2016
 *      Author: rikard
 */

#ifndef BASICALGORITHM_H_
#define BASICALGORITHM_H_

#include <vector>
#include <algorithm>
#include "DataSet.h"

using namespace std;

class BasicAlgorithm {
public:
	BasicAlgorithm();
	virtual ~BasicAlgorithm();

	/// For a give data set "data", find return the K centroids representing
	/// the clusters of "data", using the same formatting as the input data.
	DataSet findCentroids(DataSet& data, unsigned int K);

private:
	inline vector<double> subtractScalar(const vector<double>& x, double mu) {vector<double> r; r.resize(x.size()); transform(x.begin(), x.end(), r.begin(), bind2nd(minus<double>(), mu)); return r;};
	inline vector<double> square(const vector<double>& v) {vector<double> r; r.resize(v.size()); transform( v.begin(), v.end(), v.begin(), r.begin(), multiplies<double>()); return r;};

	/// Random assignment of centroids
	void randomCentroids(const DataSet& data, DataSet& centroids);

	/// Distance between a vector and a scalar in one dimension
	double squareDistance1D(vector<double>, double);

	/// Assign centroid to points and return the cost
	double assignCentroids(DataSet& data, const DataSet& centroids);

	/// Update the centroids after point assignment
	void updateCentroids(const DataSet& data, DataSet& centroids);


	/// Distance between the 3D points and a fixed point
	//double squareDistance3D(const DataSet& data, const double x, const double y, const double z);

};

#endif /* BASICALGORITHM_H_ */
