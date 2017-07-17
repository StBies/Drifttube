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
 * @author Stefan Bieschke
 * @date July 17, 2017
 * @version Alpha 2.0
 *
 * @param filename relative path to the .drift-file containing the raw data
 */
Archive::Archive(string filename)
{
	cout << "Archive ctor entered. Filename: " << filename << endl;
	convertAllEntries(filename);

	m_directory = parseDir(filename);
	m_file = parseFile(filename);
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
	string filename(name.str());
	cout << "Saving data to: " << filename << endl;
//	writeToFile(filename);
}

/**
 * Returns the filename in a string.
 *
 *
 * @brief Getter method for the filename
 *
 * @author Stefan Bieschke
 * @date July 17, 2017
 * @version Alpha 2.0
 *
 * @return string containing filename
 */
const string& Archive::getFilename() const
{
	return m_file;
}

/**
 * Returns the directory name, where the original, raw data is.
 *
 * @brief Getter method for the directory name
 *
 * @author Stefan Bieschke
 * @date July 17, 2017
 * @version Alpha 2.0
 *
 * @return string containing directory name
 */
const string& Archive::getDirname() const
{
	return m_directory;
}


/**
 * Getter method for the Drifttubes. Returns a vector containing unique pointers to the drift tubes.
 *
 * @brief Drifttubes getter.
 *
 * @author Stefan Bieschke
 * @date July 17, 2017
 * @version Alpha 2.0
 *
 * @return Const reference to the vector containing unique_ptr<Drifttube> objects.
 */
const vector<unique_ptr<Drifttube>>& Archive::getTubes() const
{
	return m_tubes;
}

///**
// * Writes the histograms to a file, that is specified with parameter filename.
// *
// * @brief Write data to file
// *
// * @author Stefan
// * @date June 22, 2016
// * @version 0.1
// *
// * @param filename relative path to the file, that is opened in mode "RECREATE", handle with care!
// *
// * @warning preliminary, does not yet contain processed data.
// */
//void Archive::writeToFile(const string filename)
//{
//	TFile file(filename, "recreate");
//	file.mkdir("rawData");
//	file.mkdir("integrated");
//	file.mkdir("dtSpect");
//	file.mkdir("rtRelation");
//
//	TH1D* hist = nullptr;
//	TH1D* integral = nullptr;
//
//	for (int i = 0; i < m_numberOfEntries; i++)
//	{
//		hist = m_rawData->getEvent(i);
//		integral = m_processedData->getEvent(i);
//		file.cd("rawData");
//		hist->Write();
//		file.cd("integrated");
//		integral->Write();
//	}
//	file.cd("dtSpect");
//	m_drifttimeSpect->Write();
//	file.cd("diffDriftTime");
//	_diffDtSpect->Write();
//	file.cd("rtRelation");
//	m_rtRelation->Write();
//	file.Close();
//	cout << "Saving complete" << endl;
//}

///**
// * Converts the entry from the TTree in the root file to the data type used internally. The resulting objects are created
// * on the heap and ownership is managed via a std::unique_ptr. After this method finished, it returns the unique ptr and transfers
// * ownership with the return.
// *
// * @brief Convert from root-file to internally used data
// *
// * @author Stefan Bieschke
// * @date June 26, 2017
// * @version Alpha 2.0
// *
// * @return unique pointer to an Event containing the data. The ownership is transferred to caller after this method.
// */
//std::unique_ptr<Event> Archive::convertEntry(unsigned int entry, TTree* tree)
//{
//	const int numberOfChannels = 800;
//
//	double voltage[800];
//	tree->SetBranchAddress("Voltage", &voltage);
//	tree->GetEntry(entry);
//
//	unique_ptr<array<uint16_t,800>> arr(new array<uint16_t,800>);
//
//	unique_ptr<Event> rawData(new Event(entry,move(arr)));
//
//	#pragma omp parallel for
//	for (int i = 0; i < numberOfChannels; i++)
//	{
//		(*rawData)[i] = (uint16_t)voltage[i];
//	}
//
//	return move(rawData);
//}

/**
 * Converts all event data stored in a binary file to the data types needed internally
 * The converted data is stored in DataSets for each drifttube.
 *
 * @brief Convert all data in tree to datatypes used internally
 *
 * @author Stefan Bieschke
 * @date July 12, 2017
 * @version Alpha 2.0
 *
 * @param filename relative path of the file containing raw data
 */
