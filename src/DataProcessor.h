#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include "TH1I.h"
#include "TFile.h"

/**
 * A class, that processes raw data measured by an FADC, that comes as
 * *.root files. It offers functions to integrate histograms, calculate
 * time-dependant currents and deposited charge in a drifttube detector.
 * Makes use of ROOT libraries by CERN.
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1 
 */
class DataProcessor
{
public:
	DataProcessor(TString filename);
	virtual ~DataProcessor();
	
	Double_t getCharge(Int_t resistance);

	TH1D getRawData();
	
private:
	TH1D* integrate(TH1D& data);
	//For testing during development only
	TH1D createTestHist();
	
	//member variables
	TFile* _dataFile;
	TH1D _rawData;
};

#endif //DATAPROCESSOR_H_
