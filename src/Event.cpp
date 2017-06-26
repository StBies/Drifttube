/*
 * Event.cpp
 *
 *  Created on: May 15, 2017
 *      Author: bieschke
 */

#include "Event.h"

using namespace std;

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
Event::Event(unsigned int eventNumber, unique_ptr<array<uint16_t,800>> data) : Data(move(data))
{
	m_event_number = eventNumber;
	//TODO implement drift time initialization
	m_drift_time = 0;
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
 * @return Drift time of the event
 */
const double Event::getDriftTime() const
{
	return m_drift_time;
}

