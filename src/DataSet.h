/*
 * DataSet.h
 *
 *  Created on: 27.06.2016
 *      Author: stefan
 */

#ifndef DATASET_H_
#define DATASET_H_

#include "EventSizeException.h"
#include <vector>
#include <array>

/**
 * A class representing a DataSet. This is a collection of raw data arrays
 * showing FADC read raw voltage. It contains a vector container that holds as many data arrays as
 * there are triggered events from the FADC.
 *
 * @brief Collection of data
 *
 * @author Stefan Bieschke
 * @date April 6, 2017
 * @version Alpha 2.0
 */
class DataSet
{
public:
	DataSet();
	DataSet(const vector<std::array<int,800>>& data);
	DataSet(const DataSet& original);
	virtual ~DataSet();

	void addData(std::array<int,800>);
	unsigned int getSize() const;
	const std::array<int,800>& getEvent(unsigned int event) const;

//	const DataSet& operator+(const DataSet& data) const;
	const std::array<int,800>& const operator[](int event) const;

private:
	unsigned int _size;
	std::vector<std::array<int,800>> _data;
};

#endif /* DATASET_H_ */
