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
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "DataSet.h"
#include "DataPresenceException.h"
#include "globals.h"
#include "Drifttube.h"

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
	~Archive();

	const size_t getSize() const;
	TString getFilename() const;
	TString getDirname() const;

	void setProcessedData(const DataSet& data);

private:
//	TH1D* convertEntryToHistogram(const unsigned int entry,TTree* tree);
//	std::unique_ptr<std::array<int,800> > convertEntry(const unsigned int entry,TTree* tree);
//
//	void convertAllEntriesToHistograms(TTree* tree);
//	void convertAllEntries(TTree* tree);
	void writeToFile(string filename);
	string parseDir(string filename);
	string parseFile(string filename);


	std::vector<Drifttube> m_tubes;
	TString m_directory;
	TString m_file;
	unsigned int m_numberOfEntries;

	//bitpattern to store information, if things are set
	char m_filled_bitpattern;
};

#endif /* SRC_ARCHIVE_H_ */
