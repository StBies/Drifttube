/*
 * Drifttube.h
 *
 *  Created on: Apr 6, 2017
 *      Author: bieschke
 */

#ifndef DRIFTTUBE_H_
#define DRIFTTUBE_H_

#include <array>
#include "DataSet.h"


/**
 * Basic implementation of a drifttube. This contains the coordinates of the drift tube as well as
 * a DataSet object containing its raw data
 *
 * @author Stefan Bieschke
 * @date April 6, 2017
 * @version 0.1
 */
class Drifttube
{
public:
	Drifttube();
	virtual ~Drifttube();

private:
	std::array<int,2> _position;
	DataSet _data;
};

#endif /* DRIFTTUBE_H_ */
