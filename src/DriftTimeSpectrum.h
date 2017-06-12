/*
 * DriftTimeSpectrum.h
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#ifndef DRIFTTIMESPECTRUM_H_
#define DRIFTTIMESPECTRUM_H_

#include "Data.cpp"

class DriftTimeSpectrum: public Data<>
{
public:
	DriftTimeSpectrum(std::unique_ptr<std::array<uint16_t,800>>,unsigned int nEntries);
	~DriftTimeSpectrum();

	unsigned int getEntries();

protected:
	unsigned int m_entries;
};

#endif /* DRIFTTIMESPECTRUM_H_ */
