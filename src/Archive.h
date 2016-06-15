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

using namespace std;


/**
 * A class that archives processed data and manages writing it to files.
 *
 * @brief Archiving tool
 *
 * @author Stefan
 * @data June 15,2016
 * @version 0.1
 */
class Archive
{
public:
	Archive();
	virtual ~Archive();

private:
	//TODO maybe a TTree would be better than a vector
	vector<TH1> _processedData;
};

#endif /* SRC_ARCHIVE_H_ */
