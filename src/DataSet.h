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
#include <memory>

/**
 * A class representing a DataSet. This is a collection of raw data arrays
 * showing FADC read raw voltage. It contains a vector container that holds as many data arrays as
 * there are triggered events from the FADC.
 * The arrays are stored in the form of unique_ptr type smart pointers (see: http://www.cplusplus.com/reference/memory/unique_ptr/ for example).
 * The smart pointers contain standard library arrays (see: http://www.cplusplus.com/reference/array/array/ ).
 * Unique smart pointers can only exist once. If you want to transfer ownership (e.g pass them around or or store them somewhere) you need to
 * use the move semantics. More on that can be read at http://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c .
 * For its usage see the methods addData(...) in file DataSet.cpp for example.
 *
 * @brief Collection of data
 *
 * @author Stefan Bieschke
 * @date May 12, 2017
 * @version Alpha 2.0
 */
class DataSet
{
public:
	DataSet();
	//TODO the constructor below needs yet to be implemented using smart pointers
//	DataSet(const std::vector<std::unique_ptr<std::array<int,800>>>& data);
	DataSet(const DataSet& original);
	virtual ~DataSet();

	void addData(std::unique_ptr<std::array<int,800> > data);
	unsigned int getSize() const;
	const std::array<int,800>& getEvent(const unsigned int event) const;

//	const DataSet& operator+(const DataSet& data) const;
	const std::array<int,800>& operator[](const unsigned int event) const;

private:
	//standard library vector, that stores unique pointers to the raw data arrays
	std::vector< std::unique_ptr<std::array<int,800> > > m_data;
};

#endif /* DATASET_H_ */
