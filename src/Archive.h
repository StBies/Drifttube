/*
 * Archive.h
 *
 *  Created on: 15.06.2016
 *      Author: stefan
 */

#ifndef SRC_ARCHIVE_H_
#define SRC_ARCHIVE_H_

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>
#include <sstream>

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

	int getSize();
	TH1** getRawData();
	TH1** getProcessedData();
	TH1* getEvent(int event);

private:
	TFile* readFile(TString filename);
	TTree* readTree(TFile* file,TString treename);
	TH1* convertEntryToHistogram(int entry,TTree* tree);

	void convertAllEntriesToHistograms(TTree* tree);
	void writeToFile(TString filename);

	TH1D* createTestHist();

	//TODO maybe a TTree would be better than a vector
	TH1** _rawData;
	TH1** _processedData;
	int _numberOfEntries;
};

#endif /* SRC_ARCHIVE_H_ */
