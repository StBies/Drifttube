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
	m_data.resize(0);
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
//TODO Overhaul this constructor to work with unique_pointers
//DataSet::DataSet(const std::vector<unique_ptr<array<int,800>>>& data)
//{
//	m_data = data;
//}

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
	m_data = std::vector<unique_ptr<array<int,800>>>(original.getSize());

	//deep copy
	//note: range based for (aka for each) does not work, since that would be a copy of the unique pointer
//	for(unique_ptr<array<int,800>> data: original.m_data)
	for(int j = 0; j < original.m_data.size(); j++)
	{

		unique_ptr<array<int,800>> temp(new array<int,800>());

		//copy contents of the unique pointer to the contents of the new temp heap object
		for(int i = 0; i < original.m_data[j]->size(); i++)
		{
			(*temp)[i] = (*original.m_data[j])[i];
		}
		m_data.push_back(move(temp));
	}
}

/**
 * Destructor of a DataSet, deletes this object as well as all data it holds
 *
 * @brief Dtor
 *
 * @author Stefan Bieschke
 * @date May 12, 2017
 * @version Alpha 2.0.1
 */
DataSet::~DataSet()
{
	/*
	 * Meant to be empty, the unique_ptr's in the stored vector are automatically deleted when the vector goes out of scope,
	 * so when this DataSet object is deleted
	 */
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
	//move the ownership of the data array to the vector m_data, that should finally store it
	m_data.push_back(move(data));
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
	return m_data.size();
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
	return *(m_data[event]);
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
 * @TODO does not yet work as intended
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
const array<int,800>& DataSet::operator[](const unsigned int event) const
{
	return getEvent(event);
}

