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

//TODO Change all doc to vector and variable length (Nov. 14, 2018)


//TODO add class Doc
class DriftTimeSpectrum: public Data<uint32_t>
{
public:
	DriftTimeSpectrum(std::unique_ptr<std::vector<uint32_t>>,unsigned int nEntries,unsigned int rejected);
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
