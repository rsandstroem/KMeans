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

vector<DataPoint> BasicAlgorithm::findCentroids(vector<DataPoint>& data, unsigned int K) {
	const double stop_precision = 1e-5;
	/// Initialize the centroids
	vector<DataPoint> centroids(K);
	cout << "bla" << endl;

	/// Assign random data point as centroids
	randomCentroids(data, centroids);
	cout << "Random centroids: " << endl;
	for (auto p : centroids) p.print();

	/// Alternate between
	/// (a) reassigning data to centroids and
	/// (b) updating the centroids
	/// to minimize the cost function.
	double previousCost = 9e9;
	double cost = 8e9;
	while (cost < previousCost - stop_precision) {
		previousCost = cost;

		/// Assign centroids to data points
		cost = assignCentroids(data, centroids);
		//cost = assignCentroidsBruteForce(data, centroids);
		cout << "cost = " << cost << endl;

		/// Update the centroids
		updateCentroids(data, centroids);
	}

	return centroids;
}

void BasicAlgorithm::randomCentroids(const vector<DataPoint>& data, vector<DataPoint>& centroids) {
	auto n = data.size();
	auto K = centroids.size();
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
		auto x = data[random_idx[icen]].getX();
		auto y = data[random_idx[icen]].getY();
		auto z = data[random_idx[icen]].getZ();
		centroids[icen] = DataPoint(x,y,z,icen);
	}
}

void BasicAlgorithm::assignCentroidsSubset(vector<DataPoint>& data, const vector<DataPoint>& centroids,
		const unsigned int first, const unsigned int last, double& sumR2) {
	double partialSumR2 = 0;
	for (unsigned int i=first; i<last; ++i){
		auto x = data[i].getX();
		auto y = data[i].getY();
		auto z = data[i].getZ();
		double smallestR2 = 1e9;
		int k_best = -1;
		for ( auto cen : centroids) {
			double r2 = (x-cen.getX())*(x-cen.getX())
					  + (y-cen.getY())*(y-cen.getY())
					  + (z-cen.getZ())*(z-cen.getZ());
			if (r2 < smallestR2) {
				smallestR2 = r2;
				k_best = cen.getK();
			}
		}
		/// Assign the data point to a centroid
		data[i].setK(k_best);
		partialSumR2 += smallestR2;
	}
	sumR2 += partialSumR2;
}

double BasicAlgorithm::assignCentroids(vector<DataPoint>& data, const vector<DataPoint>& centroids) {

	/// Calculate the distance square for each point to each centroid
	auto n_data = data.size();
	double sumR2 = 0;
	const size_t nthreads = thread::hardware_concurrency();
	vector<double> partialSumR2;
	vector<thread> threads;
	partialSumR2.resize(nthreads,0);
	threads.resize(nthreads);
	const size_t itemsPerThread = std::max(1, (int)ceil(n_data/nthreads));
	int t = 0;
	for (size_t nextIndex= 0; nextIndex < n_data; nextIndex += itemsPerThread)
	{
	    const size_t beginIndex = nextIndex;
	    const size_t endIndex = min(nextIndex+itemsPerThread, n_data);
	    auto f = std::bind(&BasicAlgorithm::assignCentroidsSubset, this, ref(data), ref(centroids), beginIndex, endIndex, ref(partialSumR2[t]));
	    threads[t] = thread(f);

	    // Only in c++14:
//	    thread myThread([&, ref(data), centroids, beginIndex, endIndex, ref(sumR2)]{ this->assignCentroidsSubset(data, centroids, beginIndex, endIndex, sumR2); });
	    ++t;
	}

    for (unsigned int t = 0; t < threads.size(); ++t) {
    	threads[t].join();
    	sumR2 += partialSumR2[t];
    }

	/// Calculate the cost function and return it
	return sumR2/n_data;
}

double BasicAlgorithm::assignCentroidsBruteForce(vector<DataPoint>& data, const vector<DataPoint>& centroids) {

	/// Calculate the distance square for each point to each centroid
	auto n_data = data.size();
	double sumR2 = 0;

	for (unsigned int i=0; i<n_data; ++i){
		auto x = data[i].getX();
		auto y = data[i].getY();
		auto z = data[i].getZ();
		double smallestR2 = 1e9;
		int k_best = -1;
		for ( auto cen : centroids) {
			double r2 = (x-cen.getX())*(x-cen.getX())
					  + (y-cen.getY())*(y-cen.getY())
					  + (z-cen.getZ())*(z-cen.getZ());
			if (r2 < smallestR2) {
				smallestR2 = r2;
				k_best = cen.getK();
			}
		}
		/// Assign the data point to a centroid
		data[i].setK(k_best);
		sumR2 += smallestR2;
	}

	/// Calculate the cost function and return it
	return sumR2/n_data;
}

void BasicAlgorithm::updateCentroids(const vector<DataPoint>& data, vector<DataPoint>& centroids) {
	/// Brute force method:
	auto K = centroids.size();

	vector<double> sumX, sumY, sumZ;
	vector<int> n_in_k;
	sumX.resize(K, 0.); sumY.resize(K, 0.); sumZ.resize(K, 0.);
	n_in_k.resize(K, 0);
	for ( auto p : data ) {
		auto k = p.getK();
		sumX[k] += p.getX();
		sumY[k] += p.getY();
		sumZ[k] += p.getZ();
		n_in_k[k]++;
	}
	/// Divide by number of elements in k to get the mean
	for (unsigned int icen = 0; icen<K; ++icen) {
		centroids[icen].setX(sumX[icen]/n_in_k[icen]);
		centroids[icen].setY(sumY[icen]/n_in_k[icen]);
		centroids[icen].setZ(sumZ[icen]/n_in_k[icen]);
	}
}
// eof

