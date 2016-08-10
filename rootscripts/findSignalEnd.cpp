#include "TString.h"

void findSignalEnd(TString filename)
{
	//find filename of ingoing .root file and prepend the word converted_ for outfile
	TSubString st = filename(filename.Last('/') + 1, filename.Length());
	TSubString st2 = filename(0, filename.Last('/') + 1);
	TFile* file = new TFile(filename, "read");
	TTree* tree = (TTree*) file->Get("Tfadc");

	TString ofName = "converted_";
	ofName.Append(st);
	TString filename_out(st2);
	filename_out.Append(ofName);
	TFile* test = new TFile(filename_out, "recreate");
	TTree* params = new TTree("params", "title");

	//define branches and variables to store there
	int drifttime;
	int pos;
	int minimumpos;
	int minheight;
	double integralmin;
	int integralminpos;
	params->Branch("Drifttime", &drifttime, "drifttime/I");
	params->Branch("signalEnd", &pos, "signalEnd/I");
	params->Branch("minimum", &minimumpos, "minimumpos/I");
	params->Branch("minimumheight", &minheight, "minheight/I");
	params->Branch("integralminimumheight", &integralmin, "integralmin/D");

	const int nch = 800;
//	tree->SetBranchAddress("nchannels", &nch);
//	tree->GetEntry(0);

	//find offset
	double sum = 0;
	for (int i = 0; i < tree->GetEntries(); i++)
	{
		double voltage[nch];
		tree->SetBranchAddress("Voltage", &voltage);
		tree->GetEntry(i);

		sum += voltage[0];
	}
	sum /= tree->GetEntries();

	int nEntries = tree->GetEntries();
	int saturation = 0;
	for (int i = 0; i < nEntries; i++)
	{
		integralminpos = 0;
		minimumpos = 0;
		minheight = 0;
		drifttime = 0;

		double voltage[nch];
		tree->SetBranchAddress("Voltage", &voltage);
		tree->GetEntry(i);

		//find minimum
		double offset = sum;

		double integral = 0.0;
		integralmin = 0.0;
		for (int j = 0; j < nch; j++)
		{
			if (voltage[j] - offset < minheight)
			{
				minheight = voltage[j] - offset;
				minimumpos = j;
			}
			//find the minimum of the voltage's integral
			integral += (voltage[j] - offset);
			if (integral < integralmin)
			{
				integralmin = integral;
				integralminpos = j;
			}
		}

		//find drifttime
		for (int j = 0; j < nch - 1; j++)
		{
			double th = offset - 50;
			if (voltage[j] < th)
			{
				drifttime = j;
				break;
			}
		}

		//find, where threshold is last succeeded
		pos = 0;
//		double threshold = offset - 0.3 * abs(minheight);
		double threshold = offset - 100;
		for (int j = 0; j < nch - 1; j++)
		{
			if (voltage[j] < threshold && voltage[j + 1] >= threshold)
			{
				pos = j;
			}
		}
		//TODO overhaul
		pos = integralminpos;
		if (minheight < -2175 && minheight >= -2250)
		{
			cout << i << endl;
			saturation++;
		}
		params->Fill();

	}
	cout << "Percent of saturation events: " <<(double)saturation / (double)nEntries * 100 << endl;;

	test->cd();
	params->Write();
	test->Close();
}
