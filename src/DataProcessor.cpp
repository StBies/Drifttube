#include "DataProcessor.h"

using namespace std;

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
 * destroyed
 * 
 * @brief Destructor
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1
 */
DataProcessor::~DataProcessor()
{
	//TODO implement
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

	for (uint16_t binContent : data.getData() )
	{
		integral += binContent;
	}

	return integral;
}

/**
 * Computes the integral of a passed array containing raw FADC data. The result is an array again, which contains the
 * integral per bin
 * 
 * @brief integrator
 * 
 * @author Stefan Bieschke
 * @date May 16, 2017
 * @version Alpha 2.0
 * 
 * @param data Reference to an Event that is to be integrated
 * 
 * @return std::array<int,800> containing the integral
 * 
 * @warning Does integrate the whole interval for that the data object provides data.
 */
const array<int,800> DataProcessor::integrate(const Event& data)
{
	array<int,800> result;
	array<uint16_t,800> dataArray = data.getData();

	//do the integration (stepsize is one)
	//TODO check for correctness
	result[0] = data[0];
	for(int i = 1; i < dataArray.size(); i++)
	{
		result[i] = dataArray[i] + result[i-1];
	}
	//TODO check if returning a copy isn't too slow
	return result;
}

///**
// * Calculate the derivative of an event data sample given as std::array<int,800> so 800 FADC bins containing the
// * raw FADC resolved measurement steps.
// *
// * @author Stefan Bieschke
// * @date April 10,2017
// * @version Alpha 2.0
// *
// * @param data Reference to a std::array containing the data, for which the derivative is to be calculated
// *
// * @return std::array containing the derivative
// */
//static const array<int,800> DataProcessor::derivate(const array<int,800>& data) const
//{
//	array<int,800> result;
//	for (int i = 0; i < data.size() - 1; i++)
//	{
//		result[i] = data[i + 1]- data[i];
//	}
//	//TODO what happens to the last bin in the derivative?
//	//TODO does the qualitative result remain the same after scaling to mV and ns?
//	return result;
//}

/**
 * Integrates all histograms, that are passed to this method in a DataSet object. The resulting histograms containing the histograms
 * are returned in a new DataSet object.
 *
 * @brief Integrate all data in a DataSet object
 *
 * @author Stefan Bieschke
 * @date April 10, 2017
 * @version Alpha 2.0
 *
 * @param data reference to the DataSet object that is to be integrated
 *
 * @return std::unique pointer to the new DataSet on heap. Be aware that ownership is transferred to caller
 *
 * @warning Heap object returned, caller needs to handle memory
 */
unique_ptr<DataSet> DataProcessor::integrateAll(const DataSet& data)
{
	vector<unique_ptr<Event>> set(data.getSize());

	#pragma omp parallel for shared(set)
	for (int i = 0; i < data.getSize(); i++)
	{
		try
		{
			unique_ptr<array<int,800>> integral(new array<int,800>());
			*integral =  integrate(data.getEvent(i));
			set[i] = integral;
		} catch (Exception& e)
		{
			cerr << e.error() << endl;
		}
	}
	return move(unique_ptr<DataSet>(new DataSet(set)));
}

