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

	TH1D* dtSpect = (TH1D*)gDirectory->Get("Drifttime spectrum");
	Int_t firstZeroBin = 0;

	//find first bin with zero entries
	for(Int_t i = 5; i <= dtSpect->GetNbinsX(); i++)
	{
		if(dtSpect->GetBinContent(i) == 0)
		{
			firstZeroBin = i;
			break;
		}
	}
	cout << "first zero bin: " << firstZeroBin << endl;

	TFitResultPtr fit = dtSpect->Fit("pol1", "S", "", (firstZeroBin*4)*0.8, (firstZeroBin*4)*1.20);

	Double_t par0 = fit->Parameter(0);
	Double_t par1 = fit->Parameter(1);
	Double_t par0err = fit->Error(0);
	Double_t par1err = fit->Error(1);

	Double_t tmax = abs(par0/par1);
	Double_t error = sqrt(pow(par0err,2)/pow(par1,2)+pow(par0,2)*pow(par1err,2)/pow(par1,4));

	cout << "tmax = " << tmax << " +- " << error << endl;
	//TODO complete
}
