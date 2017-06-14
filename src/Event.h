/*
 * Event.h
 *
 *  Created on: May 15, 2017
 *      Author: bieschke
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <array>
#include <memory>
#include <cstdlib>
#include "Data.h" //.cpp file inclusion needed as Data is a template class

/**
 * Class that represents the data for one event.
 * This is basically a wrapper class for a std::array. It contains an array and an integer which holds the number of the event. So for the third
 * triggered event of the detector, the event number will be 2, as counting starts at zero
 *
 * @brief Event class
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 */
class Event : public Data<>
{
public:
	Event(const unsigned int eventNumber, std::unique_ptr<std::array<uint16_t,800>> data);
	virtual ~Event();
	Event(const Event& original);

	const unsigned int getEventNumber() const;
	const double getDriftTime() const;

	//TODO check if offering this is bad design and if getData() should be all that is available

private:
	unsigned int m_event_number;
	double m_drift_time;
};

#endif /* EVENT_H_ */
