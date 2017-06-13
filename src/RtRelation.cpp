/*
 * RtRelation.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#include "RtRelation.h"

RtRelation::RtRelation(std::unique_ptr<std::array<double,800>> data) : Data(move(data))
{

}

RtRelation::RtRelation(const RtRelation& original) : Data(original)
{

}

RtRelation::~RtRelation()
{
	// TODO Auto-generated destructor stub
}

