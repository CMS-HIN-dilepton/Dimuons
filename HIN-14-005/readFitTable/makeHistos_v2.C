/*
  Macro that makes histograms of:
1) yields from the fit_table file that contains the fit results, that sits in a path of the form data/v2/20150817_PbPb/pp_../noWeight-weighted_.../summary/

  Before running the macro:
a) chose adjut the name of the directories (2015...) in the 'whichSample[1]' vector
b) adjust the name of the weighted-no weighted directory, in the vectors whichWeight[2], and whichWeight_pp[2]

The output root files of this macro, wich contains the histograms with the yields, are the input root file that are needed to make the v2 plots! 

 */

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
#include "Riostream.h"

using namespace std;

// remember to replace, in the case of the 'weighted' case, the rel. stat uncertianties with the one from the 'unweighted' yields

void makeHistos_v2(int nDphiBins = 4, 
                   int sample = 0, // 0=PbPb
                   int weight = 0, // 0=noWeight, 1=weight
                   const char* inputFitDataFileLocation = "../data/v2/"
                   ) 
{
  const char* whichSample[1]    = {"20150823"};
  const char* whichWeight[2]    = {"v2noW_Lxyz_pTtune_PRMC","v2W_Lxyz_pTtune_PRMC"};
  const char* outputHistDataFile[1] = {"histsV2Yields"};

  double PI = TMath::Pi();

  ifstream in;
  string filename = Form("%s/%s/%s/summary/fit_table",inputFitDataFileLocation,whichSample[sample],whichWeight[weight]);
  in.open(filename.c_str());
  if (!in.is_open()) {
    cout << "input file " << filename << " cannot be open" << endl;
    return;
  }
 
  TFile *pfOutput;
  pfOutput = new TFile(Form("%s_%s_%s_dPhiBins%d.root",outputHistDataFile[0],whichSample[sample],whichWeight[weight],nDphiBins),"RECREATE");

  TH1F *hBkg[300];
  TH1F *hPrp[300];
  TH1F *hNPrp[300];
  
  double x[300];
  double prpt[300], prptErr[300], nprpt[300], nprptErr[300];
  double bkg[300], bkgErr[300];
  double rap1[300], rap2[300], pT1[300], pT2[300];//, cent1[300], cent2[300];
  int cent1[300], cent2[300];
  char tmp[512]={' '};
  int nline = 0;

  // Read 2 header lines and throw them away to keep only numbers
  string tmpstring;
  getline(in,tmpstring);
  cout << tmpstring << " is being processed..." << endl;
  getline(in,tmpstring);

  while (in.good()) 
  {
    in >> x[0] >> x[1] >> x[2] >> x[3] >> x[4] >> x[5] >> x[6] >> x[7] >> x[8] >> x[9] >> x[10] >> x[11] >> x[12] >> x[13] >> x[14] >> x[15] >> x[16] >> x[17];
    rap1[nline]  = x[0];  rap2[nline]     = fabs(x[1]); // rapidity (second value comes with '-')
    pT1[nline]   = x[2];  pT2[nline]      = fabs(x[3]); // pt (second value comes with '-')
    cent1[nline] = x[4];  cent2[nline]    = fabs(x[5]);// centrlaity (second value comes with '-')
    // [6]&[7] is the phi interval
    bkg[nline]   = x[10];  bkgErr[nline]   = fabs(x[11]);  // bkg yield and error
    //[7]&[8] is the inclusive and inclusive error
    prpt[nline]  = x[12]; prptErr[nline]  = fabs(x[13]); // prompt yield and error
    nprpt[nline] = x[14]; nprptErr[nline] = fabs(x[15]); // non-prompt yield and error
    //[16]&[17] is the b-fraction
    
    if(nline == 11)
    {
      cout<<tmp<<endl;
      cout<<"TEST: Stored values in line : "<<nline<<endl;
      cout<<rap1[nline]<<" "<<rap2[nline]<<" ";
      cout<<pT1[nline]<<" "<<pT2[nline]<<" ";
      cout<<cent1[nline]<<" "<<cent2[nline]<<" ";
      cout<<bkg[nline]<<" "<<prpt[nline] <<" "<<prptErr[nline]<<" "<<nprpt[nline] <<" "<<nprptErr[nline]<<endl;
    }
    nline++;
  }
  printf(" found %d points\n",nline);
  
  // Make histograms
  pfOutput->cd();

  for(int j = 0; j < nline/(nDphiBins+1); j++)
  {
    if(nDphiBins==4)
    {
      // bkg histograms
      TString histBkg(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_Bkg",rap1[5*j],rap2[5*j],pT1[5*j],pT2[5*j],cent1[5*j],cent2[5*j]));
      histBkg.ReplaceAll(".","");
      hBkg[j] = new TH1F(histBkg,";#Delta #phi;",4,0,PI/2);
      
      hBkg[j]->Sumw2();
      hBkg[j]->SetBinContent(1, bkg[5*j]);
      hBkg[j]->SetBinContent(2, bkg[5*j+2]);
      hBkg[j]->SetBinContent(3, bkg[5*j+3]);
      hBkg[j]->SetBinContent(4, bkg[5*j+4]);
      hBkg[j]->SetBinError(1, bkgErr[5*j]);
      hBkg[j]->SetBinError(2, bkgErr[5*j+2]);
      hBkg[j]->SetBinError(3, bkgErr[5*j+3]);
      hBkg[j]->SetBinError(4, bkgErr[5*j+4]);
      //cout<<tmp<<endl;
  
      //prompt histograms
      TString histPrompt(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_Prp",rap1[5*j],rap2[5*j],pT1[5*j],pT2[5*j],cent1[5*j],cent2[5*j]));
      histPrompt.ReplaceAll(".","");
      hPrp[j] = new TH1F(histPrompt,";#Delta #phi;",4,0,PI/2);
      hPrp[j]->SetBinContent(1, prpt[5*j]);
      hPrp[j]->SetBinContent(2, prpt[5*j+2]);
      hPrp[j]->SetBinContent(3, prpt[5*j+3]);
      hPrp[j]->SetBinContent(4, prpt[5*j+4]);
      hPrp[j]->SetBinError(1, prptErr[5*j]);
      hPrp[j]->SetBinError(2, prptErr[5*j+2]);
      hPrp[j]->SetBinError(3, prptErr[5*j+3]);
      hPrp[j]->SetBinError(4, prptErr[5*j+4]);
      //cout<<tmp<<endl;
  
      //non-prompt histograms
      TString histNonPrompt(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_NPrp",rap1[5*j],rap2[5*j],pT1[5*j],pT2[5*j],cent1[5*j],cent2[5*j]));
      histNonPrompt.ReplaceAll(".","");
      hNPrp[j] = new TH1F(histNonPrompt,";#Delta #phi;",4,0,PI/2);
      hNPrp[j]->SetBinContent(1, nprpt[5*j]);
      hNPrp[j]->SetBinContent(2, nprpt[5*j+2]);
      hNPrp[j]->SetBinContent(3, nprpt[5*j+3]);
      hNPrp[j]->SetBinContent(4, nprpt[5*j+4]);
      hNPrp[j]->SetBinError(1, nprptErr[5*j]);
      hNPrp[j]->SetBinError(2, nprptErr[5*j+2]);
      hNPrp[j]->SetBinError(3, nprptErr[5*j+3]);
      hNPrp[j]->SetBinError(4, nprptErr[5*j+4]);
      //cout<<tmp<<endl;
    }

    if(nDphiBins==2)
    {
      // background histograms
      TString histBkg(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_Bkg",rap1[3*j],rap2[3*j],pT1[3*j],pT2[3*j],cent1[3*j],cent2[3*j]));
      histBkg.ReplaceAll(".","");
      hBkg[j] = new TH1F(histBkg,";#Delta #phi;",2,0,PI/2);
      
      hBkg[j]->Sumw2();
      hBkg[j]->SetBinContent(1, bkg[3*j]);
      hBkg[j]->SetBinContent(2, bkg[3*j+2]);
      hBkg[j]->SetBinError(1, bkgErr[3*j]);
      hBkg[j]->SetBinError(2, bkgErr[3*j+2]);

      //cout<<tmp<<endl;
  
      //prompt histograms
      TString histPrompt(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_Prp",rap1[3*j],rap2[3*j],pT1[3*j],pT2[3*j],cent1[3*j],cent2[3*j]));
      histPrompt.ReplaceAll(".","");
      hPrp[j] = new TH1F(histPrompt,";#Delta #phi;",2,0,PI/2);
      hPrp[j]->SetBinContent(1, prpt[3*j]);
      hPrp[j]->SetBinContent(2, prpt[3*j+2]);
      hPrp[j]->SetBinError(1, prptErr[3*j]);
      hPrp[j]->SetBinError(2, prptErr[3*j+2]);
      //cout<<tmp<<endl;
  
      //non-prompt histograms
      TString histNonPrompt(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_NPrp",rap1[3*j],rap2[3*j],pT1[3*j],pT2[3*j],cent1[3*j],cent2[3*j]));
      histNonPrompt.ReplaceAll(".","");
      hNPrp[j] = new TH1F(histNonPrompt,";#Delta #phi;",2,0,PI/2);
      hNPrp[j]->SetBinContent(1, nprpt[3*j]);
      hNPrp[j]->SetBinContent(2, nprpt[3*j+2]);
      hNPrp[j]->SetBinError(1, nprptErr[3*j]);
      hNPrp[j]->SetBinError(2, nprptErr[3*j+2]);
      //cout<<tmp<<endl;

    }
    
  }
  
  pfOutput->Write();
  in.close();

}

