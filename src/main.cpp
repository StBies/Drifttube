#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "DataProcessor.h"
#include "TFile.h"


using namespace std;


/**
 * Startup of the application is managed here
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
	TH1I data = processor->getRawData();
	TCanvas* c1 = new TCanvas("c1","Windowtitle",800,600);
	data.Draw();
	app->Run();
	return 0;
}
