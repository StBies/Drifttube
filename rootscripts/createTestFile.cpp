#include <cmath>

void createTestFile(TString filepath)
{
	TFile file(filepath,"RECREATE");
	TTree tree("Tfadc","title");

	double Voltage[800];
	int nch = 800;

	tree.Branch("nchannels",&nch,"nchannels/I");
	tree.Branch("Voltage",Voltage,"Voltage[nchannels]/D");

	for(int i = 0; i < nch; i++)
	{
		Voltage[i] = cos((double)i);
	}
	tree.Fill();

	tree.Write();
	file.Close();
}
