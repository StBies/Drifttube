#include "DataProcessor.h"

using namespace std;

//TODO Change all doc to vector and variable length (Nov. 14, 2018)

/**
 * Constructor, initializes the DataProcessor object. This is private as this class is not meant to be instanced
 * 
 * @brief Constructor
 * 
 * @author Stefan
 * @date June 20, 2016
 * @version 0.5
 */
DataProcessor::DataProcessor()
{
}

/**
 * Destructor, frees the allocated memory for this object, when it is
 * destroyed. In this special case: does nothing
 * 
 * @brief Destructor
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1
 */
DataProcessor::~DataProcessor()
{
}

/**
 * Computes the integral of the data given as parameter. It will count positive as well as
 * negative entries and sum up the bin content.
 *
 * @author Stefan Bieschke
 * @date April 10, 2017
 * @version Alpha 2.0
 *
 * @param data Event of which the integral is to be calculated
 * @return Value of the integral over all bins
 */
int DataProcessor::computeIntegral(const Event& data)
{
	int integral = 0;

	for(uint16_t binContent : data.getData())
	{
		integral += binContent;
	}

	return integral;
}

/**
 * Computes the integral of a passed Event containing raw FADC data. The result is an array, which contains the
 * integral per bin
 * 
 * @brief Event integrator
 * 
 * @author Stefan Bieschke
 * @date September 19, 2017
 * @version Alpha 2.0.1
 * 
 * @param data Reference to an Event that is to be integrated
 * 
 * @return std::array<int,800> containing the integral
 * 
 * @warning Does integrate the whole interval for that the data object provides data.
 */
const vector<int> DataProcessor::integrate(const Event& data)
{
	vector<uint16_t> dataArray = data.getData();
	//TODO check if returning a copy isn't too slow
	return integrate(dataArray);
}

/**
 * Computes the integral of a given Event and subtracts the integral of a constant function with value error over that same interval.
 * With the result I, Event e and error \f$\Delta e\f$ this can be described as:
 *  \f[
 *  I(x) = \int_{x_0}^{x} (e(x) - \Delta e)\, dx
 *  \f]
 *
 * @brief Event integrator with error correction
 *
 * @author Stefan Bieschke
 * @date September 19, 2017
 * @version Alpha 2.0
 *
 * @param data Event containing the data to be integrated
 * @param error constat error subtracted from each databin
 * @return the error-corrected integral of the Event
 */
const vector<int> DataProcessor::integrate(const Event& data, const uint16_t error)
{
	vector<uint16_t> dataArray = data.getData();
	return integrate(dataArray,error);
}

/**
 * Computes the integral of a passed array containing raw FADC data. The result is an array, which contains the
 * integral per bin. The integral \f$ I(x)\f$ can be described as:
 * \f[
 *  I(x) = \int_{x_0}^{x} e(x)\, dx
 * \f]
 * Here, \f$ e(x)\f$ is the eventarray at bin x.
 *
 * @brief Array integrator
 *
 * @author Stefan Bieschke
 * @date September 19, 2017
 * @version Alpha 2.0.1
 *
 * @param data Reference to an array that is to be integrated
 *
 * @return @c std::array<int,800> containing the integral
 *
 * @warning Does integrate the whole interval for that the data object provides data.
 */
const vector<int> DataProcessor::integrate(const vector<uint16_t>& data)
{
	vector<int> result(data.size());
	result[0] = 0;
	for(unsigned int i = 1; i < data.size(); ++i)
	{
		result[i] = data[i] + result[i-1];
	}
	return result;
}

/**
 * Computes the integral of a given array and subtracts the integral of a constant function with value error over that same interval.
 * With the result I, Event e and error \f$\Delta e\f$ this can be described as:
 * \f[
 * I = \int_{x_0}^{x} (e(x) - \Delta e)\, dx
 * \f]
 *
 * @brief Array integrator with error correction
 *
 * @author Stefan Bieschke
 * @date September 19, 2017
 * @version Alpha 2.0.1
 *
 * @param data array containing the data to be integrated
 * @param error constat error subtracted from each databin
 * @return the error-corrected integral of the array
 */
