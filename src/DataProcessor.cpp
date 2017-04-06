#include "DataProcessor.h"

using namespace std;

/**
 * Constructor, initializes the DataProcessor object.
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
 * @author Bene9
 * @date June 15, 2016
 * @version 0.2
 *
 * @param data TH1 family object containing the data to be integrated.
 * @return Value of the integral over all bins
 */
Double_t DataProcessor::computeIntegral(const TH1D& data) const
{
	Double_t counter = 0;

	//TODO check if parallelization is of use here

	for (int i = 0; i < data.GetNbinsX(); i++)
	{
		counter += data.GetBinContent(i);
	}

	return counter * data.GetBinWidth(1);
}

/**
 * Integrates the given data, which is in a histogram. This method will return
 * a new histogram of type TH1* containing the integrated original data.
 * 
 * @brief histogram integrator
 * 
 * @author Stefan
 * @date June 16, 2016
 * @version 0.3
 * 
 * @param data Data, that is to be integrated
 * 
 * @return TH1D* pointer to a new heap-object histogram containing the integral of data
 * 
 * @warning Does integrate the whole interval, that the data object provides data.
 * @warning Returned object must be destroyed by the user
 */
TH1D* DataProcessor::integrate(TH1D* data) const
{
	Int_t nBins = data->GetNbinsX();
	Double_t* binLowEdges = new Double_t[nBins + 1];

//	#pragma omp parallel for
	for (Int_t i = 0; i <= nBins; i++)
	{
		binLowEdges[i] = data->GetBinLowEdge(i);
	}

	//TODO Check the actual parameter type and and return the correct object accordingly
	TString name = data->GetName();
	name.Append(" (integrated)");

	TH1D* result = new TH1D(name, "Integrated FADC data", nBins, binLowEdges);
	result->GetXaxis()->SetTitle(data->GetXaxis()->GetTitle());
//	stringstream yTitle;
//	yTitle << "integral of " << data->GetYaxis()->GetTitle();
//	result->GetYaxis()->SetTitle(yTitle.str().c_str());
	result->GetYaxis()->SetTitle("a.u.");

	delete[] binLowEdges;
	double integral = 0.0;

	//TODO check, why this doesn't work
//	#pragma omp parallel for reduction(+:integral) shared(result,binWidth)
	//start at bin 1 -> do not integrate the underflow bin
	for (int i = 1; i <= nBins; i++)
	{
		integral += data->GetBinContent(i) ;
		result->Fill(data->GetBinLowEdge(i), integral);
	}

	return result;
}

/**
 * Calculate the derivative of a plot given as histogram.
 *
 * @author Stefan
 * @date November 17,2016
 * @version 0.1
 *
 * @param data Pointer to the histogram containing the data, on that the derivative is to be calculated
 *
 * @return Histogram containing the derivative
 */
TH1D* DataProcessor::derivate(TH1D* data) const
{
	Int_t nBins = data->GetNbinsX();
	Double_t* binLowEdges = new Double_t[nBins + 1];
	TString name = data->GetName();
	name.Append(" (derivative)");

	for (Int_t i = 0; i <= nBins; i++)
	{
		binLowEdges[i] = data->GetBinLowEdge(i);
	}

	TH1D* result = new TH1D(name, "derivative", nBins, binLowEdges);

	for (int i = 0; i < nBins - 1; i++)
	{
		double differentialQuotient = (data->GetBinContent(i + 1)
				- data->GetBinContent(i)) / (double) ADC_BINS_TO_TIME;
		result->SetBinContent(i, differentialQuotient);
	}

	return result;
}

/**
 * Integrates all histograms, that are passed to this method in a DataSet object. The resulting histograms containing the histograms
 * are returned in a new DataSet object.
 *
 * @brief Integrate all data in a DataSet object
 *
 * @author Stefan
 * @date August 12, 2016
 * @version 0.8
 *
 * @param data Pointer to the DataSet object that is to be integrated
 *
 * @return Pointer to a DataSet type object containing the processed data
 *
 * @warning Heap object returned, caller needs to handle memory
 */
