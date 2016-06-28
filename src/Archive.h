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
#include <cmath>
#include "omp.h"
#include <cstdlib>

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
	TH1D** getRawData();
	TH1D** getProcessedData();
	TH1D* getEvent(int event);

private:
	TFile* readFile(TString filename);
	TTree* readTree(TFile* file,TString treename);
	TH1D* convertEntryToHistogram(int entry,TTree* tree);

	void convertAllEntriesToHistograms(TTree* tree);
	void writeToFile(TString filename);
	TString* parseDir(TString filename);
	TString* parseFile(TString filename);

	TH1D* createTestHist();

	TH1D** _rawData;
	TH1D** _processedData;
	TString* _directory;
	TString* _file;
	int _numberOfEntries;
};

#endif /* SRC_ARCHIVE_H_ */
