/*
 * DataStreamManager.h
 *
 *  Created on: Aug 4, 2016
 *      Author: bieschke
 */

#ifndef DATASTREAMMANAGER_H_
#define DATASTREAMMANAGER_H_

#include "Archive.h"
#include "DataProcessor.h"


/**
 * Class for managing the programs workflow from startup to organizing when which data is passed from
 * one object to another. Manages writing everything to ROOT files at finalization as well.
 *
 * Note that this class is experimental and not finished.
 *
 * @author Stefan
 * @version 0.0000001
 * @date April 4, 2017
 */
class DataStreamManager
{
public:
	DataStreamManager();
	virtual ~DataStreamManager();

//	bool startup(std::string dataPath);
//	bool writeToRootFile(Archive& archive);
//
//private:
//	Archive _archive;
//	DataProcessor _processor;
};

#endif /* DATASTREAMMANAGER_H_ */
