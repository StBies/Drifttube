/*
 * Exception.h
 *
 *  Created on: 02.07.2016
 *      Author: stefan
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>

using namespace std;

class Exception
{
public:
	Exception();
	virtual ~Exception();
	virtual string error() = 0;
};

#endif /* EXCEPTION_H_ */
