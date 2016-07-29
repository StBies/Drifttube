/*
 * Archive.h
 *
 *  Created on: 15.06.2016
 *      Author: stefan
 */

#ifndef SRC_ARCHIVE_H_
#define SRC_ARCHIVE_H_

#include <iostream>
#include <memory>
#include <sstream>
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "DataSet.h"

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
	DataSet* getRawData();
	DataSet* getProcessedData();
	TH1D* getEvent(int event);

	void setProcessedData(DataSet* data);

private:
	TH1D* convertEntryToHistogram(int entry,TTree* tree);

	void convertAllEntriesToHistograms(TTree* tree);
	void writeToFile(TString filename);
	TString parseDir(TString filename);
	TString parseFile(TString filename);


	DataSet* _rawData;
	DataSet* _processedData;
	TString _directory;
	TString _file;
	int _numberOfEntries;
};

#endif /* SRC_ARCHIVE_H_ */
