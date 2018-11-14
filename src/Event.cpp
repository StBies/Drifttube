/*
 * Event.cpp
 *
 *  Created on: May 15, 2017
 *      Author: bieschke
 */

#include "Event.h"

using namespace std;

//TODO Change all doc to vector and variable length (Nov. 14, 2018)

/**
 * Constructor
 *
 * @brief ctor
 *
 * @author Stefan Bieschke
 * @date May 16, 2017
 * @version Alpha 2.0
 *
 * @param eventNumber number of the event
 * @param data Data that should be stored
 */
Event::Event(unsigned int eventNumber, unique_ptr<vector<uint16_t>> data) : Data(move(data))
{
	m_event_number = eventNumber;
	m_drift_time = ADC_BINS_TO_TIME * DataProcessor::findDriftTimeBin(*this,2150);
}

Event::~Event()
{

}


/**
 * Copy constructor
 *
 * @author Stefan Bieschke
 * @date May 16, 2017
 * @version Alpha 2.0
 *
 * @param original Event that shall be copied
 */
Event::Event(const Event& original) : Data(original)
{
	m_event_number = original.m_event_number;
	m_drift_time = original.m_drift_time;
}

/**
 * Getter method for the event number. This returns the number of the event (a.k.a the number of the trigger) as unsigned integer.
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 *
 * @return Number of the event
 */
const unsigned int Event::getEventNumber() const
{
	return m_event_number;
}

/**
 * Getter method for the drift time of the event in nanoseconds.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date Nov 29, 2017
 *
 * @return Drift time of the event
 */
const double Event::getDriftTime() const
{
	return m_drift_time;
}


/**
 * Assignment operator. The Event object on the right hand side (rhs) gets assigned to the left hand side (lhs) Event object. Due to the
 * constness of the parameter rhs, temporary objects can be assigned to non temporary lvalues.
 *
 * @brief assignment operator
 *
 * @author Stefan Bieschke
 * @date August 1, 2017
 * @version Alpha 2.0
 *
 * @param rhs const reference to the Event object on the right hand side of the assignment
 * @return Reference to the lhs object - basically the address that the rhs is assigned to
 */
Event& Event::operator=(const Event& rhs)
{
	Data::operator=(rhs);
	m_event_number = rhs.m_event_number;
	m_drift_time = rhs.m_drift_time;

	return *this;
}