const vector<int> DataProcessor::integrate(const vector<uint16_t>& data, const uint16_t error)
{
	vector<int> result(data.size());
	result[0] = 0;
	for(unsigned int i = 1; i < data.size(); ++i)
	{
		result[i] = data[i] + result[i-1] - error;
	}
	return result;
}

/**
 * Find the position of the minimum of the given data. Will only find the absolute
 * minimum. Can not yet find more than one negative peak.
 *
 * @author Stefan
 * @date June 8, 2017
 * @version Alpha 2.0
 *
 * @param data Reference to an Event object containing the data
 * @return bin containing the data minimum
 */
unsigned short DataProcessor::findMinimumBin(const Event& data)
{
	int minBin = 0;
	for(unsigned short i = 0; i < data.getData().size(); ++i)
	{
		minBin = data[i] < data[minBin] ? i : minBin;
	}
	return minBin;
}

/**
 * Calculates the spectrum of drifttimes for the data given in a DataSet object containing raw data.
 * The result is a histogram containing the spectrum. Note, that in order to find the correct drift time spectrum, the
 * parameters defined in globals.h must be defined for the used experiment.
 *
 * @author Stefan
 * @date November 21, 2016
 * @version 1.0
 *
 * @param data DataSet object for which the drift time spectrum is to be calculated
 *
 * @return DriftTimeSpectrum object containing the spectrum
 */
const DriftTimeSpectrum DataProcessor::calculateDriftTimeSpectrum(const DataSet& data)
{
	if(data.getSize() == 0)
	{
		unique_ptr<vector<uint32_t>> empty(new vector<uint32_t>(0));
		return DriftTimeSpectrum(move(empty), 0, 0);
	}
	//can not run in parallel - at least not this way

	unsigned int rejected = 0;

	#ifdef ZEROSUP
	unique_ptr<vector<uint32_t>> result;
	for (size_t i = 0; i < data.getSize(); ++i)
	{
		try
		{
			result = make_unique<vector<uint32_t>>(data[i].getSize(),0);
			break;
		}
		catch(const DataPresenceException& e)
		{
			continue;
		}
	}
//	#pragma omp parallel for schedule(static) reduction(+:rejected)
	for(size_t i = 0; i < data.getSize(); ++i)
	{
		try
		{
			short driftTimeBin = (short)(data[i].getDriftTime() / ADC_BINS_TO_TIME) - ADC_TRIGGERPOS_BIN;
			//TODO THIS IS BAD!!!! Maybe it should be rejected, maybe not - more thinking needed
			driftTimeBin = driftTimeBin < 0 ? 0 : driftTimeBin;
			++(*result)[driftTimeBin];
		}
		catch(const Exception& e)
		{
//			#pragma omp critical
//			{
				++rejected;
//			}
		}
	}
	#else
	unique_ptr<vector<uint32_t>> result = make_unique<vector<uint32_t>>(data[0].getSize(),0);

//	#pragma omp parallel for schedule(static) shared(rejected)
	for(size_t i = 0; i < data.getSize(); ++i)
	{
		short driftTimeBin = (short) (data[i].getDriftTime() / ADC_BINS_TO_TIME);
		if(driftTimeBin != -42)
		{
			driftTimeBin = (data[i].getDriftTime() / ADC_BINS_TO_TIME) - ADC_TRIGGERPOS_BIN;
			//TODO THIS IS BAD!!!! Maybe it should be rejected, maybe not - more thinking needed
			driftTimeBin = driftTimeBin < 0 ? 0 : driftTimeBin;
			(*result)[driftTimeBin]++;
		}
		else
		{
			rejected++;
		}
	}
	#endif

	return DriftTimeSpectrum(move(result), data.getSize(), rejected);
}

/**
 * Calculates the relation between drift time and drift radius. The relation is returned as RtRelation object.
 * It calculates the relation from a passed drift time spectrum as argument.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date June 22, 2017
 *
 * @param dtSpect DriftTimeSpectrum object reference containing the drift time spectrum
 *
 * @return RtRelation object containing the rt-relation
 *
 * @warning Needs drift tube data in globals.h to be set
 */
