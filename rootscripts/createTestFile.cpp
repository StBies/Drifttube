#include <cmath>

void createTestFile(TString filepath)
{
	TFile file(filepath,"RECREATE");
	TTree tree("Tfadc","title");

	double Voltage[800];
	int nch = 800;
	double pi = 3.14159265358979323846;

	tree.Branch("nchannels",&nch,"nchannels/I");
	tree.Branch("Voltage",Voltage,"Voltage[nchannels]/D");

	for(int i = 0; i < nch; i++)
	{
		Voltage[i] = 2200 + 4096/12.0 * cos((double)(2 * pi / 800.0 * i));
	}
	tree.Fill();

	tree.Write();
	//file.Close();
}
