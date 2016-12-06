#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
#include <iostream>

/*
 * Find the maximum drift time by fitting a linear function to the end of a drift time spectrum
 *
 * @author Stefan Bieschke
 * @version 0.1
 * @data Dec. 6, 2016
 *
 * @param filename filename, containing the processed data from the compiled program.
 */
void fitDtEnd(TString filename)
{
	TFile* f = new TFile(filename,"read");
	f->cd("dtSpect");

	TH1D* dtSpect = (TH1D*)gDirectory->Get("Drift time spectrum");
	Int_t firstZeroBin = 0;

	//find first bin with zero entries
	for(Int_t i = 1; i <= dtSpect->GetNbinsX(); i++)
	{
		if(dtSpect->GetBinContent(i) == 0)
		{
			firstZeroBin = i;
			break;
		}
	}
	//TODO complete
}
