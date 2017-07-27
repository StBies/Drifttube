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
