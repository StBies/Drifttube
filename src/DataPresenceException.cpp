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
	// TODO Auto-generated constructor stub

}

DataPresenceException::~DataPresenceException()
{
	// TODO Auto-generated destructor stub
}

string DataPresenceException::error()
{
	return "Requested data is not present";
}

