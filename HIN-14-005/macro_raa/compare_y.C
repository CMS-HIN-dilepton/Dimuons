/*
Macro to plot the Efficiency vs centrality, for all the analysis bins.(in the traditional way, yield  x correction, and the weighted way)

Input: root files produced by the readFitTable/makeHisto_raa.C (check there the input file names)

Output: the Raa vs rpaidity.

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

void compare_y(bool bSavePlots       = true,
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

  const int nInHist = 3;
  const char* yieldHistNames[nInHist] = {"y", "y_mb", "mb"};

  //-----------------------------------------
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

    double scaleFactor = ppLumi/nMbEvents;
    double scale_cent = 1/(adTaaMB[0]*adDeltaCentMB[0]);
  
    int numBins = 0;
    if(ih==0) numBins = nBinsY;
    if(ih==1) numBins = nBinsY3;
    if(ih==2) numBins = nBinsMB;

    cout << "Number of bins: " << numBins<<endl;

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
        prJpsi_y[ibin-1]       = raa_pr;
        prJpsiErr_y[ibin-1]    = raaErr_pr;
        nonPrJpsi_y[ibin-1]    = raa_npr;
        nonPrJpsiErr_y[ibin-1] = raaErr_npr;

        prJpsiTrad_y[ibin-1]       = raaTrad_pr;
        prJpsiTradErr_y[ibin-1]    = raaTradErr_pr;              
        nonPrJpsiTrad_y[ibin-1]    = raaTrad_npr;
        nonPrJpsiTradErr_y[ibin-1] = raaTradErr_npr;

        if(bDoDebug)
        {
          cout<<"yield_pr_aa "<<phCorr_pr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_pr_pp->GetBinContent(ibin)<<endl;
          cout<<"yield_npr_aa "<<phCorr_npr_aa->GetBinContent(ibin)<<"\t yield_npr_pp "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;
          cout<<"!!!!! raa_pr = "<<raa_pr<<"\t raa_npr= "<<raa_npr<<endl;
        }
        break;
   
      case 1:
        prJpsi_y_y[ibin-1]        = raa_pr;
        prJpsiErr_y_y[ibin-1]     = raaErr_pr;
        nonPrJpsi_y_y[ibin-1]     = raa_npr;
        nonPrJpsiErr_y_y[ibin-1]  = raaErr_npr;

        prJpsiTrad_y_y[ibin-1]        = raaTrad_pr;
        prJpsiTradErr_y_y[ibin-1]     = raaTradErr_pr;
        nonPrJpsiTrad_y_y[ibin-1]     = raaTrad_npr;
        nonPrJpsiTradErr_y_y[ibin-1]  = raaTradErr_npr;

        if(bDoDebug)
        {
          cout<<"yield_npr_aa "<<phCorr_npr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;
          cout<<setprecision(2);
          cout<<"!!!!! raa_pr = "<<raa_pr<<"\t raa_npr= "<<raa_npr<<endl;
        }
        break;

      case 2:
        // mb
        prJpsi_mb[0]         = raa_pr;
        prJpsiErr_mb[0]      = raaErr_pr;       
        nonPrJpsi_mb[0]      = raa_npr;
        nonPrJpsiErr_mb[0]      = raaErr_npr;

        prJpsiTrad_mb[0]     = raaTrad_pr;
        prJpsiTradErr_mb[0]  = raaTradErr_pr;       
        nonPrJpsiTrad_mb[0]  = raaTrad_npr;
        nonPrJpsiTradErr_mb[0]  = raaTradErr_npr;


        break;
      }
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++)
  }//loop end: for(int ih=0; ih<nInHist;ih++)

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsi     = new TGraphErrors(nBinsY, binsY, prJpsi_y, binsYErr, prJpsiErr_y);
  TGraphErrors *gPrJpsi_mb  = new TGraphErrors(nBinsMB, binsYMB, prJpsi_mb, binsYMBErr, prJpsiErr_mb);
  TGraphErrors *gPrJpsi_y_y = new TGraphErrors(nBinsY3, binsY3, prJpsi_y_y, binsY3Err, prJpsiErr_y_y);
 
  // nonPr   
  TGraphErrors *gNonPrJpsi    = new TGraphErrors(nBinsY, binsY, nonPrJpsi_y, binsYErr, nonPrJpsiErr_y);
  TGraphErrors *gNonPrJpsi_mb = new TGraphErrors(nBinsMB, binsYMB, nonPrJpsi_mb, binsYMBErr, nonPrJpsiErr_mb);
  TGraphErrors *gNonPrJpsi_y_y= new TGraphErrors(nBinsY3, binsY3, nonPrJpsi_y_y, binsY3Err, nonPrJpsiErr_y_y);

  //------------ trad
// pr
  TGraphErrors *gPrJpsiTrad    = new TGraphErrors(nBinsY, binsY, prJpsiTrad_y, binsYErr, prJpsiTradErr_y);
  TGraphErrors *gPrJpsiTrad_mb = new TGraphErrors(nBinsMB, binsYMB, prJpsiTrad_mb, binsYMBErr, prJpsiTradErr_mb);
  TGraphErrors *gPrJpsiTrad_y_y= new TGraphErrors(nBinsY3, binsY3, prJpsiTrad_y_y, binsY3Err, prJpsiTradErr_y_y);
 
  // nonPr   
  TGraphErrors *gNonPrJpsiTrad    = new TGraphErrors(nBinsY, binsY, nonPrJpsiTrad_y, binsYErr, nonPrJpsiTradErr_y);
  TGraphErrors *gNonPrJpsiTrad_mb = new TGraphErrors(nBinsMB, binsYMB, nonPrJpsiTrad_mb, binsYMBErr, nonPrJpsiTradErr_mb);
  TGraphErrors *gNonPrJpsiTrad_y_y= new TGraphErrors(nBinsY3, binsY3, nonPrJpsiTrad_y_y, binsY3Err, nonPrJpsiTradErr_y_y);

  //-------------------------------------------------------------------
  // **************** marker colors
  gPrJpsi->SetMarkerColor(kRed);
  gNonPrJpsi->SetMarkerColor(kOrange+2);

  //mnbias colors
  gPrJpsi_mb->SetMarkerColor(kCyan+2);
  gNonPrJpsi_mb->SetMarkerColor(kCyan+2);

  gPrJpsi_y_y->SetMarkerColor(kBlue-4);
  gNonPrJpsi_y_y->SetMarkerColor(kBlue-4);

  //--------- marker style  
  gPrJpsi->SetMarkerStyle(21);
  gPrJpsi_y_y->SetMarkerStyle(34);
  gNonPrJpsi->SetMarkerStyle(29);
  gNonPrJpsi_y_y->SetMarkerStyle(34);

  //mb
  gPrJpsi_mb->SetMarkerStyle(33);
  gNonPrJpsi_mb->SetMarkerStyle(33);
  
  // marker size
  gPrJpsi->SetMarkerSize(1.2);
  gPrJpsi_y_y->SetMarkerSize(1.7);
  gNonPrJpsi->SetMarkerSize(1.7);
  gNonPrJpsi_y_y->SetMarkerSize(1.7);

  //mb
  gPrJpsi_mb->SetMarkerSize(1.5);
  gNonPrJpsi_mb->SetMarkerSize(1.5);


  //--------------- traditional stuff
  gPrJpsiTrad->SetMarkerStyle(25);
  gPrJpsiTrad_y_y->SetMarkerStyle(28);
  gNonPrJpsiTrad->SetMarkerStyle(30);
  gNonPrJpsiTrad_y_y->SetMarkerStyle(28);

  //mb
  gPrJpsiTrad_mb->SetMarkerStyle(27);
  gNonPrJpsiTrad_mb->SetMarkerStyle(27);
  
  // marker size
  gPrJpsiTrad->SetMarkerSize(1.2);
  gPrJpsiTrad_y_y->SetMarkerSize(1.7);
  gNonPrJpsiTrad->SetMarkerSize(1.7);
  gNonPrJpsiTrad_y_y->SetMarkerSize(1.7);

  //mb
  gPrJpsiTrad_mb->SetMarkerSize(1.5);
  gNonPrJpsiTrad_mb->SetMarkerSize(1.5);

 // same for the raa denominator and nominator yields histograms
  for(int ih=0; ih<nInHist;ih++) // for each kinematic range
  {
    ahRatio_pr_pp[ih]->SetMarkerStyle(20);
    ahRatio_npr_pp[ih]->SetMarkerStyle(20);
    ahRatio_pr_aa[ih]->SetMarkerStyle(20);
    ahRatio_npr_aa[ih]->SetMarkerStyle(20);
    if(ih==2)
      {
        ahRatio_pr_pp[ih]->SetMarkerColor(kCyan+2);
        ahRatio_npr_pp[ih]->SetMarkerColor(kCyan+2);
        ahRatio_pr_aa[ih]->SetMarkerColor(kCyan+2);
        ahRatio_npr_aa[ih]->SetMarkerColor(kCyan+2);
      }
  }


  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,2.4);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("|y|");
  f4->GetXaxis()->SetNdivisions(-6);
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  //---------------- general stuff
  TLatex *lcent = new TLatex(1.1,1.03,"Cent. 0-100%");
  lcent->SetTextFont(42);
  lcent->SetTextSize(0.05);

  TLatex *lpt = new TLatex(1.1,0.9,"6.5 < p_{T} < 30 GeV/c");
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.05);

  TLatex *lPr = new TLatex(0.2,1.35,"Prompt J/#psi");
  lPr->SetTextFont(42);
  lPr->SetTextSize(0.05);

  TLatex *lNpr = new TLatex(0.2,1.35,"Non-prompt J/#psi");
  lNpr->SetTextFont(42);
  lNpr->SetTextSize(0.05);

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



  // ##################################################### pr plots
  TCanvas *c1 = new TCanvas("c1","c1",1200,400);
  c1->Divide(3,1);
  c1->cd(1);
  f4->Draw();// axis
  lPr->Draw();
  lcent->Draw();
  lpt->Draw();

  gPrJpsi->Draw("P");
  gPrJpsiTrad->Draw("P");

  c1->cd(2);
  fBin->Draw();// axis
  gPad->SetGridy();
  lPP->Draw();
  lRatio->Draw();

  ahRatio_pr_pp[0]->Draw("sames");
  
  c1->cd(3);
  fBin->Draw();// axis
  gPad->SetGridy();
  lAA->Draw();
  lRatio->Draw();

  ahRatio_pr_aa[0]->Draw("sames");

  if(bSavePlots)
  {
    c1->SaveAs(Form("%s/pdf/PrJpsi_vsY_%s.pdf",outputDir,compWhat[whichCompare]));
    c1->SaveAs(Form("%s/png/PrJpsi_vsY_%s.png",outputDir,compWhat[whichCompare]));
  }
  
  //-------------------minbias dependence
  TCanvas *c11b = new TCanvas("c11b","c11b",1200,400);
  c11b->Divide(3,1);
  c11b->cd(1);
  f4->Draw();

  lPr->Draw();
  lcent->Draw();
  lpt->Draw();

  gPrJpsi_mb->Draw("P");
  gPrJpsi_y_y->Draw("P");

  gPrJpsiTrad_mb->Draw("P");
  gPrJpsiTrad_y_y->Draw("P");
 
  c11b->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  lPP->Draw();
  lRatio->Draw();

  ahRatio_pr_pp[1]->Draw("sames");
  ahRatio_pr_pp[2]->Draw("sames");

  c11b->cd(3);
  fBin->Draw();
  gPad->SetGridy();
  lAA->Draw();
  lRatio->Draw();
  ahRatio_pr_aa[1]->Draw("sames");
  ahRatio_pr_aa[2]->Draw("sames");

  gPad->RedrawAxis();

  if(bSavePlots)
  {
    c11b->SaveAs(Form("%s/pdf/PrJpsi_vsY_mb_%s.pdf",outputDir,compWhat[whichCompare]));
    c11b->SaveAs(Form("%s/png/PrJpsi_vsY_mb_%s.png",outputDir,compWhat[whichCompare]));
  }

  //  // ############################################## non-pr   
  //  // ############################################## non-pr 
  //  // ############################################## non-pr
  TCanvas *c2 = new TCanvas("c2","c2",1200,400);
  c2->Divide(3,1);
  c2->cd(1);
  f4->Draw();// axis
  lNpr->Draw();
  lcent->Draw();
  lpt->Draw();

  gNonPrJpsi->Draw("P");
  gNonPrJpsiTrad->Draw("P");

  c2->cd(2);
  fBin->Draw();// axis
  gPad->SetGridy();
  lPP->Draw();
  lRatio->Draw();

  ahRatio_npr_pp[0]->Draw("sames");
  
  c2->cd(3);
  fBin->Draw();// axis
  gPad->SetGridy();
  lAA->Draw();
  lRatio->Draw();

  ahRatio_npr_aa[0]->Draw("sames");

  if(bSavePlots)
  {
    c2->SaveAs(Form("%s/pdf/NonPrJpsi_vsY_%s.pdf",outputDir,compWhat[whichCompare]));
    c2->SaveAs(Form("%s/png/NonPrJpsi_vsY_%s.png",outputDir,compWhat[whichCompare]));
  }
  //-------------------minbias dependence
  TCanvas *c22b = new TCanvas("c22b","c22b",1200,400);
  c22b->Divide(3,1);
  c22b->cd(1);
  f4->Draw();

  lNpr->Draw();
  lcent->Draw();
  lpt->Draw();

  gNonPrJpsi_mb->Draw("P");
  gNonPrJpsi_y_y->Draw("P");

  gNonPrJpsiTrad_mb->Draw("P");
  gNonPrJpsiTrad_y_y->Draw("P");
 
  c22b->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  lPP->Draw();
  lRatio->Draw();

  ahRatio_npr_pp[1]->Draw("sames");
  ahRatio_npr_pp[2]->Draw("sames");

  c22b->cd(3);
  fBin->Draw();
  gPad->SetGridy();
  lAA->Draw();
  lRatio->Draw();
  ahRatio_npr_aa[1]->Draw("sames");
  ahRatio_npr_aa[2]->Draw("sames");

  gPad->RedrawAxis();

  if(bSavePlots)
  {
    c11b->SaveAs(Form("%s/pdf/NonPrJpsi_vsY_mb_%s.pdf",outputDir,compWhat[whichCompare]));
    c11b->SaveAs(Form("%s/png/NonPrJpsi_vsY_mb_%s.png",outputDir,compWhat[whichCompare]));
  }

  
}