void Archive::convertAllEntries(const std::string filename)
{
	//TODO implement
	//TODO test
	ifstream file(filename, ios::binary);
	FileParams par(readHeader(file));

	const size_t nEvents = par.nEvents;
	const size_t eventSize = 800;
	const size_t nTubes = par.nTubes;

	cout << "Beginning conversion:" << endl;
	cout << "Events: " << nEvents << endl << "tubes: " << nTubes << endl << "Bins per event: " << par.eventSize << endl;
	file.seekg(par.endOfHeader);

	for(size_t i = 0; i < nTubes; i++)
	{
		vector<unique_ptr<Event>> events(nEvents);
//		cout << "vector with events initialized. Size: " << events.size() << endl;
		for(size_t j = 0; j < events.size(); j++)
		{
			unique_ptr<array<uint16_t,eventSize>> arr(new array<uint16_t,eventSize>);
//			cout << "Array for event #" << j << " initialized. Size: " << arr->size() << endl;
			for(size_t k = 0; k < arr->size(); k++)
			{

				uint16_t buffer;
				file.read((char*)&buffer,sizeof(uint16_t));
				(*arr)[k] = buffer;
//				if(j % 5000 == 0)
//				{
//					cout << "Filled event #" << j << " bin #" << k << " with value " << buffer << endl;
//				}
			}
			unique_ptr<Event> e(new Event(j,move(arr)));
//			cout << "Event built at " << e.get() << endl;
			//zero supression - if no valid drift time was found: reject (a.k.a store nullptr)
	#ifdef ZEROSUP
			if(e->getDriftTime() < 0)
			{
				cout << "Rejected event #" << e->getEventNumber() << endl;
				e.reset();
			}
	#endif
			events[j] = move(e);
		}
		cout << "Events built. Beginning building of tube object" << endl;
		//TODO implement positions init
		unique_ptr<DataSet> set(new DataSet(events));
		cout << "DataSet built. Size: " << set->getSize() << endl;

		m_tubes.push_back(unique_ptr<Drifttube>(new Drifttube(1,2,move(set))));
	}
	cout << "finished converting, closing file." << endl;
	file.close();
	cout << "file closed" << endl;
}

/**
 * Parses the directory from the given String containing the full path to file.
 *
 * Example 1: For the string '/path/to/file.drift' result is '/path/to/'
 * Example 2: For the string './../../path/file.drift' result is './../../path/'
 *
 * @brief Directory parser
 *
 * @author Stefan Bieschke
 * @date July 17, 2017
 * @version Alpha 2.0
 *
 * @param filename string containing the full path to file
 *
 * @return string containing the relative or absolute path
 */
string Archive::parseDir(const string filename)
{
	//TODO test
	size_t positionOfLastSlash = filename.find_last_of('/');
	string dir = filename.substr(0,positionOfLastSlash+1);
	return dir;
}

/**
 * Parses the file from the given string containing the full path to file.
 *
 * Example 1: For the string '/path/to/file.drift' result is 'file.drift'
 * Example 2: For the string './../../path/file.drift' result is 'file.drift'
 *
 * @brief Filename parser
 *
 * @author Stefan Bieschke
 * @date July 17, 2017
 * @version Alpha 2.0
 *
 * @param filename string containing the full path to file
 *
 * @return string containing the filename
 *
 */
string Archive::parseFile(string filename)
{
	//TODO test
	size_t positionOfLastSlash = filename.find_last_of('/');
	string file = filename.substr(positionOfLastSlash + 1);
	return file;
}

FileParams Archive::readHeader(ifstream& file)
{
	size_t nTubes, eventSize, nEvents;
	if(file.is_open())
	{
		file.seekg(0,ios::beg);
		file.read((char*)&nTubes,sizeof(size_t));
		file.read((char*)&nEvents,sizeof(size_t));
		file.read((char*)&eventSize,sizeof(size_t));
	}

	streampos pos = file.tellg();
	FileParams result;
	result.nTubes = nTubes;
	result.nEvents = nEvents;
	result.eventSize = eventSize;
	result.endOfHeader = pos;

	return result;
}
