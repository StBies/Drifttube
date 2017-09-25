#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TString.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//TODO comment
//TODO TEST!!!!
void BIN2ROOT(string file)
{
	ifstream infile(file.c_str(),ios::binary);

	//read binary file header
	size_t nTubes, eventSize, nEvents;
	if(infile.is_open())
	{
		infile.seekg(0,ios::beg);
		infile.read((char*)&nTubes,sizeof(size_t));
		infile.read((char*)&nEvents,sizeof(size_t));
		infile.read((char*)&eventSize,sizeof(size_t));
	}

	cout << nTubes << "\t" << nEvents << endl;

	//create name for the replace .drift filetype with .root
	TString ifName(file);
	TString ofName = ifName(0,ifName.Last('.'));
	ofName.Append(".root");

	cout << "creating file" << ofName << endl;

	TFile* f = new TFile(ofName,"RECREATE");
	for(size_t tube = 0; tube < nTubes; tube++)
	{
		cout << "Tube " << tube << endl;
		TString tubeDirName = "Tube";
		tubeDirName.Append(tube);
		TDirectory* tDir = f->mkdir(tubeDirName);
		tDir->mkdir("RawData");
		tDir->mkdir("integrals");
		cout << "beginning event conversion." << endl;
		for(size_t data = 0; data < nEvents; data++)
		{
			tDir->cd("RawData");
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

			tDir->cd("integrals");
			int value = 0;
			TH1I* inthist = new TH1I(title, "FADC Data",800,0,3200);
			inthist->GetXaxis()->SetTitle("time [ns]");
			inthist->GetYaxis()->SetTitle("a.u");
			for(size_t i = 0; i < eventSize; i++)
			{
				infile.read((char*)&value,sizeof(Int_t));
//				cout << i << "\t" << value << endl;
				inthist->SetBinContent(i+1,value);
			}
			inthist->Write();
			delete inthist;
		}
	}
	cout << "Beginning drift time spectrum" << endl;
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
