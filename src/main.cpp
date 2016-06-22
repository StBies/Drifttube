#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "DataProcessor.h"
#include "Archive.h"
#include "TFile.h"


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
	Archive* archive = new Archive("./testdata/fadc_data.root");
	cout << "Archive built at " << archive << endl;
	//TODO Histograms are not present anymore after writing it to the file
	//must find way to write copies to the file instead of original data
//	archive->writeToFile("./testdata/converted.root");
	TH1D* data = (TH1D*)archive->getEvent(1);
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
