/*
 * RtRelation.h
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#ifndef RTRELATION_H_
#define RTRELATION_H_

#include "Data.h"
#include "globals.h"
#include <iostream>

//TODO Change all doc to vector and variable length (Nov. 14, 2018)


//TODO Add class doc
class RtRelation: public Data<double>
{
public:
	RtRelation(std::unique_ptr<std::vector<double>> data);
	RtRelation(const RtRelation& original);
	virtual ~RtRelation();

	RtRelation& operator=(const RtRelation& rhs);
};

#endif /* RTRELATION_H_ */
