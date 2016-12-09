void SpectDifferences(TString file1, TString file2)
{
	TFile* f = new TFile(file1,"read");
	f->cd("dtSpect");

	TH1D* dtSpect1 = (TH1D*)gDirectory->Get("Drifttime spectrum");


	TFile* f2 = new TFile(file2,"read");
	f2->cd("dtSpect");
	TH1D* dtSpect2 = (TH1D*)gDirectory->Get("Drifttime spectrum");



//	dt1norm->Divide(dt2norm);
//	if(dt1norm->Divide(dt2norm))
//	{
		dtSpect->Draw("hist");
//	}
//	else
//	{
//		cout << "failed" << endl;
//	}

}
