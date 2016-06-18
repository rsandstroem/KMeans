//============================================================================
// Name        : KMeans.cpp
// Author      : Rikard Sandström
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <functional>
#include <algorithm>

#include "DataSet.h"
#include "BasicAlgorithm.h"

using namespace std;

vector<string> readFileToVector(const string& filename)
{
    ifstream source;
    source.open(filename);
    vector<string> lines;
    string line;
    while (getline(source, line))
    {
        lines.push_back(line);
    }
    return lines;
}

void writeResults(const DataSet data, const string& filename)
{
	ofstream file;
	file.open(filename);

	auto x = data.getX();
	auto y = data.getY();
	auto z = data.getZ();
	auto k = data.getK();

	for (unsigned int irow = 0; irow < k.size(); ++irow ) {
		file << x[irow] << " "
				<< y[irow] << " "
				<< z[irow] << " "
				<< k[irow] << endl;
	}
}

int main(int argc,  char **argv){
	cout << "KMeans clustering" << endl;
	 if (argc<2) {
		 cout << "Missing arguments. Use '--help' for instructions." << endl;
		 exit(EXIT_FAILURE);
	 }
	 // decode arguments
	 int optind=1;
	 while ((optind < argc) && (argv[optind][0]=='-')) {
		string sw = argv[optind];
		if (sw=="--help") {
			optind++;
			cout << "This is a help message" << endl;
			exit(EXIT_SUCCESS);
		} else {
			cout << "Unknown switch: " << argv[optind] << endl;
			exit(EXIT_FAILURE);
		}
		optind++;
	}

	string filename(argv[1]);
	auto v_rows = readFileToVector(filename);
	DataSet dataSet = DataSet(v_rows);

	// TODO: Throw exception if K not given as input
	unsigned int K = 0;
	stringstream ss(argv[2]);
	ss >> K;

	if (K>v_rows.size()) {
		cout << "K is larger than number of data points. Please rerun with smaller K." << endl;
		exit(EXIT_FAILURE);
	}

	BasicAlgorithm basic;
	DataSet centroids = basic.findCentroids(dataSet, K);
	cout << "Final centroids: " << endl;
	centroids.print();
	writeResults(dataSet, "results.txt");

	return 0;
}
