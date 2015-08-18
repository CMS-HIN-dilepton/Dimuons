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
#include "/Users/eusmartass/Documents/cmswrite/hin-14-005/macroraa/dataBinning_2015.h"

void makeHisto_raa(int sample = 0, // 0=PbPb,     1= pp
		   const char* inputFitDataFileLocation = "/Users/eusmartass/Documents/cmswrite/hin-14-005/excel",
		   const char* outputHistDataFile       = "histRaaEff"
		   ) 
{
  const char* whichSample[2] = {"pbpb_tradEff_nov12","pp_tradEff_nov12"};
 
  ifstream in;
  in.open(Form("%s/%s.dat",inputFitDataFileLocation,whichSample[sample]));
  if(in.is_open())  cout<< "Input file opened!"<<endl;
  double x[8];
 
  TFile *pfOutput;
  pfOutput = new TFile(Form("%s_%s.root",outputHistDataFile,whichSample[sample]),"RECREATE");

  double prpt[300], prptErr[300], nprpt[300], nprptErr[300];
  double inc[300], incErr[300];
  double rap1[300], rap2[300], pT1[300], pT2[300];
  int cent1[300], cent2[300];
  char tmp[512];
  int nline = 0;
  string line; //this will contain the data read from the file

  while (!in.eof()) 
    {
      in >> x[0] >> x[1] >> x[2] >> x[3] >> x[4] >> x[5] >> x[6] >> x[7];
      rap1[nline]  = x[0];  rap2[nline]     = fabs(x[1]); // rapidity (second value comes with '-')
      pT1[nline]   = x[2];  pT2[nline]      = fabs(x[3]); // pt (second value comes with '-')
      cent1[nline] = x[4];  cent2[nline]    = fabs(x[5]); // centrlaity (second value comes with '-')
      
      prpt[nline]  = x[6]; // prompt correction
      nprpt[nline] = x[7]; // non-prompt correction
  
      // cout<<"Reading "<< x[0]<<" "<<x[1]<<" "<<x[2]<<" "<<x[3]<<" "<<x[4]<<" "<<x[5]<<" "<<x[6]<<" "<<x[7]<<endl;   
      if (!in.good()) {cout<<"input file not found"<<endl; break;}
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
  TH1F *phPrp_mb      = new TH1F("phPrp_mb", ";N_{part};Yield",nBinsMB,binsMB);
  TH1F *phNPrp_mb     = new TH1F("phNPrp_mb", ";N_{part};Yield",nBinsMB,binsMB);

  TH1F *phPrp_y_mb      = new TH1F("phPrp_y_mb",  ";y;Yield",nBinsY3,binsY3);
  TH1F *phNPrp_y_mb     = new TH1F("phNPrp_y_mb", ";y;Yield",nBinsY3,binsY3);

  TH1F *phPrp_ptLow_mb  = new TH1F("phPrp_ptLow_mb", ";p_{T};Yield",nBinsPt3, binsPt3_mb);
  TH1F *phNPrp_ptLow_mb = new TH1F("phNPrp_ptLow_mb", ";p_{T};Yield",nBinsPt3, binsPt3_mb);
  
  // 1D histos
  TH1F *phPrp_cent   = new TH1F("phPrp_cent", ";N_{part};Yield",nBinsNpart12,binsNpart12);
  TH1F *phNPrp_cent  = new TH1F("phNPrp_cent",";N_{part};Yield",nBinsNpart6,binsNpart6);

  TH1F *phPrp_y    = new TH1F("phPrp_y",";y;Yield",nBinsY,binsY);
  TH1F *phNPrp_y   = new TH1F("phNPrp_y",";y;Yield",nBinsY,binsY);

  TH1F *phPrp_pt  = new TH1F("phPrp_pt", ";p_{T};Yield",nBinsPt,binsPt);
  TH1F *phNPrp_pt = new TH1F("phNPrp_pt", ";p_{T};Yield",nBinsPt,binsPt);
  
  TH1F *phPrp_ptLow  = new TH1F("phPrp_ptLow", ";p_{T};Yield",nBinsPt3,binsPt3);
  TH1F *phNPrp_ptLow = new TH1F("phNPrp_ptLow", ";p_{T};Yield",nBinsPt3,binsPt3);

  // 2D histos
  TH1F *phPrp_y012Cent = new TH1F("phPrp_y012Cent", ";N_{part};Yield",nBinsNpart6,binsNpart6);
  TH1F *phNPrp_y012Cent= new TH1F("phNPrp_y012Cent", ";N_{part};Yield",nBinsNpart6,binsNpart6);

  TH1F *phPrp_y1216Cent = new TH1F("phPrp_y1216Cent", ";N_{part};Yield",nBinsNpart6,binsNpart6);
  TH1F *phNPrp_y1216Cent= new TH1F("phNPrp_y1216Cent", ";N_{part};Yield",nBinsNpart6,binsNpart6);

  TH1F *phPrp_y1624Cent = new TH1F("phPrp_y1624Cent", ";N_{part};Yield",nBinsNpart6,binsNpart6);
  TH1F *phNPrp_y1624Cent = new TH1F("phNPrp_y1624Cent", ";N_{part};Yield",nBinsNpart6,binsNpart6);

  TH1F *phPrp_y1624LowPtCent  = new TH1F("phPrp_y1624LowPtCent", ";N_{part};Yield",nBinsNpart5,binsNpart5);
  TH1F *phNPrp_y1624LowPtCent = new TH1F("phNPrp_y1624LowPtCent", ";N_{part};Yield",nBinsNpart5,binsNpart5);
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

	  if( (il>=37 && il<=42) || (il>=8 && il<=13)) // the high pt lines
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
	  if(il>=1 && il<=7 && il!=2) // y[0,1.2]
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
	  if(il==56 || il==59 || il==60 || il==62 || il==63) // centrality
	    {
	      if(il == 56) binn = 5; // 0-10
	      if(il == 59) binn = 4; // 10-20
	      if(il == 60) binn = 3; // 20-30
	      if(il == 62) binn = 2; // 30-40
	      if(il == 63) binn = 1; // 40-100
	      
	      // fill histograms
	      phPrp_y1624LowPtCent->SetBinContent(binn,prpt[il]);
	      phPrp_y1624LowPtCent->SetBinError(binn,prptErr[il]);
	  
	      phNPrp_y1624LowPtCent->SetBinContent(binn,nprpt[il]);
	      phNPrp_y1624LowPtCent->SetBinError(binn,nprptErr[il]);
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
	      cout<<"???? bin "<<binn<<"\t promptYield= "<< prpt[il]<<endl;
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
	  

	  if( (il>=9 && il<=14) || (il>=2 && il<=7)) // the high pt lines
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

	  if(il==0 || (il>=15 && il==18) || il==26 ) // the y lines
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
	  cout<<"!! Reading-filling pp line"<<il<<endl;
	  
	}// for pp sample

    }//for each line in the fit_table file
  if(sample==0)   cout<<"#######  Done filling the PbPb data histograms!"<<endl;
  if(sample==1)   cout<<"#######  Done filling the pp data histograms!"<<endl;
      
  // write all histos in the output file
  phPrp_cent->Write();
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
  in.close();
  pfOutput->Write();
  cout<<"####### Wrote hists to file, and closed file! Good luck!"<<endl;
  
}

