#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "DataProcessor.h"
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
	DataProcessor* processor = new DataProcessor("./testdata/fadc_data.root");
	TH1D data = processor->getRawData();
	TCanvas* c1 = new TCanvas("c1","Windowtitle",800,600);
	c1->Divide(1,2);
	c1->cd(1);
	data.Draw();
	c1->cd(2);
	cout << "Beginning integration" << endl;
	TH1D* integral = processor->integrate(data);
	cout << "Done integrating" << endl;
	integral->Draw();
	app->Run();

	return 0;
}



