/*
 * DataSet.cpp
 *
 *  Created on: 27.06.2016
 *      Author: stefan
 */

#include "DataSet.h"

DataSet::DataSet(vector<TH1D*> rawData, vector<TH1D*> processedData)
{
	//check if both containers are of the same size, else throw exception
	if(_rawData.size() != _processedData.size())
	{
		throw 's';
	}
	else
	{
		_size = _rawData.size();
	}

	_rawData = rawData;
	_processedData = processedData;
	_integrated = true;
}

DataSet::DataSet(vector<TH1D*> rawData)
{
	_size = rawData.size();
	_rawData = rawData;
	_processedData.resize(_size);
	_integrated = false;
}

DataSet::~DataSet()
{
	// TODO Auto-generated destructor stub
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
void DataSet::addRawData(TH1D* histogram)
{
	_rawData.push_back(histogram);
	_size += 1;
}

//TODO comment
void DataSet::addIntegral(TH1D* histogram)
{
	//TODO size checks
	_processedData.push_back(histogram);
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
int DataSet::getSize()
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
 * @return Pointer to an Event struct containing two TH1D*: rawData and processedData
 *
 * @require event < this.getSize() && this.isIntegrated()
 * @ensure Event != nullptr && Event.rawData != nullptr && Event.processedData != nullptr
 *
 * @warning Throws an exception (char 'r' if the above mentioned requirements are not met
 * @warning Heap object returned, memory management to be done by caller
 *
 */
Event* DataSet::getEvent(int event)
{
	//only give an event if requirements are met, else throw requirement exception
	(event < _size) && _integrated ?  : throw 'r';
	Event* result = new Event();
	result->rawData = _rawData[event];
	result->integratedData = _processedData[event];
	return result;
}


/**
 * Checks, if the DataSet object contains integrated as well as raw data.
 *
 * @brief Checks if integrated
 *
 * @author Stefan
 * @date June 30, 2016
 * @version 0.1
 *
 * @return True if integrated, false else.
 */
bool DataSet::isIntegrated()
{
	return _integrated;
}


