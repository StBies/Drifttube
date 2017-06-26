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
	_dtFilled = false;
	_rtFilled = false;
	_integralsFilled = false;
	m_filled_bitpattern = 0;
	TFile file(filename, "read");
	TTree* tree = (TTree*) file.Get("Tfadc");
	cout << "Reading tree" << endl;

	m_rawData = new DataSet();
	m_numberOfEntries = tree->GetEntries();

	cout << "Beginning conversion. Entries: " << m_numberOfEntries << endl;

	convertAllEntriesToHistograms(tree);

	cout << "DataSet size is: " << m_rawData->getSize() << endl;

	m_directory = parseDir(filename);
	m_file = parseFile(filename);
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
	name << m_directory << "processed_" << m_file;
	TString filename(name.str());
	cout << "Saving data to: " << filename << endl;
	writeToFile(filename);
}

/**
 * Returns the filename in a TString
 *
 * @brief Getter method for the filename
 *
 * @author Stefan
 * @data October 18, 2016
 * @version 1.0
 *
 * @return TString containing filename
 */
TString Archive::getFilename() const
{
	return m_file;
}

/**
 * Returns the directory name, where the original, raw data is, in a TString.
 *
 * @brief Getter method for the directory name
 *
 * @author Stefan
 * @data October 18, 2016
 * @version 1.0
 *
 * @return TString containing directory name
 */
TString Archive::getDirname() const
{
	return m_directory;
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
	TFile file(filename, "recreate");
	file.mkdir("rawData");
	file.mkdir("integrated");
	file.mkdir("dtSpect");
	file.mkdir("rtRelation");

	TH1D* hist = nullptr;
	TH1D* integral = nullptr;

	for (int i = 0; i < m_numberOfEntries; i++)
	{
		hist = m_rawData->getEvent(i);
		integral = m_processedData->getEvent(i);
		file.cd("rawData");
		hist->Write();
		file.cd("integrated");
		integral->Write();
	}
	file.cd("dtSpect");
	m_drifttimeSpect->Write();
	file.cd("diffDriftTime");
	_diffDtSpect->Write();
	file.cd("rtRelation");
	m_rtRelation->Write();
	file.Close();
	cout << "Saving complete" << endl;
}

/**
 * Converts the raw data, that is contained in the TTree tree to a one dimensional histogram
 * of type TH1*
 *
 * @brief Convert data from tree to histogram
 *
 * @author Stefan
 * @date October 17, 2016
 * @version 0.9
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
	const int numberOfChannels = 800;

	double voltage[800];
	tree->SetBranchAddress("Voltage", &voltage);
	tree->GetEntry(entry);

	stringstream s;
	s << "Event #" << entry;
	TString name(s.str());

	TH1D* rawData = new TH1D(name, "FADC data", numberOfChannels, 0,
			numberOfChannels * ADC_BINS_TO_TIME);

	//#pragma omp parallel for
	for (int i = 0; i < numberOfChannels; i++)
	{
		rawData->SetBinContent(i,
				(voltage[i] - 2200) * ADC_CHANNELS_TO_VOLTAGE); //minus offset
	}
	rawData->GetXaxis()->SetTitle("time [ns]");
	rawData->GetYaxis()->SetTitle("voltage [V]");

	return rawData;
}

/**
 * Converts the entry from the TTree in the root file to the data type used internally. The resulting objects are created
 * on the heap and ownership is managed via a std::unique_ptr. After this method finished, it returns the unique ptr and transfers
 * ownership with the return.
 *
 * @brief Convert from root-file to internally used data
 *
 * @author Stefan Bieschke
 * @date June 26, 2017
 * @version Alpha 2.0
 *
 * @return unique pointer to an Event containing the data. The ownership is transferred to caller after this method.
 */
std::unique_ptr<Event> Archive::convertEntry(unsigned int entry, TTree* tree)
{
	const int numberOfChannels = 800;

	double voltage[800];
	tree->SetBranchAddress("Voltage", &voltage);
	tree->GetEntry(entry);

	unique_ptr<array<uint16_t,800>> arr(new array<uint16_t,800>);

	unique_ptr<Event> rawData(new Event(entry,move(arr)));

	#pragma omp parallel for
	for (int i = 0; i < numberOfChannels; i++)
	{
		(*rawData)[i] = (uint16_t)voltage[i];
	}

	return move(rawData);
}

///**
// * Converts all event data stored in a tree to histograms of type TH1*.
// * The converted histograms are stored in the member variable _rawData.
// *
// * @brief Convert all data in tree
// *
// * @author Stefan
// * @date June 28, 2016
// * @version 0.1
// *
// * @param tree TTree object, that contains the data to be converted
// */
//void Archive::convertAllEntriesToHistograms(TTree* tree)
//{
//	for (int i = 0; i < _numberOfEntries; i++)
//	{
//		_rawData->addData(convertEntryToHistogram(i, tree));
//	}
//}

/**
 * Converts all event data stored in a tree to unique pointers to std::arrays containing the data.
 * The converted histograms are stored in the member variable _rawData.
 *
 * @brief Convert all data in tree to datatypes used internally
 *
 * @author Stefan Bieschke
 * @date April 18, 2017
 * @version Alpha 2.0
 *
 * @param tree TTree object, that contains the data to be converted
 */
void Archive::convertAllEntries(TTree* tree)
{
	for (int i = 0; i < m_numberOfEntries; i++)
	{
		m_rawData->addData(std::move(convertEntry(i, tree)));
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
	TSubString dir = filename(0, positionOfLastSlash + 1);
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
	TSubString file = filename(positionOfLastSlash + 1, filename.Length());
	return TString(file);
}
