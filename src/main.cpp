#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "DataProcessor.h"
#include "Archive.h"
#include "TFile.h"


using namespace std;

/**
 * A struct containing information given as commandline parameters on program start.
 * Contains information about the datafile that is to be used as well as the mode
 * of operation in that the program is to be executed.
 *
 * @brief Parsed command line parameters
 *
 * @author Stefan
 * @date July 18, 2016
 * @version 0.2
 */
typedef struct
{
	TString infilename;
	char mode;
} ParsedArgs;

ParsedArgs* parseCmdArgs(int argc, char** argv);

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
	ParsedArgs* args = parseCmdArgs(argc,argv);
	TH1::AddDirectory(kFALSE);
	TApplication* app = new TApplication("main",&argc,argv);
	DataProcessor* processor = new DataProcessor();

	TString filename = args->infilename;
	cout << "using file: " << filename << endl;;

	Archive* archive = new Archive(filename);

	DataSet* dataSet = archive->getRawData();
	DataSet* integralSet = processor->integrateAll(dataSet);
	archive->setProcessedData(integralSet);
	TH1D* spect = processor->calculateDriftTimeSpectrum(dataSet);

	cout << "Data size is " << dataSet->getSize() << endl;
	cout << "Integral size is " << integralSet->getSize() << endl;

	TH1D* data;
	try
	{
		data = dataSet->getEvent(1);
	}
	catch(Exception& e)
	{
		cerr << e.error() << endl;
	}

	TH1D* integral = integralSet->getEvent(1);
	TH1D* rt = processor->integrate(spect);

	//TODO to be seperated into class DrawingTool
//	TCanvas* c1 = new TCanvas("c1","Windowtitle",800,600);
//	c1->Divide(1,2);
//	c1->cd(1);
//	spect->Draw("HIST");
//	c1->cd(2);
//	rt->Draw("HIST");
	delete archive;
	app->Run();


	return 0;
}

//TODO to be implemented
ParsedArgs* parseCmdArgs(int argc, char** argv)
{
	ParsedArgs* result = new ParsedArgs;
	if(argc > 1)
	{
		for(int i = 0; i < argc; i++)
		{
			cout << "Arg " << i << " = " << argv[i] << endl;
			if(argv[i][0] == 'i' && argv[i][1] == 'f' && argv[i][2] == '=')
			{
				TString file(argv[i]);
				int equalSignPos = file.First('=') + 1;
				TSubString filename = file(equalSignPos,file.Length());
				result->infilename = *new TString(filename);
			}
			cout << "var: infilename = " << result->infilename << endl;
		}
	}
	return result;
}
