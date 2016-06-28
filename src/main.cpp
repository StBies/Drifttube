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
	DataProcessor* processor = new DataProcessor();
	Archive* archive = new Archive("./testdata/fadc_data.root");
	cout << "Archive built at " << archive << endl;
	TH1D** rawData = archive->getRawData();
	TH1D** processedData = archive->getProcessedData();
	processor->integrateAll(rawData,processedData,archive->getSize());
	cout << rawData[5] << endl;
	TH1D* data = archive->getEvent(5);
	TH1D* integral = processedData[5];
	TCanvas* c1 = new TCanvas("c1","Windowtitle",800,600);
	c1->Divide(1,2);
	c1->cd(1);
	data->DrawCopy("HIST");
	c1->cd(2);
	integral->DrawCopy("HIST");
//	delete processor;
//	delete archive;
	app->Run();

	return 0;
}
