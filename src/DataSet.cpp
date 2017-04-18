/*
 * DataSet.cpp
 *
 *  Created on: 27.06.2016
 *      Author: stefan
 */

#include "DataSet.h"

/**
 * Default constructor, initializes an empty DataSet object containing no raw data and with size 0
 *
 * @author Stefan
 * @date Dec 31, 2016
 * @version 0.1
 */
DataSet::DataSet()
{
	_data.resize(0);
}

/**
 *  Constructor, that takes a vector containing raw data that is stored in a std::array<int,800>
 *
 *  @brief constructor with a data-vector as argument
 *
 *  @author Stefan Bieschke
 *  @date April 10, 2017
 *  @version Alpha 2.0
 */
//TODO might not work this way - at least not as intended. Might not come around working with pointers if copying is not an option
DataSet::DataSet(const std::vector<std::array<int,800>>& data)
{
	_data = data;
}

/**
 * First implementation of a copy constructor for DataSets in order to enable DataSet operators to work
 *
 * @author Stefan
 * @date March 31, 2017
 * @version 0.1
 *
 * @param original Original DataSet object that is to be copied
 */
DataSet::DataSet(const DataSet& original)
{
	//copy size of original DataSet
	//create new vector containing the raw data and go into deep copy of its content
	_data = std::vector<array<int,800>>();
	_data.resize(original.getSize());

	//deep copy
	for(array<int,800> data: original._data)
	{
		_data.push_back(data);
	}
}

/**
 * Destructor of a DataSet, deletes this object as well as all histograms it holds
 *
 * @brief Dtor
 *
 * @author Stefan
 * @date July 02, 2016
 * @version 0.1
 */
DataSet::~DataSet()
{
//	for(array<int,800> data : _data)
//	{
//		delete data;
//	}
}


/**
 * Adds a raw data histogram to the DataSet. Does increment the size of the DataSet as well.
 *
 * @brief Add raw data histogram
 *
 * @author Stefan Bieschke
 * @date April 18, 2017
 * @version Alpha 2.0
 *
 * @param data the histogram to add to the DataSet
 *
 * @require data != nullptr
 * @ensure new size = old size + 1
 */
void DataSet::addData(std::unique_ptr<std::array<int,800> > data)
{
	_data.push_back(data);
}

/**
 * Returns the size of this DataSet.
 *
 * @brief Getter method for size
 *
 * @author Stefan
 * @date June 30, 2016
 * @version 0.1
 *
 * @return Size of the DataSet, 0 if DataSet is empty
 *
 * @ensure size >= 0
 */
unsigned int DataSet::getSize() const
{
	return _data.size();
}


/**
 * Getter method for an event in this DataSet. Returns the raw data as a reference to an std::array, containing the bin entries as
 * integers. Can throw an exception if the requested eventnumber is greater than the
 * total number of events in the DataSet. See warning and requirements for details.
 *
 * @brief Getter method for an event
 *
 * @author Stefan
 * @date April 10, 2017
 * @version Alpha 2.0
 *
 * @param event Number of the requested Event
 *
 * @return Reference to a std::array containing the data for the requested event
 *
 * @require event < this->getSize()
 *
 * @warning Throws an Exception if the above mentioned requirements are not met
 */
const array<int,800>& DataSet::getEvent(const unsigned int event) const
{
	//only give an event if requirements are met, else throw requirement exception
	if(event > getSize())
	{
		throw EventSizeException(event);
	}
	return *(_data[event]);
}

//operators
/**
 * Enables the usage of the binary operator + so that one can write things like:
 * DataSet result = dataset1 + dataset2;
 * with the result containing all the events, that are stored in both, the DataSets one and two.
 *
 * @author Stefan
 * @date March 31, 2017
 * @version 0.1
 *
 *@TODO does not yet work as intended
 * @param data DataSet object on the right hand side of the binary operator
 * @return DataSet object as a const reference
 */
//const DataSet& DataSet::operator+(const DataSet& data) const
//{
//	//TODO fix: operator+ not implemented for std::vector - makes sense...
//	DataSet* result = new DataSet((this->_data + data._data));
//	result->_size = this->_size + data._size;
//	return *result;
//}

/**
 * Implementation of the [] operator enabling on to use the common dataset[event] statement to access individual events
 * of the DataSet object. Note, that the usage of this operator does not allow the caller to do certain things with the returned object:
 * 1. One can not move the returned histogram in memory
 * 2. One can not change the content of the histogram
 * 3. The call may not change the DataSet object for which data shall be accessed
 *
 * @author Stefan
 * @date March 31, 2017
 * @version 0.1
 *
 * @param event Eventnumber to be accessed.
 * @return TH1D type pointer to histogram containing the raw data
 *
 * @TODO check, if the operator works as intended on reference, value and pointer type objects
 * @warning can throw an EventSizeException
 */
const array<int,800>& DataSet::operator[](unsigned int event) const
{
	return getEvent(event);
}

