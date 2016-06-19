/*
 * DataPoint.cpp
 *
 *  Created on: Jun 18, 2016
 *      Author: rikard
 */

#include "DataPoint.h"

DataPoint::DataPoint():_x(0),_y(0),_z(0),_k(0) {
  // TODO Auto-generated constructor stub

}

DataPoint::DataPoint(double x, double y, double z, int k) {
  _x = x;
  _y = y;
  _z = z;
  _k = k;
}

DataPoint::~DataPoint() {
  // TODO Auto-generated destructor stub
}

