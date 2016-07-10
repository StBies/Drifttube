/*
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
	TFile* file = new TFile(filename,"read");
	TTree* tree = (TTree*)file->Get("Tfadc");
	cout << "Reading tree" << endl;

	_rawData = new DataSet();
	_numberOfEntries = (tree->GetEntries() - 2);

	cout << "Beginning conversion. Entries: " << _numberOfEntries << endl;

//	convertAllEntriesToHistograms(tree);

	for(int i = 0; i < _numberOfEntries; i++)
	{
		_rawData->addData(convertEntryToHistogram(i,tree));
	}

	cout << "DataSet size is: " << _rawData->getSize() << endl;

	_directory = parseDir(filename);
	_file = parseFile(filename);
	file->Close();
}


/**
 * Destructor, that MUST be called when destroying the archive. Destructor
 * calls a function to write the archives content to disk.
 *
 * @brief Dtor
 *
 * @author Stefan
 * @date June 28,2016
 * @version 0.1
 */
Archive::~Archive()
{
	writeToFile("./testdata/converted.root");
	delete _rawData;
	delete _processedData;
}


/**
 * Method to return the total number of events that are stored in this archive.
 *
 * @brief Getter method for #events
 *
 * @author Stefan
 * @date June 28, 2016
 * @version 0.1
 *
 * @return integer number of events that are stored in this archive
 */
int Archive::getSize()
{
	return _numberOfEntries;
}

//TODO overhaul
/**
 * Getter method for the raw data histograms. Returns an array containing TH1*
 * type objects for all events.
 *
 * @brief raw data getter method
 *
 * @author Stefan
 * @date June 20, 2016
 * @version 0.2
 *
 * @return Array containing histograms for all events
 *
 * @warning Pointer to original array, delete with care.
 */
DataSet* Archive::getRawData()
{
	return _rawData;
}

//TODO overhaul
/**
 * Getter method for the processed data histograms. Returns an array containing TH1*
 * type objects for all events.
 *
 * @brief raw data getter method
 *
 * @author Stefan
 * @date June 20, 2016
 * @version 0.2
 *
 * @return Array containing histograms for all events
 *
 * @warning Pointer to original array, delete with care.
 * @warning Not filled with actual TH1 objects until DataProcessor integrated
 * raw data
 */
DataSet* Archive::getProcessedData()
{
	return _processedData;
}

//TODO exceptionhandling
/**
 * Method, that returns a histogram for a single event.
 *
 * @brief Getter method for single event
 *
 * @author Stefan
 * @date June 28, 2016
 * @version 0.1
 *
 * @param event Number of the event to be returned
 *
 * @return TH1* type object histogram containing the data of the event.
 *
 * @warning Might be casted to TH1D* type object.
 *
 * @require event < #events_total
 */
TH1D* Archive::getEvent(int event)
{
	return _rawData->getEvent(event);
}

/**
 * Read a file given with total or relative path as parameter and return the
 * opened file.
 *
 * @author Stefan
 * @date June 28, 2016+
 * @version 0.1
 *
 * @param filename relative or absolute path to the file
 *
 * @return TFile* object pointer to opened file
 *
 * @warning must be closed by caller
 */
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
	file.cd();


	for(int i = 0; i < _numberOfEntries; i++)
	{
		TH1D* hist = _rawData->getEvent(i);
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
 * @return pointer to the TH1D* type histogram
 *
 * @warning Heap object returned, deletion must be taken care about by the caller.
 */
TH1D* Archive::convertEntryToHistogram(int entry, TTree* tree)
{
	int numberOfChannels;
	tree->SetBranchAddress("nchannels", &numberOfChannels);
	tree->GetEntry(0);

	double* voltage = new double[numberOfChannels];
	tree->SetBranchAddress("Voltage", voltage);
	tree->GetEntry(entry);

	//TODO check, if stringstream would work as well
	char name[20];
	sprintf(name,"Event #%d",entry);

	TH1D* rawData = new TH1D(name,"FADC data", numberOfChannels, 0,
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

/**
 * Converts all event data stored in a tree to histograms of type TH1*.
 * The converted histograms are stored in the member variable _rawData.
 *
 * @brief Convert all data in tree
 *
 * @author Stefan
 * @date June 28, 2016
 * @version 0.1
 *
 * @param tree TTree object, that contains the data to be converted
 */
void Archive::convertAllEntriesToHistograms(TTree* tree)
{
	for (int i = 0; i < _numberOfEntries; i++)
	{
		_rawData->addData(convertEntryToHistogram(i, tree));
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

	for (int i = 0; i <= nBins; i++)
	{
		lowerEdgesOfBins[i] = min + i * (max - min) / nBins;
	}

	TH1D* hist = new TH1D("test", "test1", nBins, lowerEdgesOfBins);

	for (int i = 0; i <= nBins; i++)
	{
		Double_t x = lowerEdgesOfBins[i];
		hist->Fill(x, sin(x));
	}

	delete lowerEdgesOfBins;

	return hist;
}

/**
 * Parses the directory from the given String containing the full path to file.
 *
 * Example 1: For the TString '/path/to/file.root' result is '/path/to/'
 * Example 2: For the TString './../../path/file.root' result is './../../path/'
 *
 * @brief Directory parser
 *
 * @author Stefan
 * @date June 28, 2016
 * @version 0.1
 *
 * @param filename TString containing the full path to file
 *
 * @return Pointer to a TString containing the relative or absolute path
 *
 * @warning Heap object, memory management to be done by caller
 */
TString* Archive::parseDir(TString filename)
{
	int positionOfLastSlash = filename.Last('/');
	TSubString dir = filename(0,positionOfLastSlash+1);
	return new TString(dir);
}

/**
 * Parses the file from the given String containing the full path to file.
 *
 * Example 1: For the TString '/path/to/file.root' result is 'file.root'
 * Example 2: For the TString './../../path/file.root' result is 'file.root'
 *
 * @brief Filename parser
 *
 * @author Stefan
 * @date June 28, 2016
 * @version 0.1
 *
 * @param filename TString containing the full path to file
 *
 * @return Pointer to a TString containing the filename
 *
 * @warning Heap object, memory management to be done by caller
 */
TString* Archive::parseFile(TString filename)
{
	int positionOfLastSlash = filename.Last('/');
	TSubString file = filename(positionOfLastSlash+1,filename.Length());
	return new TString(file);
}
