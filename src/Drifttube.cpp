/*
 * Drifttube.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: bieschke
 */

#include "Drifttube.h"

/**
 * Constructor of the drift tube. Initializes the Drifttube object. During the process of initialization, the drift time spectrum and
 * rt relation are calculated and stored in the Drifttube as well. The calculation is performed and results are provided by the static
 * class DataProcessor. The efficiency is ready calculated after initialization as well.
 *
 * @brief ctor
 *
 * @author Stefan Bieschke
 * @date May 19, 2017
 * @version Alpha 2.0
 *
 * @param posX x-coordinate [mm] of the tube
 * @param posY y-coordinate [mm] of the tube
 * @param data unique_ptr to the DataSet of Events in this Drifttube
 */
Drifttube::Drifttube(int posX, int posY, unique_ptr<DataSet> data)
: m_dtSpect(DataProcessor::calculateDriftTimeSpectrum(*data)), m_rtRel(DataProcessor::calculateRtRelation(m_dtSpect))
{
	m_position[0] = posX;
	m_position[1] = posY;
	m_data = move(data);

	unsigned int numberOfRealEvents = m_dtSpect.getEntries() - m_dtSpect.getRejected();
	m_efficiency = numberOfRealEvents/(double)m_dtSpect.getEntries();
//	cout << "efficiency = " << m_efficiency << " +- " << sqrt(m_efficiency*(1-m_efficiency)/(double)m_dtSpect.getEntries()) << endl;
}


/**
 * Copy constructor. Initializes a copy of a passed Drifttube object including copies of the DataSet for that Drifttube.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 26, 2017
 *
 * @param original Reference to the original Drifttube object of that a copy should be created
 */
Drifttube::Drifttube(const Drifttube& original)
: m_dtSpect(original.m_dtSpect), m_rtRel(original.m_rtRel)
{
	m_data = unique_ptr<DataSet>(new DataSet(*original.m_data));
	m_efficiency = original.m_efficiency;
	m_position = original.m_position;
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

/**
 * Getter method for the two-dimensional position of the drift tube's center (aka wire position). This returns a const reference to the original
 * 2d array.
 *
 * @brief Position getter
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 23, 2017
 *
 * @return const reference to the position array. Values in mm.
 */
const array<int,2>& Drifttube::getPosition() const
{
	return m_position;
}

/**
 * Getter method for the drift time spectrum.
 *
 * @brief drift time spectrum getter
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 23, 2017
 *
 * @return const reference to the drift time spectrum
 */
const DriftTimeSpectrum& Drifttube::getDriftTimeSpectrum() const
{
	return m_dtSpect;
}

/**
 * Getter method for the rt-relation. This maps the drift radius (mm) to the drift time (ns).
 *
 * @brief rt-relation getter
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 23, 2017
 *
 * @return const reference to the rt-relation
 */
const RtRelation& Drifttube::getRtRelation() const
{
	return m_rtRel;
}

/**
 * Getter method for the triggering efficiency of the tube. This value describes the rate of events with a voltage undershooting a given threshold
 * (voltages are negative) over the number of totally triggered events. Ensured to be between zero and one IF (and only if) the DataSet stored in this
 * Drifttube object contains any valid Events.
 *
 * @brief triggering efficiency getter
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 23, 2017
 *
 * @return Efficiency of the drift tube
 */
const double Drifttube::getEfficiency() const
{
	return m_efficiency;
}


/**
 * Getter method for the stored DataSet. The DataSet contains all the triggered Events that themselves contain every stored waveform.
 *
 * @brief DataSet getter
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 23, 2017
 *
 * @return const reference to the DataSet
 */
const DataSet& Drifttube::getDataSet() const
{
	return *m_data;
}
