/*
 * Archive.cpp
 *
 *  Created on: 15.06.2016
 *      Author: stefan
 */

#include "Archive.h"
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
	TTree* tree = readTree(file,"Tfadc");

//	TFile* file = new TFile(filename,"read");
//	TTree* tree = (TTree*)file->Get("Tfadc");

	_numberOfEntries = tree->GetEntries();

	_rawData = new TH1*[_numberOfEntries];
	_processedData = new TH1*[_numberOfEntries];

	for(int i = 0; i < _numberOfEntries; i++)
	{
		_rawData[i] = convertEntryToHistogram(i,tree);
	}


	file->Close();
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
TH1** Archive::getRawData()
{
	return _rawData;
}

//TODO comment
TH1* Archive::getEvent(int event)
{
	return _rawData[event];
}

//TODO comment
TFile* Archive::readFile(TString filename)
{
	return new TFile(filename, "read");
}


/**
 * Read a tree with given treename from the TFile file
 *
 * @brief Read a tree from a file
 *
 * @author Stefan
 * @date June 22, 2016
 * @version 0.1
 *
 * @param file Pointer to TFile object containing the tree
 * @param treename Name of the TTree object
 *
 * @return Pointer to the read TTree object.
 *
 * @warning Heap object returned, deletion must be taken care about by the caller.
 */
TTree* Archive::readTree(TFile* file, TString treename)
{
	return (TTree*) file->Get(treename);
}

/**
 * Writes the histograms to a file, that is specified with parameter filename.
 *
 * @brief Write data to file
 *
 * @author Stefan
 * @date June 22, 2016
 * @version 0.1
 *
 * @param filename relative path to the file, that is opened in mode "RECREATE", handle with care!
 *
 * @warning preliminary, does not yet contain processed data.
 */
void Archive::writeToFile(TString filename)
{
	//TODO better save a tree for raw data as well as a tree for processed data later.
	TFile file(filename,"recreate");
	for(int i = 0; i < _numberOfEntries; i++)
	{
		TH1* hist = _rawData[i];
		hist->Write();
	}
	file.Close();
}


/**
 * Converts the raw data, that is contained in the TTree tree to a one dimensional histogram
 * of type TH1*
 *
 * @brief Convert data from tree to histogram
 *
 * @author Stefan
 * @date June 22, 2016
 * @version 0.1
 *
 * @param entry Number of the entry, you want to convert. Must be smaller than the number of entries in the tree
 * @param tree Pointer to the TTree containing the raw data
 *
 * @return pointer to the TH1* type histogram
 *
 * @warning Heap object returned, deletion must be taken care about by the caller.
 */
TH1* Archive::convertEntryToHistogram(int entry, TTree* tree)
{
	if (entry >= tree->GetEntries())
	{
		cerr << "Only " << tree->GetEntries() << " events, are stored. Event "
				<< entry << " cannot be processed." << endl;
		return (TH1*) nullptr;
	}

	int numberOfChannels;
	tree->SetBranchAddress("nchannels", &numberOfChannels);
	tree->GetEntry(0);
	//voltage on stack seems not to work since it's size is not known
	//at compile time
	//double voltage[numberOfChannels];
	double* voltage = new double[numberOfChannels];
	tree->SetBranchAddress("Voltage", voltage);
	tree->GetEntry(entry);

	
	//Does not work to set as title for some reason
	//stringstream name;
	//name << "Event nr. " << entry;
	//const TString title = *new TString(name.str().c_str(),14);

	TH1D* rawData = new TH1D("Voltage", "FADC data", numberOfChannels, 0,
			numberOfChannels);
	for (int i = 0; i < numberOfChannels; i++)
	{
//		rawData->SetBinContent(i, voltage[i]);
		rawData->SetBinContent(i, voltage[i] - 2100); //minus offset
	}
	rawData->GetXaxis()->SetTitle("channel number");
	rawData->GetYaxis()->SetTitle("voltage [a.u.]");

	return rawData;
}

void Archive::convertAllEntriesToHistograms(TTree* tree)
{
	int nEntries = tree->GetEntries();

	for (int i = 0; i < nEntries; i++)
	{
		_rawData[i] = convertEntryToHistogram(i, tree);
	}
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
