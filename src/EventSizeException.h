/*
 * EventSizeException.h
 *
 *  Created on: 02.07.2016
 *      Author: stefan
 */

#ifndef EVENTSIZEEXCEPTION_H_
#define EVENTSIZEEXCEPTION_H_

#include <string>
#include <sstream>
#include "Exception.h"

using namespace std;

class EventSizeException: public Exception
{
public:
	EventSizeException(int event);
	virtual ~EventSizeException();

	string error();
private:
	int _event;
};

#endif /* EVENTSIZEEXCEPTION_H_ */
