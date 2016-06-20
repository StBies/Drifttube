#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "DataProcessor.h"
#include "Archive.h"
#include "TFile.h"
#include <vector>


using namespace std;

/**
 * Startup of the application is managed here
 * 
 * @brief main
 * 
 * @author Stefan
 * @date May 27, 2016
 * @version 0.1
 */
int main(int argc, char** argv)
{
	TApplication* app = new TApplication("main",&argc,argv);
	DataProcessor* processor = new DataProcessor("bla");
	cout << "DataProcessor built at " << processor << endl;
	Archive* archive = new Archive("./testdata/fadc_data.root");
	cout << "Archive built at " << archive << endl;
	vector<TH1*>* rawData = archive->getRawData();
	TH1D* data = (TH1D*)rawData->at(1);
	TCanvas* c1 = new TCanvas("c1","Windowtitle",800,600);
	c1->Divide(1,2);
	c1->cd(1);
	data->Draw("HIST");
	c1->cd(2);
	TH1D* integral = (TH1D*)processor->integrate(*data);
	integral->Draw("HIST");
	app->Run();

	return 0;
}
