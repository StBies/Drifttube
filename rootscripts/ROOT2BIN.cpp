#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>
#include <fstream>

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

	unsigned int nEntries = tree->GetEntries();
	const int numberOfChannels = 800;

	cout << "Beginning conversion. Entries: " << nEntries << endl;

	ofstream of("test2.drift", ios::out | ios::binary);
	of.write((char*)&nEntries,sizeof(unsigned int));
	of.write((char*)&numberOfChannels,sizeof(int));

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
