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

ParsedArgs parseCmdArgs(int argc, char** argv);

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
	ParsedArgs args = parseCmdArgs(argc,argv);
	TH1::AddDirectory(kFALSE);
//	TApplication* app = new TApplication("main",&argc,argv);
	DataProcessor processor;

	TString filename = args.infilename;
	cout << "using file: " << filename << endl;;

	Archive* archive = new Archive(filename);

	DataSet* dataSet = archive->getRawData();
	DataSet* integralSet = processor.integrateAll(dataSet);
	processor.writeResults(*dataSet,*integralSet);
	archive->setProcessedData(integralSet);
	TH1D* spect = processor.calculateDriftTimeSpectrum(dataSet);

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
	TH1D* rt = processor.integrate(spect);

	archive->setDifttimeSpect(spect);
	archive->setRtRelation(rt);

	delete archive;

	return 0;
}

//TODO to be implemented
ParsedArgs parseCmdArgs(int argc, char** argv)
{
	ParsedArgs result;
	if(argc > 1)
	{
		for(int i = 0; i < argc; i++)
		{
			if(argv[i][0] == 'i' && argv[i][1] == 'f' && argv[i][2] == '=')
			{
				TString file(argv[i]);
				int equalSignPos = file.First('=') + 1;
				TSubString filename = file(equalSignPos,file.Length());
				result.infilename = TString(filename);
			}
		}
	}
	return result;
}
