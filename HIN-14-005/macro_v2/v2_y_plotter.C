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

void v2_y_plotter(int jpsiCategory      = 2, // 1 : Prompt, 2 : Non-Prompt, 3: Bkg
		  string nDphiBins      = "4",
		  const char* outputDir = "output", 
		  const char* inputDir  = "outputNumbers",// where phi and v2 numbers are (root, and txt format)
		  bool bDoDebug         = false,
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
  int nBins                   =  nYBins_pr; 
  if(jpsiCategory==2) nBins   =  nYBins_np;
  
  cout<<" !!!!! Number of Y bins: "<< nBins<<endl;
  
  double adXaxis[nBins];//   location on x-axis  
  double adXaxis_l[nBins];// bin width to the left
  double adXaxis_h[nBins];// bin width to the right

  double adV2[nBins]      ; // v2 values
  double adV2_stat[nBins] ;// stat uncert
  double adV2_syst[nBins] ;// stat uncert
  double adV2_err0[nBins] ;// error  0
  double adWidth_systBox[nBins]; // width of the systm. uncert.

  for(int ib=0; ib<nBins; ib++)
    {
      adWidth_systBox[ib] = 0.05;
      adV2_syst[ib]    = adV2Y_pr_syst[ib];
      adXaxis[ib]      = adXaxisY_pr[ib];
      adXaxis_l[ib]    = adXaxisY_pr_l[ib];
      adXaxis_h[ib]    = adXaxisY_pr_h[ib];
      if(bDoDebug) cout<<"bin "<<ib<<"low_Err"<<adXaxis_l[ib]<<"\t high_err: "<< adXaxis_h[ib]<<endl;

      if(jpsiCategory==2)
	{
	  adV2_syst[ib]    = adV2Y_np_syst[ib];
	  adXaxis[ib]      = adXaxisY_np[ib];
	  adXaxis_l[ib]    = adXaxisY_np_l[ib];
	  adXaxis_h[ib]    = adXaxisY_np_h[ib];
	}
    }
  
  // // open the files with yields and do the math
  ifstream in;
  std::string nameVar   = outFilePlot[2]; // y
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

      adV2[iline]      = x[2];
      adV2_stat[iline] = x[3];

      cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
      cout <<"v2= "<< x[2] << "\t error= "<< x[3]<<endl;
      iline++;
    }
  in.close();
  if(bDoDebug)
    {
      for(int ib=0; ib<nBins; ib++)
	{
	  cout<<"Bin "<<ib<<"\t stat. uncert.: "<<adV2_stat[ib]<<endl;
	  cout << "adXaxis: "<< adXaxis[ib]<<"\t adXaxis_l: "<<adXaxis_l[ib]<<"\t adXaxis_h: "<<adXaxis_h[ib]<<endl;
	}
    }
  // high-pt
  TGraphAsymmErrors *pgV2     = new TGraphAsymmErrors(nBins, adXaxis, adV2, adXaxis_l, adXaxis_h, adV2_stat, adV2_stat);
  TGraphAsymmErrors *pgV2_sys = new TGraphAsymmErrors(nBins, adXaxis, adV2, adWidth_systBox,adWidth_systBox, adV2_syst, adV2_syst);
  TGraphAsymmErrors *pgV2_cont= new TGraphAsymmErrors(nBins, adXaxis, adV2, adV2_err0, adV2_err0, adV2_err0, adV2_err0);
  
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

  TH1F *phAxis = new TH1F("phAxis",";|y|;v_{2}",10,0,2.4);
  phAxis->GetYaxis()->SetRangeUser(-0.05,0.25);
  phAxis->GetXaxis()->CenterTitle();
  phAxis->GetYaxis()->CenterTitle();
  phAxis->GetXaxis()->SetNdivisions(-6);

  TF1 *line    = new TF1("line","0",0,400);
  line->SetLineWidth(1);
 
  TLatex *pre = new TLatex(0.15,0.22,Form("%s",legend[jpsiCategory]));
  pre->SetTextFont(42);
  pre->SetTextSize(0.05);

  TLatex *lpt     = new TLatex(0.15,0.2,Form("%s",ptBinsLegend[0]));
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.04);

  TLatex *lcent = new TLatex(0.15,0.18,Form("%s",centBinsLegend[0]));
  lcent->SetTextFont(42);
  lcent->SetTextSize(0.04);

 //-------------- Drawing 
  TCanvas *pc = new TCanvas("pc","pc");
  phAxis->Draw();
  CMS_lumi(pc,101,33);
  pre->Draw();
  lpt->Draw();
  lcent->Draw();

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






