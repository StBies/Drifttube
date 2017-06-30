/*
 * DriftTimeSpectrum.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#include "DriftTimeSpectrum.h"

using namespace std;

/**
 * Constructor of the DriftTimeSpectrum. Initializes an object with passed values.
 *
 * @brief ctor
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 26, 2017
 *
 * @param spect unique_ptr containing a std::array<uint32_t,800> which holds the DriftTimeSpectrum raw-data
 * @param nEntries Total number of entries contained in the spectrum. nEntries = valid entries + rejected entries
 * @param rejected Number of rejected entries. A.k.a entries without drift time found
 */
DriftTimeSpectrum::DriftTimeSpectrum(unique_ptr<array<uint32_t,800>> spect, unsigned int nEntries, unsigned int rejected)
: Data(move(spect))
{
	m_entries = nEntries;
	m_rejected_evts = rejected;
}


/**
 * Copy constructor. Creates a new copy of the DriftTimeSpectrum.
 *
 * @brief Copy ctor
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 26, 2017
 *
 * @param original Reference to the original DriftTimeSpectrum object that is meant to be copied
 */
DriftTimeSpectrum::DriftTimeSpectrum(const DriftTimeSpectrum& original)
: Data(original)
{
	m_entries = original.m_entries;
	m_rejected_evts = original.m_rejected_evts;
}

DriftTimeSpectrum::~DriftTimeSpectrum()
{

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
	return m_rejected_evts;
}

/**
 * Assignment operator, assigns the ride hand side (rhs) to a lhs variable of fitting type.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 30, 2017
 *
 * @param rhs const Reference to the ride hand side value
 * @return Reference to the bound object
 */
DriftTimeSpectrum& DriftTimeSpectrum::operator=(const DriftTimeSpectrum& rhs)
{
	Data::operator=(rhs);
	m_rejected_evts = rhs.m_rejected_evts;
	m_entries = rhs.m_entries;

	return *this;
}

