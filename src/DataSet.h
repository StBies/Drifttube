/*
 * DataSet.h
 *
 *  Created on: 27.06.2016
 *      Author: stefan
 */

#ifndef DATASET_H_
#define DATASET_H_

#include "TH1D.h"
#include <vector>

typedef struct
{
	TH1D* rawData;
	TH1D* integratedData;
} Event;

using namespace std;

/**
 * A class representing a DataSet. This is a collection of many histograms
 * showing FADC read raw voltage.
 *
 * @brief Collection of data
 *
 * @author Stefan
 * @date June 27, 2016
 * @version 0.2
 */
class DataSet
{
public:
	DataSet();
	virtual ~DataSet();

private:
	int _size;
	vector<TH1D*> _rawData;
	vector<TH1D*> _processedData;
};

#endif /* DATASET_H_ */
