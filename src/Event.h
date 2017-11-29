/*
 * Event.h
 *
 *  Created on: May 15, 2017
 *      Author: bieschke
 */

#ifndef EVENT_H_
#define EVENT_H_

//forward declaration needed due to ring inclusion
class DataProcessor;

#include <array>
#include <memory>
#include <cstdlib>
#include "Data.h"
#include "DataProcessor.h"

/**
 * Class that represents the data for one event.
 * This is basically a wrapper class for a @c std::array. It contains an @c array and an  @c int which holds the number of the event. So for the third
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

	Event& operator=(const Event& rhs);

private:
	unsigned int m_event_number;
	double m_drift_time;
};

#endif /* EVENT_H_ */
