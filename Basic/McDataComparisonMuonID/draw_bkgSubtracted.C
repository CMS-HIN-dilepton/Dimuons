#include "KYOcommonOpt.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <string>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
#include <cmath>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TRandom3.h"
#include "TClonesArray.h"
#include <TAxis.h>
#include <TLorentzRotation.h>
#include <TCut.h>

#include <TStyle.h>
#include <TPaveStats.h>

double calculInteg( TH1 *a=0, double minX=-999.21231, double maxX=-999.21231)
{
  float fac=0;
  int lowBin=1;
  int highBin=a->GetNbinsX();
  if ( minX != -999.21231)
    lowBin = a->FindBin(minX+0.000001);
  if ( maxX != -999.21231)
    highBin=a->FindBin(maxX-0.0000001);

  return fac =  a->Integral( lowBin, highBin);
}

int draw_bkgSubtracted(char *mcType = "ptJpsi69")
{
	gROOT->Macro("./JpsiStyle.C");
	
	// read-in root file
	TFile *fIn = new TFile(Form("%s/muIDvariables_isScaled0.root",mcType));
	
	// # of event range
	double nSigCand = -999.;
	double nBkgSide = -999.; // in 2.6-2.9 && 2.9-3.5 GeV
	TH1D* hSig_M = (TH1D*)fIn->Get("hSig_M");
	TH1D* hBkg_M = (TH1D*)fIn->Get("hBkg_M");
	nSigCand = hSig_M->GetEntries();
	nBkgSide = hBkg_M->GetEntries();
/*
	if (strcmp (mcType,"ptJpsi36")==0) {
		nSigCand = 3951.;
		nBkgSide = 4044.;
	}
	else if (strcmp (mcType,"ptJpsi69")==0) {
		nSigCand = 2366.;
		nBkgSide = 1511.;
	}
*/
	const double nBkg = nBkgSide*(0.4/0.5); // in 2.9-3.3
	const double nBkgToSig = nBkg/nSigCand;
	cout << "nSigCand = " << nSigCand << endl;
	cout << "nBkgSide = " << nBkgSide << endl;
	cout << "nBkg = " << nBkg << endl;
	cout << "nBkgToSig = " << nBkgToSig << endl;
	
	//// define 1D hist
	const int nbin[] = {45, 2, 20, 6, 90, 88};
	const double minbin[] = {2.6, 0, 0, 0, -4.5, -22.};
	const double maxbin[] = {3.5, 2, 20, 6, 4.5, 22.};
	//// for dashedLine
	double xmin[] = {0., 1., 6., 1., -0.3, -20.};
	double xmax[] = {531., 531., 531., 531., 0.3, 20.};
	double ymin[] = {0., 0., 0., 0., 0.00001, 0.00001};
	double ymax[] = {0.09, 1.1, 0.25, 0.8, 1, 1};
	TString histName[] = {"M", "isGoodMuon", "nTrkWMea", "nPixWMea", "dxy", "dz"};
	TString histTitle[] = {"mass (GeV/c^{2})", "good muon flag", "# of trk layers w/ measurement", "# of pixel layers w/ measurement", "dxy (cm)", "dz (cm)"};
	const int nHist = sizeof(nbin)/sizeof(int);
	cout << "nHist = "<<nHist<<endl;

	TH1D *hSig[nHist];
	TH1D *hSigSub[nHist];
	TH1D *hBkg[nHist];
	TH1D *hPRMC[nHist];

	TCanvas* c1[nHist];
	TLegend *legUR = new TLegend(0.58,0.75,0.88,0.90,NULL,"brNDC");
	SetLegendStyle(legUR);
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextFont(42);
	latex->SetTextSize(0.04);

	for (int i=1; i<nHist; i++) {
		hSig[i] = (TH1D*)fIn->Get(Form("hSig_%s",histName[i].Data()));
		hBkg[i] = (TH1D*)fIn->Get(Form("hBkg_%s",histName[i].Data()));
		hPRMC[i] = (TH1D*)fIn->Get(Form("hPRMC_%s",histName[i].Data()));
		std::cout << i <<"th hist name = " << histName[i].Data() << std::endl;
		std::cout << histName[i].Data() <<" : Sig integral = " << hSig[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : Bkg integral = " << hBkg[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : PRMC integral = " << hPRMC[i]->Integral() << std::endl;
		
		////// background subtraction
		/// subtracted signal hist.	
		hSigSub[i] = (TH1D*)hSig[i]->Clone(Form("hSigSub_%s",histName[i].Data()));
		// first normalized by their integral
		hSig[i]->Scale(1/hSig[i]->Integral());
		hSigSub[i]->Scale(1/hSigSub[i]->Integral());
		hBkg[i]->Scale(1/hBkg[i]->Integral());
		// Bkg normalized to nBkgToSig and subtract from the signal
		hBkg[i] ->Scale(nBkgToSig);
		hSigSub[i]->Add(hBkg[i],-1);
		//normalize them again by integral
		hSig[i]->Scale(1/hSig[i]->Integral());
		hSigSub[i]->Scale(1/hSigSub[i]->Integral());
		hBkg[i]->Scale(1/hBkg[i]->Integral());
		hPRMC[i]->Scale(1/hPRMC[i]->Integral());
		
		c1[i] = new TCanvas(Form("c1_%s",histName[i].Data()),"",600,600);
		c1[i]->cd();
		if (i==4 || i==5 ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(hSigSub[i],6,0);
		SetHistStyle(hPRMC[i],4,0);
		hPRMC[i]->SetFillColor(kGreen+9);
		hPRMC[i]->SetFillStyle(3003);
		hPRMC[i]->SetAxisRange(ymin[i],ymax[i],"Y");
		hPRMC[i]->Draw("hist");
		hSigSub[i]->Draw("pe same");
		if (i==5){
			legUR->AddEntry(hSigSub[i],"RD : (Bkg sub)","lp");
			legUR->AddEntry(hPRMC[i],"MC","l");
			legUR->Draw();
		}
		//// line for actual cut regions
		dashedLine(xmin[i],ymin[i],xmin[i],ymax[i]);
		dashedLine(xmax[i],ymin[i],xmax[i],ymax[i]);
		//// check integral over cut regions
		double nsig=-999, nmc=-999, nratio=-999;
		nsig = calculInteg(hSigSub[i],xmin[i],xmax[i]);
		nmc = calculInteg(hPRMC[i],xmin[i],xmax[i]);
		nratio = nsig/nmc;
		cout << "nsig = " << nsig << ", nmc = " << nmc << ", nratio = " << nratio << endl;
		if (i!=0) {
			latex->DrawLatex(0.16,0.80,Form("#int_{cut}Data/#int_{cut}MC = %.2f",nratio));		
			latex->DrawLatex(0.16,0.70,Form("( data = %.2f",nsig));		
			latex->DrawLatex(0.16,0.65,Form("  MC = %.2f  )",nmc));		
		}
		c1[i]->SaveAs(Form("%s/variables_%s_bkgSubtracted.pdf",mcType,histName[i].Data()));
	}
	
	return 0;	

}


