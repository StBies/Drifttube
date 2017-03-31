/*
 * DataSet.h
 *
 *  Created on: 27.06.2016
 *      Author: stefan
 */

#ifndef DATASET_H_
#define DATASET_H_

#include "TH1D.h"
#include "EventSizeException.h"
#include <vector>

using namespace std;

/**
 * A class representing a DataSet. This is a collection of many histograms
 * showing FADC read raw voltage.
 *
 * @brief Collection of data
 *
 * @author Stefan
 * @date July 2, 2016
 * @version 0.3
 */
class DataSet
{
public:
	DataSet();
	DataSet(vector<TH1D*>* data);
	DataSet(const DataSet& original);
	virtual ~DataSet();

	void addData(TH1D* histogram);
	int getSize() const;
	TH1D* getEvent(int event) const;

//	DataSet& operator+(DataSet& data) const;

private:
	int _size;
	vector<TH1D*> _data;
};

#endif /* DATASET_H_ */
