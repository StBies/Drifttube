/*
 * RtRelation.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#include "RtRelation.h"

RtRelation::RtRelation(std::unique_ptr<std::array<unsigned double,800>> data) : Data(data)
{
}

RtRelation::~RtRelation()
{
	// TODO Auto-generated destructor stub
}

