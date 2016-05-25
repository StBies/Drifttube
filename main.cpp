#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"


using namespace std;

int main(int argc, char** argv)
{
	TApplication* app = new TApplication("main",&argc,argv);
	TCanvas *c1 = new TCanvas("Name", "Titel", 800,600);
	TH1D *hist = new TH1D("test","test1",100,0,1);
	for(double d = 0.0; d <= 1; d += 0.1)
	{
		hist->Fill(d);
	}
	hist->Draw();

	app->Run();
	return 0;
}
