#include "teaching.h"


/**
 * This function reads all data from a DataSet object and writes a binary file that contains the raw events. This file is fairly simple:
 * it contains (on a 64bit machine) a header that is eight bytes long (64bit) which only contains the number of events written to the file.
 * No further information is contained, it assumes that 800 doubles per event are written.
 *
 *
 * @author Stefan Bieschke
 * @version 1.0
 * @date Jan 15, 2018
 *
 * @param data const reference to the DataSet object copntaining the events
 * @return number of events written
 *
 * @warning This function is only present for teaching purposes - it will probably not be maintained is its functionality is fairly simple.
 * @warning For scientific use better look into to the input-/output-methods in class Archive.
 *
 * @ensure returns the same number as written in the header.
 */
int teaching_writePythonData(const DataSet& data)
{
	size_t nEvents = data.getSize();

	ofstream numpyDump("event.npy",ios::out | ios::binary);
	size_t headerPosition = numpyDump.tellp();
	size_t eventsWritten = 0;

	//write Header: 8bytes: nEvents
	numpyDump.write((char*)&nEvents,sizeof(size_t));

	//loop over the DataSet
	for(size_t i = 0; i < nEvents; i++)
	{
		//try to get the event - might be zero supressed and not present however
		try
		{
			std::cout << "trying to write event #" << i <<".";
			Event e = data[i];
			array<uint16_t,800> raw_data = e.getData();

			for(uint16_t datum : raw_data)
			{
				double value = ((double)datum) * 12/4096;
				//std::cout << value << std::endl;
				numpyDump.write((char*)&value,sizeof(double));
			}
			std::cout << " Done." << std::endl;
			++eventsWritten;
		}
		catch(Exception& ex) //if event i is zerosupressed - go to next event
		{
			std::cout << "Not written. Zero supressed." << std::endl;
			continue;
		}
	}
	numpyDump.seekp(headerPosition);

	//overwrite header with events written after zerocompression
	numpyDump.write((char*)&eventsWritten,sizeof(size_t));

	std::cout << "Events written: " << eventsWritten << std::endl;

	numpyDump.close();

	return eventsWritten;
}
