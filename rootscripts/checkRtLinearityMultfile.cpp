#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include <fstream>
#include <iostream>
#include <vector>

Double_t findLinearFitExtrapolationValue(Int_t driftTime, Double_t par0,
		Double_t par1);
Int_t findDriftTimeOfLinearExtrapolation(Double_t extrapolatedRadius,
		Double_t par0, Double_t par1);

/**
 * Check the difference between a linear fit to drift time spectrum between 25 nanoseconds and the drift time corresponding
 * to a drift radius of 8mm.
 *
 * @author Stefan
 * @data November 25, 2016
 * @version 0.1
 */
void checkRtLinearityMultfile(TString list)
{
	vector<TH1D*> histograms;
	fstream infile(list);
	TString filename;

	while (infile >> filename)
	{
		TFile* file = new TFile(filename, "read");
		file->cd("rtRelation");

		TH1D* rtRel = (TH1D*) gDirectory->Get("rt-relation");
		cout << rtRel << endl;

		Int_t nBins = rtRel->GetNbinsX();

		Int_t end = 0;
		//find, at what time the drift radius of 8mm is reached, Fit up to this x position
		for (Int_t i = 0; i < nBins; i++)
		{
			if (rtRel->GetBinContent(i) >= 8)
			{
				end = i * 4;
				break;
			}
		}

		Int_t maxDriftTime = 0;

		for (Int_t i = 0; i < nBins; i++)
		{
			if (rtRel->GetBinContent(i) >= 19 - 19 * 0.001)
			{
				maxDriftTime = i * 4;
				cout << maxDriftTime << endl;
				break;
			}
		}

		TFitResultPtr res = rtRel->Fit("pol1", "S", "", 25, end);

		Double_t par0 = res->Parameter(0);
		Double_t par1 = res->Parameter(1);
		Double_t par0err = res->Error(0);
		Double_t par1err = res->Error(1);

		Double_t yBinning = 0.1; //mm
		Int_t nYBins = 19 / yBinning;

		Double_t yBinLowerEdge[nYBins];
		Double_t fitExtrapolationTime[nYBins];

		TH1D* differenceFromLinear = new TH1D("linearityCheck",
				"rt-Relation deviation from linearity", nYBins, 0, 19);
		differenceFromLinear->GetXaxis()->SetTitle("drift radius [mm]");
		differenceFromLinear->GetYaxis()->SetTitle(
				"deviation from linear fit [a.u]");

		for (Int_t i = 0; i < nYBins; i++)
		{
			yBinLowerEdge[i] = yBinning * i;
			fitExtrapolationTime[i] = findDriftTimeOfLinearExtrapolation(
					yBinLowerEdge[i], par0, par1);
			Double_t timediff = 0;
			Int_t measuredTime = 0;

			for (Int_t j = 0; j < nBins; j++)
			{
				if (rtRel->GetBinContent(j) >= yBinLowerEdge[i])
				{
					measuredTime = j * 4;
					break;
				}
			}
			timediff = (measuredTime - fitExtrapolationTime[i])
					/ fitExtrapolationTime[i];
			differenceFromLinear->Fill(yBinLowerEdge[i], timediff);
			histograms.push_back(differenceFromLinear);
		}
	}

	TCanvas* c1 = new TCanvas("bla", "bla1", 1920, 1080);
	for(TH1D* hist : histograms)
	{
		hist->Draw("HIST SAME");
	}
}

Double_t findLinearFitExtrapolationValue(Int_t driftTime, Double_t par0,
		Double_t par1)
{
	return par0 + par1 * driftTime;
}

Int_t findDriftTimeOfLinearExtrapolation(Double_t extrapolatedRadius,
		Double_t par0, Double_t par1)
{
	return (extrapolatedRadius - par0) / par1;
}
