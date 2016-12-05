#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include <fstream>
#include <iostream>

Double_t findLinearFitExtrapolationValue(Int_t driftTime, Double_t par0,
		Double_t par1);
Double_t findDriftTimeOfLinearExtrapolation(Double_t extrapolatedRadius,
		Double_t par0, Double_t par1);

/**
 * Check the difference between a linear fit to drift time spectrum between 25 nanoseconds and the drift time corresponding
 * to a drift radius of 8mm.
 *
 * @author Stefan
 * @data November 25, 2016
 * @version 0.1
 */
void checkRtLinearityChi2(TString filename)
{
		TFile* file = new TFile(filename, "read");
		file->cd("rtRelation");

		TH1D* rtRel = (TH1D*) gDirectory->Get("rt-relation");

		Int_t nBins = rtRel->GetNbinsX();
		Int_t maxDriftTime = 0;
		Double_t rTube = 18.15;

		//find the drifttime, correlated to a drift radius of 99.9% of the tubes radius
		for (Int_t i = 0; i < nBins; i++)
		{
			if (rtRel->GetBinContent(i) >= rTube - rTube * 0.0005)
			{
				maxDriftTime = i * 4;
				cout << maxDriftTime << endl;
				break;
			}
		}

		Double_t par0 =0;
		Double_t par1 =rTube / maxDriftTime;

		Double_t chi2 = 0;

		Double_t yBinning = 0.1; //mm
		Int_t nYBins = (Int_t)(rTube / yBinning);
		cout << "nYBins: " << nYBins << endl;

		Double_t yBinLowerEdge;
		Double_t fitExtrapolationTime;

		TH1D* differenceFromLinear = new TH1D("linearityCheck",
				"rt-Relation deviation from linearity", nYBins, 0, 19);
		differenceFromLinear->GetXaxis()->SetTitle("drift radius [mm]");
		differenceFromLinear->GetYaxis()->SetTitle("deviation from linear fit [a.u]");

		for (Int_t i = 0; i < nYBins; i++)
		{
			yBinLowerEdge = yBinning * (i+1);
			fitExtrapolationTime = findDriftTimeOfLinearExtrapolation(yBinLowerEdge, par0, par1);

			Double_t timediff = 0;
			Double_t measuredTime = 0;

			for (Int_t j = 0; j < nBins; j++)
			{
				if (rtRel->GetBinContent(j) >= yBinLowerEdge)
				{
					measuredTime = j * 4;
					break;
				}
			}
			timediff = (measuredTime - fitExtrapolationTime);
			chi2 += pow(timediff,2)/fitExtrapolationTime;
			differenceFromLinear->Fill(yBinLowerEdge, chi2);
		}
		cout << "chi2 / ndf = " << chi2/nYBins << endl;

//	TCanvas* c1 = new TCanvas("bla", "bla1", 1920, 1080);
//	differenceFromLinear->Draw("HIST");
}

Double_t findLinearFitExtrapolationValue(Int_t driftTime, Double_t par0,
		Double_t par1)
{
	return par0 + par1 * driftTime;
}

Double_t findDriftTimeOfLinearExtrapolation(Double_t extrapolatedRadius,
		Double_t par0, Double_t par1)
{
	return (extrapolatedRadius - par0) / par1;
}
