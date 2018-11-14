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
#include <fstream>
#include "DataSet.h"
#include "DataPresenceException.h"
#include "globals.h"
#include "Drifttube.h"

using namespace std;

//TODO Change all doc to vector and variable length (Nov. 14, 2018)

/**
 * Struct containing parameters that are read from the header of the binary file containing raw data.
 * This contains the number of drift tubes, the number of events per tube and the size of one event (number of data points.
 *
 * @brief Parameters from header of binary file
 *
 * @author Stefan Bieschke
 * @date August 1, 2017
 * @version Alpha 2.0
 */
typedef struct
{
	size_t nTubes;
	size_t eventSize;
	size_t nEvents;
	streampos endOfHeader;
}FileParams;

/**
 * A class that archives processed data and manages writing it to files.
 *
 * @brief Archiving tool
 *
 * @author Stefan Bieschke
 * @date July 12, 2017
 * @version Alpha 2.0
 */
class Archive
{
public:
	Archive(const std::string filename);
	~Archive();

	const std::string& getFilename() const;
	const std::string& getDirname() const;
	const std::vector<std::unique_ptr<Drifttube>>& getTubes() const;
	void writeToFile(const std::string& filename);

private:
	void convertAllEntries(const std::string filename);
	std::string parseDir(const std::string filename);
	std::string parseFile(const std::string filename);
	FileParams readHeader(ifstream& file);


	std::vector<unique_ptr<Drifttube>> m_tubes;
	std::string m_directory;
	std::string m_file;
};

#endif /* SRC_ARCHIVE_H_ */
