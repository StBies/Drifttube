/*
 * RtRelation.h
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#ifndef RTRELATION_H_
#define RTRELATION_H_

#include "Data.cpp"

class RtRelation: public Data<unsigned double>
{
public:
	RtRelation(std::unique_ptr<std::array<unsigned double,800>> data);
	virtual ~RtRelation();
};

#endif /* RTRELATION_H_ */
