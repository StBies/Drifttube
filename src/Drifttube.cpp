/*
 * Drifttube.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: bieschke
 */

#include "Drifttube.h"

Drifttube::Drifttube(int posX, int posY, unique_ptr<DataSet> data)
{
	m_position[0] = posX;
	m_position[1] = posY;
	m_data = move(data);
}

Drifttube::~Drifttube()
{

}

/**
 * Getter method for the radius of the drift tube in millimeters.
 *
 * @brief Radius getter
 *
 * @author Stefan Bieschke
 * @date May 19, 2017
 * @version Alpha 2.0
 *
 *
 * @return Radius of the drift tube in millimeters
 */
const unsigned int Drifttube::getRadius() const
{
	return m_radius;
}

/**
 * Getter method for the x coordinate of the drift tube. The coordinate is the position of the wire, a.k.a the drift tube's center. The wire
 * axis is the z-direction, so the tube's radius is in the x-y-plane.
 *
 * @brief Getter for x coordinate
 *
 * @author Stefan Bieschke
 * @date May 19, 2017
 * @version Alpha 2.0
 *
 * @return x coordinate in millimeters
 */
const int Drifttube::getPositionX() const
{
	return m_position[0];
}

/**
 * Getter method for the y coordinate of the drift tube. The coordinate is the position of the wire, a.k.a the drift tube's center. The wire
 * axis is the z-direction, so the tube's radius is in the x-y-plane.
 *
 * @brief Getter for y coordinate
 *
 * @author Stefan Bieschke
 * @date May 19, 2017
 * @version Alpha 2.0
 *
 * @return y coordinate in millimeters
 */
const int Drifttube::getPositionY() const
{
	return m_position[1];
}

const array<int,2>& Drifttube::getPosition() const
{
	return m_position;
}

const DataSet& Drifttube::getDataSet() const
{
	return *m_data;
}
