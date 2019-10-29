/*
 * Drifttube.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: bieschke
 */

#include "Drifttube.h"

using namespace std;

/**
 * Constructor of the drift tube. Initializes the Drifttube object. During the process of initialization, the drift time spectrum and
 * rt relation are calculated and stored in the Drifttube as well. The calculation is performed and results are provided by the static
 * class DataProcessor. The efficiency is ready calculated after initialization as well.
 *
 * @brief ctor
 *
 * @author Stefan Bieschke
 * @date July 20, 2017
 * @version Alpha 2.0.1
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

	//TODO check, if this is faster than putting m_rtRel.getData().size() in the for loop conditional
	size_t arraySize = m_rtRel.getData().size();
	for(size_t i = 0; i < arraySize; ++i)
	{
		if(m_rtRel[i] >= DRIFT_TUBE_RADIUS - DRIFT_TUBE_RADIUS * 0.0005)
		{
			m_max_drifttime = i * ADC_BINS_TO_TIME;
			break;
		}
	}
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
	m_max_drifttime = original.m_max_drifttime;
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
 * Returns the maximum drift time in nanoseconds. This is computed using the rtRelation stored in the tube. The maximum drift time, in the context
 * of this method, is defined as the time, where the r(t) in rtRelation suprasses 99.95% of the tube's radius. This is equivalent to integrating
 * the drift time spectrum up to the point, where 99.95% of all events are included in the integral.
 *
 * @brief Getter for maximum drift time
 *
 * @author Stefan Bieschke
 * @date July 20,2017
 * @version Alpha 2.0
 *
 *
 * @return Maximum drift time (ns)
 */
const double Drifttube::getMaxDrifttime() const
{
	return m_max_drifttime;
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


//TODO test
/**
 * Assignment operator allowing to write Drifttube a = Drifttube(...); In this case, the DataSet from the rhs is MOVED to the lhs without copying.
 * Thus, after this operation, rhs is empty. Good for temporary rvalues.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 23, 2017
 *
 * @param rhs Const reference to the object that should be assigned to the left hand side value.
 *
 * @return Reference to the bound object
 */
Drifttube& Drifttube::operator=(const Drifttube& rhs)
{
	m_dtSpect = rhs.m_dtSpect;
	m_rtRel = rhs.m_rtRel;
	//TODO check below
	//move dataset: Update - cannot work with const param... d'oh
//	m_data.reset(rhs.m_data.get());
	m_data = unique_ptr<DataSet>(new DataSet(*rhs.m_data));
	m_efficiency = rhs.m_efficiency;
	m_position = rhs.m_position;

	return *this;
}

/**
 * Assignment operator with non-constant rvalue. This operator needs a non-temporary rvalue. After completion, lvalue holds a copy of rvalue.
 *
 * @brief Assignment operator
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date July 13, 2017
 *
 * @param rhs Non-const (non-temporary) reference to the right hand side value, that should be assigned to the left hand side
 * @return Reference to the bound object
 */
Drifttube& Drifttube::operator=(Drifttube& rhs)
{
	m_dtSpect = rhs.m_dtSpect;
	m_rtRel = rhs.m_rtRel;
	//copy dataset
	m_data = unique_ptr<DataSet>(new DataSet(*rhs.m_data));
	m_efficiency = rhs.m_efficiency;
	m_position = rhs.m_position;

	return *this;
}
