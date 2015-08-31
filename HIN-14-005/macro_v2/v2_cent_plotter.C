/*
Macro to plot the
a)  v2 vs pT for prompt, non-prompt and bkg, 
b)  the phi fits for each bin
Input:
a) *.dat and *.root produced by v2_fitter.C macro in the same directory
b) the systematic uncertainties, which are calculated in excel, and hard-coded in v2_dataBinning_2015.h

 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <fstream>
#include <string>

#include <Riostream.h>
#include <TSystem.h>
#include <TProfile.h>
#include <TBrowser.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TNtuple.h>
#include <TString.h>
#include <TH1D.h>
#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TMath.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TInterpreter.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "v2_dataNumbers_2015.h"
#endif

void v2_cent_plotter(int jpsiCategory      = 2, // 1 : Prompt, 2 : Non-Prompt, 3: Bkg
		     string nDphiBins      = "4",
		     const char* outputDir = "output", 
		     const char* inputDir  = "outputNumbers",// where phi and v2 numbers are (root, and txt format)
		     bool bDoDebug         = true,
		     bool bAddCent010      = false, 
		     bool bSavePlots       = true
		     ) 
{
  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);
  
// set the style
  setTDRStyle();
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  // input files: prompt and non-prompt ones
  const char* v2InFileDirs[1] = {"histsV2Yields_20150823_v2W_Lxyz_pTtune_PRMC_dPhiBins4"};
  const char* legend[4]       = {"","Prompt J/#psi","Non-prompt J/#psi","Background"};
  const char* signal[4]       = {"", "Prp","NPrp","Bkg"};
 
  // Reminder for TGraphAssymError: gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);// n,x,y,err_x, err_y
  int nBins                   =  nCentBins_pr; 
  if(jpsiCategory==2) nBins   =  nCentBins_np;
  
  cout<<" !!!!! Number of Y bins: "<< nBins<<endl;
  
  double adXaxis[nBins];//   location on x-axis  

  double adV2[nBins]      ; // v2 values
  double adV2_stat[nBins] ;// stat uncert
  double adV2_syst[nBins] ;// stat uncert
  double adV2_err0[nBins] ;// error  0
  double adWidth_systBox[nBins]; // width of the systm. uncert.

  for(int ib=0; ib<nBins; ib++)
    {
      adWidth_systBox[ib] = 10;
      adV2_syst[ib]       = adV2Cent_pr_syst[ib];
      adXaxis[ib]         = adXaxisCent_pr[ib];

      if(jpsiCategory==2)
	{
	  adV2_syst[ib]    = adV2Cent_np_syst[ib];
	  adXaxis[ib]      = adXaxisCent_np[ib];
	}

      if(bDoDebug) cout<<"Bin "<<ib<<"\t adAxis= "<<adXaxis[ib]<<endl;
    }
  
  // // open the files with yields and do the math
  ifstream in;
  std::string nameVar   = outFilePlot[3]; // cent
  std::string nameSig   = signal[jpsiCategory]; // prompt, non-pro or bkg
  std::string nameDir   = v2InFileDirs[0];
  string inputFile      = nameVar + "_"+ nameSig + "_nphibin" + nDphiBins + ".dat";
  
  cout << "!!!!!! Input file name: "<< inputFile <<endl;
  in.open(Form("%s/%s/data/%s",inputDir,nameDir.c_str(),inputFile.c_str()));
  

  // read the v2 and v2_stat uncert from input file
  string whatBin[3];
  double x[4]={0};
  int iline=0;
  string tmpstring;
  getline(in,tmpstring);
  while ( in.good() && iline<nBins)
    {
      in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> x[0] >> x[1] >> x[2] >> x[3];

      adV2[nBins-iline-1]      = x[2];
      adV2_stat[nBins-iline-1] = x[3];

      cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
      cout <<"v2= "<< x[2] << "\t error= "<< x[3]<<endl;
      iline++;
    }
  in.close();
  if(bDoDebug)
    {
      for(int ib=0; ib<nBins; ib++)
	cout<<"Bin "<<ib<<"\t adXaxis: "<< adXaxis[ib]<<"\t v2= "<<adV2[ib]<<endl;
    }
  // high-pt
  TGraphErrors *pgV2          = new TGraphErrors(nBins, adXaxis, adV2, adV2_stat, adV2_stat);
  TGraphAsymmErrors *pgV2_sys = new TGraphAsymmErrors(nBins, adXaxis, adV2, adWidth_systBox,adWidth_systBox, adV2_syst, adV2_syst);
  TGraphErrors *pgV2_cont     = new TGraphErrors(nBins, adXaxis, adV2, adV2_err0, adV2_err0);
  
    // //-------------------------------------------------- Drawing stuff
  // colors and symbols
  // high-pt
  pgV2->SetMarkerColor(kRed+1);
  pgV2_sys->SetFillColor(kRed-9);

  pgV2->SetMarkerStyle(21);
  pgV2_sys->SetMarkerStyle(21);
  pgV2_cont->SetMarkerStyle(25); 
 
  pgV2->SetMarkerSize(1.1);
  pgV2_cont->SetMarkerSize(1.1);

  if(jpsiCategory==2)
    {
      pgV2->SetMarkerColor(kOrange+2);
      pgV2_sys->SetFillColor(kOrange-9);
    }
  if(jpsiCategory==3)// bkg
    {
      pgV2->SetMarkerColor(1);
      pgV2_sys->SetFillColor(19);
    }
 
  //-------------------------------------------------------
  // general labels 
  TLatex *lt1  = new TLatex();
  lt1->SetNDC();

  TH1F *phAxis = new TH1F("phAxis",";N_{part};v_{2}",10,0,400);
  phAxis->GetYaxis()->SetRangeUser(-0.05,0.25);
  phAxis->GetXaxis()->CenterTitle();
  phAxis->GetYaxis()->CenterTitle();

  TF1 *line    = new TF1("line","0",0,400);
  line->SetLineWidth(1);
 
  TLatex *pre = new TLatex(20.0,0.22,Form("%s",legend[jpsiCategory]));
  pre->SetTextFont(42);
  pre->SetTextSize(0.05);

  TLatex *ly     = new TLatex(20.0,0.18,Form("%s",yBinsLegend[0]));
  ly->SetTextFont(42);
  ly->SetTextSize(0.04);

  TLatex *lpt     = new TLatex(20.0,0.2,Form("%s",ptBinsLegend[0]));
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.04);

  TLatex *lcent = new TLatex(10.0,-0.025,Form("Cent."));
  lcent->SetTextFont(42);
  lcent->SetTextSize(0.04);

 //-------------- Drawing 
  TCanvas *pc = new TCanvas("pc","pc");
  phAxis->Draw();
  CMS_lumi(pc,101,33);
  pre->Draw();
  lpt->Draw();
  ly->Draw();
  lcent->Draw();

 int write2 = nBins;
  if(!bAddCent010) 
    {
      write2 = nBins-1;
      pgV2_sys->RemovePoint(write2);
      pgV2->RemovePoint(write2);
      pgV2_cont->RemovePoint(write2);
    }

  for(int ib=0; ib<write2; ib++)
    {
     if(jpsiCategory==2) lcent->DrawLatex(adXaxis[ib]-30,-0.025,Form("%s",centBinsLegend[nCentBins-1-ib]));
     else lcent->DrawLatex(adXaxis[ib]-30,-0.025,Form("%s",centBinsLegend[nCentBins-nBins-ib]));
    }
  
  pgV2_sys->Draw("2");
  pgV2->Draw("PZ");
  pgV2_cont->Draw("P");
  gPad->RedrawAxis();

 
  if(bSavePlots)
    {
      pc->SaveAs(Form("%s/png/v2_%s_%s_nphi%s.png",outputDir,outFilePlot[2],signal[jpsiCategory],nDphiBins.c_str()));
      pc->SaveAs(Form("%s/pdf/v2_%s_%s_nphi%s.pdf",outputDir,outFilePlot[2],signal[jpsiCategory],nDphiBins.c_str()));
    }

}






