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
 * a DataSet object containing its raw data.
 *
 * @brief Representation of a tube detector.
 *
 * @author Stefan Bieschke
 * @date April 6, 2017
 * @version Alpha 2.0
 */
class Drifttube
{
public:
	Drifttube(int posX, int posY, DataSet* data);
	virtual ~Drifttube();

	const unsigned int getRadius();
	int getPositionX();
	int getPositionY();

private:
	const unsigned int m_radius = 18150; //micron
	std::array<int,2> m_position;
	DataSet m_data;
};

#endif /* DRIFTTUBE_H_ */
