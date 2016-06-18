/*
 * BasicAlgorithm.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: rikard
 */

#include "BasicAlgorithm.h"

#include <numeric>
#include <functional>
#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

BasicAlgorithm::BasicAlgorithm() {
	// TODO Auto-generated constructor stub

}

BasicAlgorithm::~BasicAlgorithm() {
	// TODO Auto-generated destructor stub
}

DataSet BasicAlgorithm::findCentroids(DataSet& data, unsigned int K) {

	/// Initialize the centroids
	DataSet centroids = DataSet(K);
	// This is not really necessary...
	auto cen_k = centroids.getK();
	for(std::vector<int>::size_type k = 0; k != cen_k.size(); k++) {
		cen_k[k] = k;
	}
	centroids.setK(cen_k);

	/// Assign random data point as centroids
	randomCentroids(data, centroids);
	cout << "Random centroids: " << endl;
	centroids.print();

	/// Alternate between
	/// (a) reassigning data to centroids and
	/// (b) updating the centroids
	/// to minimize the cost function.
	double previousCost = 9e9;
	double cost = 8e9;
	while (cost < previousCost) {
		previousCost = cost;

		/// Assign centroids to data points
		//cost = assignCentroids(data, centroids);
		cost = assignCentroidsBruteForce(data, centroids);
		cout << "cost = " << cost << endl;

		/// Update the centroids
		updateCentroids(data, centroids);
	}

	return centroids;
}

void BasicAlgorithm::randomCentroids(const DataSet& data, DataSet& centroids) {
	// TODO: getN(), return single points without returning the vector
	auto n = data.getX().size();
	auto K = centroids.getX().size();
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0,n-1);

	/// Randomly pick centroids, but must ensure that centroids are unique
	bool hasDuplicate = true;
	vector<int> random_idx;
	while (hasDuplicate) {
		random_idx.clear();
		for (unsigned int icen = 0; icen<K; ++icen ){
			random_idx.push_back(uni(rng));
		}
		sort( random_idx.begin(), random_idx.end() );
		hasDuplicate =  adjacent_find( random_idx.begin(), random_idx.end() ) != random_idx.end();
	}
	for (unsigned int icen = 0; icen<K; ++icen ){
		auto x = data.getX()[random_idx[icen]];
		auto y = data.getY()[random_idx[icen]];
		auto z = data.getZ()[random_idx[icen]];
		centroids.set3DPoint(x,y,z,icen);
	}
}

double BasicAlgorithm::squareDistance1D(vector<double> x, double mu) {
	/// Residual of x and mu
	vector<double> subtracted;
	subtracted.resize(x.size());
	transform(x.begin(), x.end(), subtracted.begin(), bind2nd(std::minus<double>(), mu));

	// Sum of distance to mu squared
	double squaredSum = inner_product(subtracted.begin(), subtracted.end(), subtracted.begin(), 0);

	return squaredSum/x.size();
}

double BasicAlgorithm::assignCentroids(DataSet& data, const DataSet& centroids) {
	auto cen_x = centroids.getX();
	auto cen_y = centroids.getY();
	auto cen_z = centroids.getZ();
	auto x = data.getX();
	auto y = data.getY();
	auto z = data.getZ();

	/// Calculate the distance square for each point to each centroid
	vector<double> res_best;
	vector<int> k_best;
	int n_data = x.size();
	res_best.resize(n_data);
	k_best.resize(n_data);
	for (unsigned int icen = 0; icen<cen_x.size(); ++icen){
		/// The most time consuming part is calculating the square of the distance (residuals).
		/// Separate threads or mapreduce here:
		auto res_x2 = square(subtractScalar(x, cen_x[icen]));
		auto res_y2 = square(subtractScalar(y, cen_y[icen]));
		auto res_z2 = square(subtractScalar(z, cen_z[icen]));

		for (int i=0; i<n_data; ++i){
			auto res_r2i = res_x2[i]+res_y2[i]+res_z2[i];
			if ((icen==0) || (res_r2i < res_best[i])){
				res_best[i] = res_r2i;
				k_best[i] = icen;
			}
		}
	}

	/// Assign each data point to a centroid
	data.setK(k_best);

	/// Calculate the cost function and return it
	double cost = accumulate(res_best.begin(), res_best.end(), 0.)/n_data;
	return cost;
}

double BasicAlgorithm::assignCentroidsBruteForce(DataSet& data, const DataSet& centroids) {
	auto cen_x = centroids.getX();
	auto cen_y = centroids.getY();
	auto cen_z = centroids.getZ();
	auto x = data.getX();
	auto y = data.getY();
	auto z = data.getZ();
	auto n_data = x.size();

	/// Calculate the distance square for each point to each centroid
	vector<double> res_best;
	vector<int> k_best;
	res_best.resize(n_data);
	k_best.resize(n_data);

	vector<thread> threads(4);

	for (int i=0; i<n_data; ++i){
		double smallestR2 = 1e9;
		for (unsigned int icen = 0; icen<cen_x.size(); ++icen){
			double r2 = (x[i]-cen_x[icen])*(x[i]-cen_x[icen])
					  + (y[i]-cen_y[icen])*(y[i]-cen_y[icen])
					  + (z[i]-cen_z[icen])*(z[i]-cen_z[icen]);
			if (r2 < smallestR2) {
				smallestR2 = r2;
				k_best[i] = icen;
			}
		}
		res_best[i] = smallestR2;
	}
	/// Assign each data point to a centroid
	data.setK(k_best);

	/// Calculate the cost function and return it
	double cost = accumulate(res_best.begin(), res_best.end(), 0.)/n_data;
	return cost;
}

void BasicAlgorithm::updateCentroids(const DataSet& data, DataSet& centroids) {
	/// Brute force method:
	auto n = data.getK().size();
	auto K = centroids.getX().size();
	auto x = data.getX();
	auto y = data.getY();
	auto z = data.getZ();
	auto k = data.getK();
	vector<double> sumX, sumY, sumZ, aveX, aveY, aveZ;
	vector<int> n_in_k;
	sumX.resize(K, 0.); sumY.resize(K, 0.); sumZ.resize(K, 0.);
	n_in_k.resize(K, 0);
	for (unsigned int i = 0; i<n; ++i) {
		sumX[k[i]] += x[i];
		sumY[k[i]] += y[i];
		sumZ[k[i]] += z[i];
		n_in_k[k[i]]++;
	}
	/// Divide by number of elements in k to get the mean
	transform (sumX.begin(), sumX.end(), n_in_k.begin(), back_inserter(aveX), divides<double>());
	transform (sumY.begin(), sumY.end(), n_in_k.begin(), back_inserter(aveY), divides<double>());
	transform (sumZ.begin(), sumZ.end(), n_in_k.begin(), back_inserter(aveZ), divides<double>());
	centroids.setX(aveX);
	centroids.setY(aveY);
	centroids.setZ(aveZ);
}
// eof

