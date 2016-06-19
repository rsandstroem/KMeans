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

#include "DataPoint.h"
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

vector<DataPoint> parseData(vector<string>& input) {
	vector<DataPoint> data;
	data.resize(input.size());
	double x, y, z;
	for (unsigned int irow=0; irow<input.size(); ++irow){
		stringstream iss(input[irow]);
		iss >> x >> y >> z;
		data[irow] = DataPoint(x, y, z, -1);
	}
	return data;
}

void writeResults(const vector<DataPoint>& data, const string& filename)
{
	ofstream file;
	file.open(filename);

	for (auto p : data) {
		file << p.getX() << " " << p.getY() << " "<< p.getZ() << " " << p.getK() << endl;
	}
	file.close();
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
			cout << "Usage: KMeans /path/to/input.txt K" << endl;
			cout << "Returns: copy of input file with an additional column indicating assigned cluster." << endl;
			cout << "Cluster centroids are printed to screen." << endl;
			cout << "The parameter K indicates how many clusters should used." << endl;
			exit(EXIT_SUCCESS);
		} else {
			cout << "Unknown switch: " << argv[optind] << endl;
			exit(EXIT_FAILURE);
		}
		optind++;
	}

	string filename(argv[1]);
	auto v_rows = readFileToVector(filename);
	vector<DataPoint> data = parseData(v_rows);
	cout << "My data point " ;
	data[0].print();

	// TODO: Throw exception if K not given as input
	unsigned int K = 0;
	stringstream ss(argv[2]);
	ss >> K;

	if (K>v_rows.size()) {
		cout << "K is larger than number of data points. Please rerun with smaller K." << endl;
		exit(EXIT_FAILURE);
	}

	BasicAlgorithm basic;
	vector<DataPoint> centroids = basic.findCentroids(data, K);
	cout << "Final centroids: " << endl;
	for (auto p : centroids) p.print();
	writeResults(data, "results.txt");

	return 0;
}
