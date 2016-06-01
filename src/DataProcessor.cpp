#include "DataProcessor.h"

/**
 * Constructor, initializes the DataProcessor object.
 * 
 * @brief Constructor
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1
 * 
 * @param filename relative path to the .root-file containing the raw data
 */
DataProcessor::DataProcessor(TString filename)
{
	_dataFile = new TFile(filename,"read");
	_rawData = createTestHist();
}


/**
 * Destructor, frees the allocated memory for this object, when it is
 * destroyed
 * 
 * @brief Destructor
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1
 */
DataProcessor::~DataProcessor()
{
	
}

TH1I DataProcessor::createTestHist()
{
	TH1I* hist = new TH1I("test","test1",100,0,1);
	for(double d = 0.0; d <= 1; d += 0.1)
	{
		hist->Fill(d);
	}
	return *hist;
}


/**
 * Getter method for the raw data histogram. Returns a copy of the TH1I type
 * object.
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1
 * 
 * @return Copy of the TH1I type histogram object
 * 
 * @warning Does only return a COPY of the original histogram. Later changes
 * will not be included in this copy.
 */
TH1I DataProcessor::getRawData()
{
	return _rawData;
}
