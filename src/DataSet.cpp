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
	_size = 0;
}

/**
 *  Constructor, that takes a vector containing raw data in TH1D* format
 *
 *  @author Stefan
 *  @date Dec 31, 2016
 *  @version 0.2
 */
DataSet::DataSet(vector<TH1D*>* data)
{
	_size = data->size();
	_data = *data;
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
	_size = original._size;
	//create new vector containing the raw data and go into deep copy of its content
	_data = vector<TH1D*>();

	//deep copy
	for(TH1D* data: original._data)
	{
		_data.push_back(new TH1D(*data));
	}

	_data.shrink_to_fit();
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
	for(TH1D* data : _data)
	{
		delete data;
	}
}


/**
 * Adds a raw data histogram to the DataSet. Does increment the size of the DataSet as well
 *
 * @brief Add raw data histogram
 *
 * @author Stefan
 * @date June 30, 2016
 * @version 0.1
 *
 * @param histogram the histogram to add to the DataSet
 *
 * @require histgram != nullptr
 * @ensure new size = old size + 1
 */
void DataSet::addData(TH1D* histogram)
{
	_data.push_back(histogram);
	_size++;
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
int DataSet::getSize() const
{
	return _size;
}


/**
 * Getter method for an Event in this DataSet. Returns a pointer to an Event struct
 * containing both the raw, as well as the integrated Data for an Event with number
 * event. Can throw an exception if the requested eventnumber is greater than the
 * total number of events in the DataSet. See warning and requirements for details.
 *
 * @brief Getter method for an event
 *
 * @author Stefan
 * @date June 30, 2016
 * @version 0.1
 *
 * @param event Number of the requested Event
 *
 * @return Pointer to a TH1D containing the data
 *
 * @require event < this.getSize()
 *
 * @warning Throws an Exception if the above mentioned requirements are not met
 * @warning Heap object returned, memory management to be done by caller
 *
 */
TH1D* DataSet::getEvent(int event) const
{
	//only give an event if requirements are met, else throw requirement exception
	if(event > _size)
	{
		throw EventSizeException(event);
	}
	return _data[event];
}

//operators
/**
 * Enables the usage of the binary operator + so that one can write thinks like:
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
 *@TODO check, if the operator works as intended on reference, value and pointer type objects
 * @warning can throw an EventSizeException
 */
const TH1D*  const DataSet::operator[](int event) const
{
	return this->getEvent(event);
}

