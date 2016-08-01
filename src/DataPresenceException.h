/*
 * DataPresenceException.h
 *
 *  Created on: Aug 1, 2016
 *      Author: bieschke
 */

#ifndef DATAPRESENCEEXCEPTION_H_
#define DATAPRESENCEEXCEPTION_H_

#include "Exception.h"

using namespace std;

class DataPresenceException: public Exception
{
public:
	DataPresenceException();
	virtual ~DataPresenceException();

	string error();
};

#endif /* DATAPRESENCEEXCEPTION_H_ */
