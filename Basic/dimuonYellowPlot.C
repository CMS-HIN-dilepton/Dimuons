//
// dimuonYellowPlot.C
// Plot the dimuon invariant mass from CMS data
//


#include "tdrstyle.C"
#include "CMS_lumi.C"


dimuonYellowPlot(bool isHI=true){
  cout << "dimuonYellowPlot: Starting macro dimuonYellowPlot" << endl;
  cout << "dimuonYellowPlot: Setting styles..." << endl;
  //gROOT->SetStyle("Plain");
  // Henceforth, this macro will use only CMS TDR style:
  setTDRStyle();
  tdrStyle->SetPadTopMargin(0.05); // was 0.05
  tdrStyle->SetPadBottomMargin(0.13); // was 0.13
  tdrStyle->SetPadLeftMargin(0.13); // was 0.16
  tdrStyle->SetPadRightMargin(0.02); // was 0.02
  tdrStyle->SetTitleYOffset(1.1); // was 1.25

  cout << "dimuonYellowPlot: Opening input file, isHI=" << isHI << endl;

  TFile *inf;
  if (isHI)
    // inf = new TFile("Histos_OniaSkim_ReReco_v13_h3_sum.root","READ");
    // inf = new TFile("~/Jpsi_Histos_181611-183013_150invmub.root","READ");
    // inf = new TFile("~/Jpsi_Histos_181611-183013_lowmass_147invmub.root","READ");

    // inf = new TFile("./Jpsi_Histos_Runs_210498-211631_merged.root","READ");
    // inf = new TFile("~/Jpsi_Histos_Runs_210498-210837.root","READ");//Jpsi_Histos_Runs_210498-210534.root","READ");
    // inf = new TFile("~/LowMass_Histos_182396-182609.root","READ");
    // inf = new TFile("~/Jpsi_Histos_181912-182395.root","READ");
    inf = new TFile("pPbData_2nd_PromptReco-v1_GR_P_V43D_pileupRej_newAccCut_tot.root","READ");

  else
    // inf = new TFile("~/Jpsi_Histos_ppReReco_lowmass.root","READ");
    // inf = new TFile("/Users/eusmartass/Software/data2013/Jpsi_Histos_Runs_211739-211831_HFvars.root","READ");
    // inf = new TFile("Histos_OniaSkim_HiReReco_pp2760_v5h0_sum.root","READ");
    // inf = new TFile("~/Jpsi_Histos_ppReReco.root","READ");
    // inf = new TFile("Histos_OniaSkim_pp2760_v1h1_sum.root","READ");

  cout << "dimuonYellowPlot: booking histograms..." << endl;

  double bins[100000];
  bins[0] = 0.2;
  int nBins = 0;
  for (int i=1; bins[i-1]<200; i++) {
    //    bins[i] = bins[i-1]*1.02;//1.04;//1.015;//
    bins[i] = bins[i-1]*1.015;//1.04;//1.015;//
    //    bins[i] = bins[i-1]*1.012;//1.04;//1.015;//
    nBins++;
  }
  cout << "dimuonYellowPlot: making mass histogram with " << nBins << " bins" << endl;
  TH1F* h = new TH1F("h","h;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
  TH1F* h1 = new TH1F("h1","h1;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
  TH1F* h2 = new TH1F("h2","h2;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
  TH2F* h3 = new TH2F("h3","h3;#eta_{#mu^{+}} - #eta_{#mu^{-}};#mu^{+}#mu^{-} mass",500,-5.0,5.0,nBins,bins);

  TCanvas* yellowPlot = new TCanvas("yellowPlot","yellowPlot",500,500);
  // yellowPlot->SetTopMargin(0.03);
  // yellowPlot->SetRightMargin(0.05);
  yellowPlot->SetLogx();
  yellowPlot->SetLogy();

  cout << "dimuonYellowPlot: Draw histograms from tree..." << endl;

  if (isHI) {
    //    myTree->Draw("Reco_QQ_4mom.M()>>h","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==0","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h1","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign!=0","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h1","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==1","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h2","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==2","e");

    //    myTree->Draw("Reco_QQ_4mom.M()>>h","(HLTriggers&1)==1&&(Reco_QQ_trig&1)==1&&Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_4mom.Pt()>6.5","e");

    /*
    myTree->Draw("Reco_QQ_4mom.M()>>h","Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4.0&&Reco_QQ_mumi_4mom.Pt()>4.0","e");
    myTree->Draw("Reco_QQ_4mom.M()>>h1","Reco_QQ_type==0&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4.0&&Reco_QQ_mumi_4mom.Pt()>4.0","e");
    */
    myTree->Draw("Reco_QQ_4mom.M()>>h","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e",10000);
    //myTree->Draw("Reco_QQ_4mom.M()>>h1","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e");

    //    myTree->Draw("Reco_QQ_4mom.M():(Reco_QQ_mupl_4mom.Eta()-Reco_QQ_mumi_4mom.Eta())>>h3","(HLTriggers&1)==1&&(Reco_QQ_trig&1)==1&&Reco_QQ_type==0&&Reco_QQ_sign==0","colz");
  }
  else {
    //   myTree->Draw("Reco_QQ_4mom.M()>>h","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e");
    // myTree->Draw("Reco_QQ_4mom.M()>>h1","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e");

    myTree->Draw("Reco_QQ_4mom.M()>>h","Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e",100000);
    //myTree->Draw("Reco_QQ_4mom.M()>>h1","Reco_QQ_type==2&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e",40000);

    // myTree->Draw("Reco_QQ_4mom.M()>>h","Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e");
    // myTree->Draw("Reco_QQ_4mom.M()>>h1","Reco_QQ_type==0&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h1","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==1","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h2","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==2","e");
  }


  cout << "dimuonYellowPlot: Scaling histograms..." << endl;

  // Scale by bin width, needs to be done bin by bin because bin widths are different.
  for (int i=1; i<=h1->GetNbinsX(); i++) {
    h->SetBinContent(i, h->GetBinContent(i)/h->GetBinWidth(i));
    h1->SetBinContent(i, h1->GetBinContent(i)/h1->GetBinWidth(i));
  }
  //  h->Add(h1,-1);


  h->GetXaxis()->SetRangeUser(2,2e2);
  h->GetYaxis()->SetRangeUser(0.2,5e5);
  // h->GetXaxis()->CenterTitle(kTRUE);
  h->SetFillColor(kYellow);
  h->Draw(""); 
  //  h1->Draw("same");

  

  // Options to be used with the CMS_lumi macro
  writeExtraText = true;       // if extra text
  extraText  = "Preliminary";  // default extra text is "Preliminary"
  int iPeriod = 33;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  float H = gPad->GetWh();
  float W = gPad->GetWw();
  float l = gPad->GetLeftMargin();
  float t = gPad->GetTopMargin();
  float r = gPad->GetRightMargin();
  float b = gPad->GetBottomMargin();
  posX_ =   l + 0.045*(1-l-r)*W/H;
  posY_ = 1-t - 0.045*(1-t-b);
  float xl_0 = posX_;
  float yl_0 = posY_ - 0.1;
  float xl_1 = posX_ + 0.1*H/W;
  float yl_1 = posY_;
  TASImage* CMS_logo = new TASImage("CMS-Color.gif");
  TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
  pad_logo->Draw();
  pad_logo->cd();
  CMS_logo->Draw("X");
  pad_logo->Modified();
  yellowPlot->cd();
  
  CMS_lumi(yellowPlot,99,33);


  // Booking the TLatex class and setting the parameters
  // CMS Preliminary (not used anymore) left as a dummy
  TLatex* latex = new TLatex(0.62,0.88,"CMS Preliminary");
  latex->SetNDC();
  /*
  latex->DrawLatex(0.24,0.88,"#rho, #omega");
  latex->DrawLatex(0.32,0.88,"#phi");
  */
  latexJpsi = latex->DrawLatex(0.22,0.68,"J/#psi");
  // tex = new TLatex(0.38,0.94,"J/#psi");
  latexPsiPrime = latex->DrawLatex(0.26,0.6,"#psi(2S)");
  //  tex = new TLatex(0.45,0.82,"#psi(2S)");
  latexUpsi = latex->DrawLatex(0.45,0.7,"#varUpsilon(1,2,3S)");
  //  tex = new TLatex(0.59,0.76,"#varUpsilon(1,2,3S)");
  latexZ0 = latex->DrawLatex(0.8,0.5,"Z");
  // tex = new TLatex(0.84,0.57,"Z");

  latexPtCut = latex->DrawLatex(0.25,0.20,"p_{T}^{#mu} > 4 GeV/c");

  return;
}
