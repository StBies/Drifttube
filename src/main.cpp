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
	TH1::AddDirectory(kFALSE);
	TApplication* app = new TApplication("main",&argc,argv);
	DataProcessor* processor = new DataProcessor();
	Archive* archive = new Archive("./testdata/fadc_data.root");

//	//TODO work on real data
//	TH1D* data = archive->createTestHist();
//	TH1D* integral = processor->integrate(data);

	DataSet* dataSet = archive->getRawData();
	DataSet* integralSet = processor->integrateAll(dataSet);


	cout << "Data size is " << dataSet->getSize() << endl;
	cout << "Integral size is " << integralSet->getSize() << endl;

	TH1D* data;
	try
	{
		data = dataSet->getEvent(1);
	}
	catch(EventSizeException& e)
	{
		cerr << e.error() << endl;
	}

	TH1D* integral = integralSet->getEvent(1);

	//TODO to be seperated into class DrawingTool
	TCanvas* c1 = new TCanvas("c1","Windowtitle",800,600);
	c1->Divide(1,2);
	c1->cd(1);
	data->Draw("HIST");
	c1->cd(2);
	integral->Draw("HIST");
	app->Run();

	return 0;
}
