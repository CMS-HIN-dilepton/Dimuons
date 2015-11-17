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

void compare_cent(bool bSavePlots       = true,
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

  const int nInHist = 5;
  const char* yieldHistNames[nInHist] = {"cent","y012Cent", "y1216Cent", "y1624Cent", "y1624LowPtCent"};

  //-----------------------------------------
  // input files: are in the filesRaa_2015.h

  // open the files with yields and do the math
  // default is TnP on data with Lxyz
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
  //------------------------------------------------------------------------------------

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

    int numBins = 0;
    if(ih==0) numBins = nBinsNpart12;
    if(ih==4) numBins = nBinsNpart6;
    if(ih==1 || ih==2 || ih==3) numBins = nBinsNpart6;

    for(int ibin=1; ibin<=numBins; ibin++)
    {
      double raa_pr=0, raaErr_pr=0, raa_npr=0, raaErr_npr=0;
      double raaTrad_pr=0, raaTradErr_pr=0, raaTrad_npr=0, raaTradErr_npr=0;
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
      double yieldRatioTrad_pr =
        (phRaw_pr_aa->GetBinContent(ibin)/phRaw_pr_pp->GetBinContent(ibin)) *
        (phEff_pr_pp->GetBinContent(ibin)/phEff_pr_aa->GetBinContent(ibin));

      raa_pr      =  yieldRatio_pr * scaleFactor * scale_cent;
      raaErr_pr   = TMath::Sqrt(TMath::Power(dRelErrRaw_pr_pp,2)+TMath::Power(dRelErrRaw_pr_aa,2))*raa_pr;

      raaTrad_pr      =  yieldRatioTrad_pr * scaleFactor * scale_cent;
      raaTradErr_pr   = TMath::Sqrt(TMath::Power(dRelErrRaw_pr_pp,2)+TMath::Power(dRelErrRaw_pr_aa,2))*raaTrad_pr;
    
      //non-prompt
      // get the rel uncert from the raw sample
      double dRelErrRaw_npr_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);
      double dRelErrRaw_npr_aa  = phRaw_npr_aa->GetBinError(ibin)/phRaw_npr_aa->GetBinContent(ibin);
      double yieldRatio_npr     = phCorr_npr_aa->GetBinContent(ibin)/phCorr_npr_pp->GetBinContent(ibin);
      double yieldRatioTrad_npr =
        (phRaw_npr_aa->GetBinContent(ibin)/phRaw_npr_pp->GetBinContent(ibin)) *
        (phEff_npr_pp->GetBinContent(ibin)/phEff_npr_aa->GetBinContent(ibin));

      if(ih==0)
      {
        raa_npr     = yieldRatio_npr * scaleFactor * scale_cent_np;// the 1D nonPr has 6 bins only
        raaTrad_npr = yieldRatioTrad_npr * scaleFactor * scale_cent_np;
      }
      else 
      {
        raa_npr     = yieldRatio_npr * scaleFactor * scale_cent;
        raaTrad_npr = yieldRatioTrad_npr * scaleFactor * scale_cent;
      }
      raaErr_npr     = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raa_npr;
      raaTradErr_npr = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raaTrad_npr;
      
     
      // fill the corresponding array
      switch(ih){
      case 0:
        prJpsi_cent[ibin-1]    = raa_pr;
        prJpsiErr_cent[ibin-1] = raaErr_pr;
       
        nonPrJpsi_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_cent[ibin-1] = raaErr_npr;
       
        prJpsiTrad_cent[ibin-1]    = raaTrad_pr;
        prJpsiTradErr_cent[ibin-1] = raaTradErr_pr;
       
        nonPrJpsiTrad_cent[ibin-1]    = raaTrad_npr;
        nonPrJpsiTradErr_cent[ibin-1] = raaTradErr_npr;

         if(bDoDebug)
        {
          cout<<"weight_pr_aa = "<<phEff_pr_aa->GetBinContent(ibin)<<"\t weight_pr_pp = "<<phEff_pr_pp->GetBinContent(ibin)<<endl;
          cout<<"yield_pr_aa "<<phCorr_pr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_pr_pp->GetBinContent(ibin)<<endl;
        
          cout<<"pr_aa= "<<phRaw_pr_aa->GetBinContent(ibin)/phEff_pr_aa->GetBinContent(ibin)<<"\t pr_pp= "<<phRaw_pr_pp->GetBinContent(ibin)/phEff_pr_pp->GetBinContent(ibin)<<endl;
          //  cout<<setprecision(2);
          cout<<"!!!!! raa = "<<prJpsi_cent[ibin-1]<<endl;
          
          // cout<<"Scale_Cent= "<<scale_cent<<endl;
        }

        break;
        
      case 1:
        prJpsi_pt6530y012_cent[ibin-1]        = raa_pr;
        prJpsiErr_pt6530y012_cent[ibin-1]     = raaErr_pr;

        nonPrJpsi_pt6530y012_cent[ibin-1]     = raa_npr;
        nonPrJpsiErr_pt6530y012_cent[ibin-1]  = raaErr_npr;

        prJpsiTrad_pt6530y012_cent[ibin-1]        = raaTrad_pr;
        prJpsiTradErr_pt6530y012_cent[ibin-1]     = raaTradErr_pr;

        nonPrJpsiTrad_pt6530y012_cent[ibin-1]     = raaTrad_npr;
        nonPrJpsiTradErr_pt6530y012_cent[ibin-1]  = raaTradErr_npr;

        break;
        
      case 2:
        prJpsi_pt6530y1216_cent[ibin-1]       = raa_pr;
        prJpsiErr_pt6530y1216_cent[ibin-1]    = raaErr_pr;

        nonPrJpsi_pt6530y1216_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_pt6530y1216_cent[ibin-1] = raaErr_npr;

        prJpsiTrad_pt6530y1216_cent[ibin-1]       = raaTrad_pr;
        prJpsiTradErr_pt6530y1216_cent[ibin-1]    = raaTradErr_pr;

        nonPrJpsiTrad_pt6530y1216_cent[ibin-1]    = raaTrad_npr;
        nonPrJpsiTradErr_pt6530y1216_cent[ibin-1] = raaTradErr_npr;

        break;
        
      case 3:
        prJpsi_pt6530y1624_cent[ibin-1]       = raa_pr;
        prJpsiErr_cent[ibin-1]                = raaErr_pr;

        nonPrJpsi_pt6530y1624_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_pt6530y1624_cent[ibin-1] = raaErr_npr;

        prJpsiTrad_pt6530y1624_cent[ibin-1]       = raaTrad_pr;
        prJpsiTradErr_cent[ibin-1]                = raaTradErr_pr;

        nonPrJpsiTrad_pt6530y1624_cent[ibin-1]    = raaTrad_npr;
        nonPrJpsiTradErr_pt6530y1624_cent[ibin-1] = raaTradErr_npr;

        break;
       
      case 4:
        prJpsi_pt365y1624_cent[ibin-1]        = raa_pr;
        prJpsiErr_pt365y1624_cent[ibin-1]     = raaErr_pr;

        nonPrJpsi_pt365y1624_cent[ibin-1]     = raa_npr;
        nonPrJpsiErr_pt365y1624_cent[ibin-1]  = raaErr_npr;


        prJpsiTrad_pt365y1624_cent[ibin-1]        = raaTrad_pr;
        prJpsiTradErr_pt365y1624_cent[ibin-1]     = raaTradErr_pr;

        nonPrJpsiTrad_pt365y1624_cent[ibin-1]     = raaTrad_npr;
        nonPrJpsiTradErr_pt365y1624_cent[ibin-1]  = raaTradErr_npr;

        if(bDoDebug)
        {
          cout<<"yield_npr_aa: raw "<<phRaw_npr_aa->GetBinContent(ibin)<<"\t eff:  "<<phEff_npr_aa->GetBinContent(ibin)<<endl;
          cout<<"yield_npr_aa: corr "<<phCorr_npr_aa->GetBinContent(ibin)<<endl;
          
          cout<<"yield_npr_pp: raw "<<phRaw_npr_pp->GetBinContent(ibin)<<"\t eff:  "<<phEff_npr_pp->GetBinContent(ibin)<<endl;
          cout<<"yield_npr_pp: corr "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;

          //  cout<<setprecision(2);
          cout<<"!!!!! raa = "<<prJpsi_cent[ibin-1]<<endl;
          
          // cout<<"Scale_Cent= "<<scale_cent<<endl;
        }


        break;
      }
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++)
  
  }//loop end: for(int ih=0; ih<nInHist;ih++)

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsi     = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsi_cent, binsNpart12Err, prJpsiErr_cent);
  TGraphErrors *gPrJpsi_pt6530y012     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsi_pt6530y012_cent, binsNpart6Err, prJpsiErr_pt6530y012_cent);
  TGraphErrors *gPrJpsi_pt6530y1216     = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt6530y1216_cent, binsNpart6Err, prJpsiErr_pt6530y1216_cent);
  TGraphErrors *gPrJpsi_pt6530y1624     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsi_pt6530y1624_cent, binsNpart6Err, prJpsiErr_pt6530y1624_cent);
  TGraphErrors *gPrJpsi_pt365y1624     = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt365y1624_cent, binsNpart6Err, prJpsiErr_pt365y1624_cent);

 
  // nonPr
  TGraphErrors *gNonPrJpsi     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_cent, binsNpart6Err, nonPrJpsiErr_cent);
  TGraphErrors *gNonPrJpsi_pt6530y012     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus,nonPrJpsi_pt6530y012_cent, binsNpart6Err, nonPrJpsiErr_pt6530y012_cent);
  TGraphErrors *gNonPrJpsi_pt6530y1216     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt6530y1216_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1216_cent);
  TGraphErrors *gNonPrJpsi_pt6530y1624     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsi_pt6530y1624_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1624_cent);
  TGraphErrors *gNonPrJpsi_pt365y1624     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt365y1624_cent, binsNpart6Err, nonPrJpsiErr_pt365y1624_cent);
  
  //-------------------------- traditional stuff

   // pr
  TGraphErrors *gPrJpsiTrad     = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsiTrad_cent, binsNpart12Err, prJpsiTradErr_cent);
  TGraphErrors *gPrJpsiTrad_pt6530y012  = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsiTrad_pt6530y012_cent, binsNpart6Err, prJpsiTradErr_pt6530y012_cent);
  TGraphErrors *gPrJpsiTrad_pt6530y1216 = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsiTrad_pt6530y1216_cent, binsNpart6Err, prJpsiTradErr_pt6530y1216_cent);
  TGraphErrors *gPrJpsiTrad_pt6530y1624 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsiTrad_pt6530y1624_cent, binsNpart6Err, prJpsiTradErr_pt6530y1624_cent);
  TGraphErrors *gPrJpsiTrad_pt365y1624  = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsiTrad_pt365y1624_cent, binsNpart6Err, prJpsiTradErr_pt365y1624_cent);

 
  // nonPr
  TGraphErrors *gNonPrJpsiTrad     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsiTrad_cent, binsNpart6Err, nonPrJpsiErr_cent);
  TGraphErrors *gNonPrJpsiTrad_pt6530y012 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus,nonPrJpsiTrad_pt6530y012_cent, binsNpart6Err, nonPrJpsiTradErr_pt6530y012_cent);
  TGraphErrors *gNonPrJpsiTrad_pt6530y1216= new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsiTrad_pt6530y1216_cent, binsNpart6Err, nonPrJpsiTradErr_pt6530y1216_cent);
  TGraphErrors *gNonPrJpsiTrad_pt6530y1624= new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsiTrad_pt6530y1624_cent, binsNpart6Err, nonPrJpsiTradErr_pt6530y1624_cent);
  TGraphErrors *gNonPrJpsiTrad_pt365y1624 = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsiTrad_pt365y1624_cent, binsNpart6Err, nonPrJpsiTradErr_pt365y1624_cent);
  


  //-------------------------------------------------------------------
  // **************** marker colors
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

  gPrJpsi_pt6530y012->SetMarkerStyle(20);
  gPrJpsi_pt6530y1216->SetMarkerStyle(21);
  gPrJpsi_pt6530y1624->SetMarkerStyle(33);
  gPrJpsi_pt365y1624->SetMarkerStyle(34);

 
  // non-pr
  gNonPrJpsi->SetMarkerStyle(29);
   
  gNonPrJpsi_pt6530y012->SetMarkerStyle(20);
  gNonPrJpsi_pt6530y1216->SetMarkerStyle(21);
  gNonPrJpsi_pt6530y1624->SetMarkerStyle(33);
  gNonPrJpsi_pt365y1624->SetMarkerStyle(34);

  // marker size
  // pr
  gPrJpsi->SetMarkerSize(1.2);
  
  gPrJpsi_pt6530y012->SetMarkerSize(1.2);
  gPrJpsi_pt6530y1216->SetMarkerSize(1.2);
  gPrJpsi_pt6530y1624->SetMarkerSize(2.0);
  gPrJpsi_pt365y1624->SetMarkerSize(1.7);

  // nonPr
  gNonPrJpsi->SetMarkerSize(2.0);
 
  gNonPrJpsi_pt6530y012->SetMarkerSize(1.2);
  gNonPrJpsi_pt6530y1216->SetMarkerSize(1.2);
  gNonPrJpsi_pt6530y1624->SetMarkerSize(2.0);
  gNonPrJpsi_pt365y1624->SetMarkerSize(1.7);

  //-------------- traditional stuff
  // pr
  gPrJpsiTrad->SetMarkerStyle(25);

  gPrJpsiTrad_pt6530y012->SetMarkerStyle(24);
  gPrJpsiTrad_pt6530y1216->SetMarkerStyle(25);
  gPrJpsiTrad_pt6530y1624->SetMarkerStyle(27);
  gPrJpsiTrad_pt365y1624->SetMarkerStyle(28);

  // non-pr
  gNonPrJpsiTrad->SetMarkerStyle(30);
   
  gNonPrJpsiTrad_pt6530y012->SetMarkerStyle(24);
  gNonPrJpsiTrad_pt6530y1216->SetMarkerStyle(25);
  gNonPrJpsiTrad_pt6530y1624->SetMarkerStyle(27);
  gNonPrJpsiTrad_pt365y1624->SetMarkerStyle(28);

  // pr
  gPrJpsiTrad->SetMarkerSize(1.2);
  
  gPrJpsiTrad_pt6530y012->SetMarkerSize(1.2);
  gPrJpsiTrad_pt6530y1216->SetMarkerSize(1.2);
  gPrJpsiTrad_pt6530y1624->SetMarkerSize(2.0);
  gPrJpsiTrad_pt365y1624->SetMarkerSize(1.7);

  // nonPr
  gNonPrJpsiTrad->SetMarkerSize(2.0);
 
  gNonPrJpsiTrad_pt6530y012->SetMarkerSize(1.2);
  gNonPrJpsiTrad_pt6530y1216->SetMarkerSize(1.2);
  gNonPrJpsiTrad_pt6530y1624->SetMarkerSize(2.0);
  gNonPrJpsiTrad_pt365y1624->SetMarkerSize(1.7);

  // same for the raa denominator and nominator yields histograms
  for(int ih=0; ih<nInHist;ih++) // for each kinematic range
  {
    ahRatio_pr_pp[ih]->SetMarkerStyle(20);
    ahRatio_npr_pp[ih]->SetMarkerStyle(20);
    ahRatio_pr_aa[ih]->SetMarkerStyle(20);
    ahRatio_npr_aa[ih]->SetMarkerStyle(20);
  }
  

  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,400);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);
 
  TLegend *leg11a = new TLegend(0.6,0.5,0.85,0.65);
  leg11a->SetFillStyle(0);
  leg11a->SetFillColor(0);
  leg11a->SetBorderSize(0);
  leg11a->SetMargin(0.2);
  leg11a->SetTextSize(0.04);
  leg11a->AddEntry(gPrJpsi_pt6530y012,"|y|<1.2","P");
  leg11a->AddEntry(gPrJpsi_pt6530y1216,"1.2<|y|<1.6","P");

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

  TLegend *leg21b = new TLegend(0.55,0.5,0.8,0.65);
  leg21b->SetFillStyle(0);
  leg21b->SetFillColor(0);
  leg21b->SetBorderSize(0);
  leg21b->SetMargin(0.2);
  leg21b->SetTextSize(0.04);
  leg21b->AddEntry(gNonPrJpsi_pt6530y1624,"6.5<p_{T}<30 GeV/c","P");
  leg21b->AddEntry(gNonPrJpsi_pt365y1624, "3<p_{T}<6.5 GeV/c","P");


  //---------------- general stuff
  TLatex *pre_pr = new TLatex(20,1.35,"Prompt J/#psi");
  pre_pr->SetTextFont(42);
  pre_pr->SetTextSize(0.05);


  TLatex *lPr = new TLatex(20,1.35,"Prompt J/#psi");
  lPr->SetTextFont(42);
  lPr->SetTextSize(0.05);

  TLatex *lNpr = new TLatex(20,1.35,"Non-prompt J/#psi");
  lNpr->SetTextFont(42);
  lNpr->SetTextSize(0.05);

  TLatex *ly = new TLatex(190.0,1.05,"|y| < 2.4");
  ly->SetTextFont(42);
  ly->SetTextSize(0.05);

  TLatex *lpt = new TLatex(190.0,1.05,"6.5 < p_{T} < 30 GeV/c");
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.05);

  TLatex *lpt2 = new TLatex(190.0,0.90,"6.5 < p_{T} < 30 GeV/c");
  lpt2->SetTextFont(42);
  lpt2->SetTextSize(0.05);

  //  -----------------------for comparison purposes
  // axis for the yields
  TF1 *fBin = new TF1("fBin","1",0,10);
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
 
  // general stuff
  c1->cd(1);
  f4->Draw();// axis
  lPr->Draw();
  ly->Draw();
  lpt2->Draw();
  
  gPrJpsi->Draw("P");
  gPrJpsiTrad->Draw("P");

  // yields
  c1->cd(2);
  fBin->Draw();// axis
  gPad->SetGridy();
  
  ahRatio_pr_pp[0]->Draw("sames");
      
  lPP->Draw();
  lRatio->Draw();
  
  c1->cd(3);
  fBin->Draw();// axis
  gPad->SetGridy();
    
  ahRatio_pr_aa[0]->Draw("sames");
  
  lAA->Draw();
  lRatio->Draw();

  if(bSavePlots)
  {
    c1->SaveAs(Form("%s/pdf/PrJpsi_vsCent_%s.pdf",outputDir,compWhat[whichCompare]));
    c1->SaveAs(Form("%s/png/PrJpsi_vsCent_%s.png",outputDir,compWhat[whichCompare]));
  }
  
  //------------------- (pt, y) dependence
  TCanvas *c11a = new TCanvas("c11a","c11a",1200,800);
  c11a->Divide(3,2);
  c11a->cd(1);
  f4->Draw();

  gPrJpsi_pt6530y012->Draw("P");
  gPrJpsiTrad_pt6530y012->Draw("P");

  lPr->Draw();
  leg11a->Draw();
  lpt->Draw();
  gPad->RedrawAxis();

  c11a->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  lRatio->Draw();

  ahRatio_pr_pp[1]->Draw("sames");

  c11a->cd(3);
  fBin->Draw();
  gPad->SetGridy();
  lRatio->Draw();

  ahRatio_pr_aa[1]->Draw("sames");

  c11a->cd(4);
  f4->Draw();
  gPrJpsi_pt6530y1216->Draw("P");
  gPrJpsiTrad_pt6530y1216->Draw("P");

  c11a->cd(5);
  fBin->Draw();
  gPad->SetGridy();
  lPP->Draw();
  ahRatio_pr_pp[2]->Draw("sames");

  c11a->cd(6);
  fBin->Draw();
  gPad->SetGridy();
  lAA->Draw();
  ahRatio_pr_aa[2]->Draw("sames");

  //------------------- fwd region
  TCanvas *c11b = new TCanvas("c11b","c11b",1200,800);
  c11b->Divide(3,2);

  c11b->cd(1);
  f4->Draw();

  gPrJpsi_pt6530y1624->Draw("P");
  gPrJpsiTrad_pt6530y1624->Draw("P");

  lPr->Draw();
  leg11b->Draw();
  gPad->RedrawAxis();

  c11b->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  ahRatio_pr_pp[3]->Draw("sames");
  lPP->Draw();
  lRatio->Draw();

  c11b->cd(3);
  fBin->Draw();
  gPad->SetGridy();
  lAA->Draw();
  lRatio->Draw();

  ahRatio_pr_aa[3]->Draw("sames");

  c11b->cd(4);
  f4->Draw();
  gPrJpsi_pt365y1624->Draw("P");
  gPrJpsiTrad_pt365y1624->Draw("P");

  c11b->cd(5);
  fBin->Draw();
  gPad->SetGridy();
  ahRatio_pr_pp[4]->Draw("sames");

  c11b->cd(6);
  fBin->Draw();
  gPad->SetGridy();
  ahRatio_pr_aa[4]->Draw("sames");



  if(bSavePlots)
  {
    c11a->SaveAs(Form("%s/pdf/PrJpsi_vsCent_yMid_%s.pdf",outputDir,compWhat[whichCompare]));
    c11a->SaveAs(Form("%s/png/PrJpsi_vsCent_yMid_%s.png",outputDir,compWhat[whichCompare]));

    c11b->SaveAs(Form("%s/pdf/PrJpsi_vsCent_yFwd_%s.pdf",outputDir,compWhat[whichCompare]));
    c11b->SaveAs(Form("%s/png/PrJpsi_vsCent_yFwd_%s.png",outputDir,compWhat[whichCompare]));
  }

  // //   // ############################################## non-pr
  // //   // ############################################## non-pr
  // //   // ############################################## non-pr

  TCanvas *c2 = new TCanvas("c2","c2",1200,400);
  c2->Divide(3,1);
 
  // general stuff
  c2->cd(1);
  f4->Draw();// axis
  lNpr->Draw();
  ly->Draw();
  lpt2->Draw();
  
  gNonPrJpsi->Draw("P");
  gNonPrJpsiTrad->Draw("P");

  // yields
  c2->cd(2);
  fBin->Draw();// axis
  gPad->SetGridy();
  
  ahRatio_npr_pp[0]->Draw("sames");
      
  lPP->Draw();
  lRatio->Draw();
  
  c2->cd(3);
  fBin->Draw();// axis
  gPad->SetGridy();
    
  ahRatio_npr_aa[0]->Draw("sames");
  
  lAA->Draw();
  lRatio->Draw();


  if(bSavePlots)
  {
    c2->SaveAs(Form("%s/pdf/NonPrJpsi_vsCent_%s.pdf",outputDir,compWhat[whichCompare]));
    c2->SaveAs(Form("%s/png/NonPrJpsi_vsCent_%s.png",outputDir,compWhat[whichCompare]));
  }
  
  //------------------- (pt, y) dependence
  TCanvas *c22a = new TCanvas("c22a","c22a",1200,800);
  c22a->Divide(3,2);
  c22a->cd(1);
  f4->Draw();

  gNonPrJpsi_pt6530y012->Draw("P");
  gNonPrJpsiTrad_pt6530y012->Draw("P");

  lNpr->Draw();
  leg21a->Draw();
  lpt->Draw();
  gPad->RedrawAxis();

  c22a->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  lPP->Draw();
  lRatio->Draw();

  ahRatio_npr_pp[1]->Draw("sames");

  c22a->cd(3);
  fBin->Draw();
  gPad->SetGridy();
  lAA->Draw();
  lRatio->Draw();

  ahRatio_npr_aa[1]->Draw("sames");

  c22a->cd(4);
  f4->Draw();
  gNonPrJpsi_pt6530y1216->Draw("P");
  gNonPrJpsiTrad_pt6530y1216->Draw("P");

  c22a->cd(5);
  fBin->Draw();
  gPad->SetGridy();
  ahRatio_npr_pp[2]->Draw("sames");

  c22a->cd(6);
  fBin->Draw();
  gPad->SetGridy();
  ahRatio_npr_aa[2]->Draw("sames");

  //------------------- fwd region
  TCanvas *c22b = new TCanvas("c22b","c22b",1200,800);
  c22b->Divide(3,2);

  c22b->cd(1);
  f4->Draw();

  gNonPrJpsi_pt6530y1624->Draw("P");
  gNonPrJpsiTrad_pt6530y1624->Draw("P");

  lNpr->Draw();
  leg21b->Draw();
  gPad->RedrawAxis();

  c22b->cd(2);
  fBin->Draw();
  gPad->SetGridy();
  ahRatio_npr_pp[3]->Draw("sames");
  lPP->Draw();
  lRatio->Draw();

  c22b->cd(3);
  fBin->Draw();
  gPad->SetGridy();
  lAA->Draw();
  lRatio->Draw();

  ahRatio_npr_aa[3]->Draw("sames");

  c22b->cd(4);
  f4->Draw();
  gNonPrJpsi_pt365y1624->Draw("P");
  gNonPrJpsiTrad_pt365y1624->Draw("P");

  c22b->cd(5);
  fBin->Draw();
  gPad->SetGridy();

  ahRatio_npr_pp[4]->Draw("sames");

  c22b->cd(6);
  fBin->Draw();
  gPad->SetGridy();
  ahRatio_npr_aa[4]->Draw("sames");

  if(bSavePlots)
  {
    c22a->SaveAs(Form("%s/pdf/NonPrJpsi_vsCent_yMid_%s.pdf",outputDir,compWhat[whichCompare]));
    c22a->SaveAs(Form("%s/png/NonPrJpsi_vsCent_yMid_%s.png",outputDir,compWhat[whichCompare]));
   
    c22b->SaveAs(Form("%s/pdf/NonPrJpsi_vsCent_yFwd_%s.pdf",outputDir,compWhat[whichCompare]));
    c22b->SaveAs(Form("%s/png/NonPrJpsi_vsCent_yFwd_%s.png",outputDir,compWhat[whichCompare]));
  }


 
}
