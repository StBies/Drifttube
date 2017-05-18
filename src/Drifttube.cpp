/*
 * Drifttube.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: bieschke
 */

#include "Drifttube.h"

Drifttube::Drifttube(int posX, int posY, DataSet* data)
{
	m_position[0] = posX;
	m_position[1] = posY;
	m_data = *DataSet();
}

Drifttube::~Drifttube()
{
	delete &m_data;
}

const unsigned int Drifttube::getRadius()
{
	return m_radius;
}

int Drifttube::getPositionX()
{
	return m_position[0];
}
int Drifttube::getPositionY()
{
	return m_position[1];
}
