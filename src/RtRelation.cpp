/*
 * RtRelation.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#include "RtRelation.h"

using namespace std;

/**
 * Constructor. Initializes a RtRelation object.
 *
 * @brief ctor
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 26, 2017
 *
 * @param data unique_ptr to an std::array<double,800> containing the rt-relation raw data.
 */
RtRelation::RtRelation(unique_ptr<array<double,800>> data) : Data(move(data))
{

}


/**
 * Copy constructor. Initializes a copy of a passed RtRelation object.
 *
 * @brief copy ctor
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 26, 2017
 *
 * @param original
 */
RtRelation::RtRelation(const RtRelation& original) : Data(original)
{

}

/**
 * dtor. Doing nothing special.
 *
 * @brief dtor
 */
RtRelation::~RtRelation()
{
}

//TODO comment
//TODO test
RtRelation& RtRelation::operator=(const RtRelation& rhs)
{
	Data::operator=(rhs);

	return *this;
}

