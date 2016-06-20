/*
 * Archive.cpp
 *
 *  Created on: 15.06.2016
 *      Author: stefan
 */

#include "Archive.h"

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

	//TODO maybe better use it on stack.
	_rawData = *new vector<TH1*>;

	for (int i = 0; i < tree->GetEntries(); i++)
	{
		_rawData.push_back(convertEntryToHistogram(i, tree));
	}

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
	return &_rawData;
}

TFile* Archive::readFile(TString filename)
{
	cout << "reading file " << filename << endl;
	return new TFile(filename, "read");
}

TTree* Archive::readTree(TFile* file, TString treename)
{
	cout << "reading tree " << treename << " from file." << endl;
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
		cout << i << "\t" << voltage[i] << endl;
	}
	rawData->GetXaxis()->SetTitle("channel number");
	rawData->GetYaxis()->SetTitle("voltage [a.u.]");

	//TODO check, why deleting this array results in segmentation violation

	return rawData;
}
