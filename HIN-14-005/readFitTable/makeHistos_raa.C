/*
  Macro that makes histograms of:
1) yields from the fit_table file that contains the fit results, that sits in a path of the form data/raa/20150817_PbPb/pp_../noWeight-weighted_.../summary/
2) efficiency histograms out of the traditional efficiencies stored in excel sheet

  Before running the macro:
a) chose adjut the name of the directories (2015...) in the 'whichSample[2]' vector
b) adjust the name of the weighted-no weighted directory, in the vectors whichWeight[2], and whichWeight_pp[2]
c) chose whether is the yields files or efficiency that you want to 'histogramize'

The output root files of this macro, wich contains the histograms with the yields, are the input root file that are needed to make the Raa plots! 

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
#include "dataBinning_2015.h"

void makeHistos_raa(int sample = 1, // 0=PbPb,     1= pp
                   int weight = 1, // 0=noWeight, 1=weight
                   int isEffFile = 0,// 0=no, 1=yes needed when making efficiency histograms
                   const char* inputFitDataFileLocation = "../data/raa"
                   ) 
{

  const char* whichSample[2]     = {"20151111_PbPb",      "20151111_pp"};
  const char* whichWeight[2]    = {"noWeight_TnPAtRD3_4DEff_RapPtEffMap2_Ratio", "weightedEff_TnPAtRD3_4DEff_RapPtEffMap2_Ratio"};
  const char* whichWeight_pp[2] = {"noWeight_TnPAtRD3_4DEff_RapPtEffMap2_Ratio", "weightedEff_TnPAtRD3_4DEff_RapPtEffMap2_Ratio"};


  const char* effFileName[2]        = {"20151028_PbPb_Eff_newTnP","20151028_pp_Eff_newTnP"};
  const char* outputHistDataFile[2] = {"histsRaaYields","histEff"};

  TFile *pfOutput;
  ifstream in;
  if(!isEffFile)
  {
    if(sample==0)
    {
      in.open(Form("%s/%s/%s/summary/fit_table",inputFitDataFileLocation,whichSample[sample],whichWeight[weight]));
      pfOutput = new TFile(Form("%s_%s_%s.root",outputHistDataFile[isEffFile],whichSample[sample],whichWeight[weight]),"RECREATE");
      cout <<"##### Lucky you: you are reading the PbPb fit files" <<endl;
    }
    if(sample==1)
    {
      in.open(Form("%s/%s/%s/summary/fit_table",inputFitDataFileLocation,whichSample[sample],whichWeight_pp[weight]));
      pfOutput = new TFile(Form("%s_%s_%s.root",outputHistDataFile[isEffFile],whichSample[sample],whichWeight_pp[weight]),"RECREATE");
      cout <<"##### Lucky you: you are reading the pp fit files" <<endl;
    }
  }
  else// these are the traditional efficiency files
  {
    in.open(Form("%s/excel/%s.dat",inputFitDataFileLocation,effFileName[sample]));
    pfOutput = new TFile(Form("%s_%s.root",outputHistDataFile[isEffFile],effFileName[sample]),"RECREATE");
    
  }

  double x[17];
  double prpt[300], prptErr[300], nprpt[300], nprptErr[300];
  double inc[300], incErr[300];
  double rap1[300], rap2[300], pT1[300], pT2[300];
  int cent1[300], cent2[300];
  int nline = 0;

  // Read 2 header lines and throw them away to kepp only numbers
  string tmpstring;
  getline(in,tmpstring);
  getline(in,tmpstring);
  while (in.good()) 
  {
    if(!isEffFile)// yield files
    {
      in >> x[0] >> x[1] >> x[2] >> x[3] >> x[4] >> x[5] >> x[6] >> x[7] >> x[8] >> x[9] >> x[10] >> x[11] >> x[12] >> x[13] >> x[14] >> x[15] >> x[16] >> x[17];
      rap1[nline]  = x[0];  rap2[nline]     = fabs(x[1]); // rapidity (second value comes with '-')
      pT1[nline]   = x[2];  pT2[nline]      = fabs(x[3]); // pt (second value comes with '-')
      cent1[nline] = x[4];  cent2[nline]    = fabs(x[5]); // centrlaity (second value comes with '-')
      // [6]&[7] is the phi interval
      inc[nline]   = x[8];  incErr[nline]   = fabs(x[9]); // inclusive yield and error
      //[10]&[11] is the bkg and bkg error
      prpt[nline]  = x[12]; prptErr[nline]  = fabs(x[13]);// prompt yield and error
      nprpt[nline] = x[14]; nprptErr[nline] = fabs(x[15]);// non-prompt yield and error
      //[16]&[17] is the b-fraction
    }
    else // efficiency files
    {
      in >> x[0] >> x[1] >> x[2] >> x[3] >> x[4] >> x[5] >> x[6] >> x[7];
      rap1[nline]  = x[0];  rap2[nline]     = fabs(x[1]); // rapidity (second value comes with '-')
      pT1[nline]   = x[2];  pT2[nline]      = fabs(x[3]); // pt (second value comes with '-')
      cent1[nline] = x[4];  cent2[nline]    = fabs(x[5]); // centrlaity (second value comes with '-')
      prpt[nline]  = x[6]; // prompt correction
      nprpt[nline] = x[7]; // non-prompt correction
    }
    
    if(nline == 28 || nline == 10)
    {
      cout<<"Stored values in line : "<<nline<<endl;
      cout<<"rapidity: "<<rap1[nline]<<" "<<rap2[nline]<<" "<<endl;
      cout<<"pt: "<<pT1[nline]<<" "<<pT2[nline]<<" "<<endl;
      cout<<"cent: "<<cent1[nline]<<" "<<cent2[nline]<<" "<<endl;
      cout<<"prompt: "<<prpt[nline] <<"\n non-prompt: "<<nprpt[nline] <<endl;
    }
    nline++;
  }
  printf(" found %d points\n",nline);
  cout<<"#######  Done reading the input dat file!"<<endl;
  pfOutput->cd();
 

  // Make histograms
  // minbias histos
  TH1F *phPrp_mb      = new TH1F("phPrp_mb", ";N_{part};Yield",nBinsMB,bins1);
  TH1F *phNPrp_mb     = new TH1F("phNPrp_mb", ";N_{part};Yield",nBinsMB,bins1);

  TH1F *phPrp_y_mb      = new TH1F("phPrp_y_mb",  ";y;Yield",nBinsY3,bins3);
  TH1F *phNPrp_y_mb     = new TH1F("phNPrp_y_mb", ";y;Yield",nBinsY3,bins3);

  TH1F *phPrp_ptLow_mb  = new TH1F("phPrp_ptLow_mb", ";p_{T};Yield",nBinsPt3, bins3);
  TH1F *phNPrp_ptLow_mb = new TH1F("phNPrp_ptLow_mb", ";p_{T};Yield",nBinsPt3, bins3);
  
  // 1D histos
  TH1F *phPrp_cent   = new TH1F("phPrp_cent", ";N_{part};Yield",nBinsNpart12,bins12);
  TH1F *phNPrp_cent  = new TH1F("phNPrp_cent",";N_{part};Yield",nBinsNpart6,bins6);

  TH1F *phPrp_y    = new TH1F("phPrp_y",";y;Yield",nBinsY,bins6);
  TH1F *phNPrp_y   = new TH1F("phNPrp_y",";y;Yield",nBinsY,bins6);

  TH1F *phPrp_pt  = new TH1F("phPrp_pt", ";p_{T};Yield",nBinsPt,bins7);
  TH1F *phNPrp_pt = new TH1F("phNPrp_pt", ";p_{T};Yield",nBinsPt,bins7);
  
  TH1F *phPrp_ptLow  = new TH1F("phPrp_ptLow", ";p_{T};Yield",nBinsPt3,bins3);
  TH1F *phNPrp_ptLow = new TH1F("phNPrp_ptLow", ";p_{T};Yield",nBinsPt3,bins3);

  // 2D histos
  TH1F *phPrp_y012Cent = new TH1F("phPrp_y012Cent", ";N_{part};Yield",nBinsNpart6,bins6);
  TH1F *phNPrp_y012Cent= new TH1F("phNPrp_y012Cent", ";N_{part};Yield",nBinsNpart6,bins6);

  TH1F *phPrp_y1216Cent = new TH1F("phPrp_y1216Cent", ";N_{part};Yield",nBinsNpart6,bins6);
  TH1F *phNPrp_y1216Cent= new TH1F("phNPrp_y1216Cent", ";N_{part};Yield",nBinsNpart6,bins6);

  TH1F *phPrp_y1624Cent = new TH1F("phPrp_y1624Cent", ";N_{part};Yield",nBinsNpart6,bins6);
  TH1F *phNPrp_y1624Cent = new TH1F("phNPrp_y1624Cent", ";N_{part};Yield",nBinsNpart6,bins6);

  TH1F *phPrp_y1624LowPtCent  = new TH1F("phPrp_y1624LowPtCent", ";N_{part};Yield",nBinsNpart6,bins6);
  TH1F *phNPrp_y1624LowPtCent = new TH1F("phNPrp_y1624LowPtCent", ";N_{part};Yield",nBinsNpart6,bins6);

// ------------------
  phPrp_mb->Sumw2();
  phNPrp_mb->Sumw2();

  phPrp_y_mb->Sumw2();
  phNPrp_y_mb->Sumw2();

  phPrp_ptLow_mb->Sumw2();
  phNPrp_ptLow_mb->Sumw2();

  phPrp_cent->Sumw2();
  phNPrp_cent->Sumw2();

  phPrp_y->Sumw2();
  phNPrp_y->Sumw2();

  phPrp_pt->Sumw2();
  phNPrp_pt->Sumw2();
  
  phPrp_ptLow->Sumw2();
  phNPrp_ptLow->Sumw2();

  phPrp_y012Cent->Sumw2();
  phNPrp_y012Cent->Sumw2();

  phPrp_y1216Cent->Sumw2();
  phNPrp_y1216Cent->Sumw2();

  phPrp_y1624Cent->Sumw2();
  phNPrp_y1624Cent->Sumw2();

  phPrp_y1624LowPtCent->Sumw2();
  phNPrp_y1624LowPtCent->Sumw2(); 

  cout<<"#######  Done declaring the histograms!"<<endl;
//--------------------------
  // loop over all entries in file, and fill histos
 
  for (int il=0; il<=nline; il++)
  {
    int binn = 0;

    if(sample==0)// PbPb data
    {
      // ************************** yields for 1D RAA 
      if(il==16 || il==17 || il==19 || il==20 ||il==22 || il==23 || il==25 || il==26 || il==28 || il==29 || il==32 || il==34) 
      {
        // *prompt*  centrality
        if(il == 16) binn = 12; // 0-5
        if(il == 29) binn = 11; // 5-10
        if(il == 17) binn = 10; // 10-15
        if(il == 19) binn = 9; // 15-20
        if(il == 20) binn = 8; // 20-25
        if(il == 22) binn = 7; // 25-30
        if(il == 23) binn = 6; // 30-35
        if(il == 25) binn = 5; // 35-40
        if(il == 26) binn = 4; // 40-45
        if(il == 28) binn = 3; // 45-50
        if(il == 32) binn = 2; // 50- 60
        if(il == 34) binn = 1; // 60-100
        
        // fill histograms
        phPrp_cent->SetBinContent(binn,prpt[il]);
        phPrp_cent->SetBinError(binn,prptErr[il]);
        
      }

      if(il==14 || il==18 ||il==21 || il==24 || il==27 || il==30)
      {
        // *non-prompt* centrality
        if(il == 14) binn = 6; // 0-10
        if(il == 18) binn = 5; // 10-20
        if(il == 21) binn = 4; // 20-30
        if(il == 24) binn = 3; // 30-40
        if(il == 27) binn = 2; // 40-50
        if(il == 30) binn = 1; // 50-100
        phNPrp_cent->SetBinContent(binn,nprpt[il]);
        phNPrp_cent->SetBinError(binn,nprptErr[il]);
      }

      if( il==37 || il==39 || il==41 || il==42 || il==10 || il==11 || il==13) // the high pt lines
      {
        if(il == 37) binn = 1; // 6.5-7.5
        if(il == 39) binn = 2; // 7.5-8.5
        if(il == 41) binn = 3; // 8.5-9.5
        if(il == 42) binn = 4; // 9.5-11
        if(il == 10) binn = 5; // 11-13
        if(il == 11) binn = 6; // 13-16
        if(il == 13) binn = 7; // 16-30
        
        // fill histograms
        phPrp_pt->SetBinContent(binn,prpt[il]);
        phPrp_pt->SetBinError(binn,prptErr[il]);
        
        phNPrp_pt->SetBinContent(binn,nprpt[il]);
        phNPrp_pt->SetBinError(binn,nprptErr[il]);
      }//pt

      if(il==0 || il==43 || il==44 || il==46 || il==52 || il==75 ) // the y lines
      {
        if(il == 0) binn = 1;  // 0-0.4
        if(il == 43) binn = 2; // 0.4-0.8
        if(il == 44) binn = 3; // 0.8-1.2
        if(il == 46) binn = 4; // 1.2-1.6
        if(il == 52) binn = 5; // 1.6-2
        if(il == 75) binn = 6; // 2-2.4
        
        // fill histograms
        phPrp_y->SetBinContent(binn,prpt[il]);
        phPrp_y->SetBinError(binn,prptErr[il]);
        
        phNPrp_y->SetBinContent(binn,nprpt[il]);
        phNPrp_y->SetBinError(binn,nprptErr[il]);
      }//y

      // ************************** yields for 2D RAA --- high-pt
      if(il==1 || il==3 || il==4 || il==5 || il==6 || il==7) // y[0,1.2]
      {
        if(il == 1) binn = 6; // 0-10
        if(il == 3) binn = 5; // 10-20
        if(il == 4) binn = 4; // 20-30
        if(il == 5) binn = 3; // 30-40
        if(il == 6) binn = 2; // 40-50
        if(il == 7) binn = 1; // 50-100

        // fill histograms
        phPrp_y012Cent->SetBinContent(binn,prpt[il]);
        phPrp_y012Cent->SetBinError(binn,prptErr[il]);
        
        phNPrp_y012Cent->SetBinContent(binn,nprpt[il]);
        phNPrp_y012Cent->SetBinError(binn,nprptErr[il]);
      }//y12
      
      if(il>=45 && il<=51 && il!=46) // y[1.2,1.6]
      {
        if(il == 45) binn = 6; // 0-10
        if(il == 47) binn = 5; // 10-20
        if(il == 48) binn = 4; // 20-30
        if(il == 49) binn = 3; // 30-40
        if(il == 50) binn = 2; // 40-50
        if(il == 51) binn = 1; // 50-100
        
        // fill histograms
        phPrp_y1216Cent->SetBinContent(binn,prpt[il]);
        phPrp_y1216Cent->SetBinError(binn,prptErr[il]);
        
        phNPrp_y1216Cent->SetBinContent(binn,nprpt[il]);
        phNPrp_y1216Cent->SetBinError(binn,nprptErr[il]);
      }//y1216
      
      if(il>=68 && il<=74 && il!=69) // y[1.6,2.4]
      {
        if(il == 68) binn = 6; // 0-10
        if(il == 70) binn = 5; // 10-20
        if(il == 71) binn = 4; // 20-30
        if(il == 72) binn = 3; // 30-40
        if(il == 73) binn = 2; // 40-50
        if(il == 74) binn = 1; // 50-100

        // fill histograms
        phPrp_y1624Cent->SetBinContent(binn,prpt[il]);
        phPrp_y1624Cent->SetBinError(binn,prptErr[il]);
        
        phNPrp_y1624Cent->SetBinContent(binn,nprpt[il]);
        phNPrp_y1624Cent->SetBinError(binn,nprptErr[il]);
      }//y1624
      
      // ************************** yields for 2D RAA --- low-pt
      if (nBinsNpart6 == phPrp_y1624LowPtCent->GetNbinsX()) {
        if(il==56 || il==59 || il==60 || il==62 || il==64 || il==65) // centrality
        {
          if(il == 56) binn = 6; // 0-10
          if(il == 59) binn = 5; // 10-20
          if(il == 60) binn = 4; // 20-30
          if(il == 62) binn = 3; // 30-40
          if(il == 64) binn = 2; // 40-50
          if(il == 65) binn = 1; // 50-100
          
          cout << il << "prompt yield: " <<prpt[il]<<endl;
          // fill histograms
          phPrp_y1624LowPtCent->SetBinContent(binn,prpt[il]);
          phPrp_y1624LowPtCent->SetBinError(binn,prptErr[il]);
      
          phNPrp_y1624LowPtCent->SetBinContent(binn,nprpt[il]);
          phNPrp_y1624LowPtCent->SetBinError(binn,nprptErr[il]);
        }
      } else if (nBinsNpart5 == phPrp_y1624LowPtCent->GetNbinsX()) {
        if(il==56 || il==59 || il==60 || il==62 || il==63) // centrality
        {
          if(il == 56) binn = 5; // 0-10
          if(il == 59) binn = 4; // 10-20
          if(il == 60) binn = 3; // 20-30
          if(il == 62) binn = 2; // 30-40
          if(il == 63) binn = 1; // 40-100
          
          cout << il << "prompt yield: " <<prpt[il]<<endl;
          // fill histograms
          phPrp_y1624LowPtCent->SetBinContent(binn,prpt[il]);
          phPrp_y1624LowPtCent->SetBinError(binn,prptErr[il]);
      
          phNPrp_y1624LowPtCent->SetBinContent(binn,nprpt[il]);
          phNPrp_y1624LowPtCent->SetBinError(binn,nprptErr[il]);
        }
      }//y1624low

      if( il==54 || il==66 || il==67) // the 3 pt bins
      {
        if(il == 54) binn = 1; // 3.5-4.5
        if(il == 66) binn = 2; // 4.5-5.5
        if(il == 67) binn = 3; // 5.5-6.5
        
        // fill histograms
        phPrp_ptLow->SetBinContent(binn,prpt[il]);
        phPrp_ptLow->SetBinError(binn,prptErr[il]);
        
        phNPrp_ptLow->SetBinContent(binn,nprpt[il]);
        phNPrp_ptLow->SetBinError(binn,nprptErr[il]);
      }//y1624Pt

      // ************************** minbias values
      if(il==15)// minbias, pt [6.5, 30]GeV/c, |y|<2.4
      {
        binn=1;
        phPrp_mb->SetBinContent(binn,prpt[il]);
        phPrp_mb->SetBinError(binn,prptErr[il]);
        phNPrp_mb->SetBinContent(binn,nprpt[il]);
        phNPrp_mb->SetBinError(binn,nprptErr[il]);
      }
      
      if(il==1 || il==46 || il==69)// 0-100%, pt[6.5,30]
      {
        if(il==1) binn  = 1;//y[0,1.2]
        if(il==46) binn = 2;//[1.2,1.6]
        if(il==69) binn = 3;//[1.6,2.4]
     
        phPrp_y_mb->SetBinContent(binn,prpt[il]);
        phPrp_y_mb->SetBinError(binn,prptErr[il]);

        phNPrp_y_mb->SetBinContent(binn,nprpt[il]);
        phNPrp_y_mb->SetBinError(binn,nprptErr[il]);
      }
      
      if(il==57 || il==69 || il==53)// fwd bin: 3-6.5, 6.5-30, 3-30
      {
        if(il==57)binn = 1;//pT 3-6.5
        if(il==69)binn = 2;//pT 6.5-30
        if(il==53)binn = 3;//pT 3-30
        cout<<"???? bin "<<binn<<"\t prompt= "<< prpt[il]<<endl;
        phPrp_ptLow_mb->SetBinContent(binn,prpt[il]);
        phPrp_ptLow_mb->SetBinError(binn,prptErr[il]);

        phNPrp_ptLow_mb->SetBinContent(binn,nprpt[il]);
        phNPrp_ptLow_mb->SetBinError(binn,nprptErr[il]);

      }
    }// for Pbpb sample
    //-----------------------------------------
    //-----------------------------------------
    //-----------------------------------------
    //-----------------------------------------

    if(sample==1)// pp data
    {
      // ************************** yields for 1D RAA 
      if(il==8) // the high-pt MB yield, used for centrality Raa 
      {
        // *prompt* 
        for(int ibin=1; ibin<=nBinsNpart12; ibin++)
        {
          binn = ibin; 
          phPrp_cent->SetBinContent(binn,prpt[il]);
          phPrp_cent->SetBinError(binn,prptErr[il]);
        }
        // *non-prompt* 
        for(int ibin=1; ibin<=nBinsNpart6; ibin++)
        {
          binn = ibin; 
          phNPrp_cent->SetBinContent(binn,nprpt[il]);
          phNPrp_cent->SetBinError(binn,nprptErr[il]);
        }
      }// centrality

      if( il==9 || il==11 || il==13 || il==14 || il==4 || il==5 || il==7) // the high pt lines
      {
        if(il == 9)  binn = 1; // 6.5-7.5
        if(il == 11) binn = 2; // 7.5-8.5
        if(il == 13) binn = 3; // 8.5-9.5
        if(il == 14) binn = 4; // 9.5-11
        if(il == 4)  binn = 5; // 11-13
        if(il == 5)  binn = 6; // 13-16
        if(il == 7)  binn = 7; // 16-30
        
        // fill histograms
        phPrp_pt->SetBinContent(binn,prpt[il]);
        phPrp_pt->SetBinError(binn,prptErr[il]);
        
        phNPrp_pt->SetBinContent(binn,nprpt[il]);
        phNPrp_pt->SetBinError(binn,nprptErr[il]);
      }//pt

      if(il==0 || il==15 || il==16 || il==17 || il==18 || il==26 ) // the y lines
      {
        if(il == 0) binn = 1;  // 0-0.4
        if(il == 15) binn = 2; // 0.4-0.8
        if(il == 16) binn = 3; // 0.8-1.2
        if(il == 17) binn = 4; // 1.2-1.6
        if(il == 18) binn = 5; // 1.6-2
        if(il == 26) binn = 6; // 2-2.4
        
        // fill histograms
        phPrp_y->SetBinContent(binn,prpt[il]);
        phPrp_y->SetBinError(binn,prptErr[il]);
        
        phNPrp_y->SetBinContent(binn,nprpt[il]);
        phNPrp_y->SetBinError(binn,nprptErr[il]);
      }//y

      // ************************** yields for 2D RAA --- high-pt
      if(il==1) 
      {
        for(int ibin=1; ibin<=nBinsNpart6; ibin++)
        {
          binn = ibin; 
          phPrp_y012Cent->SetBinContent(binn,prpt[il]);
          phPrp_y012Cent->SetBinError(binn,prptErr[il]);
          
          phNPrp_y012Cent->SetBinContent(binn,nprpt[il]);
          phNPrp_y012Cent->SetBinError(binn,nprptErr[il]);
        }         
      }//y12
      
      if(il==17) // y[1.2,1.6]
      {
        for(int ibin=1; ibin<=nBinsNpart6; ibin++)
        {
          binn = ibin; 
          phPrp_y1216Cent->SetBinContent(binn,prpt[il]);
          phPrp_y1216Cent->SetBinError(binn,prptErr[il]);
          
          phNPrp_y1216Cent->SetBinContent(binn,nprpt[il]);
          phNPrp_y1216Cent->SetBinError(binn,nprptErr[il]);
        }
      }//y1216

      if(il==25) // y[1.6,2.4]
      {
        for(int ibin=1; ibin<=nBinsNpart6; ibin++)
        {
          binn = ibin; 
          phPrp_y1624Cent->SetBinContent(binn,prpt[il]);
          phPrp_y1624Cent->SetBinError(binn,prptErr[il]);
          
          phNPrp_y1624Cent->SetBinContent(binn,nprpt[il]);
          phNPrp_y1624Cent->SetBinError(binn,nprptErr[il]);
        }
      }//y1624
      
      // ************************** yields for 2D RAA --- low-pt
      if(il==22) // centrality
      {
        for(int ibin=1; ibin<=nBinsNpart6; ibin++)
        {
          binn = ibin; 
          phPrp_y1624LowPtCent->SetBinContent(binn,prpt[il]);
          phPrp_y1624LowPtCent->SetBinError(binn,prptErr[il]);
          
          phNPrp_y1624LowPtCent->SetBinContent(binn,nprpt[il]);
          phNPrp_y1624LowPtCent->SetBinError(binn,nprptErr[il]);
        }
      }//y1624low

      if( il==20 || il==23 || il==24) // the 3 pt bins
      {
        if(il == 20) binn = 1; // 3.5-4.5
        if(il == 23) binn = 2; // 4.5-5.5
        if(il == 24) binn = 3; // 5.5-6.5
        
        // fill histograms
        phPrp_ptLow->SetBinContent(binn,prpt[il]);
        phPrp_ptLow->SetBinError(binn,prptErr[il]);
        
        phNPrp_ptLow->SetBinContent(binn,nprpt[il]);
        phNPrp_ptLow->SetBinError(binn,nprptErr[il]);
      }//y1624Pt

      // ************************** minbias values
      if(il==8)// minbias, pt [6.5, 30]GeV/c, |y|<2.4
      {
        binn=1;
        phPrp_mb->SetBinContent(binn,prpt[il]);
        phPrp_mb->SetBinError(binn,prptErr[il]);
        phNPrp_mb->SetBinContent(binn,nprpt[il]);
        phNPrp_mb->SetBinError(binn,nprptErr[il]);
      }
    
      if(il==1 || il==17 || il==25)// 0-100%, pt[6.5,30]
      {
        if(il==1) binn  = 1;//y[0,1.2]
        if(il==17) binn = 2;//[1.2,1.6]
        if(il==25) binn = 3;//[1.6,2.4]
     
        phPrp_y_mb->SetBinContent(binn,prpt[il]);
        phPrp_y_mb->SetBinError(binn,prptErr[il]);

        phNPrp_y_mb->SetBinContent(binn,nprpt[il]);
        phNPrp_y_mb->SetBinError(binn,nprptErr[il]);
      }
    
      if(il==22 || il==25 || il==19)// fwd bin: 3-6.5, 6.5-30, 3-30
      {
        if(il==22) binn = 1;//pT 3-6.5
        if(il==25) binn = 2;//pT 6.5-30
        if(il==19) binn = 3;//pT 3-30
   
        phPrp_ptLow_mb->SetBinContent(binn,prpt[il]);
        phPrp_ptLow_mb->SetBinError(binn,prptErr[il]);

        phNPrp_ptLow_mb->SetBinContent(binn,nprpt[il]);
        phNPrp_ptLow_mb->SetBinError(binn,nprptErr[il]);

      }
    }// for pp sample

  }//for each line in the fit_table file
  if(sample==0)   cout<<"#######  Done filling the PbPb data histograms!"<<endl;
  if(sample==1)   cout<<"#######  Done filling the pp data histograms!"<<endl;
      
  // write all histos in the output file
/*  phPrp_cent->Write();
  phNPrp_cent->Write();

  phPrp_y->Write();
  phNPrp_y->Write();

  phPrp_pt->Write();
  phNPrp_pt->Write();
  
  phPrp_ptLow->Write();
  phNPrp_ptLow->Write();

  phPrp_y012Cent->Write();
  phNPrp_y012Cent->Write();

  phPrp_y1216Cent->Write();
  phNPrp_y1216Cent->Write();

  phPrp_y1624Cent->Write();
  phNPrp_y1624Cent->Write();

  phPrp_y1624LowPtCent->Write();
  phNPrp_y1624LowPtCent->Write(); 
*/
  in.close();
  pfOutput->Write();
  cout<<"####### Wrote hists to file, and closed file! Good luck!"<<endl;
  
}

