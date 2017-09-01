/*
 * Track.h
 *
 *  Created on: Aug 29, 2017
 *      Author: bieschke
 */

#ifndef TRACK_H_
#define TRACK_H_

/**
 * A track that is reconstructed from several drift times measured for one triggersignal. A track does always correspond to a
 * TriggerEventCollection.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date September 1, 2017
 */
class Track
{
public:
	Track();
	~Track();

private:
	double m_alpha;
};

#endif /* TRACK_H_ */
