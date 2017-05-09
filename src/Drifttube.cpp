/*
 * Drifttube.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: bieschke
 */

#include "Drifttube.h"

Drifttube::Drifttube(int posX, int posY)
{
	m_position[0] = posX;
	m_position[1] = posY;
//	m_data = DataSet();
}

Drifttube::~Drifttube()
{

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
