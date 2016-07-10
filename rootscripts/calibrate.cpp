#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

void calibrate(TString triggerDataFile)
{
	TFile file(triggerDataFile,"read");
	TFile calib("calib.root","recreate");
	file.cd();

	TTree* triggerDataTree = (TTree*)file.Get("Tfadc");
	int nEntries = triggerDataTree->GetEntries();
	int nChannels;
	triggerDataTree->SetBranchAddress("nchannels",&nChannels);
	triggerDataTree->GetEntry(0);

	double* voltage = new double[nChannels];
	triggerDataTree->SetBranchAddress("Voltage",voltage);

	Double_t triggerpos = 0;

	for(int i = 0; i < nEntries - 1; i++)
	{
		triggerDataTree->GetEntry(i);
		int minBin = 0;
		double minContent = voltage[0];
		for(int j = 0; j < nChannels; j++)
		{
			if(voltage[j] < minContent)
			{
				minBin = j;
			}
		}
		triggerpos += minBin;
	}
	triggerpos /= nEntries;

	file.Close();
	calib.cd();

	TTree calibTree("trigger","triggerdata");
	TBranch *branch = calibTree.Branch("calib",&triggerpos,"triggerpos/D");
	branch->Fill();
	calibTree.Write();
	calib.Close();

}
