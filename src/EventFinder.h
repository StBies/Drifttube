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

//ROOT headers
#include "TH1D.h"

class EventFinder
{
public:
	EventFinder();
	virtual ~EventFinder();

	vector<int> listEventNumbers(string expression);
	DataSet* getEvents(string expression);

private:
	bool parseExpression(string expression,TH1D* data);
};

#endif /* EVENTFINDER_H_ */
