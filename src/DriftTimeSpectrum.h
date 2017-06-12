/*
 * DriftTimeSpectrum.h
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#ifndef DRIFTTIMESPECTRUM_H_
#define DRIFTTIMESPECTRUM_H_

#include "Data.cpp"

class DriftTimeSpectrum: public Data<uint32_t>
{
public:
	DriftTimeSpectrum(std::unique_ptr<std::array<uint32_t,800>>,unsigned int nEntries);
	virtual ~DriftTimeSpectrum();

	unsigned int getEntries();

protected:
	unsigned int m_entries;
};

#endif /* DRIFTTIMESPECTRUM_H_ */
