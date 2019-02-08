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


/**
 * Basic implementation of an exception that can be thrown. Note, that this class
 * Exception is abstract/pure virtual and can not be constructed. However, other exceptions
 * such as EventSizeException can extend this exception by implementing the method
 * error()
 *
 * @brief Abstract exception
 *
 * @author Stefan
 * @date July 18, 2016
 * @version 1.0
 *
 * @warning Abstract class, can not create objects of dynamic type Exception.
 */
class Exception
{
public:
//	Exception();
//	virtual ~Exception() = 0;
	virtual string error() = 0;
};

#endif /* EXCEPTION_H_ */
