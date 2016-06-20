/*
 * Archive.h
 *
 *  Created on: 15.06.2016
 *      Author: stefan
 */

#ifndef SRC_ARCHIVE_H_
#define SRC_ARCHIVE_H_

#include <vector>
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

using namespace std;


/**
 * A class that archives processed data and manages writing it to files.
 *
 * @brief Archiving tool
 *
 * @author Stefan
 * @data June 20,2016
 * @version 0.15
 */
class Archive
{
public:
	Archive(TString filename);
	virtual ~Archive();

	vector<TH1*>* getRawData();

private:
	TFile* readFile(TString filename);
	TTree* readTree(TFile* file,TString treename);
	TH1* convertEntryToHistogram(int entry,TTree* tree);
	void storeToFile();

	//TODO maybe a TTree would be better than a vector
	vector<TH1*> _rawData;
	vector<TH1*> _processedData;
};

#endif /* SRC_ARCHIVE_H_ */
