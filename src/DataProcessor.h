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
	DataProcessor();
	virtual ~DataProcessor();
	
	Double_t getCharge(Int_t resistance);
	
private:
	TH1I* integrate(TH1I& data);
	
	TFile _dataFile;
	TH1D _rawData;
};

#endif //DATAPROCESSOR_H_
