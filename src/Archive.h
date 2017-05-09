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
	Archive(std::string filename);
	virtual ~Archive();

	const unsigned int getSize() const;
	const DataSet& getRawData() const;
	const DataSet& getProcessedData() const;
	const std::array<int,800>& getEvent(int event) const;
	const std::array<int,800>& getDrifttimeSpectrum() const;
	const std::array<int,800>& getDtDerivative() const;
	const std::array<int,800>& getRtRelation() const;
	TString getFilename() const;
	TString getDirname() const;

	void setProcessedData(DataSet* data);
	void setDifttimeSpect(const std::array<int,800> spect);
	void setDiffDrifttimeSpect(const std::array<int,800> spect);
	void setRtRelation(const std::array<int,800> data);

private:
	TH1D* convertEntryToHistogram(const unsigned int entry,TTree* tree);
	std::unique_ptr<std::array<int,800> > convertEntry(const unsigned int entry,TTree* tree);

	void convertAllEntriesToHistograms(TTree* tree);
	void convertAllEntries(TTree* tree);
	void writeToFile(TString filename);
	TString parseDir(TString filename);
	TString parseFile(TString filename);

	DataSet _rawData;
	DataSet _processedData;
	std::array<int,800> _drifttimeSpect;
	std::array<int,800> _rtRelation;
	std::array<int,800> _diffDtSpect;
	TString _directory;
	TString _file;
	unsigned int _numberOfEntries;

	bool _dtFilled;
	bool _rtFilled;
	bool _integralsFilled;
	bool _diffDtFilled;
	//bitpattern to store information, if things are set
	char m_filled_bitpattern;
};

#endif /* SRC_ARCHIVE_H_ */
