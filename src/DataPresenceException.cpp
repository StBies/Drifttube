/*
 * DataPresenceException.cpp
 *
 *  Created on: Aug 1, 2016
 *      Author: bieschke
 */

#include "DataPresenceException.h"

DataPresenceException::DataPresenceException()
: Exception()
{

}

DataPresenceException::~DataPresenceException()
{
}

string DataPresenceException::error()
{
	return "Requested data is not present";
}

