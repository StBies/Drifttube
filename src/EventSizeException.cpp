/*
 * EventSizeException.cpp
 *
 *  Created on: 02.07.2016
 *      Author: stefan
 */

#include "EventSizeException.h"

EventSizeException::EventSizeException(int event)
: Exception()
{
	_event = event;
}

EventSizeException::~EventSizeException()
{
}

string EventSizeException::error()
{
	stringstream error;
	error << "EventSizeException: Event " << _event << "does not match size of DataSet.";
	return error.str();
}