const RtRelation DataProcessor::calculateRtRelation(const DriftTimeSpectrum& dtSpect)
{
	if(dtSpect.getSize() == 0)
	{
		return RtRelation(move(unique_ptr<vector<double>>(new vector<double>(0))));
	}
	size_t nBins = dtSpect.getSize();
	unique_ptr<vector<double>> result(new vector<double>(nBins,0.0));

	double integral = 0.0;
	double scalingFactor = DRIFT_TUBE_RADIUS / (dtSpect.getEntries() - dtSpect.getRejected());

	for(size_t i = 0; i < nBins; ++i)
	{
		(*result)[i] = integral;
		integral += dtSpect[i] * scalingFactor;
	}

	return RtRelation(move(result));
}


//TODO test
/**
 * Counts the number of pulses undershooting a given threshold voltage. Returns a list of two element arrays. These contain
 * the times of the falling and the rising edge of the pulse.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date February 4, 2019
 *
 * @param data Event for that the pulses over threshold are to be analyzed
 * @param threshold threshold that must be undershot in order to identify a "pulse"
 * @return A vector containing a list of pulses, each with time of falling and rising edge in ns
 *
 * @warning vector contains raw heap pointers, caller should manage deletion himself
 */
const vector<array<uint16_t,2>*> DataProcessor::pulses_over_threshold(const Event& data, unsigned short threshold)
{
	return pulses_over_threshold(data,threshold,0,data.getSize());
}


//TODO test
/**
 * Counts the number of pulses undershooting a given threshold voltage. Returns a list of two element arrays. These contain
 * the times of the falling and the rising edge of the pulse. Here, only pulses between the bins from and to are counted.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date February 4, 2019
 *
 * @param data Event for that the pulses over threshold are to be analyzed
 * @param threshold threshold that must be undershot in order to identify a "pulse"
 * @param from
 * @param to
 * @return A vector containing a list of pulses, each with time of falling and rising edge in ns
 *
 * @warning vector contains raw heap pointers, caller should manage deletion himself
 */
const vector<array<uint16_t, 2>*> DataProcessor::pulses_over_threshold(
		const Event& data, unsigned short threshold, size_t from, size_t to)
{
	vector<array<uint16_t,2>*> result(0);
	bool first_is_rising = data[from] <= threshold ? true : false;
	bool pulse_ended = !first_is_rising;
	//comment this if block when we don't want to count cases where the first edge is rising
	if(first_is_rising)
	{
		result.push_back(new array<uint16_t,2>());
		(*result.back())[0] = 0xFFFF; //error for first is rising - results in negative time over threshold
	}

	//from maximum drift time on: loop over the event to even higher drift times
	for (unsigned int i = from; i < to; ++i)
	{
		//if-else switches a variable in order not to count a single pulse bin per bin
		if (data[i] <= threshold && pulse_ended)
		{
			result.push_back(new array<uint16_t,2>());
			(*result.back())[0] = ADC_BINS_TO_TIME * i; //ns
			pulse_ended = false;
		}
		//in case we don't want to count cases where the first edge is rising
//		else if (data[i] > threshold && !pulse_ended && result.size() == 0)
//		{
//			pulse_ended = true;
//		}
		else if (data[i] > threshold && !pulse_ended)
		{
			(*result.back())[1] = ADC_BINS_TO_TIME * i; //ns
			pulse_ended = true;
		}
	}
	if(!pulse_ended)
	{
		(*result.back())[1] = 0xFFFF; //error for last rising edge missing
	}

	return result;
}

/**
 * Counts the time over threshold for a set of pulse edge times that is passed to this method as parameter.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date February 5, 2019
 *
 * @param pulses A vector containing arrays with times for falling and rising edges of each pulse. Thus, this array
 * has is as long as the number of identified pulses is long and each contained array has the time of the falling edge in its
 * [0] entry and the rising edge in its [1] entry.
 *
 * @return A vector that contains the time over threshold in nanoseconds for each identified pulse in the passed parameter vector.
 *
 * @ensure result.size() == pulses.size()
 * @note As the time over threshold is an unsigned integer the case of a missing edge should result in very high time over thresholds, as
 * the time of a missing edge is set to 0xFFFF, which in either case should result in a high time over threshold.
 */
