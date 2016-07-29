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
 * @date July 28, 2016
 * @version 0.3
 *
 * @param filename relative path to the .root-file containing the raw data
 */
Archive::Archive(TString filename)
{
	TFile file(filename,"read");
	TTree* tree = (TTree*)file.Get("Tfadc");
	cout << "Reading tree" << endl;

	_rawData = new DataSet();
	_numberOfEntries = tree->GetEntries();

	cout << "Beginning conversion. Entries: " << _numberOfEntries << endl;

	convertAllEntriesToHistograms(tree);

	cout << "DataSet size is: " << _rawData->getSize() << endl;

	_directory = parseDir(filename);
	_file = parseFile(filename);
	file.Close();
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
	stringstream name;
	name << _directory << "processed_" << _file;
	TString filename(name.str());
	cout << "Saving data to: " << filename << endl;
	writeToFile(filename);
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

/**
 * Getter method for the raw data histograms. Returns a DataSet* object pointer
 *
 * @brief raw data getter method
 *
 * @author Stefan
 * @date June 20, 2016
 * @version 0.2
 *
 * @return DataSet* containing histograms for all events
 *
 * @warning Pointer to original DataSet, delete with care.
 */
DataSet* Archive::getRawData()
{
	return _rawData;
}

//TODO overhaul
/**
 * Getter method for the processed data DataSet. Returns a DataSet* object pointer
 * containing the processed data
 *
 * @brief raw data getter method
 *
 * @author Stefan
 * @date June 20, 2016
 * @version 0.2
 *
 * @return DataSet* containing histograms for all events
 *
 * @warning Pointer to original object, delete with care.
 * @warning Not filled with actual TH1 objects until DataProcessor integrated
 * raw data
 */
DataSet* Archive::getProcessedData()
{
	return _processedData;
}

/**
 * Sets a DataSet as this objects member _processedData.
 *
 * @brief Setter method for processed data
 *
 * @author Stefan
 * @date July 19, 2016
 * @version 0.1
 *
 * @param data Pointer to the dataset containing the processed data
 */
void Archive::setProcessedData(DataSet* data)
{
	_processedData = data;
}

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
	try
	{
		return _rawData->getEvent(event);
	}
	catch(Exception& e)
	{
		cerr << e.error() << endl;
	}
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
	TFile file(filename,"recreate");
	file.mkdir("rawData");
	file.mkdir("integrated");

	TH1D* hist = nullptr;
	TH1D* integral = nullptr;

	for(int i = 0; i < _numberOfEntries; i++)
	{
		hist = _rawData->getEvent(i);
		integral = _processedData->getEvent(i);
		file.cd("rawData");
		hist->Write();
		file.cd("integrated");
		integral->Write();
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
	//Not reading this entry from the tree makes the program be run in seconds
	//instead of hours!!!!
	//No idea why, HELP WANTED
	const int numberOfChannels = 800;
//	tree->SetBranchAddress("nchannels", &numberOfChannels);
//	tree->GetEntry(0);

	double voltage[800];
	tree->SetBranchAddress("Voltage", &voltage);
	tree->GetEntry(entry);

	stringstream s;
	s << "Event #" << entry;
	TString name(s.str());

	TH1D* rawData = new TH1D(name,"FADC data", numberOfChannels, 0,
			numberOfChannels);
	for (int i = 0; i < numberOfChannels; i++)
	{
		rawData->SetBinContent(i, voltage[i] - 2200); //minus offset
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
 * @return TString containing the relative or absolute path
 */
TString Archive::parseDir(TString filename)
{
	int positionOfLastSlash = filename.Last('/');
	TSubString dir = filename(0,positionOfLastSlash+1);
	return TString(dir);
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
 * @return TString containing the filename
 *
 */
TString Archive::parseFile(TString filename)
{
	int positionOfLastSlash = filename.Last('/');
	TSubString file = filename(positionOfLastSlash+1,filename.Length());
	return TString(file);
}
