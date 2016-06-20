/*
 * Archive.cpp
 *
 *  Created on: 15.06.2016
 *      Author: stefan
 */

#include "Archive.h"
#include <malloc.h>
#include <cmath>
#include "omp.h"

using namespace std;

/**
 * Constructor, initializes the Archive object.
 *
 * @brief Constructor
 *
 * @author Stefan
 * @date June 20, 2016
 * @version 0.1
 *
 * @param filename relative path to the .root-file containing the raw data
 */
Archive::Archive(TString filename)
{
	TFile* file = readFile(filename);
	TTree* tree = readTree(file, "Tfadc");

	//TODO _rawData corrupts memory

	//TODO maybe better use it on stack.
	_rawData = convertAllEntriesToHistograms(tree);

	file->Close();
//	delete file;
}

Archive::~Archive()
{
	// TODO Auto-generated destructor stub
}

/**
 * Getter method for the raw data histogram. Returns a vector containing TH1*
 * type objects for all events.
 *
 * @brief raw data getter method
 *
 * @author Stefan
 * @date June 20, 2016
 * @version 0.2
 *
 * @return vector containing histograms for all events
 *
 * @warning Pointer to original vector, delete with care.
 */
vector<TH1*>* Archive::getRawData()
{
	return _rawData;
}

TFile* Archive::readFile(TString filename)
{
	return new TFile(filename, "read");
}

TTree* Archive::readTree(TFile* file, TString treename)
{
	return (TTree*) file->Get(treename);
}

TH1* Archive::convertEntryToHistogram(int entry, TTree* tree)
{
	if (entry >= tree->GetEntries())
	{
		cerr << "Only " << tree->GetEntries() << " events, are stored. Event "
				<< entry << " cannot be processed." << endl;
		return (TH1*) nullptr;
	}

	int numberOfChannels;
	double* voltage = new double[numberOfChannels];
	tree->SetBranchAddress("nchannels", &numberOfChannels);

	tree->GetEntry(0);
	cout << "Read branch nchannels = " << numberOfChannels << endl;
	tree->SetBranchAddress("Voltage", voltage);
	tree->GetEntry(entry);

	TH1D* rawData = new TH1D("Voltage", "FADC data", numberOfChannels, 0,
			numberOfChannels);
	for (int i = 0; i < numberOfChannels; i++)
	{
		rawData->SetBinContent(i, voltage[i]);
//		rawData->SetBinContent(i, voltage[i] - 2100); minus offset
	}
	rawData->GetXaxis()->SetTitle("channel number");
	rawData->GetYaxis()->SetTitle("voltage [a.u.]");

	delete voltage;

	//TODO check, why deleting this array results in segmentation violation

	return rawData;
}

vector<TH1*>* Archive::convertAllEntriesToHistograms(TTree* tree)
{
	int nEntries = tree->GetEntries();

	vector<TH1*>* result = new vector<TH1*>;

	for (int i = 0; i < nEntries; i++)
	{
		//TODO what the f*** happens here with memory?!
		cout << mallinfo().uordblks << endl;
		result->push_back(convertEntryToHistogram(i, tree));
	}

	return result;
}

TH1D* Archive::createTestHist()
{
	double min = 0;
	double max = 2 * M_PI;

	int nBins = 5 * 1e2;

	//must use lower edges of bins in order not to fill bins twice because of too low floating point precision
	//could as well use a local variable array if nBins is not too high to fit into the CPU cache
	Double_t* lowerEdgesOfBins = new Double_t[nBins + 1];

	double start = omp_get_wtime();
//	#pragma omp parallel for
	for (int i = 0; i <= nBins; i++)
	{
		lowerEdgesOfBins[i] = min + i * (max - min) / nBins;
	}

	TH1D* hist = new TH1D("test", "test1", nBins, lowerEdgesOfBins);

//#pragma omp parallel for
	for (int i = 0; i <= nBins; i++)
	{
		Double_t x = lowerEdgesOfBins[i];
		hist->Fill(x, sin(x));
	}

	cout << "Filling took " << omp_get_wtime() - start << " seconds." << endl;

	delete lowerEdgesOfBins;

	return hist;
}
