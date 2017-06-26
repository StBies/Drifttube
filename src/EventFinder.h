/*
 * EventFinder.h
 *
 *  Created on: Aug 4, 2016
 *      Author: bieschke
 */

#ifndef EVENTFINDER_H_
#define EVENTFINDER_H_

//C++ libs
#include <vector>

//own headers
#include "DataSet.h"

class EventFinder
{
public:
	EventFinder();
	virtual ~EventFinder();

	vector<unsigned int> listEventNumbers(string expression) const;
	DataSet* getEvents(string expression);

private:
	bool parseExpression(string expression, const Event& data) const;
};

#endif /* EVENTFINDER_H_ */
