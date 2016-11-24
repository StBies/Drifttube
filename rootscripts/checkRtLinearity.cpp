#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"

void checkRtLinearity(TString filename)
{
	TFile* file = new TFile(filename,"read");
	file->cd("rtRelation");

	gDirectory->ls("-d");

	TH1D* rtRel = (TH1D*)gDirectory->Get("rt-relation");
	cout << rtRel << endl;

	Int_t nBins = rtRel->GetNbinsX();

	for(Int_t i = 0; i < nBins, i++)
	{
		if(rtRel->GetBinContent(i));
	}


	rtRel->Fit("pol1");

	TCanvas* c1 = new TCanvas("bla","bla1",1920,1080);
	rtRel->Draw();
}
