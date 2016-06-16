#include "DataProcessor.h"

using namespace std;

/**
 * Constructor, initializes the DataProcessor object.
 * 
 * @brief Constructor
 * 
 * @author Stefan
 * @date June 16, 2016
 * @version 0.3
 * 
 * @param filename relative path to the .root-file containing the raw data
 */
DataProcessor::DataProcessor(TString filename)
{

	//TODO clean up here
	//TODO implement a way to read ALL events stored in the .root-file
	//TODO read the raw data in a seperate, private method
	//TODO store the histograms in an archive

	_dataFile = new TFile(filename,"read");
	_rawTree = (TTree*)_dataFile->Get("Tfadc");
	int numberOfChannels;
	_rawTree->SetBranchAddress("nchannels",&numberOfChannels);
	_rawTree->GetEntry(0);
	double* voltage = new double[numberOfChannels];
	_rawTree->SetBranchAddress("Voltage",voltage);
	_rawTree->GetEntry(6);
	_rawData = *new TH1D("Voltage","FADC data",numberOfChannels,0,numberOfChannels);
	_rawData.GetXaxis()->SetTitle("channel number");
	_rawData.GetYaxis()->SetTitle("voltage [a.u.]");

	for(int i = 0; i < numberOfChannels; i++)
	{
		_rawData.SetBinContent(i,voltage[i]);
	}

	delete voltage;
	//_rawData = createTestHist();
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

TH1D DataProcessor::createTestHist()
{
	double min = 0;
	double max = 2* M_PI;

	int nBins = 5 * 1e2;
	
	//must use lower edges of bins in order not to fill bins twice because of too low floating point precision
	//could as well use a local variable array if nBins is not too high to fit into the CPU cache
	Double_t* lowerEdgesOfBins = new Double_t[nBins+1];
	

	double start = omp_get_wtime();
//	#pragma omp parallel for
	for(int i = 0; i <= nBins; i++)
	{
		lowerEdgesOfBins[i] = min + i * (max - min) / nBins;
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
Double_t DataProcessor::computeIntegral(TH1& data)
{
	//Double_t nBins;
	Double_t counter = 0;

	//nBins = data.GetNbinsX();

	//TODO check if parallelization is of use here

	for(int i = 0; i < data.GetNbinsX(); i++)
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
 * @warning Returned object must be destoyed by the user
 */
TH1* DataProcessor::integrate(TH1& data)
{
	Int_t nBins = data.GetNbinsX();
	Double_t* binLowEdges = new Double_t[nBins+1];

	for(Int_t i = 0; i <= nBins; i++)
	{
		binLowEdges[i] = data.GetBinLowEdge(i);
	}

	//TODO Check the actual parameter type and and return the correct object accordingly
	TH1D* result = new TH1D("not final","yet",nBins,binLowEdges);
	result->GetXaxis()->SetTitle(data.GetXaxis()->GetTitle());
	stringstream yTitle;
	yTitle << "integral of " << data.GetYaxis()->GetTitle();
	result->GetYaxis()->SetTitle(yTitle.str().c_str());


	delete binLowEdges;
	//choose random bin width since all bins are the same size - might change
	Double_t binWidth = result->GetBinWidth(1);

	double integral = 0.0;

	for(int i = 0; i <= nBins; i++)
	{
		integral += data.GetBinContent(i)*binWidth;
		result->Fill(data.GetBinLowEdge(i),integral);
	}

	return result;
}
