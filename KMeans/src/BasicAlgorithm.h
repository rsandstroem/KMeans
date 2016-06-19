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
#include "DataPoint.h"

using namespace std;

class BasicAlgorithm {
public:
	BasicAlgorithm();
	virtual ~BasicAlgorithm();

	/// For a give data set "data", find return the K centroids representing
	/// the clusters of "data", using the same formatting as the input data.
	vector<DataPoint> findCentroids(vector<DataPoint>& data, unsigned int K);

private:
	/// Random assignment of centroids
	void randomCentroids(const vector<DataPoint>& data, vector<DataPoint>& centroids);

	/// Assign centroid to points and return the cost
	double assignCentroids(vector<DataPoint>& data, const vector<DataPoint>& centroids);

	/// For a given range of data, assign the centroids and return the sum of square distance
	void assignCentroidsSubset(vector<DataPoint>& data, const vector<DataPoint>& centroids, const unsigned int first, const unsigned int last, double& sumR2);

	/// Assign centroid to points and return the cost using a straight forward loop
	double assignCentroidsBruteForce(vector<DataPoint>& data, const vector<DataPoint>& centroids);

	/// Update the centroids after point assignment
	void updateCentroids(const vector<DataPoint>& data, vector<DataPoint>& centroids);

};

#endif /* BASICALGORITHM_H_ */
