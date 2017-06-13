/*
 * data.h
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#ifndef DATA_H_
#define DATA_H_

#include <array>
#include <memory>
#include <cstdlib>


//TODO update comment
/**
 * A class for basic data types. This is basically a wrapper for arrays of any primitive data type values. The Data class is a base class for Events and drift time spectra.
 *
 * @brief Data class, wrapper for arrays
 *
 * @author Stefan Bieschke
 * @date June 12, 2017
 * @version Alpha 2.0
 *
 * @warning This is not meant for instantiation.
 */
template<typename T = uint16_t>
class Data
{
public:
	virtual ~Data();

	const std::array<T,800>& getData() const;
	T& operator[](const unsigned short bin) const;

protected:
	//TODO check, if constructor should be private instead of protected
	Data(std::unique_ptr<std::array<T,800>> data); //not meant for instantiation
	Data(const Data& data);
	std::unique_ptr<std::array<T,800>> m_data;
};

#endif /* DATA_H_ */

using namespace std;

/**
 * Constructor of the Data class. This is protected, Data objects are not meant to be instantiated. This transferres ownership of the passed
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
template<typename T>
Data<T>::Data(unique_ptr<array<T,800>> data)
{
	m_data = move(data);
}


/**
 * Copy constructor of the Data class. This is protected, Data objects are not meant to be instantiated. This copies a Data object
 *
 * @author Stefan Bieschke
 * @date June 12, 2017
 * @version Alpha 2.0
 *
 * @param data constant reference to the object that should be copied
 *
 * @warning Protected: should only be called by inheriting class' constructors
 */
template<typename T>
Data<T>::Data(const Data& data)
{
	unique_ptr<array<T,800>> temp(new array<T,800>);

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
template<typename T>
Data<T>::~Data()
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
template<typename T>
const std::array<T,800>& Data<T>::getData() const
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
template<typename T>
T& Data<T>::operator[](const unsigned short bin) const
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
