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


/**
 * A class for basic data types. This is basically a wrapper for arrays of any primitive data type values. The Data class is a base class for Events and drift time spectra.
 * If no typename is provided, the default is @c uint16_t. So if you plan to build your own class, let's call it @c Child for now, that inherits
 * from Data and you want it to carry @c uint16_t type data you can simply write the following:
 * @code{.cpp}
 * class Child : public Data<>
 * { ... }
 * @endcode
 * A class that inherits from Data and holds @c double type numbers would be declared as:
 * @code{.cpp}
 * class DoubleChild : public Data<double>
 * { ... }
 * @endcode
 *
 * @brief Data class, wrapper for arrays
 *
 * @author Stefan Bieschke
 * @date June 12, 2017
 * @version Alpha 2.0
 *
 * @warning Abstract class, cannot be instantiated.
 */
template<typename T = uint16_t>
class Data
{
public:
	virtual ~Data() = 0;

	const std::array<T,800>& getData() const;
	T& operator[](const unsigned short bin);
	const T& operator[](const unsigned short bin) const;
	std::unique_ptr<std::array<double,800>> normalized() const;


protected:
	Data<T>& operator=(const Data<T>& rhs);
	Data(std::unique_ptr<std::array<T,800>> data); //not meant for instantiation
	Data(const Data<T>& data);

	std::unique_ptr<std::array<T,800>> m_data;
};


using namespace std;

/**
 * Constructor of the Data class. This is protected, Data objects are not meant to be instantiated. This transfers ownership of the passed
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
Data<T>::Data(const Data<T>& data)
{
	unique_ptr<array<T,800>> temp(new array<T,800>);

	//deep copy
	#pragma omp parallel for
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
const array<T,800>& Data<T>::getData() const
{
	return *m_data;
}

/**
 * Implementation of the bracket operator [] commonly used to address a single element of an array. This returns a reference to the requested
 * value so that @c ata[i] can be used as lvalue.
 *
 * @brief Bracket operator for addressing
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 *
 * @param bin
 * @return Content of the requested bin. This is a reference to the content so that it can be used as lvalue. E.g. @c data[i] = 5;
 */
template<typename T>
const T& Data<T>::operator[](const unsigned short bin) const
{
	return (*m_data)[bin];
}

/**
 * Assignment operator. The Data<T> object on the right hand side (rhs) gets assigned to the left hand side (lhs) Data<T> object. Due to the
 * constness of the parameter rhs, temporary objects can be assigned to non temporary lvalues.
 *
 * @brief assignment operator
 *
 * @author Stefan Bieschke
 * @date August 1, 2017
 * @version Alpha 2.0
 *
 * @param rhs const reference to the Data<T> object on the right hand side of the assignment
 * @return Reference to the lhs object - basically the adress that the lhs is assigned to
 */
template<typename T>
Data<T>& Data<T>::operator=(const Data<T>& rhs)
{
	for(size_t i = 0; i < m_data->size(); i++)
	{
		(*m_data)[i] = (*rhs.m_data)[i];
	}
	return *this;
}

/**
 * Computes a normalized version of this Data object. This method does not change the calling data object but computes the normalized version and
 * returns it in the form of an @c array<double,800> in a @c unique_ptr.
 *
 * @brief Compute normalized data
 *
 * @author Stefan Bieschke
 * @date July 27, 2017
 * @version Alpha 2.0
 *
 * @return @c std::unique_ptr<std::array<double,800>> containing the normalized data
 */
template<typename T>
unique_ptr<array<double,800>> Data<T>::normalized() const
{
	unique_ptr<array<double,800>> result(new array<double,800>);
	//TODO include DataProcessor and let it perform the integration

	double integral = 0;
//	#pragma omp parallel for reduction(+,integral)
	for(size_t i = 0; i < m_data->size(); i++)
	{
		integral += (*m_data)[i];
	}

	for(size_t i = 0; i < m_data->size(); i++)
	{
		(*result)[i] = (*this)[i] / integral;
	}

	return move(result);
}

/**
 * Implementation of the bracket operator [] commonly used to address a single element of an array. This returns a reference to the requested
 * value so that @c data[i] can be used as lvalue.
 *
 * @brief Bracket operator for addressing
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @date May 15, 2017
 *
 * @param bin Number of the bin as a @c size_t, which is basically an unsigned integer
 * @return Content of the requested bin. This is a reference to the content so that it can be used as lvalue. E.g. @c data[i] = 5;
 */
template<typename T>
T& Data<T>::operator[](const unsigned short bin)
{
	return (*m_data)[bin];
}

#endif /* DATA_H_ */
