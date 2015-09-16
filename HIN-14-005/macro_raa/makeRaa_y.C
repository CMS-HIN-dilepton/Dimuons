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

void makeRaa_y(bool bSavePlots=1,
	       bool bDoDebug = 0, // adds some numbers, numerator, denominator, to help figure out if things are read properly
	       bool bAddLumi = 0, // add the lumi boxes at raa=1
	       int  whichSample     = 1,//0: no TnP corrections; 1: w/ TnP corr on Data; 2: w/ TnP corr on MC; 3: lxy w/ TnP on MC
	       const char* inputDir="../readFitTable", // the place where the input root files, with the histograms are
	       const char* outputDir="figs")// where the output figures will be
{
  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);

  // set the style
  setTDRStyle();
 
  // type of available comparisons:
  const char* sample[4] = {"noTnP","dataTnP","mcTnP","lxyTnP"};

  const int nInHist = 3;
  const char* yieldHistNames[nInHist] = {"y", "y_mb", "mb"};

  // input files: are in the filesRaa_2015.h

  // open the files with yields and do the math
  TFile *fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[0]));
  TFile *fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[1]));
  
  TFile *fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[0]));
  TFile *fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[1]));

  TFile *fEffFile_aa = new TFile(Form("%s/%s",inputDir,effHistFile[0]));
  TFile *fEffFile_pp = new TFile(Form("%s/%s",inputDir,effHistFile[1]));

 switch(whichSample){
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
  
    int numBins = 0;
    if(ih==0) numBins = nBinsY;
    if(ih==1) numBins = nBinsY3;
    if(ih==2) numBins = nBinsMB;

    cout << "Number of bins: " << numBins<<endl;

    for(int ibin=1; ibin<=numBins; ibin++)
    {
      double raa_pr=0, raaErr_pr=0, raa_npr=0, raaErr_npr=0;

      //prompt
      double dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
      double dRelErrRaw_pr_aa  = phRaw_pr_aa->GetBinError(ibin)/phRaw_pr_aa->GetBinContent(ibin);
      double yieldRatio_pr     = phCorr_pr_aa->GetBinContent(ibin)/phCorr_pr_pp->GetBinContent(ibin);
  
      raa_pr      =  yieldRatio_pr * scaleFactor * scale_cent;
      raaErr_pr   = TMath::Sqrt(TMath::Power(dRelErrRaw_pr_pp,2)+TMath::Power(dRelErrRaw_pr_aa,2))*raa_pr;

      //non-prompt
      // get the rel uncert from the raw sample
      double dRelErrRaw_npr_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);
      double dRelErrRaw_npr_aa  = phRaw_npr_aa->GetBinError(ibin)/phRaw_npr_aa->GetBinContent(ibin);
      double yieldRatio_npr     = phCorr_npr_aa->GetBinContent(ibin)/phCorr_npr_pp->GetBinContent(ibin);
   
      raa_npr    = yieldRatio_npr * scaleFactor * scale_cent;
      raaErr_npr = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raa_npr;
     
      // fill the corresponding array
      switch(ih){
      case 0:
        prJpsi_y[ibin-1]    = raa_pr;
        prJpsiErr_y[ibin-1] = raaErr_pr;
              
        nonPrJpsi_y[ibin-1]    = raa_npr;
        nonPrJpsiErr_y[ibin-1] = raaErr_npr;

        if(bDoDebug)
        {
	  cout<<"yield_pr_aa "<<phCorr_pr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_pr_pp->GetBinContent(ibin)<<endl;
          cout<<"yield_npr_aa "<<phCorr_npr_aa->GetBinContent(ibin)<<"\t yield_npr_pp "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;
	  cout<<setprecision(2);
          cout<<"!!!!! raa_pr = "<<raa_pr<<"\t raa_npr= "<<raa_npr<<endl;
        }
        break;
   
      case 1:
        prJpsi_y_y[ibin-1]        = raa_pr;
        prJpsiErr_y_y[ibin-1]     = raaErr_pr;

        nonPrJpsi_y_y[ibin-1]     = raa_npr;
        nonPrJpsiErr_y_y[ibin-1]  = raaErr_npr;

        if(bDoDebug)
        {
	  cout<<"yield_npr_aa "<<phCorr_npr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;
	  cout<<setprecision(2);
          cout<<"!!!!! raa_pr = "<<raa_pr<<"\t raa_npr= "<<raa_npr<<endl;
        }
        break;

      case 2:
        // mb
        prJpsi_mb[0]     = raa_pr;
        prJpsiErr_mb[0]  = raaErr_pr;
        
        nonPrJpsi_mb[0]     = raa_npr;
        nonPrJpsiErr_mb[0]  = raaErr_npr;
        break;
      }
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++)
  }//loop end: for(int ih=0; ih<nInHist;ih++)

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsi     = new TGraphErrors(nBinsY, binsY, prJpsi_y, binsYErr, prJpsiErr_y);
  TGraphErrors *gPrJpsiP    = new TGraphErrors(nBinsY, binsY, prJpsi_y, binsYErr, prJpsiErr_y);
  TGraphErrors *gPrJpsiSyst = new TGraphErrors(nBinsY, binsY, prJpsi_y, binsYX, prJpsiErrSyst_y);

  TGraphErrors *gPrJpsi_mb     = new TGraphErrors(nBinsMB, binsYMB, prJpsi_mb, binsYMBErr, prJpsiErr_mb);
  TGraphErrors *gPrJpsiP_mb    = new TGraphErrors(nBinsMB, binsYMB, prJpsi_mb, binsYMBErr, prJpsiErr_mb);
  TGraphErrors *gPrJpsiSyst_mb = new TGraphErrors(nBinsMB, binsYMB, prJpsi_mb, binsYMBX  , prJpsiErrSyst_mb);
 
  TGraphErrors *gPrJpsi_y_y     = new TGraphErrors(nBinsY3, binsY3, prJpsi_y_y, binsY3Err, prJpsiErr_y_y);
  TGraphErrors *gPrJpsiP_y_y    = new TGraphErrors(nBinsY3, binsY3, prJpsi_y_y, binsY3Err, prJpsiErr_y_y);
  TGraphErrors *gPrJpsiSyst_y_y = new TGraphErrors(nBinsY3, binsY3, prJpsi_y_y, binsY3X,   prJpsiErrSyst_y_y);
 
  // nonPr   
  TGraphErrors *gNonPrJpsi     = new TGraphErrors(nBinsY, binsY, nonPrJpsi_y, binsYErr, nonPrJpsiErr_y);
  TGraphErrors *gNonPrJpsiP    = new TGraphErrors(nBinsY, binsY, nonPrJpsi_y, binsYErr, nonPrJpsiErr_y);
  TGraphErrors *gNonPrJpsiSyst = new TGraphErrors(nBinsY, binsY, nonPrJpsi_y, binsYX, nonPrJpsiErrSyst_y);

  TGraphErrors *gNonPrJpsi_mb     = new TGraphErrors(nBinsMB, binsYMB, nonPrJpsi_mb, binsYMBErr, nonPrJpsiErr_mb);
  TGraphErrors *gNonPrJpsiP_mb    = new TGraphErrors(nBinsMB, binsYMB, nonPrJpsi_mb, binsYMBErr, nonPrJpsiErr_mb);
  TGraphErrors *gNonPrJpsiSyst_mb = new TGraphErrors(nBinsMB, binsYMB, nonPrJpsi_mb, binsYMBX, nonPrJpsiErrSyst_mb);


  TGraphErrors *gNonPrJpsi_y_y     = new TGraphErrors(nBinsY3, binsY3, nonPrJpsi_y_y, binsY3Err, nonPrJpsiErr_y_y);
  TGraphErrors *gNonPrJpsiP_y_y    = new TGraphErrors(nBinsY3, binsY3, nonPrJpsi_y_y, binsY3Err, nonPrJpsiErr_y_y);
  TGraphErrors *gNonPrJpsiSyst_y_y = new TGraphErrors(nBinsY3, binsY3, nonPrJpsi_y_y, binsY3X,   nonPrJpsiErrSyst_y_y);

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
  // pr
  gPrJpsi->SetMarkerStyle(21);
  gPrJpsiP->SetMarkerStyle(25);

  gPrJpsi_y_y->SetMarkerStyle(34);
  gPrJpsiP_y_y->SetMarkerStyle(28);

  // non-pr
  gNonPrJpsi->SetMarkerStyle(29);
  gNonPrJpsiP->SetMarkerStyle(30);
  
  gNonPrJpsi_y_y->SetMarkerStyle(34);
  gNonPrJpsiP_y_y->SetMarkerStyle(28);

  //mb
  gPrJpsi_mb->SetMarkerStyle(28);
  gNonPrJpsi_mb->SetMarkerStyle(28);
  
  // ************** contour
   // pr
  gPrJpsiP->SetMarkerColor(kBlack);
  gPrJpsiP_mb->SetMarkerColor(kBlack);

  // nonPr
  gNonPrJpsiP->SetMarkerColor(kBlack);
  gNonPrJpsiP_mb->SetMarkerColor(kBlack);


  // marker size
  // pr
  gPrJpsi->SetMarkerSize(1.2);
  gPrJpsiP->SetMarkerSize(1.2);

  gPrJpsiP_y_y->SetMarkerSize(1.7);
  gPrJpsi_y_y->SetMarkerSize(1.7);

  gPrJpsi_mb->SetMarkerSize(1.5);
  gNonPrJpsi_mb->SetMarkerSize(1.5);

  // nonPr
  gNonPrJpsi->SetMarkerSize(1.7);
  gNonPrJpsiP->SetMarkerSize(1.7);
  
  gNonPrJpsiP_y_y->SetMarkerSize(1.7);
  gNonPrJpsi_y_y->SetMarkerSize(1.7);

  //stat boxes
  gPrJpsiSyst->SetFillColor(kRed-9);
  gPrJpsiSyst_y_y->SetFillColor(kViolet-9);

  // non-pr
  gNonPrJpsiSyst->SetFillColor(kOrange-9);
  gNonPrJpsiSyst_y_y->SetFillColor(kViolet-9);

  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,2.4);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("|y|");
  f4->GetXaxis()->SetNdivisions(-6);
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

 // sqrt(sig_lumi(6%)*sig_lumi(6%)+sig_taa(5.7%)*sig_taa(5.7%)) = 0.083
  TBox *lumi = new TBox(0.0,0.917,1.,1.083);
  lumi->SetFillColor(kGray+1);


  //---------------- general stuff
  TLatex *lPr = new TLatex(0.2,1.35,"Prompt J/#psi");
  lPr->SetTextFont(42);
  lPr->SetTextSize(0.05);

  TLatex *lNpr = new TLatex(0.2,1.35,"Non-prompt J/#psi");
  lNpr->SetTextFont(42);
  lNpr->SetTextSize(0.05);

  TLatex *lpt = new TLatex(0.12,0.075,"6.5 < p_{T} < 30 GeV/c");
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.05);
  TLatex *lcent = new TLatex(19,1.03,"Cent. 0-100%");
  lcent->SetTextFont(42);
  lcent->SetTextSize(0.05);


  // ##################################################### pr plots
  TCanvas *c1 = new TCanvas("c1","c1");
  // general stuff
  f4->Draw();// axis
  if(bAddLumi) 
  {
    lumi->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c1,103,33);
  lPr->Draw();
  lcent->Draw();
  lpt->Draw();

  gPrJpsiSyst->Draw("2");
  gPrJpsi->Draw("P");
  gPrJpsiP->Draw("P");

  if(bSavePlots)
  {
    c1->SaveAs(Form("%s/pdf/PrJpsi_vsY_%s.pdf",outputDir,sample[whichSample]));
    c1->SaveAs(Form("%s/png/PrJpsi_vsY_%s.png",outputDir,sample[whichSample]));
  }
  
  //-------------------minbias dependence
  TCanvas *c11b = new TCanvas("c11b","c11b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi->Draw();
    f4->Draw("same");
  }
 
  lPr->Draw();
  lcent->Draw();
  lpt->Draw();
  CMS_lumi(c11b,103,33);

  gPrJpsiSyst_mb->Draw("2");
  gPrJpsi_mb->Draw("P");
 
  gPrJpsiSyst_y_y->Draw("2");
  gPrJpsi_y_y->Draw("P");
 
  gPad->RedrawAxis();

  if(bSavePlots)
  {
    c11b->SaveAs(Form("%s/pdf/PrJpsi_vsY_mb_%s.pdf",outputDir,sample[whichSample]));
    c11b->SaveAs(Form("%s/png/PrJpsi_vsY_mb_%s.png",outputDir,sample[whichSample]));
  }

  
  // ############################################## non-pr 
  // ############################################## non-pr
  TCanvas *c2 = new TCanvas("c2","c2");
  // general stuff
  f4->Draw();// axis
  if(bAddLumi) 
  {
    lumi->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c2,103,33);
  lNpr->Draw();
  lcent->Draw();
  lpt->Draw();

  gNonPrJpsiSyst->Draw("2");
  gNonPrJpsi->Draw("P");
  gNonPrJpsiP->Draw("P");

  if(bSavePlots)
  {
    c2->SaveAs(Form("%s/pdf/nonPrJpsi_vsY_%s.pdf",outputDir,sample[whichSample]));
    c2->SaveAs(Form("%s/png/nonPrJpsi_vsY_%s.png",outputDir,sample[whichSample]));
  }
  
  //-------------------minbias dependence
  TCanvas *c22b = new TCanvas("c22b","c22b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c22b,103,33);
  lNpr->Draw();
  lcent->Draw();
  lpt->Draw();

  gNonPrJpsiSyst_mb->Draw("2");
  gNonPrJpsi_mb->Draw("P");
 
  gNonPrJpsiSyst_y_y->Draw("2");
  gNonPrJpsi_y_y->Draw("P");

  gPad->RedrawAxis();

  if(bSavePlots)
  {
    c22b->SaveAs(Form("%s/pdf/nonPrJpsi_vsY_mb_%s.pdf",outputDir,sample[whichSample]));
    c22b->SaveAs(Form("%s/png/nonPrJpsi_vsY_mb_%s.png",outputDir,sample[whichSample]));
  }

  
}