DataSet* DataProcessor::integrateAll(DataSet* data) const
{
//	DataSet* result = new DataSet();
	vector<TH1D*>* set = new vector<TH1D*>;
	set->resize(data->getSize());

	#pragma omp parallel for shared(set)
	for (int i = 0; i < data->getSize(); i++)
	{
		try
		{
			TH1D* integral = integrate(data->getEvent(i));
			(*set)[i] = integral;
		} catch (Exception& e)
		{
			cerr << e.error() << endl;
		}
	}
	return new DataSet(set);
}

/**
 * Find the position of the minimum of the given data. Will only find the absolute
 * minimum. Can not yet find more than one negative peak.
 *
 * @author Stefan
 * @date July 2, 2016
 * @version 0.1
 *
 * @param data Data as a pointer to a TH1D histogram containing the data
 * @return bin containing the data minimum
 */
inline int DataProcessor::findMinimumBin(TH1D* data) const
{
	return data->GetMinimumBin();
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
 * @param data Pointer to a DataSet object containing the raw data out of which the spectrum is to be calculated
 *
 * @return TH1D* histogram containing the spectrum of drifttimes
 */
TH1D* DataProcessor::calculateDriftTimeSpectrum(DataSet* data) const
{
	int triggerpos = ADC_TRIGGERPOS_BIN;
	TH1D* result = new TH1D("Drifttime spectrum", "Drift time spectrum", 800, 0,
			800 * ADC_BINS_TO_TIME);
	result->GetXaxis()->SetTitle("drift time [ns]");
	result->GetYaxis()->SetTitle("# counts");

//	#pragma omp parallel for
	for (int i = 0; i < data->getSize(); i++)
	{
//		TH1D* event = data->getEvent(i);
		//TODO hint, that the operator[] of DataSet seems not quite to work, const return is a problem and operator does not work on pointer to object
		//TODO maybe last mention is intended
		const TH1D* event = (*data)[i];
		int diff = findDriftTime(*event, -50 * ADC_CHANNELS_TO_VOLTAGE)
				- triggerpos;
		result->Fill(diff * ADC_BINS_TO_TIME);
	}

	return result;
}

/**
 * Calculates the reation between drift time and drift radius. The relation is returned as TH1D* pointer to a histogram.
 * It calculates the relation from a passed drift time spectrum as argument.
 *
 * @author Stefan Bieschke
 * @version 1.0
 * @date November 21, 2016
 *
 * @param dtSpect TH1D object reference containing the drift time spectrum
 *
 * @return Pointer to a TH1D object histogram containing the rt-relation plot
 *
 * @warning Needs drift tube data in globals.h to be set
 */
TH1D* DataProcessor::calculateRtRelation(TH1D& dtSpect) const
{
	Int_t nBins = dtSpect.GetNbinsX();
	Double_t* binLowEdges = new Double_t[nBins + 1];

	//	#pragma omp parallel for
	for (Int_t i = 0; i <= nBins; i++)
	{
		binLowEdges[i] = dtSpect.GetBinLowEdge(i);
	}

	TH1D* result = new TH1D("rt-relation", "rt relation", nBins, binLowEdges);
	result->GetXaxis()->SetTitle(dtSpect.GetXaxis()->GetTitle());
	result->GetYaxis()->SetTitle("drift radius [mm]");

	delete[] binLowEdges;
	//choose random bin width since all bins are the same size - might change
	Double_t binWidth = result->GetBinWidth(1);

	double integral = 0.0;
	//TODO own method
	int numberOfRealEvents = dtSpect.GetEntries() - dtSpect.GetBinContent(0);
	double eff = numberOfRealEvents/(double)dtSpect.GetEntries();
	cout << "efficiency = " << eff << " +- " << sqrt(eff*(1-eff)/(double)dtSpect.GetEntries()) << endl;
	double scalingFactor = ((double)DRIFT_TUBE_RADIUS) / ((double)numberOfRealEvents);

	//TODO check, why this doesn't work
	//	#pragma omp parallel for reduction(+:integral) shared(result,binWidth)
	//start at bin 1 -> do not integrate the underflow bin
	for (int i = 1; i <= nBins; i++)
	{
		integral += dtSpect.GetBinContent(i) * scalingFactor;
		result->Fill(dtSpect.GetBinLowEdge(i), integral);
	}

	return result;
}

//TODO threshold as parameter?
//TODO possibility to use any other t_max as parameter
/**
 * Count the number of afterpulses in a DataSet containing voltage pulses. An afterpulse is counted,
 * if the after the maximum drift time, a threshold voltage is undershot. This maximum drift time is
 * calculated from the rtRelation as the time, at which it reaches 99.95% of the tube's inner radius.
 *
 * @brief Count afterpulses. Multiple afterpulses per event are allowed.
 *
 * @author Stefan Bieschke
 * @version 0.9
 * @date Dec. 15, 2016
 *
 * @param rawData DataSet object containing voltage pulses
 * @param rtRelation TH1D histogram object containing the rt-Relation.
 *
 * @return number of afterpulses
 */
int DataProcessor::countAfterpulses(const DataSet& rawData, const TH1D& rtRelation) const
{
	int maxDriftTimeBin = 0;
	int nAfterPulses = 0;

	//calculate maxDriftTime
	for(int i = 1; i <=rtRelation.GetNbinsX(); i++)
	{
		if(rtRelation.GetBinContent(i) >= DRIFT_TUBE_RADIUS - DRIFT_TUBE_RADIUS * 0.0005)
		{
			maxDriftTimeBin = i;
			cout << "maxDriftTime: " << maxDriftTimeBin * 4 << endl;
			break;
		}
	}

	//counting loop
	for(int i = 0; i < rawData.getSize(); i++)
	{
		bool pulseEnded = true;
		TH1D* voltage = rawData.getEvent(i);
		int nBins = voltage->GetNbinsX();

		//check, if the signal already ended at max drift time
		if(voltage->GetBinContent(maxDriftTimeBin) <= -50*ADC_CHANNELS_TO_VOLTAGE)
		{
			pulseEnded = false;
		}

		for(int j = maxDriftTimeBin + ADC_TRIGGERPOS_BIN; j < nBins; j++)
		{
			//if-else switches a variable in order not to count a single pulse bin per bin
			if(voltage->GetBinContent(j) <= -50*ADC_CHANNELS_TO_VOLTAGE && pulseEnded)
			{
//				cout << "event " <<i << " time: " << j*4 << endl;
				++nAfterPulses;
				pulseEnded = false;
			}
			else if(voltage->GetBinContent(j) > -50*ADC_CHANNELS_TO_VOLTAGE && !pulseEnded)
			{
				pulseEnded = true;
			}
		}
	}

	return nAfterPulses;
}

//int DataProcessor::countAfterpulsesDt(const DataSet& rawData, const TH1D& dtSpect) const
//{
//	Int_t firstZeroBin = 0;
//	int nAfterPulses = 0;
//
//	//find first bin with zero entries
//	for(Int_t i = 5; i <= dtSpect.GetNbinsX(); i++)
//	{
//		if(dtSpect.GetBinContent(i) == 0)
//		{
//			firstZeroBin = i;
//			break;
//		}
//	}
//
//
//	TFitResultPtr fit = dtSpect->Fit("pol1", "S", "", (firstZeroBin*4)*0.8, (firstZeroBin*4)*1.20);
//
//	Double_t par0 = fit->Parameter(0);
//	Double_t par1 = fit->Parameter(1);
//
//	Double_t tmax = abs(par0/par1);
//
//	Int_t maxDriftTimeBin = (Int_t)(tmax/4);
//
//	for(int i = 0; i < rawData.getSize(); i++)
//	{
//		bool pulseEnded = true;
//		TH1D* voltage = rawData.getEvent(i);
//		int nBins = voltage->GetNbinsX();
//
//		cout << "max drift time bin: " << maxDriftTimeBin << " tmax: " << (Int_t)(tmax/4) << endl;
//
//		//check, if the signal already ended at max drift time
//		if(voltage->GetBinContent(maxDriftTimeBin) <= -50*ADC_CHANNELS_TO_VOLTAGE)
//		{
//			pulseEnded = false;
//		}
//
//		for(int j = maxDriftTimeBin + ADC_TRIGGERPOS_BIN; j < nBins; j++)
//		{
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
//	return nAfterPulses;
//}

//TODO decide, if this should return the "real time" in nanoseconds instead of the bin number
/**
 * Finds the bin number in a passed histogram, in which a passed threshold in volt is surpassed.
 * The threshold should be negative since it only checks, when signals are LOWER than the given threshold.
 * However, a positive threshold will be multiplied with -1 internally.
 *
 * @author Stefan
 * @data October 17, 2016
 * @version 0.9
 *
 * @param data Pointer to a TH1D histogram where the first occurance of a signal bigger than threshold is to be found
 * @param threshold in volt that is to be surpassed. Should be given negative, positive values are multiplied by -1 internally
 *
 * @return Bin number of the first occurance of a signal larger than threshold
 */
int DataProcessor::findDriftTime(const TH1D& data, double threshold) const
{
	//if threshold given positive, change sign
	threshold *= (threshold < 0 ? 1 : -1);

	for (int i = 0; i < data.GetNbinsX(); i++)
	{
		if (data.GetBinContent(i) < threshold)
		{
			return i;
		}
	}
	return -42;
}

/**
 * Finds the last bin, where a threshold voltage is reached.
 *
 * @author Stefan
 * @date November 20, 2016
 * @version 0.1
 *
 * @param data TH1D histogram object containing the voltage, in which the last filled bin is to be found
 * @param threshold voltage in Volt, that must be reached
 *
 * @return Number of the bin, where the voltage given in threshold is last reached
 */
int DataProcessor::findLastFilledBin(const TH1D& data, double threshold) const
{
	int bin = 0;
	for (int i = 0; i < data.GetNbinsX() - 1; i++)
	{
		if (data.GetBinContent(i) < threshold
				&& data.GetBinContent(i + 1) >= threshold)
		{
			bin = i;
		}
	}
	return bin;
}

/**
 * A method to write some parameters like drifttime, position of signalend, position of voltage minimum as well as minimum
 * height and the height of the minimum of the integral to disk. The resulting .root file will contain a tree with those
 * parameters for each event in the given DataSets.
 *
 * @brief writes some extracted parameters to disk
 *
 * @author Stefan
 * @date November 21, 2016
 * @version 0.8
 *
 * @TODO refactor
 *
 * @param raw raw data DataSet
 * @param integrated integrated data DataSet
 * @param filename parsed filename of the raw data .root file from the FADC
 * @param dirname parsed directory name of the raw data .root file relative to the location from where the program is called
 */
void DataProcessor::writeResults(const DataSet& raw, const DataSet& integrated,
		const TString filename, const TString dirname) const
{
	stringstream name;
	name << dirname << "converted_" << filename;
	TString outFilename(name.str());

	TFile* file = new TFile(outFilename, "recreate");
	TTree* params = new TTree("params", "extracted parameters");

	int triggertime = ADC_TRIGGERPOS_BIN * ADC_BINS_TO_TIME;

	int drifttime;
	int endPos;
	int minimumpos;
	int integralminpos;
	double minheight;
	double integralmin;

	params->Branch("Drifttime", &drifttime, "drifttime/I");
	params->Branch("signalEnd", &endPos, "signalEnd/I");
	params->Branch("integralminpos", &integralminpos, "integralminpos/I");
	params->Branch("minimum", &minimumpos, "minimumpos/I");
	params->Branch("minimumheight", &minheight, "minheight/D");
	params->Branch("integralminimumheight", &integralmin, "integralmin/D");

	const int nEvents = raw.getSize();

	for (int i = 0; i < nEvents; i++)
	{
		drifttime = 0;
		endPos = 0;
		minimumpos = 0;
		minheight = 0;
		integralmin = 0;

		try
		{
			TH1D* rawHist = raw.getEvent(i);
			TH1D* intHist = integrated.getEvent(i);

			drifttime = findDriftTime(*rawHist, -50 * ADC_CHANNELS_TO_VOLTAGE)
					* ADC_BINS_TO_TIME - triggertime;
			minimumpos = findMinimumBin(rawHist) - triggertime;
			minheight = rawHist->GetBinContent(minimumpos);
			endPos = findLastFilledBin(*rawHist, -100 * ADC_CHANNELS_TO_VOLTAGE)
					* ADC_BINS_TO_TIME - triggertime;
			integralminpos = findMinimumBin(intHist) * ADC_BINS_TO_TIME
					- triggertime;
			integralmin = intHist->GetBinContent(endPos);
			minimumpos *= ADC_BINS_TO_TIME - triggertime;
		} catch (Exception& e)
		{
			cerr << e.error() << endl;
		}
		params->Fill();
	}
	params->Write();
	file->Close();
	//TODO check if, I can handle memory myself. For now, ROOT deletes file and params
	cout << "writing short param tree done" << endl;
}
