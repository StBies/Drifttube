/*
 * EventSizeException.h
 *
 *  Created on: 02.07.2016
 *      Author: stefan
 */

#ifndef EVENTSIZEEXCEPTION_H_
#define EVENTSIZEEXCEPTION_H_

#include <sstream>
#include "Exception.h"

using namespace std;

/**
 * A simple Exception, that carries the number of an event for which it was thrown.
 * It inherits from the class Exception and implements its virtual/abstract method
 * error()
 *
 * @brief Exception, that carries an eventnumber
 *
 * @author Stefan
 * @data July 18, 2016
 * @version 0.1
 */
class EventSizeException: public Exception
{
public:
	EventSizeException(int event);
	virtual ~EventSizeException();

	virtual string error();
private:
	int _event;
};

#endif /* EVENTSIZEEXCEPTION_H_ */
