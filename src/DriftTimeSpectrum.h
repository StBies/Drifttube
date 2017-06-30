/*
 * DriftTimeSpectrum.h
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#ifndef DRIFTTIMESPECTRUM_H_
#define DRIFTTIMESPECTRUM_H_

#include <array>
#include <cstdlib>
#include "Data.h"

class DriftTimeSpectrum: public Data<uint32_t>
{
public:
	DriftTimeSpectrum(std::unique_ptr<std::array<uint32_t,800>>,unsigned int nEntries,unsigned int rejected);
	DriftTimeSpectrum(const DriftTimeSpectrum& original);
	virtual ~DriftTimeSpectrum();
	//TODO check if a constructor should be given, where a DataSet is passed and the dt-spect is autocalculating itself - kind of

	unsigned int getEntries() const;
	unsigned int getRejected() const;

	DriftTimeSpectrum& operator=(const DriftTimeSpectrum& rhs);

protected:
	unsigned int m_entries;
	unsigned int m_rejected_evts;
};

#endif /* DRIFTTIMESPECTRUM_H_ */
