//
// dimuonYellowPlot.C
// Plot the dimuon invariant mass from CMS data
// input: oniaTree
// you have to provide the location+filename of the input oniaTree
//


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <fstream>
#include <string>
#include <Riostream.h>

#include <TInterpreter.h>
#include <TROOT.h>
#include <TSystem.h>

#include <TCanvas.h>
#include <TColor.h>
#include <TFile.h>
#include <TH1F.h>

#include <TLatex.h>
#include <TLegend.h>

#include <TString.h>
#include <TStyle.h>
#include <TTree.h>

#include "CMS/tdrstyle.C"
#include "CMS/CMS_lumi.C"

#endif

void dimuonYellowPlot(bool isAA=false,
		      const char* inFileAA = "/Users/eusmartass/Software/data2011/ForYellowPlot_Jpsi_Histos_181611-183013_lowmass_147invmub.root",
		      bool isPA=false,
		      const char* inFilePA = "/Users/eusmartass/Software/data2013/Jpsi_Histos_Runs_211739-211831_HFvars.root", // this is still the pp file
		      bool isPP=true,
		      const char* inFilePP = "/Users/eusmartass/Software/data2015/OniaTree_262163_262277.root",
		      bool doSameXXCompare = false,
		      const char* inFileXXCompare  = "/Users/eusmartass/Software/data2013/All_v2.24_Histos_Runs_211739-211831_GlbGlb_woPileUpRej_muLessPV.root",
		      int nEvents=1000000000,
		      bool addLogo = false,
		      float ptMu=4)
{
  cout << "dimuonYellowPlot: Starting macro dimuonYellowPlot" << endl;
  cout << "dimuonYellowPlot: Setting styles..." << endl;

  setTDRStyle();
 
  // binning of the histo
  double bins[100000];
  bins[0] = 0.2;
  int nBins = 0;
  for (int i=1; bins[i-1]<200; i++) {
    bins[i] = bins[i-1]*1.015;//1.04;//1.015;//
    nBins++;
  }


  cout << "dimuonYellowPlot: making mass histogram with " << nBins << " bins" << endl;
  TH1F *phAA,*phPA,*phPP, *phRef;

  // reading the input oniaTree
  cout << "dimuonYellowPlot: Opening input file ..."<< endl;
  TFile *infAA, *infPA, *infPP, *infXX;
  if (isAA)
    {
      infAA = new TFile(Form("%s",inFileAA),"READ");
      // each muons is (glb&Trk), trigger = HLT_HIL1DoubleMu0_HighQ
      TTree *tree=(TTree*)infAA->Get("myTree");
      tree->Draw("Reco_QQ_4mom.M()>>phAA",Form("(Reco_QQ_trig&1)==1&&Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>%f && Reco_QQ_mumi_4mom.Pt()>%f",ptMu,ptMu),"e",nEvents);
    }
  if (isPA)
    {
      infPA = new TFile(Form("%s",inFilePA),"READ");
      phPA  = new TH1F("phPA","phPA;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
      TTree *tree=(TTree*)infPA->Get("myTree");
      // each muon is (trk), trigger = HLT_PAL1DoubleMu0_HighQ
      tree->Draw("Reco_QQ_4mom.M()>>phPA",Form("(Reco_QQ_trig&2)==2&&Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>%f&&Reco_QQ_mumi_4mom.Pt()>%f",ptMu,ptMu),"e",nEvents);
    }
  //---------------------------------------------
  if (isPP)
    {
      infPP = new TFile(Form("%s",inFilePP),"READ");
      phPP  = new TH1F("phPP","phPP;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
      TTree *tree=(TTree*)infPP->Get("myTree");
      // each muon is (trk), trigger = HLT_HIL1DoubleMu0_v1
      tree->Draw("Reco_QQ_4mom.M()>>phPP",Form("(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>%f&&Reco_QQ_mumi_4mom.Pt()>%f",ptMu,ptMu),"e",nEvents);
     
    }
  //---------------------------------------------

  TH1F *phMain = new TH1F("phMain","phMain;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
  if(isAA || isPA || isPP)// this is the main figure
    {
      if(isAA)
	phMain = (TH1F*)gDirectory->Get("phAA");
      if(isPP)
	phMain = (TH1F*)gDirectory->Get("phPP");
      if(isPA)
	phMain = (TH1F*)gDirectory->Get("phPA");
    }
  phMain->SetDirectory(0);
 
  if (doSameXXCompare)// for 2013 pp data, glbglb , and HLT_PAL1DoubleMu0_HighQ
    {
      infXX = new TFile(Form("%s",inFileXXCompare),"READ");
      phRef = new TH1F("phRef","phRef;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
      TTree *tree=(TTree*)infXX->Get("myTree");
      tree->Draw("Reco_QQ_4mom.M()>>phRef",Form("(Reco_QQ_trig&2)==2&&Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>%f&&Reco_QQ_mumi_4mom.Pt()>%f",ptMu,ptMu),"e",nEvents);
    }
 
  TH1F *phComp = new TH1F("phComp","phComp;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
  if(doSameXXCompare) phComp = (TH1F*)gDirectory->Get("phRef");
  phComp->SetDirectory(0);

  // Scale by bin width, needs to be done bin by bin because bin widths are different.
  for (int i=1; i<=phMain->GetNbinsX(); i++) {
    phMain->SetBinContent(i, phMain->GetBinContent(i)/phMain->GetBinWidth(i));
  }

 for (int i=1; i<=phComp->GetNbinsX(); i++) {
   phComp->SetBinContent(i, phComp->GetBinContent(i)/phComp->GetBinWidth(i));
  }

  //------------------------------------- DRAWING SECTION !!!!!
  TH1 *phAxis = new TH1D("phAxis","phAxis;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",1,2,200);
  phAxis->GetYaxis()->SetRangeUser(0.1,2e7);
  phAxis->SetDirectory(0);

  TCanvas* yellowPlot = new TCanvas("yellowPlot","yellowPlot");
  phAxis->Draw();  
  gPad->SetLogx();
  gPad->SetLogy();
 
  phMain->SetLineWidth(2);
  phMain->SetLineColor(kBlue+1);
  phMain->SetFillColor(kYellow);
 
  phComp->SetLineColor(kRed);
  phComp->SetFillStyle(3004);
  phComp->SetFillColor(kRed-9);

  phMain->Draw("HIST same");
 
  if(doSameXXCompare) phComp->Draw("HIST same");
  gPad->RedrawAxis();

  //
  // Options to be used with the CMS_lumi macro (for what options are available see CMS_lumi.h and CMS_lumi.C)
  //
  //
  // iPeriod options: 99 for pPb 5.02 TeV, 101 for PbPb 2011, 102 for pp 2013, 104 for pp 2015, 105 for PbPb 2015
  //
    int iPeriod      = 104;   
    if(isAA) iPeriod = 101;
    if(isPA) iPeriod = 99;   
    lumiTextOffset   = 0.3; // default 0.28

  // Call the CMS_lumi macro to draw:
  // CMS preliminary, aligned on the right and justified (iPos=33, third argument)
  // iPeriod: integrated luminosity (drawn on top left, out of frame, or use lumiTextOffset)
  // center of mass energy (drawn on top right, out of frame, or use lumiTextOffset)
  
  CMS_lumi(yellowPlot,iPeriod,33);

  float H = gPad->GetWh();
  float W = gPad->GetWw();
  float l = gPad->GetLeftMargin();
  float t = gPad->GetTopMargin();
  float r = gPad->GetRightMargin();
  float b = gPad->GetBottomMargin();
  float posX_ =   l + 0.045*(1-l-r)*W/H;
  float posY_ = 1-t - 0.045*(1-t-b);
  float xl_0 = posX_;
  float yl_0 = posY_ - 0.1;
  float xl_1 = posX_ + 0.1*H/W;
  float yl_1 = posY_;
  TASImage* CMS_logo = new TASImage("CMS-Color.gif");
  TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
  if(addLogo)
    {
      pad_logo->Draw();
      pad_logo->cd();
      CMS_logo->Draw("X");
      pad_logo->Modified();
    }
  yellowPlot->cd();
  
  // Booking the TLatex class and setting the parameters
  // CMS Preliminary (not used anymore) left as a dummy
  TLatex* latex = new TLatex(0.62,0.88,"CMS Preliminary");
  latex->SetNDC();
  /*
  latex->DrawLatex(0.24,0.88,"#rho, #omega");
  latex->DrawLatex(0.32,0.88,"#phi");
  */
  latex->DrawLatex(0.22,0.8,"J/#psi");
  // tex = new TLatex(0.38,0.94,"J/#psi");
  latex->DrawLatex(0.3,0.7,"#psi(2S)");
  //  tex = new TLatex(0.45,0.82,"#psi(2S)");
  latex->DrawLatex(0.45,0.7,"#varUpsilon(1,2,3S)");
  //  tex = new TLatex(0.59,0.76,"#varUpsilon(1,2,3S)");
  latex->DrawLatex(0.8,0.5,"Z");
  // tex = new TLatex(0.84,0.57,"Z");

  latex->DrawLatex(0.7,0.7,Form("p_{T}^{#mu} > %.0f GeV/c",ptMu));

  yellowPlot->SaveAs(Form("%s_ptMu%.0f_isAA%d_isPA%d_isPP%d_hasComp%d_hasLogo%d.pdf",yellowPlot->GetTitle(),ptMu,isAA,isPA,isPP,doSameXXCompare,addLogo));
   yellowPlot->SaveAs(Form("%s_ptMu%.0f_isAA%d_isPA%d_isPP%d_hasComp%d_hasLogo%d.png",yellowPlot->GetTitle(),ptMu,isAA,isPA,isPP,doSameXXCompare,addLogo));
  //-----------------------------------
  // TCanvas* pc1 = new TCanvas("pc1","pc1");
  // phMain->Divide(phComp);
  // phMain->Draw();
  return;
}