///**
// * Find the position of the minimum of the given data. Will only find the absolute
// * minimum. Can not yet find more than one negative peak.
// *
// * @author Stefan
// * @date April 10, 2017
// * @version Alpha 2.0
// *
// * @param data Reference to a std::array containing the raw data
// * @return bin containing the data minimum
// */
//static inline int DataProcessor::findMinimumBin(const array<int,800>& data) const
//{
//	int minBin = 0;
//	for(int i = 0; i < data.size(); i++)
//	{
//		minBin = data[i] < data[minBin] ? i : minBin;
//	}
//	return minBin;
//}
//
///**
// * Calculates the spectrum of drifttimes for the data given in a DataSet object containing raw data.
// * The result is a histogram containing the spectrum. Note, that in order to find the correct drift time spectrum, the
// * parameters defined in globals.h must be defined for the used experiment.
// *
// * @author Stefan
// * @date November 21, 2016
// * @version 1.0
// *
// * @param data Pointer to a DataSet object containing the raw data out of which the spectrum is to be calculated
// *
// * @return TH1D* histogram containing the spectrum of drifttimes
// */
//static TH1D* DataProcessor::calculateDriftTimeSpectrum(const DataSet& data) const
//{
//	int triggerpos = ADC_TRIGGERPOS_BIN;
//
////	#pragma omp parallel for
//	for (int i = 0; i < data->getSize(); i++)
//	{
//		//TODO hint, that the operator[] of DataSet seems not quite to work, const return is a problem and operator does not work on pointer to object
//		//TODO maybe last mention is intended
//		const TH1D* event = (*data)[i];
//		int diff = findDriftTime(*event, -50 * ADC_CHANNELS_TO_VOLTAGE)
//				- triggerpos;
//		result->Fill(diff * ADC_BINS_TO_TIME);
//	}
//
//	return result;
//}
//
///**
// * Calculates the reation between drift time and drift radius. The relation is returned as TH1D* pointer to a histogram.
// * It calculates the relation from a passed drift time spectrum as argument.
// *
// * @author Stefan Bieschke
// * @version 1.0
// * @date November 21, 2016
// *
// * @param dtSpect TH1D object reference containing the drift time spectrum
// *
// * @return Pointer to a TH1D object histogram containing the rt-relation plot
// *
// * @warning Needs drift tube data in globals.h to be set
// */
//static TH1D* DataProcessor::calculateRtRelation(TH1D& dtSpect) const
//{
//	int nBins = dtSpect.GetNbinsX();
//	Double_t* binLowEdges = new Double_t[nBins + 1];
//
//	double integral = 0.0;
//	//TODO own method
//	int numberOfRealEvents = dtSpect.GetEntries() - dtSpect.GetBinContent(0);
//	double eff = numberOfRealEvents/(double)dtSpect.GetEntries();
//	cout << "efficiency = " << eff << " +- " << sqrt(eff*(1-eff)/(double)dtSpect.GetEntries()) << endl;
//	double scalingFactor = ((double)DRIFT_TUBE_RADIUS) / ((double)numberOfRealEvents);
//
//	//TODO check, why this doesn't work
//	//	#pragma omp parallel for reduction(+:integral) shared(result,binWidth)
//	//start at bin 1 -> do not integrate the underflow bin
//	for (int i = 1; i <= nBins; i++)
//	{
//		integral += dtSpect.GetBinContent(i) * scalingFactor;
//		result->Fill(dtSpect.GetBinLowEdge(i), integral);
//	}
//
//	return result;
//}
//
////TODO threshold as parameter?
////TODO possibility to use any other t_max as parameter
///**
// * Count the number of afterpulses in a DataSet containing voltage pulses. An afterpulse is counted,
// * if the after the maximum drift time, a threshold voltage is undershot. This maximum drift time is
// * calculated from the rtRelation as the time, at which it reaches 99.95% of the tube's inner radius.
// *
// * @brief Count afterpulses. Multiple afterpulses per event are allowed.
// *
// * @author Stefan Bieschke
// * @version 0.9
// * @date Dec. 15, 2016
// *
// * @param rawData DataSet object containing voltage pulses
// * @param rtRelation TH1D histogram object containing the rt-Relation.
// *
// * @return number of afterpulses
// */
//static int DataProcessor::countAfterpulses(const DataSet& rawData, const TH1D& rtRelation) const
//{
//	int maxDriftTimeBin = 0;
//	int nAfterPulses = 0;
//
//	//calculate maxDriftTime
//	for(int i = 1; i <=rtRelation.GetNbinsX(); i++)
//	{
//		if(rtRelation.GetBinContent(i) >= DRIFT_TUBE_RADIUS - DRIFT_TUBE_RADIUS * 0.0005)
//		{
//			maxDriftTimeBin = i;
//			cout << "maxDriftTime: " << maxDriftTimeBin * 4 << endl;
//			break;
//		}
//	}
//
//	//counting loop
//	for(int i = 0; i < rawData.getSize(); i++)
//	{
//		bool pulseEnded = true;
//		TH1D* voltage = rawData.getEvent(i);
//		int nBins = voltage->GetNbinsX();
//
//		//check, if the signal already ended at max drift time
//		if(voltage->GetBinContent(maxDriftTimeBin) <= -50*ADC_CHANNELS_TO_VOLTAGE)
//		{
//			pulseEnded = false;
//		}
//
//		for(int j = maxDriftTimeBin + ADC_TRIGGERPOS_BIN; j < nBins; j++)
//		{
//			//if-else switches a variable in order not to count a single pulse bin per bin
//			if(voltage->GetBinContent(j) <= -50*ADC_CHANNELS_TO_VOLTAGE && pulseEnded)
//			{
////				cout << "event " <<i << " time: " << j*4 << endl;
//				++nAfterPulses;
//				pulseEnded = false;
//			}
//			else if(voltage->GetBinContent(j) > -50*ADC_CHANNELS_TO_VOLTAGE && !pulseEnded)
//			{
//				pulseEnded = true;
//			}
//		}
//	}
//
//	return nAfterPulses;
//}
//
////TODO decide, if this should return the "real time" in nanoseconds instead of the bin number
///**
// * Finds the bin number in a passed histogram, in which a passed threshold in volt is surpassed.
// * The threshold should be negative since it only checks, when signals are LOWER than the given threshold.
// * However, a positive threshold will be multiplied with -1 internally.
// *
// * @author Stefan
// * @data October 17, 2016
// * @version 0.9
// *
// * @param data Pointer to a TH1D histogram where the first occurance of a signal bigger than threshold is to be found
// * @param threshold in volt that is to be surpassed. Should be given negative, positive values are multiplied by -1 internally
// *
// * @return Bin number of the first occurance of a signal larger than threshold
// */
//static int DataProcessor::findDriftTime(const TH1D& data, double threshold) const
//{
//	//if threshold given positive, change sign
//	threshold *= (threshold < 0 ? 1 : -1);
//
//	for (int i = 0; i < data.GetNbinsX(); i++)
//	{
//		if (data.GetBinContent(i) < threshold)
//		{
//			return i;
//		}
//	}
//	return -42;
//}
//
///**
// * Finds the last bin, where a threshold voltage is reached.
// *
// * @author Stefan
// * @date November 20, 2016
// * @version 0.1
// *
// * @param data TH1D histogram object containing the voltage, in which the last filled bin is to be found
// * @param threshold voltage in Volt, that must be reached
// *
// * @return Number of the bin, where the voltage given in threshold is last reached
// */
//static int DataProcessor::findLastFilledBin(const TH1D& data, double threshold) const
//{
//	int bin = 0;
//	for (int i = 0; i < data.GetNbinsX() - 1; i++)
//	{
//		if (data.GetBinContent(i) < threshold
//				&& data.GetBinContent(i + 1) >= threshold)
//		{
//			bin = i;
//		}
//	}
//	return bin;
//}
