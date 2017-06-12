#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include <omp.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "DataSet.h"
#include "EventSizeException.h"
#include "globals.h"
#include <array>
#include <memory>
#include "Event.h"

/**
 * A class, that processes raw data measured by an FADC, that comes as
 * *.root files. It offers functions to integrate histograms, calculate
 * time-dependant currents and deposited charge in a drifttube detector.
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
	static int computeIntegral(const Event& data);
	static const std::array<int,800> integrate(const Event& data);
//	static const std::array<uint16_t,800> derivate(const Event& data) const;
//	static std::unique_ptr<DataSet> integrateAll(const DataSet& data) const;
	static unsigned short findMinimumBin(const Event& data);
	static short findDriftTime(const Event& data, unsigned short threshold);
	static unsigned short findLastFilledBin(const Event& data, unsigned short threshold);
	static const std::array<uint16_t,800> calculateDriftTimeSpectrum(const DataSet& data);
//	static const std::array<uint16_t,800> calculateRtRelation(const std::array<int,800>& dtSpect) const;
	static unsigned int countAfterpulses(const DataSet& rawData, const std::array<int,800>& rtRelation);

private:
	DataProcessor();
	~DataProcessor();
};

#endif //DATAPROCESSOR_H_
