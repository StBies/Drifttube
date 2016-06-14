#include "DataProcessor.h"
#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

/**
 * Constructor, initializes the DataProcessor object.
 * 
 * @brief Constructor
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1
 * 
 * @param filename relative path to the .root-file containing the raw data
 */
DataProcessor::DataProcessor(TString filename)
{
	_dataFile = new TFile(filename,"read");
	_rawData = createTestHist();

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
	
}

TH1D DataProcessor::createTestHist()
{
	double min = 0.0;
	double max = M_PI;

	int nBins =5 * 1e6;
	
	//must use lower edges of bins in order not to fill bins twice because of too low floating point precision
	//could as well use a local variable array if nBins is not too high to fit into the CPU cache
	Double_t* lowerEdgesOfBins = new Double_t[nBins + 1];
	

	double start = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i <= nBins; i++)
	{
		lowerEdgesOfBins[i] = i * (max - min) / nBins;
	}

	TH1D* hist = new TH1D("test","test1",nBins,lowerEdgesOfBins);
	
	#pragma omp parallel for
	for(int i = 0; i <= nBins; i++)
	{
		Double_t x = lowerEdgesOfBins[i];
		hist->Fill(x,sin(x));
	}
	
	cout << "Filling took " << omp_get_wtime() - start << " seconds." << endl;

	delete lowerEdgesOfBins;
	
	return *hist;
}


/**
 * Getter method for the raw data histogram. Returns a copy of the TH1I type
 * object.
 * 
 * @brief raw data getter method
 * 
 * @author Stefan
 * @date May 30, 2016
 * @version 0.1
 * 
 * @return Copy of the TH1I type histogram object
 * 
 * @warning Does only return a COPY of the original histogram. Later changes
 * will not be included in this copy.
 */
TH1D DataProcessor::getRawData()
{
	return _rawData;
}



Double_t DataProcessor::computeIntegral(TH1& data)
{
	//Double_t nBins;
	Double_t zaehler = 0;

	//nBins = data.GetNbinsX();

	for(int i = 0; i < data.GetNbinsX(); i++)
		{
			zaehler = zaehler + data.GetBinContent(i);
		}
	return zaehler;

}


/**
 * Integrates the given data, which is in a histogram. This method will return
 * a new histogram of type TH1I* containing the integrated original data.
 * 
 * @brief histogram integrator
 * 
 * @author Stefan
 * @date June 9, 2016
 * @version 0.1
 * 
 * @param data Data, that is to be integrated
 * 
 * @return TH1I* pointer to a new heap-object histogram containing the integral of data
 * 
 * @warning Not yet implemented
 * @warning Returned object must be destoyed by the user
 */
TH1D* DataProcessor::integrate(TH1D& data)
{
	return new TH1D("not final","yet",1337,42,1337);
}
