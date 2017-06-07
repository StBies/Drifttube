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
 * Constructor that takes a vector containing raw data that is stored in a Event objects.
 * This method passes ownership of the unique pointers in the vector, that is passed as argument to the constructed DataSet.
 * Please note that the passed vector WILL be changed during the execution of this method. It will do the following:
 *  1.) Pass ownership of the unique pointers in the vector to the DataSet member m_data and overwrite with nullpointers
 *  2.) Resize the passed vector data to a size of 0 and clear the contents
 *
 *
 * @brief constructor with a data-vector as argument
 *
 * @author Stefan Bieschke
 * @date May 15, 2017
 * @version Alpha 2.0
 *
 * @param data Reference to a vector containing unique pointers to Events. Those will be transferred to the DataSet member.
 *
 * @warning After finishing of this method, the passed vector will be empty and size 0, if it lives on heap, it needs to be deleted manually.
 */
DataSet::DataSet(vector<unique_ptr<Event>>& data)
{
//TODO Think about design of this method, might be unclear to caller, what happens here.
//TODO Excessive testing needed.
	size_t size = data.size();
	m_data.resize(size);

	#pragma omp parallel for
	for(int i = 0; i < size; i++)
	{
		m_data[i] = move(data[i]);
	}
	data.clear();
	data.resize(0);
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
	m_data = std::vector<unique_ptr<Event>>(original.getSize());

	//deep copy
	//note: range based for (aka for each) does not work, since that would be a copy of the unique pointer
	for(int i = 0; i < original.getSize(); i++)
	{
		unique_ptr<Event> temp(new Event(original[i]));

		m_data[i] = move(temp);
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
 * Adds an Event to the DataSet. Does increment the size of the DataSet as well.
 *
 * @brief Add Event to the DataSet
 *
 * @author Stefan Bieschke
 * @date April 18, 2017
 * @version Alpha 2.0
 *
 * @param data the Event to add to the DataSet
 *
 * @require data != nullptr
 * @ensure new size = old size + 1
 */
void DataSet::addData(std::unique_ptr<Event> data)
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
size_t DataSet::getSize() const
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
const Event& DataSet::getEvent(const unsigned int event) const
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
 * Implementation of the [] operator enabling on to use the common DataSet[event] statement to access individual events
 * of the DataSet object. Note, that the usage of this operator does not allow the caller to do certain things with the returned object:
 * 1. One can not move the returned histogram in memory
 * 2. One can not change the content of the histogram
 * 3. The call may not change the DataSet object for which data shall be accessed
 *
 * @author Stefan
 * @date March 31, 2017
 * @version Alpha 2.0
 *
 * @param event Eventnumber to be accessed.
 * @return array containing the raw data as reference
 *
 * @warning can throw an EventSizeException
 */
//TODO check, if the operator works as intended on reference, value and pointer type objects
const Event& DataSet::operator[](const unsigned int event) const
{
	return getEvent(event);
}

