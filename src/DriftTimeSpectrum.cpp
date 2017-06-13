/*
 * DriftTimeSpectrum.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#include "DriftTimeSpectrum.h"

using namespace std;

DriftTimeSpectrum::DriftTimeSpectrum(unique_ptr<array<uint32_t,800>> spect, unsigned int nEntries) : Data(move(spect))
{
	m_entries = nEntries;
}

DriftTimeSpectrum::~DriftTimeSpectrum()
{
	// TODO Auto-generated destructor stub
}

unsigned int DriftTimeSpectrum::getEntries()
{
	return m_entries;
}

