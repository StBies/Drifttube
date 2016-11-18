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
#include "DataPresenceException.h"
#include "globals.h"

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

	int getSize() const;
	DataSet* getRawData() const;
	DataSet* getProcessedData() const;
	TH1D* getEvent(int event) const;
	TH1D* getDrifttimeSpectrum() const;
	TH1D* getDtDerivative() const;
	TH1D* getRtRelation() const;
	TString getFilename() const;
	TString getDirname() const;

	void setProcessedData(DataSet* data);
	void setDifttimeSpect(TH1D* spect);
	void setDiffDrifttimeSpect(TH1D* spect);
	void setRtRelation(TH1D* data);

private:
	TH1D* convertEntryToHistogram(int entry,TTree* tree);

	void convertAllEntriesToHistograms(TTree* tree);
	void writeToFile(TString filename);
	TString parseDir(TString filename);
	TString parseFile(TString filename);


	DataSet* _rawData;
	DataSet* _processedData;
	TH1D* _drifttimeSpect;
	TH1D* _rtRelation;
	TH1D* _diffDtSpect;
	TString _directory;
	TString _file;
	int _numberOfEntries;
	bool _dtFilled;
	bool _rtFilled;
	bool _integralsFilled;
	bool _diffDtFilled;
};

#endif /* SRC_ARCHIVE_H_ */
