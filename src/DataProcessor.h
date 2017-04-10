#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include <omp.h>
#include <iostream>
#include <sstream>
#include <cmath>
#include "DataSet.h"
#include "EventSizeException.h"
#include "TFile.h"
#include "TTree.h"
#include "globals.h"
#include <array>
#include <memory>

/**
 * A class, that processes raw data measured by an FADC, that comes as
 * *.root files. It offers functions to integrate histograms, calculate
 * time-dependant currents and deposited charge in a drifttube detector.
 * Makes use of ROOT libraries by CERN.
 * 
 * @brief Processes data from FADC.
 *
 * @author Stefan
 * @date April 10, 2017
 * @version Alpha 2.0
 */
class DataProcessor
{
public:
	DataProcessor();
	virtual ~DataProcessor();
	
	int computeIntegral(const std::array<int,800>& data) const;
	const std::array<int,800> integrate(const std::array<int,800>& data) const;
	const std::array<int,800> derivate(const std::array<int,800>& data) const;
	std::unique_ptr<DataSet> integrateAll(const DataSet& data) const;
	int findMinimumBin(const std::array<int,800>& data) const;
	int findDriftTime(const std::array<int,800>& data, double threshold) const;
	int findLastFilledBin(const std::array<int,800>& data, double threshold) const;
	const std::array<int,800> calculateDriftTimeSpectrum(const DataSet& data) const;
	const std::array<int,800> calculateRtRelation(const std::array<int,800>& dtSpect) const;
	int countAfterpulses(const DataSet& rawData, const TH1D& rtRelation) const;
	int countAfterpulsesDt(const DataSet& rawData, const TH1D& rtRelation) const;

	void calibrate(TString triggerDataFile);
	void writeResults(const DataSet& raw, const DataSet& integrated, const TString filename, const TString dirname) const;
};

#endif //DATAPROCESSOR_H_