const vector<uint16_t> DataProcessor::time_over_threshold(const vector<array<uint16_t, 2>*>& pulses)
{
	vector<uint16_t> result(pulses.size());
	for(int i = 0; i < pulses.size(); ++i)
	{
		result[i] = (*pulses[i])[1] - (*pulses[i])[0];
	}
	return result;
}


//TODO threshold as parameter?
//TODO possibility to use any other t_max as parameter
//TODO Test
/**
 * Count the number of afterpulses in a Drifttube. An afterpulse is counted,
 * if the after the maximum drift time, a threshold voltage is undershot. This maximum drift time is
 * calculated from the rtRelation as the time, at which it reaches 99.95% of the tube's inner radius.
 *
 * @brief Count afterpulses. Multiple afterpulses per event are allowed.
 *
 * @author Stefan Bieschke
 * @version Alpha 2.0
 * @date February 4, 2019
 *
 * @param tube Drifttube object for that the afterpulses should be count
 *
 * @return number of afterpulses
 */
const unsigned int DataProcessor::countAfterpulses(const Drifttube& tube)
{
	unsigned short maxDriftTimeBin = tube.getMaxDrifttime() / ADC_BINS_TO_TIME;
	unsigned int nAfterPulses = 0;

	//counting loop
	for (unsigned int i = 0; i < tube.getDataSet().getSize(); ++i)
	{
		try
		{
			//assume the pulse has already ended
			bool pulseEnded = true;
			Event voltage = tube.getDataSet().getEvent(i);
			uint16_t threshold = ABSOLUTE_OFFSET_ZERO_VOLTAGE + ABSOLUTE_EVENT_THRESHOLD_VOLTAGE;
			vector<array<uint16_t, 2>*> pulses = pulses_over_threshold(voltage,
					threshold, maxDriftTimeBin, voltage.getSize());
			nAfterPulses += pulses.size();
			for(array<uint16_t,2>* arr : pulses)
			{
				delete [] arr;
			}
		} catch (Exception& e)
		{
			continue;
		}
	}

	return nAfterPulses;
}

//TODO Change it to return it corrected for triggertime offset
/**
 * Finds the bin number in a passed Event, in which a passed threshold is first surpassed.
 * Note that this method finds the bin number, in which the drift time is.
 *
 * @author Stefan Bieschke
 * @data June 8, 2017
 * @version Alpha 2.0
 *
 * @param data Event in which the drift time is to be found
 * @param threshold threshold in FADC units (arbitrary). This must be UNDERSHOT if a drift time exists.
 *
 * @return Bin number of the first occurance of a signal larger than threshold
 */
short DataProcessor::findDriftTimeBin(const Event& data, unsigned short threshold)
{
	for(unsigned short i = 0; i < data.getData().size(); ++i)
	{
		if(data[i] < threshold)
		{
			return i;
		}
	}
	return -42;
}

/**
 * Finds the last bin, where a threshold voltage is reached (a.k.a where the bin content is EQUAL to the threshold).
 *
 * @author Stefan Bieschke
 * @date June 28, 2017
 * @version Alpha 2.0.1
 *
 * @param data Event object reference, containing the data on which the last filled bin is to be found
 * @param threshold Threshold in FADC units. The last time will be found, for that the data entry is SMALLER THAN the threshold.
 *
 * @return Number of the bin, where the voltage given in threshold is last undershot
 */
unsigned short DataProcessor::findLastFilledBin(const Event& data, unsigned short threshold)
{
	unsigned short defaultBin = 0;
	for(unsigned short i = data.getData().size() - 1; i > 0; --i)
	{
		if(data[i] <= threshold)
		{
			return i;
		}
	}
	return defaultBin;
}
