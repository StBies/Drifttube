#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include <omp.h>
#include <iostream>
#include <sstream>
#include <cmath>
#include "TH1D.h"
#include "DataSet.h"
#include "EventSizeException.h"
#include "TFile.h"
#include "TTree.h"
#include "globals.h"

/**
 * A class, that processes raw data measured by an FADC, that comes as
 * *.root files. It offers functions to integrate histograms, calculate
 * time-dependant currents and deposited charge in a drifttube detector.
 * Makes use of ROOT libraries by CERN.
 * 
 * @brief Processes data from FADC.
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
	
	Double_t computeIntegral(const TH1D& data) const;
	Double_t computeIntegral(const TH1D& data, const unsigned short from, const unsigned short to) const;
	TH1D* integrate(TH1D* data) const;
	TH1D* derivate(TH1D* data) const;
	DataSet* integrateAll(DataSet* data) const;
	int findMinimumBin(TH1D* data) const;
	int findDriftTime(const TH1D& data, double threshold) const;
	int findLastFilledBin(const TH1D& data, double threshold) const;
	TH1D* calculateDriftTimeSpectrum(DataSet* data) const;
	TH1D* calculateRtRelation(TH1D& dtSpect) const;
	int countAfterpulses(const DataSet& rawData, const TH1D& rtRelation) const;
	int countAfterpulsesDt(const DataSet& rawData, const TH1D& rtRelation) const;

	void calibrate(TString triggerDataFile);
	void writeResults(const DataSet& raw, const DataSet& integrated, const TString filename, const TString dirname) const;
};

#endif //DATAPROCESSOR_H_
