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
	DataSet(vector<TH1D*> rawData);
	DataSet(vector<TH1D*> rawData, vector<TH1D*> processedData);
	virtual ~DataSet();

	void addRawData(TH1D* histogram);
	void addIntegral(TH1D* histogram);
	int getSize();
	bool isIntegrated();
	Event* getEvent(int event);

private:
	int _size;
	bool _integrated;
	vector<TH1D*> _rawData;
	vector<TH1D*> _processedData;
};

#endif /* DATASET_H_ */
