#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TString.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
void BIN2ROOT(string file)
{
	ifstream infile(file.c_str(),ios::binary);

	size_t nTubes, eventSize, nEvents;
	if(infile.is_open())
	{
		infile.seekg(0,ios::beg);
		infile.read((char*)&nTubes,sizeof(size_t));
		infile.read((char*)&nEvents,sizeof(size_t));
		infile.read((char*)&eventSize,sizeof(size_t));
	}

	cout << nTubes << "\t" << nEvents << endl;

	TFile* f = new TFile("bla.root","RECREATE");
	for(size_t tube = 0; tube < nTubes; tube++)
	{
		cout << "Tube " << tube << endl;
		TString tubeDirName = "Tube";
		tubeDirName.Append(tube);
		TDirectory* tDir = f->mkdir(tubeDirName);
		tDir->mkdir("RawData");
		tDir->cd("RawData");
		for(size_t data = 0; data < nEvents; data++)
		{
			double datum = 0;
			size_t evtNr = 0;
			infile.read((char*)&evtNr,sizeof(size_t));
			stringstream titleBuild;
			titleBuild << "Event #" << evtNr;
			TString title(titleBuild.str());
			TH1D* hist = new TH1D(title, "FADC Data",800,0,3200);
			hist->GetXaxis()->SetTitle("time [ns]");
			hist->GetYaxis()->SetTitle("voltage [V]");
			for(size_t i = 0; i < eventSize; i++)
			{
				infile.read((char*)&datum,sizeof(double));
				hist->SetBinContent(i+1,datum);
			}
			hist->Write();
			delete hist;
		}
	}
	TH1I* dtSpect = new TH1I("dtSpect","Drift time spectrum",800,0,3200);
	for(size_t i = 0; i < eventSize; i++)
	{
		UInt_t entries;
		infile.read((char*)&entries,sizeof(UInt_t));
		dtSpect->SetBinContent(i+1,entries);
	}
	dtSpect->Write();

	infile.close();
	f->Close();
}
