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

int rootAna_muIDvariables(char* mcType = "ptJpsi69", bool isScaled = true, bool isSmall =false){

	using namespace std;
	//// # of event range ( for isSmall)
	const int nEntry = 10000;
	gROOT->Macro("./JpsiStyle.C");

	//// read-in root file
	TFile *fData = new TFile("OniaTree_262548_263035_noCUT.root");
	TFile *fPRMC = new TFile(Form("OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_%s_noCut.root",mcType));
	TTree *treeData = (TTree*)fData->Get("myTree");
	TTree *treePRMC = (TTree*)fPRMC->Get("myTree");

	cout << "mcType : " << mcType <<endl;
	double rapmin = -2.4;
	double rapmax = 2.4;
	double ptmin;
	double ptmax;
	if (strcmp (mcType,"ptJpsi36")==0) {ptmin = 3.; ptmax = 6.;}
	else if (strcmp (mcType,"ptJpsi69")==0) {ptmin = 6.; ptmax = 9;}
	
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

	TH1D *hData[nHist];
	TH1D *hSig[nHist];
	TH1D *hBkg[nHist];
	TH1D *hPRMC[nHist];

	//// 1) general cut definitions
	//TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )"; //L1DoubleMu0
	TCut recoCut = "Reco_QQ_size>=1 && Reco_QQ_sign==0";	
	TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
	TCut rapRange = Form("Reco_QQ_4mom.Rapidity() > %f && Reco_QQ_4mom.Rapidity() < %f",rapmin,rapmax);
	TCut ptRange = Form("Reco_QQ_4mom.Pt() > %f && Reco_QQ_4mom.Pt() < %f",ptmin,ptmax);
	cout << "rapRange = " << rapRange << endl;
	cout << "ptRange = " << ptRange << endl;

	TCut totalCut = recoCut && rapRange && ptRange;
	TCut totalCutMC = recoCut && rapRange && ptRange;

	//// 2) muID cut definition
	TCut c_mupl_isGoodMuon = "Reco_QQ_mupl_isGoodMuon==1";
	TCut c_mupl_nTrkWMea = "Reco_QQ_mupl_nTrkWMea > 5";
	TCut c_mupl_nPixWMea = "Reco_QQ_mupl_nPixWMea > 0";
	TCut c_mupl_dxy = "TMath::Abs(Reco_QQ_mupl_dxy) < 0.3";
	TCut c_mupl_dz = "TMath::Abs(Reco_QQ_mupl_dz) < 20.";
	TCut c_mupl_tot[nHist];
	c_mupl_tot[0] = c_mupl_isGoodMuon && c_mupl_nTrkWMea && c_mupl_nPixWMea && c_mupl_dxy && c_mupl_dz; 
	c_mupl_tot[1] = c_mupl_nTrkWMea && c_mupl_nPixWMea && c_mupl_dxy && c_mupl_dz; 
	c_mupl_tot[2] = c_mupl_isGoodMuon && c_mupl_nPixWMea && c_mupl_dxy && c_mupl_dz; 
	c_mupl_tot[3] = c_mupl_isGoodMuon && c_mupl_nTrkWMea && c_mupl_dxy && c_mupl_dz; 
	c_mupl_tot[4] = c_mupl_isGoodMuon && c_mupl_nTrkWMea && c_mupl_nPixWMea && c_mupl_dz; 
	c_mupl_tot[5] = c_mupl_isGoodMuon && c_mupl_nTrkWMea && c_mupl_nPixWMea && c_mupl_dxy; 
	TCut c_mumi_isGoodMuon = "Reco_QQ_mumi_isGoodMuon==1";
	TCut c_mumi_nTrkWMea = "Reco_QQ_mumi_nTrkWMea > 5";
	TCut c_mumi_nPixWMea = "Reco_QQ_mumi_nPixWMea > 0";
	TCut c_mumi_dxy = "TMath::Abs(Reco_QQ_mumi_dxy) < 0.3";
	TCut c_mumi_dz = "TMath::Abs(Reco_QQ_mumi_dz) < 20.";
	TCut c_mumi_tot[nHist];
	c_mumi_tot[0] = c_mumi_isGoodMuon && c_mumi_nTrkWMea && c_mumi_nPixWMea && c_mumi_dxy && c_mumi_dz; 
	c_mumi_tot[1] = c_mumi_nTrkWMea && c_mumi_nPixWMea && c_mumi_dxy && c_mumi_dz; 
	c_mumi_tot[2] = c_mumi_isGoodMuon && c_mumi_nPixWMea && c_mumi_dxy && c_mumi_dz; 
	c_mumi_tot[3] = c_mumi_isGoodMuon && c_mumi_nTrkWMea && c_mumi_dxy && c_mumi_dz; 
	c_mumi_tot[4] = c_mumi_isGoodMuon && c_mumi_nTrkWMea && c_mumi_nPixWMea && c_mumi_dz; 
	c_mumi_tot[5] = c_mumi_isGoodMuon && c_mumi_nTrkWMea && c_mumi_nPixWMea && c_mumi_dxy; 

	/////////////////////////////////////////////////////////////////////////
	TCanvas* c1[nHist];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	SetLegendStyle(legUR);
	legUR->SetTextSize(0.031);
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextFont(42);
	latex->SetTextSize(0.04);

	for (int i=0; i<nHist; i++) {
		hData[i] = new TH1D(Form("hData_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hSig[i] = new TH1D(Form("hSig_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hBkg[i] = new TH1D(Form("hBkg_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hPRMC[i] = new TH1D(Form("hPRMC_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hData[i]->Sumw2();
		hSig[i]->Sumw2();
		hBkg[i]->Sumw2();
		hPRMC[i]->Sumw2();
		std::cout << i <<"th hist name = " << histName[i].Data() << std::endl;
		if (i==0) { // mass :: 2.6 - 3.5
			//apply all the mu ID cuts
			if (isSmall) {
				treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hData[i]->GetName()),totalCut && massWide && c_mupl_tot[i],"",nEntry);
				treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hSig[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"",nEntry);
				treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hBkg[i]->GetName()),totalCut && massWide && (!massNarrow) && c_mupl_tot[i],"",nEntry);
				treePRMC->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hPRMC[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"",nEntry);
			}
			else {
				treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hData[i]->GetName()),totalCut && massWide && c_mupl_tot[i],"");
				treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hSig[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"");
				treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hBkg[i]->GetName()),totalCut && massWide && (!massNarrow) && c_mupl_tot[i],"");
				treePRMC->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hPRMC[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"");
			}
		}		
		else { //muID variables
			if (isSmall) {
				treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hData[i]->GetName()),totalCut && massWide && c_mupl_tot[i],"",nEntry);
				treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hSig[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"",nEntry);
				treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hBkg[i]->GetName()),totalCut && massWide && (!massNarrow) && c_mupl_tot[i],"",nEntry);
				treePRMC->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hPRMC[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"",nEntry);
				treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hData[i]->GetName()),totalCut && massWide && c_mumi_tot[i],"",nEntry);
				treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hSig[i]->GetName()),totalCut && massNarrow && c_mumi_tot[i],"",nEntry);
				treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hBkg[i]->GetName()),totalCut && massWide && (!massNarrow) && c_mumi_tot[i],"",nEntry);
				treePRMC->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hPRMC[i]->GetName()),totalCut && massNarrow && c_mumi_tot[i],"",nEntry);
			}
			else {
				treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hData[i]->GetName()),totalCut && massWide && c_mupl_tot[i],"");
				treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hSig[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"");
				treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hBkg[i]->GetName()),totalCut && massWide && (!massNarrow) && c_mupl_tot[i],"");
				treePRMC->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[i].Data(),hPRMC[i]->GetName()),totalCut && massNarrow && c_mupl_tot[i],"");
				treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hData[i]->GetName()),totalCut && massWide && c_mumi_tot[i],"");
				treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hSig[i]->GetName()),totalCut && massNarrow && c_mumi_tot[i],"");
				treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hBkg[i]->GetName()),totalCut && massWide && (!massNarrow) && c_mumi_tot[i],"");
				treePRMC->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[i].Data(),hPRMC[i]->GetName()),totalCut && massNarrow && c_mumi_tot[i],"");
			}
		}		
		//std::cout << histName[i].Data() <<" : Data integral = " << hData[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : Sig integral = " << hSig[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : Bkg integral = " << hBkg[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : PRMC integral = " << hPRMC[i]->Integral() << std::endl;
		//// scaled
		if (isScaled){
			hData[i]->Scale(1/hData[i]->Integral());
			hSig[i]->Scale(1/hSig[i]->Integral());
			hBkg[i]->Scale(1/hBkg[i]->Integral());
			hPRMC[i]->Scale(1/hPRMC[i]->Integral());
		}
	}

	//////////////////////////////////////////////////////////////////////
	//// Draw	 histograms
	for (int i=1; i<nHist; i++) { // except mass
		c1[i] = new TCanvas(Form("c1_%s",histName[i].Data()),"",600,600);
		c1[i]->cd();
		if (i==4 || i==5 ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(hData[i],0,0);
		SetHistStyle(hSig[i],1,0);
		SetHistStyle(hBkg[i],2,10);
		SetHistStyle(hPRMC[i],4,0);
		hPRMC[i]->SetFillColor(kGreen+9);
		hPRMC[i]->SetFillStyle(3003);
		if (isScaled) hPRMC[i]->SetAxisRange(ymin[i],ymax[i],"Y");
		hPRMC[i]->Draw("hist");
		hSig[i]->Draw("pe same");
		hBkg[i]->Draw("pe same");
		if (i==5){
			legUR->AddEntry(hSig[i],"RD : mass window","lp");
			legUR->AddEntry(hBkg[i],"RD :sideband","lp");
			legUR->AddEntry(hPRMC[i],"MC : mass window","l");
			legUR->Draw();
		}
		//// line for actual cut regions
		dashedLine(xmin[i],ymin[i],xmin[i],ymax[i]);
		dashedLine(xmax[i],ymin[i],xmax[i],ymax[i]);
		//// check integral over cut regions
		double nsig=-999, nmc=-999, nratio=-999;
		nsig = calculInteg(hSig[i],xmin[i],xmax[i]);
		nmc = calculInteg(hPRMC[i],xmin[i],xmax[i]);
		nratio = nsig/nmc;
		cout << "nsig = " << nsig << ", nmc = " << nmc << ", nratio = " << nratio << endl;
		if (i!=0) {
			latex->DrawLatex(0.16,0.80,Form("#int_{cut}Data/#int_{cut}MC = %.2f",nratio));		
			latex->DrawLatex(0.16,0.70,Form("( data = %.2f",nsig));		
			latex->DrawLatex(0.16,0.65,Form("  MC = %.2f  )",nmc));		
		}
		c1[i]->SaveAs(Form("%s/variables_%s_isScaled%d.pdf",mcType,histName[i].Data(),(int)isScaled));
	}

	//if (isSmall) return 0;
	//Save as a root file
	TFile* outFile = new TFile(Form("%s/muIDvariables_isScaled%d.root",mcType,(int)isScaled),"RECREATE");
	outFile->cd();
	for (int i=0; i<nHist; i++) {
		hData[i]->Write();
		hSig[i]->Write();
		hBkg[i]->Write();
		hPRMC[i]->Write();
	}	
	outFile->Close();

	return 0;	

}

/*
void scaleInt( TH1 *a=0, double normFac=1., double minX=-999.21231, double maxX=-999.21231)
{
  float fac=0;
  int lowBin=1;
  int highBin=a->GetNbinsX();
  if ( minX != -999.21231)
    lowBin = a->FindBin(minX);
  if ( maxX != -999.21231)
    highBin=a->FindBin(maxX);

  fac =  a->Integral( lowBin, highBin);
  if ( fac>0) a->Scale(normFac/fac);
}
*/
