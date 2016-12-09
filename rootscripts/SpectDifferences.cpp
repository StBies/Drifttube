void normalize(TH1D* hist);

/**
 * Check the differences between two drift time spectra by dividing the normalized spectra
 *
 * @author Stefan Bieschke
 * @date Dec. 09, 2016
 * @version 0.1
 *
 * @param file1 first processed_ file containing one dataset
 * @param file2 second processed_ file containing the other dataset
 */
void SpectDifferences(TString file1, TString file2)
{
	TFile* f = new TFile(file1,"read");
	f->cd("dtSpect");
	TH1D* dtSpect1 = (TH1D*)gDirectory->Get("Drifttime spectrum");

	TFile* f2 = new TFile(file2,"read");
	f2->cd("dtSpect");
	TH1D* dtSpect2 = (TH1D*)gDirectory->Get("Drifttime spectrum");

	normalize(dtSpect1);
	normalize(dtSpect2);

	dtSpect1->Divide(dtSpect2);
//	dtSpect1->Add(dtSpect2,-1);
	dtSpect1->Draw("hist");
}

void normalize(TH1D* hist)
{
	hist->Scale(1/(hist->Integral()));
}
