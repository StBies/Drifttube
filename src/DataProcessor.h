#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

//forward declaration needed due to ring inclusion
class Event;
class DataSet;
class Drifttube;

#include <omp.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "Drifttube.h"
#include "DataSet.h"
#include "EventSizeException.h"
#include "globals.h"
#include <array>
#include <memory>
#include "Event.h"
#include "RtRelation.h"
#include "DriftTimeSpectrum.h"

//TODO Change all doc to vector and variable length (Nov. 14, 2018)

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
	//TODO implement functions for Event as parameter as Template Data<typename T>
	static int computeIntegral(const Event& data);
	static const std::vector<int> integrate(const Event& data);
	static const std::vector<int> integrate(const Event& data, const uint16_t error);
	static const std::vector<int> integrate(const vector<uint16_t>& data);
	static const std::vector<int> integrate(const vector<uint16_t>& data, const uint16_t error);
//	static const std::array<uint16_t,800> derivate(const Event& data) const;
//	static std::unique_ptr<DataSet> integrateAll(const DataSet& data) const;
	static unsigned short findMinimumBin(const Event& data);
	static short findDriftTimeBin(const Event& data, unsigned short threshold);
	static unsigned short findLastFilledBin(const Event& data, unsigned short threshold);
	static const DriftTimeSpectrum calculateDriftTimeSpectrum(const DataSet& data);
	static const RtRelation calculateRtRelation(const DriftTimeSpectrum& dtSpect);
	static const std::vector<std::array<uint16_t,2>> pulses_over_threshold(const Event& data, unsigned short threshold);
	static const std::vector<std::array<uint16_t,2>> pulses_over_threshold(const Event& data, unsigned short threshold, size_t from, size_t to);
	static const std::vector<uint16_t> time_over_threshold(const std::vector<array<uint16_t,2>>& pulses);
	static const unsigned int countAfterpulses(const Drifttube& tube);

private:
	DataProcessor();
	~DataProcessor();
};

#endif //DATAPROCESSOR_H_
