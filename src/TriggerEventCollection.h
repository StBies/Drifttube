/*
 * TriggerEventCollection.h
 *
 *  Created on: Aug 28, 2017
 *      Author: bieschke
 */

#include <vector>
//#include <stdint.h>
#include "Drifttube.h"

#ifndef TRIGGEREVENTCOLLECTION_H_
#define TRIGGEREVENTCOLLECTION_H_

/**
 * A triggersignal results in the readout of a raw voltage signal for each drift tube channel on the FADC.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date September 1, 2017
 */
class TriggerEventCollection
{
public:
	TriggerEventCollection();
	~TriggerEventCollection();

private:
	//TODO don't store raw pointers here
	std::vector<Drifttube*> m_tubes;
	unsigned int m_triggerID;
};

#endif /* TRIGGEREVENTCOLLECTION_H_ */
