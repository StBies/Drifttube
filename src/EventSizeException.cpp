/*
 * EventSizeException.cpp
 *
 *  Created on: 02.07.2016
 *      Author: stefan
 */

#include "EventSizeException.h"

/**
 * Constructor, initializes the EventSizeException with the number of the event, this
 * exception was thrown at.
 *
 * @brief Ctor
 *
 * @author Stefan
 * @data July 18, 2016
 * @version 0.1
 *
 * @param event Number of the event, this exception was thrown for
 */
EventSizeException::EventSizeException(int event)
: Exception()
{
	_event = event;
}

EventSizeException::~EventSizeException()
{
}

/**
 * Gives an error-description containing the eventnumber, for that this exception was
 * thrown.
 * Implementation of abstract method error() from abstract class Exception.
 *
 * @brief Gives errormessage
 *
 * @author Stefan
 * @data July 18, 2016
 * @version 0.1
 *
 * @return error as a string
 *
 */
string EventSizeException::error()
{
	stringstream error;
	error << "EventSizeException: Event " << _event << "does not match size of DataSet.";
	return error.str();
}
