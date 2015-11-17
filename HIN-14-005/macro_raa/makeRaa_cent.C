/*
Macro to plot the Efficiency vs centrality, for all the analysis bins.(in the traditional way, yield x correction, and the weighted way)

Input: root files produced by the readFitTable/makeHisto_raa.C (check there the input file names)

Output: the Raa vs cent.

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

using namespace std;

void makeRaa_cent( bool bSavePlots=1,
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

  const int nInHist = 5;
  const char* yieldHistNames[nInHist] = {"cent","y012Cent", "y1216Cent", "y1624Cent", "y1624LowPtCent"};

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

    int numBins = 0;
    if(ih==0) numBins = nBinsNpart12;
    if(ih==4) numBins = nBinsNpart6;
    if(ih==1 || ih==2 || ih==3) numBins = nBinsNpart6;

    for(int ibin=1; ibin<=numBins; ibin++)
    {
      double raa_pr=0, raaErr_pr=0, raa_npr=0, raaErr_npr=0;
      double scale_cent = 1;
      double scale_cent_np = 1;

      if(ih==0)
      {
        scale_cent = 1/(adTaa12[ibin-1]*adDeltaCent12[ibin-1]);
        scale_cent_np = 1/(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
      }
      if(ih==4)          scale_cent = 1/(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
      if(ih!=0 && ih!=4) scale_cent = 1/(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
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
    
      if(ih==0)raa_npr= yieldRatio_npr * scaleFactor * scale_cent_np;// the 1D nonPr has 6 bins only
      else raa_npr    = yieldRatio_npr * scaleFactor * scale_cent;
      raaErr_npr = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raa_npr;

     
      // fill the corresponding array
      switch(ih){
      case 0:
        prJpsi_cent[ibin-1]    = raa_pr;
        prJpsiErr_cent[ibin-1] = raaErr_pr;
        if(bDoDebug)
        {
          cout<<"weight_pr_aa = "<<phEff_pr_aa->GetBinContent(ibin)<<"\t weight_pr_pp = "<<phEff_pr_pp->GetBinContent(ibin)<<endl;
          cout<<"yield_pr_aa "<<phCorr_pr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_pr_pp->GetBinContent(ibin)<<endl;
        
          cout<<"pr_aa= "<<phRaw_pr_aa->GetBinContent(ibin)/phEff_pr_aa->GetBinContent(ibin)<<"\t pr_pp= "<<phRaw_pr_pp->GetBinContent(ibin)/phEff_pr_pp->GetBinContent(ibin)<<endl;
	  cout<<setprecision(2);
          cout<<"!!!!! raa = "<<prJpsi_cent[ibin-1]<<endl;
          
          // cout<<"Scale_Cent= "<<scale_cent<<endl;
        }

        nonPrJpsi_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_cent[ibin-1] = raaErr_npr;
        break;
        
      case 1:
        prJpsi_pt6530y012_cent[ibin-1]        = raa_pr;
        prJpsiErr_pt6530y012_cent[ibin-1]     = raaErr_pr;

        nonPrJpsi_pt6530y012_cent[ibin-1]     = raa_npr;
        nonPrJpsiErr_pt6530y012_cent[ibin-1]  = raaErr_npr;
        break;
        
      case 2:
        prJpsi_pt6530y1216_cent[ibin-1]       = raa_pr;
        prJpsiErr_pt6530y1216_cent[ibin-1]    = raaErr_pr;

        nonPrJpsi_pt6530y1216_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_pt6530y1216_cent[ibin-1] = raaErr_npr;
        break;
        
      case 3:
        prJpsi_pt6530y1624_cent[ibin-1]       = raa_pr;
        prJpsiErr_cent[ibin-1]                = raaErr_pr;

        nonPrJpsi_pt6530y1624_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_pt6530y1624_cent[ibin-1] = raaErr_npr;
        break;
       
      case 4:
        prJpsi_pt365y1624_cent[ibin-1]        = raa_pr;
        prJpsiErr_pt365y1624_cent[ibin-1]     = raaErr_pr;

        nonPrJpsi_pt365y1624_cent[ibin-1]     = raa_npr;
        nonPrJpsiErr_pt365y1624_cent[ibin-1]  = raaErr_npr;
        break;
      }
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++)
  
  }//loop end: for(int ih=0; ih<nInHist;ih++)

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsi     = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsi_cent, binsNpart12Err, prJpsiErr_cent);
  TGraphErrors *gPrJpsiP    = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsi_cent, binsNpart12Err, prJpsiErr_cent);
  TGraphErrors *gPrJpsiSyst = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsi_cent, binsNpart12X, prJpsiErrSyst_cent);
 
 
  TGraphErrors *gPrJpsi_pt6530y012     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsi_pt6530y012_cent, binsNpart6Err, prJpsiErr_pt6530y012_cent);
  TGraphErrors *gPrJpsiP_pt6530y012    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsi_pt6530y012_cent, binsNpart6Err, prJpsiErr_pt6530y012_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y012 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsi_pt6530y012_cent, binsNpart6X, prJpsiErrSyst_pt6530y012_cent);


  TGraphErrors *gPrJpsi_pt6530y1216     = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt6530y1216_cent, binsNpart6Err, prJpsiErr_pt6530y1216_cent);
  TGraphErrors *gPrJpsiP_pt6530y1216    = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt6530y1216_cent, binsNpart6Err, prJpsiErr_pt6530y1216_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y1216 = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt6530y1216_cent, binsNpart6X, prJpsiErrSyst_pt6530y1216_cent);
 

  TGraphErrors *gPrJpsi_pt6530y1624     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsi_pt6530y1624_cent, binsNpart6Err, prJpsiErr_pt6530y1624_cent);
  TGraphErrors *gPrJpsiP_pt6530y1624    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsi_pt6530y1624_cent, binsNpart6Err, prJpsiErr_pt6530y1624_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y1624 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsi_pt6530y1624_cent, binsNpart6X, prJpsiErrSyst_pt6530y1624_cent);


  TGraphErrors *gPrJpsi_pt365y1624     = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt365y1624_cent, binsNpart6Err, prJpsiErr_pt365y1624_cent);
  TGraphErrors *gPrJpsiP_pt365y1624    = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt365y1624_cent, binsNpart6Err, prJpsiErr_pt365y1624_cent);
  TGraphErrors *gPrJpsiSyst_pt365y1624 = new TGraphErrors(nBinsNpart6, binsNpart6,    prJpsi_pt365y1624_cent, binsNpart6X, prJpsiErrSyst_pt365y1624_cent);

 
  // nonPr
  TGraphErrors *gNonPrJpsi     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_cent, binsNpart6Err, nonPrJpsiErr_cent);
  TGraphErrors *gNonPrJpsiP    = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_cent, binsNpart6Err, nonPrJpsiErr_cent);
  TGraphErrors *gNonPrJpsiSyst = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_cent, binsNpart6X, nonPrJpsiErrSyst_cent);


  TGraphErrors *gNonPrJpsi_pt6530y012     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus,nonPrJpsi_pt6530y012_cent, binsNpart6Err, nonPrJpsiErr_pt6530y012_cent);
  TGraphErrors *gNonPrJpsiP_pt6530y012    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus,nonPrJpsi_pt6530y012_cent, binsNpart6Err, nonPrJpsiErr_pt6530y012_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y012 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, nonPrJpsi_pt6530y012_cent, binsNpart6X,   nonPrJpsiErrSyst_pt6530y012_cent);
  
  TGraphErrors *gNonPrJpsi_pt6530y1216     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt6530y1216_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1216_cent);
  TGraphErrors *gNonPrJpsiP_pt6530y1216    = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt6530y1216_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1216_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y1216 = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt6530y1216_cent, binsNpart6X,    nonPrJpsiErrSyst_pt6530y1216_cent);
 

  TGraphErrors *gNonPrJpsi_pt6530y1624     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsi_pt6530y1624_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1624_cent);
  TGraphErrors *gNonPrJpsiP_pt6530y1624    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsi_pt6530y1624_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1624_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y1624 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsi_pt6530y1624_cent, binsNpart6X,    nonPrJpsiErrSyst_pt6530y1624_cent);


  TGraphErrors *gNonPrJpsi_pt365y1624     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt365y1624_cent, binsNpart6Err, nonPrJpsiErr_pt365y1624_cent);
  TGraphErrors *gNonPrJpsiP_pt365y1624    = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt365y1624_cent, binsNpart6Err, nonPrJpsiErr_pt365y1624_cent);
  TGraphErrors *gNonPrJpsiSyst_pt365y1624 = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt365y1624_cent, binsNpart6X, nonPrJpsiErrSyst_pt365y1624_cent);


  //-------------------------------------------------------------------
  // **************** marker colors
  //prompt
  gPrJpsi->SetMarkerColor(kRed);
  gPrJpsi_pt6530y012->SetMarkerColor(kAzure+7);
  gPrJpsi_pt6530y1216->SetMarkerColor(kRed);
  gPrJpsi_pt6530y1624->SetMarkerColor(kGreen+2);   
  gPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);
  
  // non-prompt
  gNonPrJpsi->SetMarkerColor(kOrange+2);
  gNonPrJpsi_pt6530y012->SetMarkerColor(kAzure+7);
  gNonPrJpsi_pt6530y1216->SetMarkerColor(kRed);
  gNonPrJpsi_pt6530y1624->SetMarkerColor(kGreen+2);  
  gNonPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);
  
  //--------- marker style  
  // pr
  gPrJpsi->SetMarkerStyle(21);
  gPrJpsiP->SetMarkerStyle(25);

  gPrJpsi_pt6530y012->SetMarkerStyle(20);
  gPrJpsi_pt6530y1216->SetMarkerStyle(21);
  gPrJpsi_pt6530y1624->SetMarkerStyle(33);
  gPrJpsi_pt365y1624->SetMarkerStyle(34);

  gPrJpsiP_pt6530y012->SetMarkerStyle(24);
  gPrJpsiP_pt6530y1216->SetMarkerStyle(25);
  gPrJpsiP_pt6530y1624->SetMarkerStyle(27);
  gPrJpsiP_pt365y1624->SetMarkerStyle(28);

  // non-pr
  gNonPrJpsi->SetMarkerStyle(29);
  gNonPrJpsiP->SetMarkerStyle(30);
  
  gNonPrJpsi_pt6530y012->SetMarkerStyle(20);
  gNonPrJpsi_pt6530y1216->SetMarkerStyle(21);
  gNonPrJpsi_pt6530y1624->SetMarkerStyle(33);
  gNonPrJpsi_pt365y1624->SetMarkerStyle(34);

  gNonPrJpsiP_pt6530y012->SetMarkerStyle(24);
  gNonPrJpsiP_pt6530y1216->SetMarkerStyle(25);
  gNonPrJpsiP_pt6530y1624->SetMarkerStyle(27);
  gNonPrJpsiP_pt365y1624->SetMarkerStyle(28);


  // ************** contour
  // pr
  gPrJpsiP->SetMarkerColor(kBlack);
  gPrJpsiP_pt6530y012->SetMarkerColor(kBlack);
  gPrJpsiP_pt6530y1216->SetMarkerColor(kBlack);
  gPrJpsiP_pt6530y1624->SetMarkerColor(kBlack);
  gPrJpsiP_pt365y1624->SetMarkerColor(kBlack);

  // nonPr
  gNonPrJpsiP->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt6530y012->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt6530y1216->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt6530y1624->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt365y1624->SetMarkerColor(kBlack);

  // marker size
  // pr
  gPrJpsi->SetMarkerSize(1.2);
  gPrJpsiP->SetMarkerSize(1.2);

  gPrJpsiP_pt6530y012->SetMarkerSize(1.2);
  gPrJpsiP_pt6530y1216->SetMarkerSize(1.2);
  gPrJpsiP_pt6530y1624->SetMarkerSize(2.0);
  gPrJpsiP_pt365y1624->SetMarkerSize(1.7);

  gPrJpsi_pt6530y012->SetMarkerSize(1.2);
  gPrJpsi_pt6530y1216->SetMarkerSize(1.2);
  gPrJpsi_pt6530y1624->SetMarkerSize(2.0);
  gPrJpsi_pt365y1624->SetMarkerSize(1.7);

  // nonPr
  gNonPrJpsi->SetMarkerSize(2.0);
  gNonPrJpsiP->SetMarkerSize(2.0);
  
  gNonPrJpsiP_pt6530y012->SetMarkerSize(1.2);
  gNonPrJpsiP_pt6530y1216->SetMarkerSize(1.2);
  gNonPrJpsiP_pt6530y1624->SetMarkerSize(2.0);
  gNonPrJpsiP_pt365y1624->SetMarkerSize(1.7);

  gNonPrJpsi_pt6530y012->SetMarkerSize(1.2);
  gNonPrJpsi_pt6530y1216->SetMarkerSize(1.2);
  gNonPrJpsi_pt6530y1624->SetMarkerSize(2.0);
  gNonPrJpsi_pt365y1624->SetMarkerSize(1.7);

  //stat boxes
  gPrJpsiSyst->SetFillColor(kRed-9);
 
  gPrJpsiSyst_pt6530y012->SetFillColor(kAzure-9);
  gPrJpsiSyst_pt6530y1216->SetFillColor(kRed-9);
  gPrJpsiSyst_pt6530y1624->SetFillColor(kGreen-9); 
  gPrJpsiSyst_pt365y1624->SetFillColor(kViolet-9);

  // non-pr
  gNonPrJpsiSyst->SetFillColor(kOrange-9);
  gNonPrJpsiSyst_pt6530y012->SetFillColor(kAzure-9);
  gNonPrJpsiSyst_pt6530y1216->SetFillColor(kRed-9);
  gNonPrJpsiSyst_pt6530y1624->SetFillColor(kGreen-9);
  gNonPrJpsiSyst_pt365y1624->SetFillColor(kViolet-9);

  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,400);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  // just pp lumi here, 6%
  TBox *lumi = new TBox(0.0,0.94,10.0,1.06);
  lumi->SetFillColor(kGray+1);

  // pp lumi + pp stat+pp syst
  TBox *lumi_pr_y024_pt6530  = new TBox(0,0.93,10.0,1.07);
  TBox *lumi_pr_y012_pt6530  = new TBox(390.0,0.91,400.0,1.09);
  TBox *lumi_pr_y1216_pt6530 = new TBox(380.0,0.89,390.0,1.11);
  TBox *lumi_pr_y1624_pt6530 = new TBox(370.0,0.91,380.0,1.09);
  TBox *lumi_pr_y1624_pt6530_pty = new TBox(390.0,0.91,400.0,1.09);
  TBox *lumi_pr_y1624_pt365      = new TBox(380.0,0.92,390.0,1.08);

  lumi_pr_y024_pt6530->SetFillColor(kGray+1);
  lumi_pr_y012_pt6530->SetFillColor(kAzure-9);
  lumi_pr_y1216_pt6530->SetFillColor(kRed-9);
  lumi_pr_y1624_pt6530->SetFillColor(kGreen-9);
  lumi_pr_y1624_pt6530_pty->SetFillColor(kGreen-9);
  lumi_pr_y1624_pt365->SetFillColor(kViolet-9);

  TBox *lumi_npr_y024_pt6530  = new TBox(0,0.93,10.0,1.07);
  TBox *lumi_npr_y012_pt6530  = new TBox(390.0,0.91,400.0,1.09);
  TBox *lumi_npr_y1216_pt6530 = new TBox(380.0,0.89,390.0,1.11);
  TBox *lumi_npr_y1624_pt6530 = new TBox(370.0,0.91,380.0,1.09);
  TBox *lumi_npr_y1624_pt6530_pty = new TBox(390.0,0.91,400.0,1.09);
  TBox *lumi_npr_y1624_pt365      = new TBox(380.0,0.92,390.0,1.08);

  lumi_npr_y024_pt6530->SetFillColor(kGray+1);
  lumi_npr_y012_pt6530->SetFillColor(kAzure-9);
  lumi_npr_y1216_pt6530->SetFillColor(kRed-9);
  lumi_npr_y1624_pt6530->SetFillColor(kGreen-9);
  lumi_npr_y1624_pt6530_pty->SetFillColor(kGreen-9);
  lumi_npr_y1624_pt365->SetFillColor(kViolet-9);

  
  TLegend *leg11a = new TLegend(0.6,0.5,0.85,0.65);
  leg11a->SetFillStyle(0);
  leg11a->SetFillColor(0);
  leg11a->SetBorderSize(0);
  leg11a->SetMargin(0.2);
  leg11a->SetTextSize(0.04);

  leg11a->AddEntry(gPrJpsi_pt6530y012,"|y|<1.2","P");
  leg11a->AddEntry(gPrJpsi_pt6530y1216,"1.2<|y|<1.6","P");
  leg11a->AddEntry(gPrJpsi_pt6530y1624,"1.6<|y|<2.4","P");

 TLegend *leg11b = new TLegend(0.5,0.52,0.8,0.65);
  leg11b->SetFillStyle(0);
  leg11b->SetFillColor(0);
  leg11b->SetBorderSize(0);
  leg11b->SetMargin(0.2);
  leg11b->SetTextSize(0.04);
  leg11b->AddEntry(gPrJpsi_pt6530y1624,"6.5<p_{T}<30 GeV/c","P");
  leg11b->AddEntry(gPrJpsi_pt365y1624,"3<p_{T}<6.5 GeV/c","P");

TLegend *leg21a = new TLegend(0.6,0.5,0.85,0.65);
  leg21a->SetFillStyle(0);
  leg21a->SetFillColor(0);
  leg21a->SetBorderSize(0);
  leg21a->SetMargin(0.2);
  leg21a->SetTextSize(0.04);
  leg21a->AddEntry(gNonPrJpsi_pt6530y012,"|y|<1.2","P");
  leg21a->AddEntry(gNonPrJpsi_pt6530y1216,"1.2<|y|<1.6","P");
  leg21a->AddEntry(gNonPrJpsi_pt6530y1624,"1.6<|y|<2.4","P");

 TLegend *leg21b = new TLegend(0.55,0.5,0.8,0.65);
  leg21b->SetFillStyle(0);
  leg21b->SetFillColor(0);
  leg21b->SetBorderSize(0);
  leg21b->SetMargin(0.2);
  leg21b->SetTextSize(0.04);
  leg21b->AddEntry(gNonPrJpsi_pt6530y1624,"6.5<p_{T}<30 GeV/c","P");
  leg21b->AddEntry(gNonPrJpsi_pt365y1624, "3<p_{T}<6.5 GeV/c","P");

  //---------------- general stuff
  TLatex *lPr = new TLatex(20,1.35,"Prompt J/#psi");
  lPr->SetTextFont(42);
  lPr->SetTextSize(0.05);

  TLatex *lNpr = new TLatex(20,1.35,"Non-prompt J/#psi");
  lNpr->SetTextFont(42);
  lNpr->SetTextSize(0.05);

  TLatex *ly = new TLatex(190.0,1.05,"|y| < 2.4");
  ly->SetTextFont(42);
  ly->SetTextSize(0.05);

  TLatex *lyfwd = new TLatex(190.0,1.05,"1.6 < |y| < 2.4");
  lyfwd->SetTextFont(42);
  lyfwd->SetTextSize(0.05);

  TLatex *lpt = new TLatex(190.0,1.05,"6.5 < p_{T} < 30 GeV/c");
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.05);

  TLatex *lpthigh = new TLatex(190.0,0.95,"6.5 < p_{T} < 30 GeV/c");
  lpthigh->SetTextFont(42);
  lpthigh->SetTextSize(0.05);


  // ##################################################### pr plots
  TCanvas *c1 = new TCanvas("c1","c1");
  // general stuff
  f4->Draw();// axis
  if(bAddLumi) 
  {
    lumi_pr_y024_pt6530->Draw();
    f4->Draw("same");
  }
  lPr->Draw();
  ly->Draw();
  lpthigh->Draw();
  CMS_lumi(c1,103,33);
    
  gPrJpsiSyst->Draw("2");
  gPrJpsi->Draw("P");
  gPrJpsiP->Draw("P");

  c1->SetTitle(" ");

  if(bSavePlots)
  {
    c1->SaveAs(Form("%s/pdf/PrJpsi_vsCent_%s.pdf",outputDir,sample[whichSample]));
    c1->SaveAs(Form("%s/png/PrJpsi_vsCent_%s.png",outputDir,sample[whichSample]));
  }
  
  //------------------- (pt, y) dependence
  TCanvas *c11a = new TCanvas("c11a","c11a");
  f4->Draw();
  if(bAddLumi)
  {
    lumi_pr_y012_pt6530->Draw();
    lumi_pr_y1216_pt6530->Draw();
    lumi_pr_y1624_pt6530->Draw();
    f4->Draw("same");
  }

  lPr->Draw();
  lpt->Draw();
  leg11a->Draw();
  CMS_lumi(c11a,103,33);
 
  gPrJpsiSyst_pt6530y012->Draw("2");
  gPrJpsiSyst_pt6530y1216->Draw("2");
  gPrJpsiSyst_pt6530y1624->Draw("2");

  gPrJpsi_pt6530y012->Draw("P");
  gPrJpsi_pt6530y1216->Draw("P");
  gPrJpsi_pt6530y1624->Draw("P");

  gPrJpsiP_pt6530y012->Draw("P");
  gPrJpsiP_pt6530y1216->Draw("P");
  gPrJpsiP_pt6530y1624->Draw("P");
 

  gPad->RedrawAxis();

  TCanvas *c11b = new TCanvas("c11b","c11b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi_pr_y1624_pt6530_pty->Draw();
    lumi_pr_y1624_pt365->Draw();
    f4->Draw("same");
  }
 
  lPr->Draw();
  lyfwd->Draw();
  CMS_lumi(c11b,103,33);
  leg11b->Draw();
  
  gPrJpsiSyst_pt6530y1624->Draw("2");
  gPrJpsiSyst_pt365y1624->Draw("2");
  
  gPrJpsi_pt6530y1624->Draw("P");
  gPrJpsi_pt365y1624->Draw("P");

  gPrJpsiP_pt6530y1624->Draw("P");
  gPrJpsiP_pt365y1624->Draw("P");
  
  gPad->RedrawAxis();

  if(bSavePlots)
  {

    c11a->SaveAs(Form("%s/pdf/PrJpsi_vsCent_yHighPt_%s.pdf",outputDir,sample[whichSample]));
    c11a->SaveAs(Form("%s/png/PrJpsi_vsCent_yHighPt_%s.png",outputDir,sample[whichSample]));

    c11b->SaveAs(Form("%s/pdf/PrJpsi_vsCent_yLowPt_%s.pdf",outputDir,sample[whichSample]));
    c11b->SaveAs(Form("%s/png/PrJpsi_vsCent_yLowPt_%s.png",outputDir,sample[whichSample]));

  }

  //   // ############################################## non-pr
  //   // ############################################## non-pr
  //   // ############################################## non-pr
  TCanvas *c2 = new TCanvas("c2","c2");
  // general stuff
  f4->Draw();
  if(bAddLumi) lumi_npr_y024_pt6530->Draw();
  lNpr->Draw();
  ly->Draw();
  lpthigh->Draw();
  CMS_lumi(c2,103,33);

  gNonPrJpsiSyst->Draw("2");
  gNonPrJpsi->Draw("P");
  gNonPrJpsiP->Draw("P");
      
  gPad->RedrawAxis();

  if(bSavePlots)
  {
    c2->SaveAs(Form("%s/pdf/nonPrJpsi_vsCent_%s.pdf",outputDir,sample[whichSample]));
    c2->SaveAs(Form("%s/png/nonPrJpsi_vsCent_%s.png",outputDir,sample[whichSample]));
  }

  //------------------- (pt, y) dependence
  TCanvas *c21a = new TCanvas("c21a","c21a");
  f4->Draw();
  // lumi->Draw();
  if(bAddLumi)
  {
    lumi_npr_y012_pt6530->Draw();
    lumi_npr_y1216_pt6530->Draw();
    lumi_npr_y1624_pt6530->Draw();
    f4->Draw("same");
  }
  lNpr->Draw();
  lpt->Draw();
  leg21a->Draw();
  CMS_lumi(c21a,103,33);

  gNonPrJpsiSyst_pt6530y012->Draw("2");
  gNonPrJpsiSyst_pt6530y1216->Draw("2");
  gNonPrJpsiSyst_pt6530y1624->Draw("2");
   
  gNonPrJpsi_pt6530y012->Draw("P");
  gNonPrJpsi_pt6530y1216->Draw("P");
  gNonPrJpsi_pt6530y1624->Draw("P");
 
  gNonPrJpsiP_pt6530y012->Draw("P");
  gNonPrJpsiP_pt6530y1216->Draw("P");
  gNonPrJpsiP_pt6530y1624->Draw("P");

  gPad->RedrawAxis();

  // pT dependence
  TCanvas *c21b = new TCanvas("c21b","c21b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi_npr_y1624_pt6530_pty->Draw();
    lumi_npr_y1624_pt365->Draw();
    f4->Draw("same");
  }

  lNpr->Draw();
  lyfwd->Draw(0);
  leg21b->Draw();
  CMS_lumi(c21b,103,33);

  gNonPrJpsiSyst_pt6530y1624->Draw("2");
  gNonPrJpsiSyst_pt365y1624->Draw("2");
    
  gNonPrJpsi_pt6530y1624->Draw("P");
  gNonPrJpsi_pt365y1624->Draw("P");

  gNonPrJpsiP_pt6530y1624->Draw("P");
  gNonPrJpsiP_pt365y1624->Draw("P");

  gPad->RedrawAxis();


  if(bSavePlots)
  {
    c21a->SaveAs(Form("%s/pdf/NonPrJpsi_vsCent_yHighPt_%s.pdf",outputDir,sample[whichSample]));
    c21a->SaveAs(Form("%s/png/NonPrJpsi_vsCent_yHighPt_%s.png",outputDir,sample[whichSample]));
    
    c21b->SaveAs(Form("%s/pdf/NonPrJpsi_vsCent_yLowPt_%s.pdf",outputDir,sample[whichSample]));
    c21b->SaveAs(Form("%s/png/NonPrJpsi_vsCent_yLowPt_%s.png",outputDir,sample[whichSample]));
  }
 
}
