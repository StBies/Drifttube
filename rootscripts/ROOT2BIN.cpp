#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <stdint.h>

using namespace std;

/**
 * First version of a binary file conversion tool. For now, only works with a single drift tube
 * @param filename
 */
void ROOT2BIN(TString filename)
{
	TFile file(filename, "read");
	TTree* tree = (TTree*) file.Get("Tfadc");
	cout << "Reading tree" << endl;

	uint32_t nEntries = tree->GetEntries();
	const uint32_t numberOfChannels = 800;
	uint32_t nTubes = 1;

	cout << "Beginning conversion. Entries: " << nEntries << endl;

	ofstream of("unitTestingData_new.drift", ios::out | ios::binary);
	//write header
	of.write((char*)&nTubes,sizeof(uint32_t));
	of.write((char*)&nEntries,sizeof(uint32_t));
	of.write((char*)&numberOfChannels,sizeof(uint32_t));

	//write data
	double voltage[800];
	tree->SetBranchAddress("Voltage", &voltage);
	for (unsigned int entry = 0; entry < nEntries; entry++)
	{
		tree->GetEntry(entry);
		for (unsigned short i = 0; i < numberOfChannels; i++)
		{
			UShort_t value = (UShort_t)voltage[i];
			of.write((char*)&value,sizeof(UShort_t));
		}
	}

	of.close();
	file.Close();
}
