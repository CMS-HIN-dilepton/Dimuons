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

void v2_fitter(int jpsiCategory = 3, // 1 : Inclusive, 2 : Prompt, 3 : Non-Prompt
	       int varCategory  = 3, // 1: pt; 2: rapidity; 3: centrality; 4: integrated
	       int nDphiBins    = 4, 
	       // const char* inHistYieldFile_yesWeight  = "/Users/eusmartass/Documents/cmswrite/hin-14-005/v2/plots_v2/yieldsData/rootFile/20141028_yesW_2dphi_histsDPhiYields.root",
	       //const char* inHistYieldFile_noWeight  = "/Users/eusmartass/Documents/cmswrite/hin-14-005/v2/plots_v2/yieldsData/rootFile/20141028_yesW_2dphi_histsDPhiYields.root",
	       const char* inHistYieldFile_yesWeight  = "/Users/eusmartass/Documents/cmswrite/hin-14-005/v2/plots_v2/yieldsData/rootFile/20140807_weightedEff_histsDPhiYields.root",
	       const char* inHistYieldFile_noWeight  = "/Users/eusmartass/Documents/cmswrite/hin-14-005/v2/plots_v2/yieldsData/rootFile/20140807_noWeight_histsDPhiYields.root",
	       // const char* inHistYieldFile_yesWeight  = "/Users/eusmartass/Documents/cmswrite/hin-14-005/v2/plots_v2/yieldsData/rootFile/20141020_weightedEff_histsDPhiYields.root",
	       // const char* inHistYieldFile_noWeight  = "/Users/eusmartass/Documents/cmswrite/hin-14-005/v2/plots_v2/yieldsData/rootFile/20141020_noWeight_histsDPhiYields.root",

	       const char* outputFile = "rootFiles/v2Fitter_regit",
	       bool bSavePlots = true){

  const char* signal[5]      = {"","Inc","Prp","NPrp","NBkg"};
  const char* legend[5]      = {"","Inclusive J/#psi","Prompt J/#psi","Non-prompt J/#psi","Background"};
  const char* outFilePlot[4] = {"","pt","rap","cent"};

  // bin0= minbias; then have promptBins, then non-prompt bins; they are all together stored
  const int nPtBins   = 8;
  const int nYBins    = 6;
  const int nCentBins = 8;

  const char* ptBinsName[nPtBins]     = {"65300","3065","6580","80100","100300","3065","65100","100300"};
  const char* yBinsName[nYBins]       = {"0024","0012","1216","1624","0012","1224"};
  const char* centBinsName[nCentBins] = {"1060","010", "1020","2030","3060", "010","1030","3060"};

  // event plane resolution corrections
  //---------------------------------10-60%, 0-10%, 10-20%, 20-30%, 30-60%, 0-10%, 10-30%, 30-60% (minbias, prompt , non-prompt bins)
  double dEvPlResCorr[]    = { 0.8168, 0.6311, 0.8166, 0.8418, 0.7812, 0.6311,  0.8270, 0.7812};
  double dEvPlResCorrErr[] = { 0.0013, 0.0023, 0.0018, 0.0025, 0.0029, 0.0023, 0.0020, 0.0029};

  const char* ptBinsLegend[nPtBins]     = {"6.5<p_{T}<30","3<p_{T}<6.5","6.5<p_{T}<8","8<p_{T}<10","10<p_{T}<30","3<p_{T}<6.5","6.5<p_{T}<10","10<p_{T}<30"};
  const char* yBinsLegend[nYBins]       = {"|y|<2.4","|y|<1.2","1.2<|y|<1.6","1.6<|y|<2.4","|y|<1.2","1.2<|y|<2.4"};
  const char* centBinsLegend[nCentBins] = {"Cent. 10--60\%","Cent. 0-10\%", "Cent. 10-20\%","Cent. 20-30\%","Cent. 30-60\%","Cent. 0-10\%","Cent. 10-30\%","Cent. 30-60\%"};

  const int nPtBins_np   = 3;
  const int nYBins_np    = 2;
  const int nCentBins_np = 3;
  const int nPtBins_pr   = 4;
  const int nYBins_pr    = 3;
  const int nCentBins_pr = 4;

  double ptBins[nPtBins_pr+1]     = {3.0, 6.5, 8.0, 10.0, 30.0};
  double yBins[nYBins_pr+1]       = {0.0, 1.2, 1.6, 2.4};
  double centBins[nCentBins_pr+1] = {0.0, 10.0, 20.0, 30.0, 60.0};

  // non-prompt bins
  double ptBins_np[nPtBins_np+1]     = {3.0, 6.5, 10.0, 30.0};
  double yBins_np[nYBins_np+1]       = {0.0, 1.2, 2.4};
  double centBins_np[nCentBins_np+1] = {0.0, 10.0, 30.0, 60.0};



  //  gROOT->Macro("/Users/dmoon/Documents/Analysis/HiData/Run2013/rootlogon.C");
  gROOT->Macro("/Users/eusmartass/Documents/cmswrite/hin-10-006/logon.C+");
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(5);

  gStyle->SetOptFit(0);
  gStyle->SetTitleFont(62,"xyz");
  gStyle->SetLabelFont(62,"xyz");
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetMarkerSize(1.8);
  gStyle->SetErrorX(0); // disable if you want to draw horizontal error bars, e.g. when having variable bin size
  gStyle->SetEndErrorSize(3.5);
 
  // -------------- get the yields from input files
  TFile *fWeighFile   = new TFile(Form("%s",inHistYieldFile_yesWeight));
  TFile *fUnweighFile = new TFile(Form("%s",inHistYieldFile_noWeight));

  TH1F *phPhi[10]; 
  TH1F *phPhi_noWeight[10]; 
  int nBins=0;
  int nBins_np=0;
  int nBins_pr=0;
  char anaBins[10];
  const char* rapidity = yBinsName[0];
  const char* ptbin    = ptBinsName[0];
  const char* centbin  = centBinsName[0];
  switch(varCategory){
  case 1://pt
    cout<<"You are doing pt dependence!"<<endl;
    nBins = nPtBins; nBins_np = nPtBins_np;nBins_pr = nPtBins_pr;
    for(int iBin = 0; iBin < nBins; iBin++)
      {
	if(iBin==1 || iBin==5) rapidity = yBinsName[3]; // low-pt bin 3->6.5 and fwd rapidity; we add it to the pT dependence 
	else rapidity = yBinsName[0];
	TString histInc(Form("Rap_%s_pT_%s_Cent_%s_%s",rapidity,ptBinsName[iBin],centbin,signal[jpsiCategory]));
     
	cout<<"histogram input name: "<< histInc<<endl; 
	phPhi[iBin]          = (TH1F*)fWeighFile->Get(histInc);
	phPhi_noWeight[iBin] = (TH1F*)fUnweighFile->Get(histInc);
   
      }
    break;
  case 2:
    cout<<"You are doing rapidity dependence!"<<endl;
    nBins = nYBins; nBins_np = nYBins_np;nBins_pr = nYBins_pr;
    for(int iBin = 0; iBin < nBins; iBin++)
      {
	TString histInc(Form("Rap_%s_pT_%s_Cent_%s_%s",yBinsName[iBin],ptbin,centbin,signal[jpsiCategory]));
	cout<<"histogram input name: "<< histInc<<endl; 
	phPhi[iBin]          = (TH1F*)fWeighFile->Get(histInc);
	phPhi_noWeight[iBin] = (TH1F*)fUnweighFile->Get(histInc);

      }
    break;
  case 3:
    cout<<"You are doing centrality dependence!"<<endl;
    nBins = nCentBins; nBins_np = nCentBins_np; nBins_pr = nCentBins_pr;
    for(int iBin = 0; iBin < nBins; iBin++)
      {
	TString histInc(Form("Rap_%s_pT_%s_Cent_%s_%s",rapidity,ptbin,centBinsName[iBin],signal[jpsiCategory]));
	cout<<"histogram input name: "<< histInc<<endl; 
	phPhi[iBin]          = (TH1F*)fWeighFile->Get(histInc);
	phPhi_noWeight[iBin] = (TH1F*)fUnweighFile->Get(histInc);

      }
    break;
  default:
    cout<<"You are calculating the integrated bin!"<<endl;
    nBins =1;
    TString histInc(Form("Rap_%s_pT_%s_Cent_%s_%s",rapidity,ptbin,centbin,signal[jpsiCategory]));
    cout<<"histogram input name: "<< histInc<<endl; 
    phPhi[0]          = (TH1F*)fWeighFile->Get(histInc);
    phPhi_noWeight[0] = (TH1F*)fUnweighFile->Get(histInc);


    break; 
  }
 
  cout<< "Got the histograms!!!" <<endl;
 
 // -------------create the normalized histograms (1/(sum yield in all 4 dPhi bins) )
  int nDphi_yield[20][20]       = {{0}};
  double nDphi_yieldErr[20][20] = {{0}};
  int nSumDphiYield[20]={0};

  TH1F *phPhiNor[40];
  for(int iKin = 0; iKin < nBins; iKin++) // for all pt or y or cent  bins, including the minbias one
    {
      for(int ibin = 0; ibin < nDphiBins; ibin++) // add the yield in all dPhi bins
      {
	// get relative error from the 'unweighted case'
	double dRelErr_unweight = phPhi_noWeight[iKin]->GetBinError(ibin+1)/phPhi_noWeight[iKin]->GetBinContent(ibin+1);
	nDphi_yieldErr[iKin][ibin] = dRelErr_unweight*phPhi[iKin]->GetBinContent(ibin+1);
	nDphi_yield[iKin][ibin]    = phPhi[iKin]->GetBinContent(ibin+1);
	nSumDphiYield[iKin] += nDphi_yield[iKin][ibin];

	cout<<"yieldErr : "<<nDphi_yieldErr[iKin][ibin]<<"\t yield: "<< nDphi_yield[iKin][ibin] <<"\t Total: "<<nSumDphiYield[iKin]<<endl;
      }

    char tmp[512];
    // ----------------  create the normalized histograms
    double wbin = TMath::Pi()/8;
    sprintf(tmp,"phPhiNor_%d",iKin);
    phPhiNor[iKin] = new TH1F(tmp,tmp,4,0,TMath::Pi()/2);
    if(nDphiBins==2) 
      {
	phPhiNor[iKin] = new TH1F(tmp,tmp,2,0,TMath::Pi()/2);
	wbin = TMath::Pi()/4;
      }
    phPhiNor[iKin]->Sumw2();
      
    for(int ibin = 0; ibin < nDphiBins; ibin++)
      {
	phPhiNor[iKin]->SetBinContent(ibin+1,(double)nDphi_yield[iKin][ibin]/((double)nSumDphiYield[iKin]*wbin));
	phPhiNor[iKin]->SetBinError(ibin+1,(double)nDphi_yieldErr[iKin][ibin]/((double)nSumDphiYield[iKin]*wbin));
	cout<<ibin<<"\t content "<<phPhiNor[iKin]->GetBinContent(ibin+1)<<endl;
      }
    cout<<"nSumDphiYield : "<<nSumDphiYield[iKin]<<endl;
    }
  

  cout<<"Finished with the histograms!!!!!!"<<endl;


  //--------------- fit to get v2 , and store the fit parameters
  TF1 *v2Fit     = new TF1("v2Fit","[1]*(1+2*[0]*TMath::Cos(2.0*x))",-TMath::PiOver2(),TMath::PiOver2());
  //  TF1 *v2Fit    = new TF1("v2Fit","[1]*(1+2*[0]*TMath::Cos(2.0*x))",0,TMath::PiOver2());
  v2Fit->SetLineColor(kMagenta+2);
  v2Fit->SetLineStyle(2);
  v2Fit->SetLineWidth(4);

  double dFit_v2[10]    = {0.0}; 
  double dFit_v2Err[10] = {0.0};
  for(int iBin = 0; iBin < nBins; iBin++)
    {
      cout<<iBin<<" fitted!!"<<endl;
      phPhiNor[iBin]->Fit(v2Fit,"rqm"); 
      dFit_v2[iBin]    = v2Fit->GetParameter(0);
      dFit_v2Err[iBin] = v2Fit->GetParError(0);
    }

  
  // --------------- make EP resolution corrections (for all bins, used and not used)
  double dFit_v2Final[10]    = {0.0}; 
  double dFit_v2FinalErr[10] = {0.0};
  double resCorrection    = dEvPlResCorr[0];
  double resCorrectionErr = dEvPlResCorrErr[0];
  for(int iBin = 0; iBin < nBins; iBin++)
    {
      if(varCategory==3)
  	{
  	  resCorrection    = dEvPlResCorr[iBin];
  	  resCorrectionErr = dEvPlResCorrErr[iBin];
  	}
      dFit_v2Final[iBin]    = dFit_v2[iBin]/resCorrection;
      dFit_v2FinalErr[iBin] =  dFit_v2[iBin]/resCorrection * sqrt( TMath::Power(dFit_v2Err[iBin]/dFit_v2[iBin],2) + TMath::Power(resCorrectionErr/resCorrection,2) );
    }

  
  // ----- Setting v2 histograms, with different bins for prompt and non-prompt 
  TH1F *phV2;  // raw v2
  TH1F *phV2Cor;// corrected for resolution
  switch(varCategory){
  case 1://pt
    cout<<"You are doing pt dependence!"<<endl;
    phV2    = new TH1F("phV2",";p_{T} GeV/c;",nBins_pr,ptBins); 
    phV2Cor = new TH1F("phV2Cor",";p_{T} GeV/c;",nBins_pr,ptBins);
    if(jpsiCategory==3) 
    {
      phV2    = new TH1F("phV2",";p_{T} GeV/c;",nBins_np,ptBins_np);
      phV2Cor = new TH1F("phV2Cor",";p_{T} GeV/c;",nBins_np,ptBins_np);
    }

    break;
  case 2:// rap
    cout<<"You are doing rapidity dependence!"<<endl;
    phV2     = new TH1F("phV2",";|y|;",nBins_pr,yBins);
    phV2Cor  = new TH1F("phV2Cor",";|y|;",nBins_pr,yBins);
    if(jpsiCategory==3) 
    {
      phV2     = new TH1F("phV2",";|y|;",nBins_np,yBins_np);
      phV2Cor  = new TH1F("phV2Cor",";|y|;",nBins_np,yBins_np);
      cout<<"Gate 2"<<endl;
    }
    break;
  case 3://cent
    cout<<"You are doing centrality dependence!"<<endl;
    phV2    = new TH1F("phV2",";Centrality Bin;",nBins_pr,centBins);
    phV2Cor = new TH1F("phV2Cor",";Centrality Bin;",nBins_pr,centBins);

  if(jpsiCategory==3) 
    {
      phV2    = new TH1F("phV2",";Centrality Bin;",nBins_np,centBins_np);
      phV2Cor = new TH1F("phV2Cor",";Centrality Bin;",nBins_np,centBins_np);
    }

    break;
  default://minbias
    cout<<"You are calculating the integrated bin!"<<endl;

    break; 
  }
  phV2Cor->SetDirectory(0);
  phV2->SetDirectory(0);
  phV2Cor->Sumw2();
  phV2->Sumw2();

  // fill the results histograms; 
  //attention, the v2 vector contains, minbias (element 0), prompt (middle) and non-prompt (last nBins_np) results
  int binStart = 1;
  int binEnd = nBins-nBins_np;
  if(jpsiCategory==3)
    {
      binStart = nBins-nBins_np;
      binEnd   = nBins;
    }
  int bin =1;
  for(int iBin = binStart; iBin < binEnd; iBin++)
    {
      phV2->SetBinContent(bin, dFit_v2[iBin]);
      phV2->SetBinError(bin,   dFit_v2Err[iBin]); 
      
      phV2Cor->SetBinContent(bin, dFit_v2Final[iBin]);
      phV2Cor->SetBinError(bin,   dFit_v2FinalErr[iBin]); 
      bin++;
      cout<<"Bin "<<bin<<"\t v2_raw= "<<dFit_v2[iBin]<<"\t v2= "<<dFit_v2Final[iBin]<<endl;
    }
    
  
  // ----------------------------------------------------------------- DRAWING
  TLatex *lt1 = new TLatex();
  lt1->SetNDC();
  lt1->SetTextSize(0.050);

  // Draw the dPhi distributions
  TCanvas *pcDPhi = new TCanvas("pcDPhi","",900,900);
  pcDPhi->Divide(3,3);
  
  for(int iBin = 0; iBin < nBins; iBin++)
    {
      pcDPhi->cd(iBin+1);
      phPhiNor[iBin]->SetMaximum(1.0);
      phPhiNor[iBin]->SetMinimum(0.2);
      phPhiNor[iBin]->GetXaxis()->CenterTitle();
      phPhiNor[iBin]->GetYaxis()->CenterTitle();
      phPhiNor[iBin]->GetYaxis()->SetTitleOffset(1.3);
      phPhiNor[iBin]->GetYaxis()->SetTitleSize(0.05);
      phPhiNor[iBin]->GetYaxis()->SetTitle("#frac{1}{N_{total J/#psi}} #frac{dN}{d#phi}(rad^{-1})");
      phPhiNor[iBin]->GetXaxis()->SetTitle("|#phi^{J/#psi} - #Psi_{2}| (rad)");
      phPhiNor[iBin]->SetMarkerStyle(20);
      phPhiNor[iBin]->SetMarkerColor(kBlack);
      phPhiNor[iBin]->SetMarkerSize(1.5);
      phPhiNor[iBin]->Draw("e1");
      lt1->DrawLatex(0.65,0.18,Form("v_{2}= %.3f #pm %.3f",dFit_v2[iBin],dFit_v2Err[iBin]));
      if(varCategory==1) lt1->DrawLatex(0.45,0.40,Form("%s",ptBinsLegend[iBin]));
      if(varCategory==2) lt1->DrawLatex(0.60,0.40,Form("%s",yBinsLegend[iBin]));
      if(varCategory==3) lt1->DrawLatex(0.60,0.40,Form("%s",centBinsLegend[iBin]));
    }

  pcDPhi->cd(1);
  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.20,0.87,"CMS Preliminary");
  lt1->DrawLatex(0.20,0.80,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->DrawLatex(0.65,0.87,"L_{int} = 150 #mub^{-1}");
  lt1->DrawLatex(0.20,0.32,legend[jpsiCategory]);
 
  if(varCategory==1) 
    {
      lt1->DrawLatex(0.20,0.24,"Cent. 10 - 60 %, |y| < 2.4");
      pcDPhi->cd(2);
      lt1->DrawLatex(0.20,0.24,"1.6< |y| < 2.4");
      pcDPhi->cd(6);
      lt1->DrawLatex(0.20,0.24,"1.6< |y| < 2.4");
    }
  if(varCategory==2) lt1->DrawLatex(0.20,0.24,"Cent. 10 - 60 %, 6.5 < p_{T} < 30 GeV/c");
  if(varCategory==3) lt1->DrawLatex(0.20,0.24,"6.5 < p_{T} < 30 GeV/c, |y| < 2.4");

  if (bSavePlots)
    {
      pcDPhi->SaveAs(Form("figs/png/rawYield_%s_%s_ndphibins%d.png", outFilePlot[varCategory],signal[jpsiCategory],nDphiBins ));
      pcDPhi->SaveAs(Form("figs/png/rawYield_%s_%s_ndphibins%d.png", outFilePlot[varCategory],signal[jpsiCategory],nDphiBins ));
    }

  // -------------- make the results plots ---------------....
  
  TCanvas *pcResult = new TCanvas("pcResult","");

  TH1F *hPad = new TH1F("hPad",";p_{T} GeV/c;v_{2}",10,0,30);
  if(varCategory==2) hPad = new TH1F("hPad",";|y|;v_{2}",10,0,2.4);
  if(varCategory==3) hPad = new TH1F("hPad",";Centrality Bin (%);v_{2}",10,0,100);

  hPad->GetXaxis()->CenterTitle();
  hPad->GetYaxis()->CenterTitle();
  hPad->SetMaximum(0.25);
  hPad->SetMinimum(-0.05);
  hPad->Draw();

  
  TH1F *phV22    = (TH1F*)phV2->Clone("phV22");
  TH1F *phV2Cor2 = (TH1F*)phV2Cor->Clone("phV2Cor2");
  phV22->SetDirectory(0);
  phV2Cor2->SetDirectory(0);

  phV2->SetMarkerSize(1.4);
  phV22->SetMarkerSize(1.6);
  phV2Cor->SetMarkerSize(1.4);
  phV2Cor2->SetMarkerSize(1.6);

  if(jpsiCategory == 1) {// inclusive
    phV2->SetMarkerStyle(20);  
    phV2->SetMarkerColor(kBlue+1);
    phV2->SetLineColor(kBlue+1);
   
    phV2Cor->SetMarkerStyle(20);
    phV2Cor->SetMarkerColor(kBlue+1);
    phV2Cor->SetLineColor(kBlue+1);

    phV22->SetMarkerStyle(24);  
    phV22->SetMarkerColor(kBlack);
    phV22->SetLineColor(kBlue+1);
   
    phV2Cor2->SetMarkerStyle(24);   
    phV2Cor2->SetMarkerColor(kBlack);
    phV2Cor2->SetLineColor(kBlue+1);

  }
  if(jpsiCategory == 2){// prompt
    phV2->SetMarkerStyle(21);
    phV2->SetMarkerColor(kRed+2);
    phV2->SetLineColor(kRed+2);
  
    phV2Cor->SetMarkerStyle(21);
    phV2Cor->SetMarkerColor(kRed+2);
    phV2Cor->SetLineColor(kRed+2);

    phV22->SetMarkerStyle(25);
    phV22->SetMarkerColor(kBlack);
    phV22->SetLineColor(kRed+2);
   
    phV2Cor2->SetMarkerStyle(25);
    phV2Cor2->SetMarkerColor(kBlack);
    phV2Cor2->SetLineColor(kRed+2);

  }
  if(jpsiCategory == 3){// non-prompt
    phV2->SetMarkerStyle(29);
    phV2->SetMarkerColor(kOrange+2);
    phV2->SetLineColor(kOrange+2);
    phV2->SetMarkerSize(2.0);
 
    phV2Cor->SetMarkerStyle(29);
    phV2Cor->SetMarkerColor(kOrange+2);
    phV2Cor->SetLineColor(kOrange+2);
    phV2Cor->SetMarkerSize(2.0);

    phV22->SetMarkerStyle(30);
    phV22->SetMarkerSize(2.2);
    phV22->SetMarkerColor(kBlack);
    phV22->SetLineColor(kOrange+2);
  
    phV2Cor2->SetMarkerStyle(30);
    phV2Cor2->SetMarkerSize(2.2);
    phV2Cor2->SetMarkerColor(kBlack);
    phV2Cor2->SetLineColor(kOrange+2);

  }
  // ---------------- draw the v2 w/o the EP res correction
  phV2->Draw("e1 same");
  phV22->Draw("e1 same");

  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.48,0.88,"CMS Preliminary");
  lt1->DrawLatex(0.48,0.81,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->DrawLatex(0.48,0.74,"L_{int} = 150 #mub^{-1}");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.18,0.88,legend[jpsiCategory]);
 
  if(varCategory==1)// pt
    {
      lt1->DrawLatex(0.48,0.66,"Cent. 10 - 60 %");
      lt1->DrawLatex(0.48,0.60,"|y| < 2.4, 1.6<|y| < 2.4");
    }
  if(varCategory==2)//rap
    {
      lt1->DrawLatex(0.64,0.87,"Cent. 10 - 60 %");
      lt1->DrawLatex(0.21,0.28,"6.5 < p_{T} < 30 GeV/c");
    }

  if(varCategory==3)// centrality
    {
      lt1->DrawLatex(0.48,0.66,"p_{T} > 6.5 GeV/c");
      lt1->DrawLatex(0.48,0.60,"|y| < 2.4");
    }
  // save the v2 w/o resolution correction
  if(bSavePlots)
    {
      pcResult->SaveAs(Form("figs/png/v2Raw_%s_%s_nphibin%d.png", outFilePlot[varCategory],signal[jpsiCategory],nDphiBins));
      pcResult->SaveAs(Form("figs/pdf/v2Raw_%s_%s_nphibin%d.pdf", outFilePlot[varCategory],signal[jpsiCategory],nDphiBins));
    }
  
  // ------------------  draw the EP -resolution corrected v2 
  hPad->Draw();
  phV2Cor->Draw("e1 same");
  phV2Cor2->Draw("e1 same");
  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.48,0.88,"CMS Preliminary");
  lt1->DrawLatex(0.48,0.81,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->DrawLatex(0.48,0.74,"L_{int} = 150 #mub^{-1}");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.18,0.88,legend[jpsiCategory]);
 
  if(varCategory==1)// pt
    {
      lt1->DrawLatex(0.48,0.66,"Cent. 10 - 60 %");
      lt1->DrawLatex(0.48,0.60,"|y| < 2.4, 1.6<|y| < 2.4");
    }
  if(varCategory==2)//rap
    {
      lt1->DrawLatex(0.64,0.66,"Cent. 10 - 60 %");
      lt1->DrawLatex(0.21,0.28,"6.5 < p_{T} < 30 GeV/c");
    }

  if(varCategory==3)// centrality
    {
      lt1->DrawLatex(0.48,0.66,"p_{T} > 6.5 GeV/c");
      lt1->DrawLatex(0.48,0.60,"|y| < 2.4");
    }
  // save the v2 w/o resolution correction
  if(bSavePlots)
    {
      pcResult->SaveAs(Form("figs/png/v2_%s_%s_nphibin%d.png", outFilePlot[varCategory],signal[jpsiCategory],nDphiBins));
      pcResult->SaveAs(Form("figs/pdf/v2_%s_%s_nphibin%d.pdf", outFilePlot[varCategory],signal[jpsiCategory],nDphiBins));
    }


  // ---------------------------OUTPUT FILE------------------------------
  // output file with all numbers/histograms
  TFile *out = new TFile(Form("%s_%s_%s_nphibin%d.root",outputFile,outFilePlot[varCategory],signal[jpsiCategory],nDphiBins),"RECREATE");
  out->cd();

  rapidity = yBinsName[0];
  ptbin    = ptBinsName[0];
  centbin  = centBinsName[0];
  switch(varCategory){
  case 1://pt
    cout<<"You are writting pt dependence!"<<endl;
    for(int iBin = 0; iBin < nBins; iBin++)
      {
	phPhiNor[iBin]->SetName(Form("Rap_%s_pT_%s_Cent_%s_%s",rapidity,ptBinsName[iBin],centbin,signal[jpsiCategory]));
	phPhiNor[iBin]->Write();
      }
      break;
  case 2:
    cout<<"You are writting rapidity dependence!"<<endl;
    for(int iBin = 0; iBin < nBins; iBin++)
      {
	phPhiNor[iBin]->SetName(Form("Rap_%s_pT_%s_Cent_%s_%s",yBinsName[iBin],ptbin,centbin,signal[jpsiCategory]));
	phPhiNor[iBin]->Write();
      }
    break;
  case 3:
    cout<<"You are doing centrality dependence!"<<endl;
    for(int iBin = 0; iBin < nBins; iBin++)
      {
	phPhiNor[iBin]->SetName(Form("Rap_%s_pT_%s_Cent_%s_%s",rapidity,ptbin,centBinsName[iBin],signal[jpsiCategory]));
	phPhiNor[iBin]->Write();
      }
    break;
  default:
    cout<<"You are writting the integrated bin!"<<endl;
    nBins =1;
    phPhiNor[0]->SetName(Form("Rap_%s_pT_%s_Cent_%s_%s",rapidity,ptbin,centbin,signal[jpsiCategory]));
    phPhiNor[0]->Write();
    break; 
  }

  phV2->Write();
  phV22->Write();
  
  phV2Cor->Write();
  phV2Cor2->Write();
  
  out->Write();
  out->Close();
  
}


