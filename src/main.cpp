#include <iostream>
#include "DataProcessor.h"
#include "Archive.h"
#include "omp.h"
#include <cmath>


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
	string infilename;
	char mode;
} ParsedArgs;

ParsedArgs parseCmdArgs(int argc, char** argv);

//TODO rework comment
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
//	DataProcessor processor;

	string filename = args.infilename;
	cout << "using file: " << filename << endl;

	double beginRuntime = omp_get_wtime();

	Archive archive(filename);
	string outFileName = archive.getDirname();
	outFileName.append("processed_");
	outFileName.append(archive.getFilename());
	vector<array<int,800>> integrals;
	for(size_t i = 0; i < archive.getTubes()[0]->getDataSet().getSize(); i++)
	{
		try
		{
			integrals.push_back(DataProcessor::integrate(archive.getTubes()[0]->getDataSet()[i]));
		}
		catch(Exception& e)
		{
			continue;
		}
	}

	double endRuntime = omp_get_wtime();
	archive.writeToFile(outFileName);
//
//	DataSet* dataSet = archive->getRawData();
//	DataSet* integralSet = processor.integrateAll(dataSet);
//	archive->setProcessedData(integralSet);
//	TH1D* spect = processor.calculateDriftTimeSpectrum(dataSet);
//
//
//	TH1D* rt = processor.calculateRtRelation(*spect);
//	TH1D* derivSpec = processor.derivate(spect);
//
//	archive->setDifttimeSpect(spect);
//	archive->setRtRelation(rt);
//	archive->setDiffDrifttimeSpect(derivSpec);
//	int nAfterPulses = processor.countAfterpulses(*dataSet,*rt);
//	int nEvents = spect->GetEntries() - spect->GetBinContent(0);
//
//	cout <<"# afterpulses: " << nAfterPulses
//			<< " probability: " << (double)nAfterPulses/nEvents
//			<< " +- " << sqrt(nAfterPulses/pow(nEvents,2) + pow(nAfterPulses * sqrt(nEvents),2)/pow(nEvents,4))
//			<< endl;


	cout << "Computation without saving took " << endRuntime - beginRuntime << " seconds" << endl;
//	processor.writeResults(*dataSet,*integralSet,archive->getFilename(),archive->getDirname());

//	delete archive;

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
				string file(argv[i]);
				int equalSignPos = file.find_first_of('=') + 1;
				string filename = file.substr(equalSignPos,file.length());
				result.infilename = filename;
			}
		}
	}
	return result;
}
