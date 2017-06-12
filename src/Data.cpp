/*
 * data.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#include "Data.h"

using namespace std;

/**
 * Constructor of the Data class. This is protected, Data objects are not meant to be instanciated. This transferres ownership of the passed
 * array to the member m_data.
 *
 * @brief Constructor (protected)
 *
 * @author Stefan Bieschke
 * @date June 12, 2017
 * @version Alpha 2.0
 *
 * @param data unique pointer to the array that shall be stored
 *
 * @warning Protected: should only be called by inheriting class' constructors
 */
Data::Data(unique_ptr<array<uint16_t,800>> data)
{
	m_data = move(data);
}


/**
 * Copy constructor of the Data class. This is protected, Data objects are not meant to be instanciated. This copies a Data object
 *
 * @author Stefan Bieschke
 * @date June 12, 2017
 * @version Alpha 2.0
 *
 * @param data constant reference to the object that should be copied
 *
 * @warning Protected: should only be called by inheriting class' constructors
 */
Data::Data(const Data& data)
{
	unique_ptr<array<uint16_t,800>> temp(new array<uint16_t,800>);

	//deep copy
	for(int i = 0; i < data.m_data->size(); i++)
	{
		(*temp)[i] = (*data.m_data)[i];
	}

	m_data = move(temp);
}

/**
 * Virtual destructor. Does nothing, object tears down automatically when out of scope.
 */
Data::~Data()
{
}

/**
 * Getter method for the data. This returns a reference to the data array that is contained within an object of this class.
 * @brief Getter for the data
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 *
 *
 * @return Reference to the array that is contained
 */
const std::array<uint16_t,800>& Data::getData() const
{
	return *m_data;
}

/**
 * Implementation of the bracket operator [] commonly used to address a single element of an array. This returns a reference to the requested
 * value so that data[i] can be used as lvalue.
 *
 * @brief Bracket operator for addressing
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 *
 * @param bin
 * @return Content of the requested bin. This is a reference to the content so that it can be used as lvalue. E.g. data[i] = 5;
 */
uint16_t& Data::operator[](const unsigned short bin) const
{
	if(bin < m_data->size())
	{
		return (*m_data)[bin];
	}
	else
	{
		//TODO throw an Exception for index out of bounds
		throw 'i';
	}
}

