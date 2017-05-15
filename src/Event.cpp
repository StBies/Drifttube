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
 * @param eventNumber number of the event
 * @param data Data that should be stored
 */
Event::Event(unsigned int eventNumber, unique_ptr<array<uint16_t,800>> data)
{
	m_event_number = eventNumber;
	m_data = move(data);
}

Event::~Event()
{

}

/**
 * Getter method for the data. This returns a reference to the data array that is contained within an object of this class.
 * @brief Getter for the data
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 *
 *
 * @return Reference to the array that is contained
 */
const std::array<uint16_t,800>& Event::getData()
{
	return *m_data;
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
const unsigned int Event::getEventNumber()
{
	return m_event_number;
}


/**
 * Implementation of the bracket operator [] commonly used to address a single element of an array. This returns a reference to the requested
 * value so that event[i] can be used as lvalue.
 *
 * @brief Bracket operator for addressing
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 *
 * @param bin
 * @return Content of the requested bin. This is a reference to the content so that it can be used as lvalue. E.g. event[i] = 5;
 */
uint16_t& Event::operator[](const unsigned short bin) const
{
	if(bin < m_data->size())
	{
		return (*m_data)[bin];
	}
	else
	{
		//TODO throw an Exception for index out of bounds
		throw 'i';
	}
}

