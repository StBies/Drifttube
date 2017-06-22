/*
 * DriftTimeSpectrum.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#include "DriftTimeSpectrum.h"

using namespace std;

DriftTimeSpectrum::DriftTimeSpectrum(unique_ptr<array<uint32_t,800>> spect, unsigned int nEntries, unsigned int rejected)
: Data(move(spect))
{
	m_entries = nEntries;
	m_rejectec_evts = rejected;
}

DriftTimeSpectrum::~DriftTimeSpectrum()
{
	// TODO Auto-generated destructor stub
}

/**
 * Getter method for the total number of events used to calculate this drift time spectrum.
 * @brief Getter for number of entries
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 22, 2017
 *
 * @return Total number of entries
 */
unsigned int DriftTimeSpectrum::getEntries() const
{
	return m_entries;
}

/**
 * Getter method for the number of rejected events during the calculation of this drift time spectrum. Events get rejected,
 * when they do not pass the threshold given.
 * @brief Getter for number of rejected entries
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 22, 2017
 *
 * @return Number of events that were rejected during calculation
 */
unsigned int DriftTimeSpectrum::getRejected() const
{
	return m_rejectec_evts;
}

