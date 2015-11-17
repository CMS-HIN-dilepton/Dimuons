/*
Macro to compare the yields corrected in the traditional way, yield*correction, and the weighted ev-by-ev way

Input: root files produced by the readFitTable/makeHisto_raa.C (check there the input file names)

Output: yields and raa (traditional vs ev-by-ev)

 */

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TLegend.h"

#include "dataBinning_2015.h"
#include "filesRaa_2015.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#endif

void compare_pt( bool bSavePlots    = true,
              bool bDoDebug         = 1, // adds some numbers, numerator, denominator, to help figure out if things are read properly
              int whichCompare      = 1,//0: no TnP corrections; 1: w/ TnP corr on Data; 2: w/ TnP corr on MC; 3: lxy w/ TnP on MC
              const char* inputDir  = "../readFitTable", // the place where the input root files, with the histograms are
              const char* outputDir = "figs/compare")// where the output figures will be
{
 
  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);
 
 // set the style
  setTDRStyle();

  // type of available comparisons:
  const char* compWhat[4] = {"noTnP","dataTnP","mcTnP","lxyTnP"};

  const int nInHist = 4;
  const char* yieldHistNames[nInHist] = {"pt", "ptLow", "ptLow_mb", "mb"};

  // input files: are in the filesRaa_2015.h

  // open the files with yields and do the math
  TFile *fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[0]));
  TFile *fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[1]));
  
  TFile *fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[0]));
  TFile *fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[1]));

  TFile *fEffFile_aa = new TFile(Form("%s/%s",inputDir,effHistFile[0]));
  TFile *fEffFile_pp = new TFile(Form("%s/%s",inputDir,effHistFile[1]));

 switch(whichCompare){
  case 0:
    cout << "You are making Raa, with NOT TnP corrections whatsoever!"<<endl;
    fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_0[0]));
    fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_0[1]));
  
    fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_0[0]));
    fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_0[1]));

    fEffFile_aa = new TFile(Form("%s/%s",inputDir,effHistFile_noTnP[0]));
    fEffFile_pp = new TFile(Form("%s/%s",inputDir,effHistFile_noTnP[1]));
    break;

  case 2:
    cout << "You are making Raa, with TnP corrections applied on MC!"<<endl;
    fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_2[0]));
    fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_2[1]));
  
    fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_2[0]));
    fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_2[1]));
    break;

  case 3: 
    cout << "You are making Raa, with Lxy and TnP corrections applie on MC!"<<endl;
    fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_3[0]));
    fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_3[1]));
  
    fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_3[0]));
    fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_3[1]));
    break;

  case 1:
  default:
    cout<<" You are doing Raa Nominal: TnP on data!"<<endl;
    break;
  }

  if (!fYesWeighFile_aa->IsOpen() || !fYesWeighFile_pp->IsOpen()|| !fNoWeighFile_aa->IsOpen() || !fNoWeighFile_pp->IsOpen() || !fEffFile_aa->IsOpen() || !fEffFile_pp->IsOpen()) {
    cout << "One or more input files are missing" << endl;
    return ;
  }

  TH1F *phRaw_pr_pp; 
  TH1F *phCorr_pr_pp;
  TH1F *phEff_pr_pp;
  TH1F *phRaw_pr_aa; 
  TH1F *phCorr_pr_aa;
  TH1F *phEff_pr_aa;

  TH1F *phRaw_npr_pp;
  TH1F *phCorr_npr_pp;
  TH1F *phEff_npr_pp;
  TH1F *phRaw_npr_aa; 
  TH1F *phCorr_npr_aa;
  TH1F *phEff_npr_aa;

  // to store the ratio of ev-by-ev  and traditionally corrected yields in each case
  TH1F *ahRatio_pr_pp[nInHist];
  TH1F *ahRatio_npr_pp[nInHist];
  TH1F *ahRatio_pr_aa[nInHist];
  TH1F *ahRatio_npr_aa[nInHist];


  for(int ih=0; ih<nInHist;ih++) // for each kinematic range
  {
    TString hist_pr(Form("phPrp_%s",yieldHistNames[ih]));
    TString hist_npr(Form("phNPrp_%s",yieldHistNames[ih]));
    
    cout<<"histogram input name: "<< hist_pr<<"\t"<<hist_npr<<endl; 

    // prompt histos
    phRaw_pr_pp  = (TH1F*)fNoWeighFile_pp->Get(hist_pr);
    phCorr_pr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_pr);
    phRaw_pr_aa  = (TH1F*)fNoWeighFile_aa->Get(hist_pr);
    phCorr_pr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_pr);
    
    // non-prompt histos
    phRaw_npr_pp  = (TH1F*)fNoWeighFile_pp->Get(hist_npr);
    phCorr_npr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_npr);
    phRaw_npr_aa  = (TH1F*)fNoWeighFile_aa->Get(hist_npr);
    phCorr_npr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_npr);

    // efficiency histos
    phEff_pr_pp  = (TH1F*)fEffFile_pp->Get(hist_pr);
    phEff_npr_pp = (TH1F*)fEffFile_pp->Get(hist_npr);
    phEff_pr_aa  = (TH1F*)fEffFile_aa->Get(hist_pr);
    phEff_npr_aa = (TH1F*)fEffFile_aa->Get(hist_npr);

    double scaleFactor = ppLumi/nMbEvents;
    double scale_cent = 1/(adTaaMB[0]*adDeltaCentMB[0]);

    //---------------  corrected yields in the traditional way (simple division for the moment)
    // store the yield_Ratio histos:  ev_by_ev/(raw*eff), 
    ahRatio_pr_pp[ih]  = (TH1F *)phCorr_pr_pp->Clone();
    ahRatio_pr_pp[ih]->SetDirectory(0);
    ahRatio_pr_pp[ih]->Divide(phRaw_pr_pp);
    ahRatio_pr_pp[ih]->Multiply(phEff_pr_pp);// correct the raw with it's efficiency

    ahRatio_npr_pp[ih]  = (TH1F *)phCorr_npr_pp->Clone();
    ahRatio_npr_pp[ih]->SetDirectory(0);
    ahRatio_npr_pp[ih]->Divide(phRaw_npr_pp);
    ahRatio_npr_pp[ih]->Multiply(phEff_npr_pp);

    ahRatio_pr_aa[ih]  = (TH1F *)phCorr_pr_aa->Clone();
    ahRatio_pr_aa[ih]->SetDirectory(0);
    ahRatio_pr_aa[ih]->Divide(phRaw_pr_aa);
    ahRatio_pr_aa[ih]->Multiply(phEff_pr_aa);

    ahRatio_npr_aa[ih]  = (TH1F *)phCorr_npr_aa->Clone();
    ahRatio_npr_aa[ih]->SetDirectory(0);
    ahRatio_npr_aa[ih]->Divide(phRaw_npr_aa);
    ahRatio_npr_aa[ih]->Multiply(phEff_npr_aa);

    int numBins = 0;
    if(ih==0) numBins = nBinsPt;// pT
    if(ih==1 || ih==2) numBins = nBinsPt3; // pt low // ptlow_mb
    if(ih==3) numBins = nBinsMB; // mb

    for(int ibin=1; ibin<=numBins; ibin++)
    {
      double raa_pr=0, raaErr_pr=0, raa_npr=0, raaErr_npr=0;
      double raaTrad_pr=0, raaTradErr_pr=0, raaTrad_npr=0, raaTradErr_npr=0;

      //prompt
      double dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
      double dRelErrRaw_pr_aa  = phRaw_pr_aa->GetBinError(ibin)/phRaw_pr_aa->GetBinContent(ibin);
    
      double yieldRatio_pr     = phCorr_pr_aa->GetBinContent(ibin)/phCorr_pr_pp->GetBinContent(ibin);
      
      double yieldRatioTrad_pr = (phRaw_pr_aa->GetBinContent(ibin)/phRaw_pr_pp->GetBinContent(ibin))
        * (phEff_pr_pp->GetBinContent(ibin)/phEff_pr_aa->GetBinContent(ibin));

      raa_pr      =  yieldRatio_pr * scaleFactor * scale_cent;
      raaErr_pr   = TMath::Sqrt(TMath::Power(dRelErrRaw_pr_pp,2)+TMath::Power(dRelErrRaw_pr_aa,2))*raa_pr;

      raaTrad_pr      =  yieldRatioTrad_pr * scaleFactor * scale_cent;
      raaTradErr_pr   = TMath::Sqrt(TMath::Power(dRelErrRaw_pr_pp,2)+TMath::Power(dRelErrRaw_pr_aa,2))*raaTrad_pr;

      //non-prompt
      // get the rel uncert from the raw sample
      double dRelErrRaw_npr_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);
      double dRelErrRaw_npr_aa  = phRaw_npr_aa->GetBinError(ibin)/phRaw_npr_aa->GetBinContent(ibin);
      double yieldRatio_npr     = phCorr_npr_aa->GetBinContent(ibin)/phCorr_npr_pp->GetBinContent(ibin);
      double yieldRatioTrad_npr = phRaw_npr_aa->GetBinContent(ibin)/phRaw_npr_pp->GetBinContent(ibin)
          * (phEff_npr_pp->GetBinContent(ibin)/phEff_npr_aa->GetBinContent(ibin));

      raa_npr    = yieldRatio_npr * scaleFactor * scale_cent;
      raaErr_npr = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raa_npr;

      raaTrad_npr= yieldRatioTrad_npr * scaleFactor * scale_cent;
      raaTradErr_npr = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raaTrad_npr;
     
      // fill the corresponding array
      switch(ih){
      case 0:
        prJpsi_pt[ibin-1]           = raa_pr;
        prJpsiErr_pt[ibin-1]        = raaErr_pr;
              
        nonPrJpsi_pt[ibin-1]        = raa_npr;
        nonPrJpsiErr_pt[ibin-1]     = raaErr_npr;

        prJpsiTrad_pt[ibin-1]       = raaTrad_pr;
        prJpsiTradErr_pt[ibin-1]    = raaTradErr_pr;
              
        nonPrJpsiTrad_pt[ibin-1]    = raaTrad_npr;
        nonPrJpsiTradErr_pt[ibin-1] = raaTradErr_npr;

        break;

      case 1:
        prJpsi_pt365y1624_pt[ibin-1]            = raa_pr;
        prJpsiErr_pt365y1624_pt[ibin-1]         = raaErr_pr;

        nonPrJpsi_pt365y1624_pt[ibin-1]         = raa_npr;
        nonPrJpsiErr_pt365y1624_pt[ibin-1]      = raaErr_npr;

        prJpsiTrad_pt365y1624_pt[ibin-1]        = raaTrad_pr;
        prJpsiTradErr_pt365y1624_pt[ibin-1]     = raaTradErr_pr;

        nonPrJpsiTrad_pt365y1624_pt[ibin-1]     = raaTrad_npr;
        nonPrJpsiTradErr_pt365y1624_pt[ibin-1]  = raaTradErr_npr;

        if(bDoDebug)
          {
            cout<<"yield_npr_aa: raw "<<phRaw_npr_aa->GetBinContent(ibin)<<"\t eff:  "<<phEff_npr_aa->GetBinContent(ibin)<<endl;
            cout<<"yield_npr_aa: corr "<<phCorr_npr_aa->GetBinContent(ibin)<<endl;
          
            cout<<"yield_npr_pp: raw "<<phRaw_npr_pp->GetBinContent(ibin)<<"\t eff:  "<<phEff_npr_pp->GetBinContent(ibin)<<endl;
            cout<<"yield_npr_pp: corr "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;

          }

        break;

      case 2:
        prJpsi_y1624MB_pt[ibin-1]            = raa_pr;
        prJpsiErr_y1624MB_pt[ibin-1]         = raaErr_pr;

        nonPrJpsi_y1624MB_pt[ibin-1]         = raa_npr;
        nonPrJpsiErr_y1624MB_pt[ibin-1]      = raaErr_npr;

        prJpsiTrad_y1624MB_pt[ibin-1]        = raaTrad_pr;
        prJpsiTradErr_y1624MB_pt[ibin-1]     = raaTradErr_pr;

        nonPrJpsiTrad_y1624MB_pt[ibin-1]     = raaTrad_npr;
        nonPrJpsiTradErr_y1624MB_pt[ibin-1]  = raaTradErr_npr;

        if(bDoDebug)
        {
          cout<<"yield_npr_aa "<<phCorr_npr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;
          cout<<setprecision(2);
          cout<<"!!!!! raa_pr = "<<raa_pr<<"\t raa_npr= "<<raa_npr<<endl;
        }
        break;
      case 3:
        // mb
        prJpsi_mb[0]           = raa_pr;
        prJpsiErr_mb[0]        = raaErr_pr;
        
        nonPrJpsi_mb[0]        = raa_npr;
        nonPrJpsiErr_mb[0]     = raaErr_npr;


        prJpsiTrad_mb[0]       = raaTrad_pr;
        prJpsiTradErr_mb[0]    = raaTradErr_pr;
        
        nonPrJpsiTrad_mb[0]    = raaTrad_npr;
        nonPrJpsiTradErr_mb[0] = raaTradErr_npr;
        break;
      }
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++)
  }//loop end: for(int ih=0; ih<nInHist;ih++)

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsi     = new TGraphErrors(nBinsPt, binsPt, prJpsi_pt, binsPtErr, prJpsiErr_pt);
  TGraphErrors *gPrJpsi_mb     = new TGraphErrors(nBinsMB, binsPtMB, prJpsi_pt, binsPtMBErr, prJpsiErr_pt);
   
  TGraphErrors *gPrJpsi_y1624_mb     = new TGraphErrors(nBinsPt3, binsPt3_mb, prJpsi_y1624MB_pt, binsPt3Err_mb, prJpsiErr_y1624MB_pt);
  TGraphErrors *gPrJpsi_pt365y1624     = new TGraphErrors(nBinsPt3, binsPt3, prJpsi_pt365y1624_pt, binsPt3Err, prJpsiErr_pt365y1624_pt);
   
  // nonPr   
  TGraphErrors *gNonPrJpsi     = new TGraphErrors(nBinsPt, binsPt, nonPrJpsi_pt, binsPtErr, nonPrJpsiErr_pt);
  TGraphErrors *gNonPrJpsi_mb     = new TGraphErrors(nBinsMB, binsPtMB, nonPrJpsi_pt, binsPtMBErr, nonPrJpsiErr_pt);
  
  TGraphErrors *gNonPrJpsi_y1624_mb     = new TGraphErrors(nBinsPt3, binsPt3_mb, nonPrJpsi_y1624MB_pt, binsPt3Err_mb, nonPrJpsiErr_y1624MB_pt);
  TGraphErrors *gNonPrJpsi_pt365y1624     = new TGraphErrors(nBinsPt3, binsPt3, nonPrJpsi_pt365y1624_pt, binsPt3Err, nonPrJpsiErr_pt365y1624_pt);
  
  //------------------- traditional results
 // pr
  TGraphErrors *gPrJpsiTrad               = new TGraphErrors(nBinsPt, binsPt, prJpsiTrad_pt, binsPtErr, prJpsiTradErr_pt);
  TGraphErrors *gPrJpsiTrad_mb            = new TGraphErrors(nBinsMB, binsPtMB, prJpsiTrad_pt, binsPtMBErr, prJpsiTradErr_pt);
 
  TGraphErrors *gPrJpsiTrad_y1624_mb      = new TGraphErrors(nBinsPt3, binsPt3_mb, prJpsiTrad_y1624MB_pt, binsPt3Err_mb, prJpsiTradErr_y1624MB_pt);
  TGraphErrors *gPrJpsiTrad_pt365y1624    = new TGraphErrors(nBinsPt3, binsPt3, prJpsiTrad_pt365y1624_pt, binsPt3Err, prJpsiTradErr_pt365y1624_pt);
   
  // nonPr   
  TGraphErrors *gNonPrJpsiTrad            = new TGraphErrors(nBinsPt, binsPt, nonPrJpsiTrad_pt, binsPtErr, nonPrJpsiTradErr_pt);
  TGraphErrors *gNonPrJpsiTrad_mb         = new TGraphErrors(nBinsMB, binsPtMB, nonPrJpsiTrad_pt, binsPtMBErr, nonPrJpsiTradErr_pt);

  TGraphErrors *gNonPrJpsiTrad_y1624_mb   = new TGraphErrors(nBinsPt3, binsPt3_mb, nonPrJpsiTrad_y1624MB_pt, binsPt3Err_mb, nonPrJpsiTradErr_y1624MB_pt);
  TGraphErrors *gNonPrJpsiTrad_pt365y1624 = new TGraphErrors(nBinsPt3, binsPt3, nonPrJpsiTrad_pt365y1624_pt, binsPt3Err, nonPrJpsiTradErr_pt365y1624_pt);


  //-------------------------------------------------------------------
  // **************** marker colors 
  gPrJpsi->SetMarkerColor(kRed);
  gPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);
  gNonPrJpsi->SetMarkerColor(kOrange+2);
  gNonPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);

  gPrJpsi_mb->SetMarkerColor(kCyan+2);
  gPrJpsi_y1624_mb->SetMarkerColor(kViolet+2);
 
  gNonPrJpsi_mb->SetMarkerColor(kCyan+2);
  gNonPrJpsi_y1624_mb->SetMarkerColor(kViolet+2);

  //--------- marker style  
  gPrJpsi->SetMarkerStyle(21);
  gPrJpsi_pt365y1624->SetMarkerStyle(34);
  gNonPrJpsi->SetMarkerStyle(29);
  gNonPrJpsi_pt365y1624->SetMarkerStyle(34);

  gPrJpsi_mb->SetMarkerStyle(29);
  gPrJpsi_y1624_mb->SetMarkerStyle(21);   
  gNonPrJpsi_mb->SetMarkerStyle(29);
  gNonPrJpsi_y1624_mb->SetMarkerStyle(21);

  // --------- marker size
  gPrJpsi->SetMarkerSize(1.2);
  gPrJpsi_pt365y1624->SetMarkerSize(1.7);
  gPrJpsi_mb->SetMarkerSize(2.1);
  gPrJpsi_y1624_mb->SetMarkerSize(1.2);

  gNonPrJpsi->SetMarkerSize(2.0);
  gNonPrJpsi_pt365y1624->SetMarkerSize(1.7);
  gNonPrJpsi_mb->SetMarkerSize(2.1);
  gNonPrJpsi_y1624_mb->SetMarkerSize(1.2);
  
 
  // ------------------------------------ traditional plots: all in black
  // raa
  gPrJpsiTrad->SetMarkerStyle(25);
  gPrJpsiTrad_pt365y1624->SetMarkerStyle(28);
  gNonPrJpsiTrad->SetMarkerStyle(30);
  gNonPrJpsiTrad_pt365y1624->SetMarkerStyle(28);

  gPrJpsiTrad_mb->SetMarkerStyle(30);
  gPrJpsiTrad_y1624_mb->SetMarkerStyle(25);   
  gNonPrJpsiTrad_mb->SetMarkerStyle(30);
  gNonPrJpsiTrad_y1624_mb->SetMarkerStyle(25);
 
  // size
  gPrJpsiTrad->SetMarkerSize(1.2);
  gPrJpsiTrad_pt365y1624->SetMarkerSize(1.7);
  gNonPrJpsiTrad->SetMarkerSize(2.1);
  gNonPrJpsiTrad_pt365y1624->SetMarkerSize(1.7);

  gPrJpsiTrad_mb->SetMarkerSize(2.1);
  gPrJpsiTrad_y1624_mb->SetMarkerSize(1.2);
  gNonPrJpsiTrad_mb->SetMarkerSize(2.1);
  gNonPrJpsiTrad_y1624_mb->SetMarkerSize(1.2);

 // same for the raa denominator and nominator yields histograms
  for(int ih=0; ih<nInHist;ih++) // for each kinematic range
  {
    ahRatio_pr_pp[ih]->SetMarkerStyle(20);
    ahRatio_npr_pp[ih]->SetMarkerStyle(20);
    ahRatio_pr_aa[ih]->SetMarkerStyle(20);
    ahRatio_npr_aa[ih]->SetMarkerStyle(20);

    if(ih==1 || ih==2)
    {
      ahRatio_pr_pp[ih]->SetMarkerColor(kViolet+2);
      ahRatio_npr_pp[ih]->SetMarkerColor(kViolet+2);
      ahRatio_pr_aa[ih]->SetMarkerColor(kViolet+2);
      ahRatio_npr_aa[ih]->SetMarkerColor(kViolet+2);
    }
    if(ih==3)
    {
      ahRatio_pr_pp[ih]->SetMarkerColor(kCyan+2);
      ahRatio_npr_pp[ih]->SetMarkerColor(kCyan+2);
      ahRatio_pr_aa[ih]->SetMarkerColor(kCyan+2);
      ahRatio_npr_aa[ih]->SetMarkerColor(kCyan+2);
    }

  }
  //------------------------------------------------------ Drawing settings
  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,30);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);
 
  //---------------- general stuff
  TLatex *ly     = new TLatex(20.0,0.05,"|y| < 2.4");
  ly->SetTextFont(42);
  ly->SetTextSize(0.05);
  TLatex *lcent = new TLatex(19,1.03,"Cent. 0-100%");
  lcent->SetTextFont(42);
  lcent->SetTextSize(0.05);

  TLatex *lPr = new TLatex(2,1.35,"Prompt J/#psi");
  lPr->SetTextFont(42);
  lPr->SetTextSize(0.05);

  TLatex *lNpr = new TLatex(2,1.35,"Non-prompt J/#psi");
  lNpr->SetTextFont(42);
  lNpr->SetTextSize(0.05);

  TLegend *leg11a = new TLegend(0.65,0.52,0.8,0.65);
  leg11a->SetFillStyle(0);
  leg11a->SetFillColor(0);
  leg11a->SetBorderSize(0);
  leg11a->SetMargin(0.2);
  leg11a->SetTextSize(0.04);
  leg11a->AddEntry(gPrJpsi,"|y|<2.4","P");
  leg11a->AddEntry(gPrJpsi_pt365y1624,"1.6<|y|<2.4","P");

  TLegend *leg11b = new TLegend(0.65,0.52,0.8,0.65);
  leg11b->SetFillStyle(0);
  leg11b->SetFillColor(0);
  leg11b->SetBorderSize(0);
  leg11b->SetMargin(0.2);
  leg11b->SetTextSize(0.04);
  
  leg11b->AddEntry(gPrJpsi_mb,"|y|<2.4","P");
  leg11b->AddEntry(gPrJpsi_y1624_mb,"1.6<|y|<2.4","P");

  TLegend *leg22a = new TLegend(0.65,0.52,0.8,0.65);
  leg22a->SetFillStyle(0);
  leg22a->SetFillColor(0);
  leg22a->SetBorderSize(0);
  leg22a->SetMargin(0.2);
  leg22a->SetTextSize(0.04);
  leg22a->AddEntry(gNonPrJpsi,"|y|<2.4","P");
  leg22a->AddEntry(gNonPrJpsi_pt365y1624,"1.6<|y|<2.4","P");
 
  TLegend *leg22b = new TLegend(0.65,0.52,0.8,0.65);
  leg22b->SetFillStyle(0);
  leg22b->SetFillColor(0);
  leg22b->SetBorderSize(0);
  leg22b->SetMargin(0.2);
  leg22b->SetTextSize(0.04);
  
  leg22b->AddEntry(gNonPrJpsi_mb,"|y|<2.4","P");
  leg22b->AddEntry(gNonPrJpsi_y1624_mb,"1.6<|y|<2.4","P");
 
  //  -----------------------for comparison purposes
  // axis for the yields
  TF1 *fBin = new TF1("fBin","1",0,8);
  fBin->SetLineWidth(1);
  fBin->GetXaxis()->SetTitle("Bin number");
  fBin->GetYaxis()->SetTitle("Yield ratio");
  fBin->GetYaxis()->SetRangeUser(0.5,2);
  fBin->GetXaxis()->CenterTitle(kTRUE);

  TLatex *lRatio = new TLatex(0.5,1.7,"Yield: Ev-by-Ev/Trad. correction");
  lRatio->SetTextFont(42);
  lRatio->SetTextSize(0.05);

  TLatex *lPP = new TLatex(0.5,1.8,"pp@2.76TeV");
  lPP->SetTextFont(42);
  lPP->SetTextSize(0.05);

  TLatex *lAA = new TLatex(0.5,1.8,"PbPb@2.76TeV");
  lAA->SetTextFont(42);
  lAA->SetTextSize(0.05);

  //------------------------------------------------------------------------- compare yields and raa-s
  //------------------------------------------------------------------------- compare yields and raa-s
  //const char* yieldHistNames[nInHist] = {"pt", "ptLow", "ptLow_mb", "mb"};

  // canveas with minbias pT dependence, from low to high-pt
  TCanvas *c1 = new TCanvas("c1","c1",1200,400);
  c1->Divide(3,1);
  // general stuff
  c1->cd(1);
  f4->Draw();// axis
           
  gPrJpsi->Draw("P");
  gPrJpsi_pt365y1624->Draw("P");
      
  gPrJpsiTrad->Draw("P");
  gPrJpsiTrad_pt365y1624->Draw("P");
       
  leg11a->Draw();
  lPr->Draw();
  lcent->Draw();  
  gPad->RedrawAxis();

  // yields
  c1->cd(2);
  fBin->Draw();// axis
  gPad->SetGridy();
  lPP->Draw();

  ahRatio_pr_pp[1]->Draw("sames");
  ahRatio_pr_pp[0]->Draw("sames");
  
  lRatio->Draw();
  
  c1->cd(3);
  fBin->Draw();// axis
  gPad->SetGridy();
    
  ahRatio_pr_aa[1]->Draw("sames");
  ahRatio_pr_aa[0]->Draw("sames");
  
  lAA->Draw();
  lRatio->Draw();
  
  if(bSavePlots)
  {
    c1->SaveAs(Form("%s/pdf/PrJpsi_vsPt_%s.pdf",outputDir,compWhat[whichCompare]));
    c1->SaveAs(Form("%s/png/PrJpsi_vsPt_%s.png",outputDir,compWhat[whichCompare]));
  }
      
      //-------------------minbias dependence
  // raa minbias integrated in pt, y and cent; for low-pt and high-pt
  TCanvas *c11b = new TCanvas("c11b","c11b",1200,400);
  c11b->Divide(3,1);

  c11b->cd(1);
  f4->Draw();
   
  gPrJpsi_mb->Draw("P");
  gPrJpsi_y1624_mb->Draw("P");

  gPrJpsiTrad_mb->Draw("P");
  gPrJpsiTrad_y1624_mb->Draw("P");
 
  lcent->Draw();
  leg11b->Draw();
  lPr->Draw();
  gPad->RedrawAxis();

  c11b->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  
  ahRatio_pr_pp[2]->Draw("sames");
  ahRatio_pr_pp[3]->Draw("sames");
  
  lPP->Draw();
  lRatio->Draw();
  
  c11b->cd(3);
  fBin->Draw();
  gPad->SetGridy();
  
  ahRatio_pr_aa[2]->Draw("sames");
  ahRatio_pr_aa[3]->Draw("sames");
  
  lAA->Draw();
  lRatio->Draw();
  
  
  if(bSavePlots)
  {
    c11b->SaveAs(Form("%s/pdf/PrJpsi_vsPt_mb_%s.pdf",outputDir,compWhat[whichCompare]));
    c11b->SaveAs(Form("%s/png/PrJpsi_vsPt_mb_%s.png",outputDir,compWhat[whichCompare]));
  }
      
  // //   // ############################################## non-pr 
  // //   // ############################################## non-pr
  // //   // ############################################## non-pr 
  // //   // ############################################## non-pr
  // raa vs pT, 0-100%
  TCanvas *c2 = new TCanvas("c2","c2",1200,400);
  c2->Divide(3,1);
  c2->cd(1);
  f4->Draw();// axis
   
  gNonPrJpsi->Draw("P");
  gNonPrJpsi_pt365y1624->Draw("P");
  
  gNonPrJpsiTrad->Draw("P");
  gNonPrJpsiTrad_pt365y1624->Draw("P");
  
  lcent->Draw(); 
  leg22a->Draw();
  lNpr->Draw();
  
  c2->cd(2);
  fBin->Draw();// axis
  gPad->SetGridy();
    
  ahRatio_npr_pp[1]->Draw("sames");
  ahRatio_npr_pp[0]->Draw("sames");
      
  lPP->Draw();
  lRatio->Draw();

  c2->cd(3);
  fBin->Draw();// axis
  gPad->SetGridy();
    
  ahRatio_npr_aa[1]->Draw("sames");
  ahRatio_npr_aa[0]->Draw("sames");
  
  lAA->Draw();
  lRatio->Draw();
  
  if(bSavePlots)
  {
    c2->SaveAs(Form("%s/pdf/nonPrJpsi_vsPt_%s.pdf",outputDir,compWhat[whichCompare]));
    c2->SaveAs(Form("%s/png/nonPrJpsi_vsPt_%s.png",outputDir,compWhat[whichCompare]));
  }
  
  //-------------------minbias dependence
  // raa integrated in pt and cent, for low-pt and high-pt
  TCanvas *c22b = new TCanvas("c22b","c22b",1200,400);
  c22b->Divide(3,1);
  
  c22b->cd(1);
  f4->Draw();
  
  gNonPrJpsi_mb->Draw("P");
  gNonPrJpsi_y1624_mb->Draw("P");
  
  gNonPrJpsiTrad_mb->Draw("P");
  gNonPrJpsiTrad_y1624_mb->Draw("P");
  
  
  lcent->Draw();
  leg22b->Draw();
  lNpr->Draw();
      
  gPad->RedrawAxis();
      
  c22b->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  
  ahRatio_npr_pp[2]->Draw("sames");
  ahRatio_npr_pp[3]->Draw("sames");

  lPP->Draw();
  lRatio->Draw();
  
  c22b->cd(3);
  fBin->Draw();
  gPad->SetGridy();
      
  ahRatio_npr_aa[2]->Draw("sames");
  ahRatio_npr_aa[3]->Draw("sames");
  
  lAA->Draw();
  lRatio->Draw();

  if(bSavePlots)
  {
    c22b->SaveAs(Form("%s/pdf/nonPrJpsi_vsPt_mb_%s.pdf",outputDir,compWhat[whichCompare]));
    c22b->SaveAs(Form("%s/png/nonPrJpsi_vsPt_mb_%s.png",outputDir,compWhat[whichCompare]));
  }
  
}
