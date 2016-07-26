#include "TString.h"

void findSignalEnd(TString filename)
{
	TSubString st = filename(filename.Last('/')+1,filename.Length());
	TSubString st2 = filename(0,filename.Last('/')+1);
	TFile* file = new TFile(filename, "read");
	TTree* tree = (TTree*) file->Get("Tfadc");

	TString ofName = "converted_";
	ofName.Append(st);
	TString filename_out(st2);
	filename_out.Append(ofName);
	TFile* test = new TFile(filename_out, "recreate");
	TTree* params = new TTree("params", "title");

	int drifttime;
	int pos;
	params->Branch("Drifttime", &drifttime, "drifttime/I");
	params->Branch("relPeakhight", &pos, "relPeakhight/I");

	const int nch;
	tree->SetBranchAddress("nchannels", &nch);
	tree->GetEntry(0);

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
	cout << sum << endl;

	std::vector<int> minPos;
	std::vector<int> drifttimes;

	int nEntries = tree->GetEntries();
	for (int i = 0; i < nEntries; i++)
	{
		drifttime = 0;
//		std::cout << "Event nr: " << i << endl;
		double voltage[nch];
		tree->SetBranchAddress("Voltage", &voltage);
		tree->GetEntry(i);

		//find minimum
		double offset = sum;
		double min = offset;
		for (int j = 0; j < nch; j++)
		{
			if (voltage[j] < min)
			{
				min = voltage[j];
			}
		}

		//find drifttime
		for (int j = 0; j < nch - 1; j++)
		{
			double th = offset - 50;
			if (voltage[j] < th)
			{
				drifttime = j;
				cout << j << endl;
				break;
			}
		}

		//find, where threshold is last succeeded
			pos = 0;
			double threshold = offset - 0.3 * (offset - min);

			for (int j = 0; j < nch - 1; j++)
			{
				if (voltage[j] < threshold && voltage[j + 1] >= threshold)
				{
					pos = j;
				}
			}
			minPos.push_back(pos);
			params->Fill();
	}

	test->cd();
	params->Write();
	test->Close();
//	TCanvas* c1 = new TCanvas("c1", "Windowtitle", 800, 600);
//	c1->cd();
//	TH1D* hist = new TH1D("name", "title", nch, 0, nch - 1);
//	for (int i = 0; i < minPos.size(); i++)
//	{
//		hist->Fill(minPos[i]);
//	}
//	hist->Draw("HIST");
}
