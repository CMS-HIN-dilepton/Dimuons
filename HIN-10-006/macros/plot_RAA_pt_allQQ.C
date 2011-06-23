#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TROOT.h"
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

// define the graphs:
// forward
TGraphErrors *pgPhenix_pt_y1222;
TGraphAsymmErrors *pgPhenix_pt_y1222_box;


// barrel
TGraphErrors *pgPhenix_pt_y035;
TGraphErrors *pgPhenix_pt_y035_box;

TGraphErrors *pgStar_pt;
TGraphErrors *pgStar_pt_box;

void LoadSTAR();
void LoadPHENIX();
void LoadCMS();
void LoadCDF();

TGraphAsymmErrors *gB_mid, *gB_inter, *gB_FWD;
TGraphErrors *gB_mid_syst, *gB_inter_syst, *gB_FWD_syst;
TGraphErrors *gB_mid_tot, *gB_inter_tot, *gB_FWD_tot;
TGraphErrors *gB_CDF_syst, *gB_CDF, *gB_CDF_tot;

int const kNbRap = 3;
double rapRange[kNbRap+1] = {0.0, 0.0, 1.6, 2.4};
int const kNbpTMid = 3;
int const kNbpTInter = 4;
int const kNbpTFWD = 8;

int const colourLHCb = 2;
int const colourALICE = kMagenta-3;
int colourCMS[3] = {kCyan-3, kBlue-3, kBlue+3};
int const colourATLAS[3] = {3,kGreen-1,3};
//
int  markerCMS[3] = {21,20,20}; 
int const markerLHCb = 29;
int const markerALICE = 25;
int const markerATLAS[3] = {22,23,22};

void plot_RAA_pt_allQQ(bool isPaper=false, bool plotPhenix=true, bool plotStar=true)
{
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(5);
  
  double ptBins[] = {8.11,13.22}; // 0--2.4
  double ptBinsCoarse[] = {8.92}; //for 1.6--2.4
  double ptError[] = {0.0,0.0};
  double ptErrorCoarse[] = {0.0};

  double ptBinsLow[] = {1.608,3.22};
  double ptBinsUp[] = {1.89,16.778};

  double ptBinsCoarseLow[] = {2.418};
  double ptBinsCoarseUp[] = {21.078};
  
  double upsPtBins[] = {3.1, 8.2, 13.1}; // check!
  double upsPtBinsError[] = {0.0, 0.0, 0.0};

  double upsPtBinsLow[] = {3.08,1.7,3.1};
  double upsPtBinsUp[] = {3.4,1.8,6.88};
  

  double inclJpsiPt[] = {0.307, 0.298};
  double inclJpsiPtError[] = {0.027, 0.035};
  double inclJpsiPtSyst[] = {0.027, 0.019};
  
  double promptJpsiPt[] = {0.296, 0.299}; 
  double promptJpsiPtError[] = {0.030, 0.042};
  double promptJpsiPtSyst[] = {0.026, 0.019};
  
  double upsPt[] = {0.427, 0.883, 1.723}; 
  double upsPtError[] = {0.101, 0.374, 0.742};
  double upsPtSyst[] = {0.066, 0.140, 0.251};
  
  double ptSystX[] = {0.75, 0.75, 0.75};



  TGraphErrors *gInclJpsiPt = new TGraphErrors(2, ptBins, inclJpsiPt, ptError, inclJpsiPtError);
  TGraphErrors *gPromptJpsiPt = new TGraphErrors(2, ptBins, promptJpsiPt, ptError, promptJpsiPtError);
  TGraphErrors *gUpsPt = new TGraphErrors(3, upsPtBins, upsPt, upsPtBinsError, upsPtError);

  TGraphErrors *gInclJpsiPtSyst = new TGraphErrors(2, ptBins, inclJpsiPt, ptSystX, inclJpsiPtSyst);
  TGraphErrors *gPromptJpsiPtSyst = new TGraphErrors(2, ptBins, promptJpsiPt, ptSystX, promptJpsiPtSyst);
  TGraphErrors *gUpsPtSyst = new TGraphErrors(3, upsPtBins, upsPt, ptSystX, upsPtSyst);

  TGraphAsymmErrors *gInclJpsiPtP = new TGraphAsymmErrors(2, ptBins, inclJpsiPt, ptBinsLow, ptBinsUp, ptError, ptError);
  TGraphAsymmErrors *gPromptJpsiPtP = new TGraphAsymmErrors(2, ptBins, promptJpsiPt, ptBinsLow, ptBinsUp,  ptError, ptError);
  TGraphAsymmErrors *gUpsPtP = new TGraphAsymmErrors(3, upsPtBins, upsPt, upsPtBinsLow, upsPtBinsUp, upsPtBinsError, upsPtBinsError);

  gInclJpsiPtSyst->SetFillColor(kAzure-9);
  gPromptJpsiPtSyst->SetFillColor(kRed-9);
  gUpsPtSyst->SetFillColor(kGreen-9);

  if (isPaper) {
    gInclJpsiPt->SetMarkerColor(kBlue+1);
    gPromptJpsiPt->SetMarkerColor(kRed+2);
  }
  else {
    gInclJpsiPt->SetMarkerColor(kBlue);
    gPromptJpsiPt->SetMarkerColor(kRed);
  }
  gUpsPt->SetMarkerColor(kGreen+2);

  gInclJpsiPt->SetMarkerStyle(20);
  gPromptJpsiPt->SetMarkerStyle(21);
  gUpsPt->SetMarkerStyle(33);

  gInclJpsiPt->SetMarkerSize(1.2);
  gPromptJpsiPt->SetMarkerSize(1.2);
  gUpsPt->SetMarkerSize(2.0);

  gInclJpsiPtP->SetMarkerStyle(24);
  gPromptJpsiPtP->SetMarkerStyle(25);
  gUpsPtP->SetMarkerStyle(27);

  gInclJpsiPtP->SetMarkerSize(1.2);
  gPromptJpsiPtP->SetMarkerSize(1.2);
  gUpsPtP->SetMarkerSize(2.0);
  
  // gInclJpsiPtP->SetLineColor(kBlue+1);
  // gPromptJpsiPtP->SetLineColor(kRed+2);
  //  gUpsPtP->SetLineColor(kGreen+2);

  // gInclJpsiPtP->SetLineWidth(2);
  // gPromptJpsiPtP->SetLineWidth(2);
  //  gUpsPtP->SetLineWidth(2);

  LoadSTAR();
  if (plotPhenix)
    pgStar_pt->SetMarkerStyle(29);
  else
    pgStar_pt->SetMarkerStyle(30);
  pgStar_pt->SetMarkerColor(kGreen+2);
  pgStar_pt->SetMarkerSize(2);

  LoadPHENIX();
  pgPhenix_pt_y1222->SetMarkerStyle(24);
  pgPhenix_pt_y1222_box->SetMarkerStyle(24);

  pgPhenix_pt_y1222->SetMarkerColor(kBlue);
  pgPhenix_pt_y1222->SetLineColor(kBlue);

  pgPhenix_pt_y1222_box->SetLineColor(kBlue);
  pgPhenix_pt_y1222_box->SetFillStyle(0);

  pgPhenix_pt_y1222->SetMarkerSize(1.0);

  pgPhenix_pt_y035->SetMarkerStyle(25);
  pgPhenix_pt_y035_box->SetMarkerStyle(25);

  pgPhenix_pt_y035->SetMarkerColor(kBlack);
  pgPhenix_pt_y035->SetLineColor(kBlack);

  pgPhenix_pt_y035_box->SetLineColor(kBlack);
  pgPhenix_pt_y035_box->SetFillStyle(0);

  pgPhenix_pt_y035->SetMarkerSize(1.0);

  TF1 *f1 = new TF1("f1","1",0,30);
  f1->SetLineWidth(1);
  f1->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f1->GetYaxis()->SetTitle("R_{AA}");
  f1->GetYaxis()->SetRangeUser(0.0,1.5);
  f1->GetXaxis()->CenterTitle(kTRUE);

  TBox *lumiPhenix;
  lumiPhenix = new TBox(28.48,0.901,29.98,1.099);
  lumiPhenix->SetFillStyle(0);
  lumiPhenix->SetLineWidth(2);
  lumiPhenix->SetLineColor(kBlue);

  TBox *lumi = new TBox(0.0,0.93,1.5,1.07);
  lumi->SetFillColor(kGray+1);

  TCanvas *c1 = new TCanvas("c1","c1");
  f1->Draw();
  lumi->Draw();
  f1->Draw("same");

  //  gInclJpsiPtSyst->Draw("2");
  gPromptJpsiPtSyst->Draw("2");
  if (plotPhenix) {
    lumiPhenix->Draw();
    pgPhenix_pt_y1222_box->Draw("2");
    pgPhenix_pt_y035_box->Draw("2");
    pgPhenix_pt_y1222->Draw("PZ");
    pgPhenix_pt_y035->Draw("PZ");
  }
  if (plotStar) {
    pgStar_pt->Draw("PZ");
  }

  //  gInclJpsiPt->Draw("PZ");
  gPromptJpsiPt->Draw("PZ");
  //  gInclJpsiPtP->Draw("PX");
  gPromptJpsiPtP->Draw("PX");
  // gInclJpsiPtP->Draw("||");
  // gPromptJpsiPtP->Draw("||");



  //  TLegend *leg1 = new TLegend(0.22,0.56,0.90,0.76);
  TLegend *leg1a;
  if (plotPhenix && plotStar) {
    leg1a = new TLegend(0.42,0.35,0.75,0.65);
    leg1a->SetFillStyle(0);
    leg1a->SetFillColor(0);
    leg1a->SetBorderSize(0);
    leg1a->SetMargin(0.2);
    leg1a->SetTextSize(0.035);
  }
  else if (plotPhenix) {
    leg1a = new TLegend(0.42,0.45,0.75,0.65);
    leg1a->SetFillStyle(0);
    leg1a->SetFillColor(0);
    leg1a->SetBorderSize(0);
    leg1a->SetMargin(0.2);
    leg1a->SetTextSize(0.035);
  }
  else if (plotStar) {
    leg1a = new TLegend(0.42,0.50,0.75,0.65);
    leg1a->SetFillStyle(0);
    leg1a->SetFillColor(0);
    leg1a->SetBorderSize(0);
    leg1a->SetMargin(0.2);
    leg1a->SetTextSize(0.035);
  }

  TLegend *leg1 = new TLegend(0.37,0.72,0.83,0.78);
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->SetMargin(0.2);
  leg1->SetTextSize(0.04);

  //  leg1->AddEntry(gInclJpsiPt,"Inclusive J/#psi","P");
  leg1->AddEntry(gPromptJpsiPt,"Prompt J/#psi","P");
  if (plotPhenix && plotStar) {
    leg1a->SetHeader("AuAu  #sqrt{s_{NN}} = 200 GeV");
    leg1a->AddEntry(pgPhenix_pt_y035,"PHENIX: |y|<0.35","P");
    leg1a->AddEntry(pgPhenix_pt_y1222,"PHENIX: 1.2<|y|<2.2","P");
    leg1a->AddEntry(pgPhenix_pt_y1222,"(both PRL 98, 232301 (2007))","");
    leg1a->AddEntry(pgStar_pt,"STAR preliminary","P");
    leg1a->AddEntry(pgStar_pt,"0-60%, |y|<1.0","");
    leg1a->Draw();
  }
  else if (plotPhenix) {
    leg1a->SetHeader("AuAu  #sqrt{s_{NN}} = 200 GeV");
    leg1a->AddEntry(pgPhenix_pt_y035,"PHENIX: |y|<0.35","P");
    leg1a->AddEntry(pgPhenix_pt_y1222,"PHENIX: 1.2<|y|<2.2","P");
    leg1a->AddEntry(pgPhenix_pt_y1222,"(both PRL 98, 232301 (2007))","");
    leg1a->Draw();
  }
  else if (plotStar) {
    leg1a->SetHeader("AuAu  #sqrt{s_{NN}} = 200 GeV");
    leg1a->AddEntry(pgStar_pt,"STAR preliminary","P");
    leg1a->AddEntry(pgStar_pt,"0-60%, |y|<1.0","");
    leg1a->Draw();
  }
  leg1->Draw();


  TLatex *pre = new TLatex(10,1.375,"CMS Preliminary");
  pre->SetTextFont(42);
  pre->SetTextSize(0.05);
  pre->Draw();

  TLatex *l1 = new TLatex(10,1.25,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1->SetTextFont(42);
  l1->SetTextSize(0.05);
  l1->Draw();

  TLatex *lcent1 = new TLatex(21,0.16875,"0-100%");
  lcent1->Draw();
  TLatex *lpt1 = new TLatex(21,0.075,"0.0 < |y| < 2.4");
  lpt1->Draw();

  gPad->RedrawAxis();


  TF1 *f3 = new TF1("f3","1",0,20);
  f3->SetLineWidth(1);
  f3->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f3->GetYaxis()->SetTitle("R_{AA}");
  f3->GetYaxis()->SetRangeUser(0.0,2.5);
  f3->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c3 = new TCanvas("c3","c3");

  TBox *lumi3 = new TBox(0.0,0.93,1,1.07);
  lumi3->SetFillColor(kGray+1);

  f3->Draw();
  lumi3->Draw();
  f3->Draw("same");
  //  gPromptJpsiPtSyst->Draw("2");
  gUpsPtSyst->Draw("2");
  //  gPromptJpsiPt->Draw("PZ");
  gUpsPt->Draw("PZ");
  // gPromptJpsiPtP->Draw("PX");
  // gPromptJpsiPtP->Draw("||");
  gUpsPtP->Draw("PX");
  //  gUpsPtP->Draw("||");

  TLegend *leg3 = new TLegend(0.20,0.68,0.50,0.74);
  //  TLegend *leg3 = new TLegend(0.41,0.69,0.92,0.87);
  leg3->SetFillStyle(0);
  leg3->SetFillColor(kWhite);
  leg3->SetBorderSize(0);
  leg3->SetMargin(0.2);
  leg3->SetTextSize(0.04);
  //  leg3->AddEntry(gPromptJpsiPt,"Prompt J/#psi","P"); // show 1S alone?
  leg3->AddEntry(gUpsPt,"#Upsilon(1S)","P");
  leg3->Draw();


  TLatex *pre2 = new TLatex(6.66667,2.292,"CMS Preliminary");
  pre2->SetTextSize(0.05);
  pre2->Draw();

  TLatex *l2 = new TLatex(6.66667,2.0833,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l2->SetTextSize(0.05);
  l2->Draw();

  gPad->RedrawAxis();


  TLatex *lcent2 = new TLatex(14,0.28,"0-100%");
  lcent2->Draw();
  TLatex *lpt2 = new TLatex(14,0.125,"0.0 < |y| < 2.4");
  lpt2->Draw();

  if (plotPhenix && plotStar) {
    c1->SaveAs("PromptJpsi_RAA_pt_PHENIX_STAR.pdf");
    c1->SaveAs("PromptJpsi_RAA_pt_PHENIX_STAR.png");
  } 
  else if (plotPhenix) {
    c1->SaveAs("PromptJpsi_RAA_pt_PHENIX.pdf");
    c1->SaveAs("PromptJpsi_RAA_pt_PHENIX.png");
  } 
  else if (plotStar) {
    c1->SaveAs("PromptJpsi_RAA_pt_STAR.pdf");
    c1->SaveAs("PromptJpsi_RAA_pt_STAR.png");
  } 
  else {
    c1->SaveAs("PromptJpsi_RAA_pt.pdf");
    c1->SaveAs("PromptJpsi_RAA_pt.png");
  }
  c3->SaveAs("upsilon_RAA_pt.pdf");
  c3->SaveAs("upsilon_RAA_pt.png");
  return;
  /*
  LoadCMS();
  LoadCDF();

  double bfractionPt[] = {0.2368, 0.3181};  // 0--2.4
  double bfractionPtError[] = {0.0440, 0.0626};
  double bfractionPtSyst[] = {0.008, 0.00121};
 

  double bfractionCoarsePt[] = {0.1753}; // 1.6--2.4
  double bfractionCoarsePtError[] = {0.0520};
  double bfractionCoarsePtSyst[] = {0.0067};
  

  TGraphErrors *gBfractionPt = new TGraphErrors(2, ptBins, bfractionPt, ptError, bfractionPtError);
  TGraphErrors *gBfractionCoarsePt = new TGraphErrors(1, ptBinsCoarse, bfractionCoarsePt, ptErrorCoarse, bfractionCoarsePtError);

  TGraphErrors *gBfractionPtSyst = new TGraphErrors(2, ptBins, bfractionPt, ptSystX, bfractionPtSyst);
  TGraphErrors *gBfractionCoarsePtSyst = new TGraphErrors(1, ptBinsCoarse, bfractionCoarsePt, ptSystX, bfractionCoarsePtSyst);

  TGraphAsymmErrors *gBfractionPtP = new TGraphAsymmErrors(2, ptBins, bfractionPt, ptBinsLow, ptBinsUp,  ptError, ptError);
  TGraphAsymmErrors *gBfractionCoarsePtP = new TGraphAsymmErrors(1, ptBinsCoarse, bfractionCoarsePt, ptBinsCoarseLow, ptBinsCoarseUp, ptErrorCoarse, ptErrorCoarse);

  gBfractionPt->SetName("gBfractionPt");
  gBfractionCoarsePt->SetName("gBfractionCoarsePt");

  gBfractionPtSyst->SetName("gBfractionPtSyst");
  gBfractionCoarsePtSyst->SetName("gBfractionCoarsePtSyst");

  gBfractionPtP->SetName("gBfractionPtP");
  gBfractionCoarsePtP->SetName("gBfractionCoarsePtP");

  gBfractionPtSyst->SetFillColor(kRed-9);
  gBfractionCoarsePtSyst->SetFillColor(kAzure-9);

  gBfractionPt->SetMarkerStyle(20);
  gBfractionCoarsePt->SetMarkerStyle(21);

  gBfractionPtP->SetMarkerStyle(24);
  gBfractionCoarsePtP->SetMarkerStyle(25);

  gBfractionPt->SetMarkerSize(1.2);
  gBfractionCoarsePt->SetMarkerSize(1.2);

  gBfractionPtP->SetMarkerSize(1.2);
  gBfractionCoarsePtP->SetMarkerSize(1.2);

  gBfractionPt->SetMarkerColor(kRed+2);
  gBfractionCoarsePt->SetMarkerColor(kBlue+1);

  gBfractionPtP->SetLineColor(kRed+2);
  gBfractionCoarsePtP->SetLineColor(kBlue+1);

  gBfractionPtP->SetLineWidth(2);
  gBfractionCoarsePtP->SetLineWidth(2);


  TF1 *f4 = new TF1("f4","1",0,30);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f4->GetYaxis()->SetTitle("B-fraction");
  f4->GetYaxis()->SetRangeUser(0.0,1.0);
  f4->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c4 = new TCanvas("c4","c4");

  f4->Draw();

  gBfractionPtSyst->Draw("2");
  gBfractionCoarsePtSyst->Draw("2");

  gB_mid_tot->Draw("p");
  gB_inter_tot->Draw("p");
  gB_FWD_tot->Draw("p");
  gB_CDF_tot->Draw("p");

  gBfractionPt->Draw("PZ");
  gBfractionCoarsePt->Draw("PZ");

  gBfractionPtP->Draw("PX");
  gBfractionCoarsePtP->Draw("PX");
  gBfractionPtP->Draw("||");
  gBfractionCoarsePtP->Draw("||");

  TLegend *leg4 = new TLegend(0.10,0.60,0.70,0.95);
  leg4->SetFillStyle(0);
  leg4->SetFillColor(kWhite);
  leg4->SetBorderSize(0);
  leg4->SetMargin(0.2);
  leg4->SetTextSize(0.04);
  leg4->AddEntry(gBfractionPt,"CMS PbPb  #sqrt{s_{NN}} = 2.76 TeV (|y|<2.4)","P"); // add pt info
  leg4->AddEntry(gBfractionCoarsePt,"CMS PbPb  #sqrt{s_{NN}} = 2.76 TeV (1.6<|y|<2.4)","P");
  leg4->AddEntry(gB_FWD_tot,"CMS pp  #sqrt{s} = 7 TeV","");
  leg4->AddEntry(gB_FWD_tot, "1.6 < |y| < 2.4", "p");
  leg4->AddEntry(gB_inter_tot, "1.2 < |y| < 1.6", "p");
  leg4->AddEntry(gB_mid_tot, "|y| < 1.2", "p");
  leg4->AddEntry(gB_CDF,"CDF pp  #sqrt{s} = 1.96 TeV (|y|<0.6)","P");
  leg4->Draw();

  TLatex *l4 = new TLatex(17,0.90,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l4->SetTextSize(0.05);
  //  l4->Draw();

  TLatex *pre4 = new TLatex(17,0.05,"CMS Preliminary");
  pre4->SetTextSize(0.05);
  pre4->Draw();

  gPad->RedrawAxis();

  c4->SaveAs("bFraction_pt.pdf");

  */
  return;
}

void LoadCMS(){

  // int colourCMS[] = {2,4,1};
  // int markerCMS[] = {24,25,27};
  // int markerSizeCMS[] = {0.9, 0.7, 0.8};

  //redefine the CMS styles:
  colourCMS[0] = kRed+2; colourCMS[1] = kBlue+1; colourCMS[2] = 1;
  markerCMS[0] = 24; markerCMS[1] = 27; markerCMS[2] = 25;
  // markerCMS[0] = 20; markerCMS[1] = 21; markerCMS[2] = 29;
  double markerSizeCMS[] = {1.0, 2.0, 1.0};

  //================================
  //BPH-10-002, 314 nb-1
  //================================
  //mid-rapidity, |y| < 1.2
  double bFraction_mid[kNbpTMid] = {0.179, 0.257, 0.395};
  double bFrac_Stat_mid[kNbpTMid] = {0.024, 0.015, 0.018};
  double bFrac_Syst_mid[kNbpTMid] = {0.015, 0.014, 0.005};
  double bFrac_tot_mid[kNbpTMid];
  double pTMid[kNbpTMid] = {5.46, 8.14, 13.5};
  double pTMid_Err[kNbpTMid] = {0.56, 0.97, 3.53};
  double pTRange_mid[kNbpTMid+1] = {4.5,6.5,10.,30.};
  double pTL_mid[kNbpTMid], pTR_mid[kNbpTMid];
  double errPTSyst_mid[kNbpTMid];
  for(int ipT = 0; ipT < kNbpTMid; ipT++){
    // pTL_mid[ipT] = 0.;
    // pTR_mid[ipT] = 0.;
    pTMid_Err[ipT] = 0.; //don't show the RMS of the data
    pTL_mid[ipT] = pTMid_Err[ipT];
    pTR_mid[ipT] = pTMid_Err[ipT];
    errPTSyst_mid[ipT] = 0.1;
    bFrac_tot_mid[ipT] = sqrt(pow(bFrac_Stat_mid[ipT],2.) + pow(bFrac_Syst_mid[ipT],2.));
  }
  gB_mid = new TGraphAsymmErrors(kNbpTMid, pTMid, bFraction_mid, pTL_mid, pTR_mid, bFrac_Stat_mid, bFrac_Stat_mid);
  gB_mid->SetMarkerStyle(markerCMS[0]);
  gB_mid->SetMarkerColor(colourCMS[0]);
  gB_mid->SetLineColor(colourCMS[0]);
  //  gB_mid->SetLineWidth(2.);
  gB_mid->SetMarkerSize(markerSizeCMS[0]);
  gB_mid_syst = new TGraphErrors(kNbpTMid, pTMid, bFraction_mid, errPTSyst_mid, bFrac_Syst_mid);
  // gB_mid_syst->SetLineColor(colourCMS[0]);
  gB_mid_syst->SetLineColor(colourCMS[0]);
  gB_mid_syst->SetFillStyle(0);

  gB_mid_tot = new TGraphErrors(kNbpTMid, pTMid, bFraction_mid, pTMid_Err, bFrac_tot_mid);
  gB_mid_tot->SetLineColor(colourCMS[0]);
  //  gB_mid_tot->SetLineWidth(2.);
  gB_mid_tot->SetMarkerColor(colourCMS[0]);
  gB_mid_tot->SetMarkerStyle(markerCMS[0]);
  gB_mid_tot->SetMarkerSize(markerSizeCMS[0]);

  //remove the first data point:
  gB_mid->RemovePoint(0);
  gB_mid_syst->RemovePoint(0);
  gB_mid_tot->RemovePoint(0);

  //intermediate-rapidity, 1.2 < |y| < 1.6
  double bFraction_inter[kNbpTInter] = {0.146, 0.180, 0.203, 0.360};
  double bFrac_Stat_inter[kNbpTInter] = {0.021, 0.017, 0.017, 0.031};
  double bFrac_Syst_inter[kNbpTInter] = {0.028, 0.019, 0.014, 0.016};
  double bFrac_tot_inter[kNbpTInter];
  double pTInter[kNbpTInter] = {3.27, 5.48, 7.89, 12.96};
  double pTInter_Err[kNbpTInter] = {0.75, 0.55, 0.93, 3.06};
  double pTRange_inter[kNbpTInter+1] = {2.0,4.5,6.5,10.,30.};
  double pTL_inter[kNbpTInter], pTR_inter[kNbpTInter];
  double errPTSyst_inter[kNbpTInter];
  for(int ipT = 0; ipT < kNbpTInter; ipT++){
    // pTL_inter[ipT] = 0.;
    // pTR_inter[ipT] = 0.;
    pTInter_Err[ipT] = 0.; //don't show the RMS of the data
    pTL_inter[ipT] = pTInter_Err[ipT];
    pTR_inter[ipT] = pTInter_Err[ipT];
    errPTSyst_inter[ipT] = 0.1;
    bFrac_tot_inter[ipT] = sqrt(pow(bFrac_Stat_inter[ipT],2.) + pow(bFrac_Syst_inter[ipT],2.));
  }
  gB_inter = new TGraphAsymmErrors(kNbpTInter, pTInter, bFraction_inter, pTL_inter, pTR_inter, bFrac_Stat_inter, bFrac_Stat_inter);
  gB_inter->SetMarkerStyle(markerCMS[1]);
  gB_inter->SetMarkerColor(colourCMS[1]);
  gB_inter->SetLineColor(colourCMS[1]);
  gB_inter->SetMarkerSize(markerSizeCMS[1]);
  //  gB_inter->SetLineWidth(2.);
  gB_inter_syst = new TGraphErrors(kNbpTInter, pTInter, bFraction_inter, errPTSyst_inter, bFrac_Syst_inter);
  gB_inter_syst->SetLineColor(colourCMS[1]);
  gB_inter_syst->SetFillStyle(0);

  gB_inter_tot = new TGraphErrors(kNbpTInter, pTInter, bFraction_inter, pTInter_Err, bFrac_tot_inter);
  gB_inter_tot->SetLineColor(colourCMS[1]);
  //  gB_inter_tot->SetLineWidth(2.);
  gB_inter_tot->SetMarkerColor(colourCMS[1]);
  gB_inter_tot->SetMarkerStyle(markerCMS[1]);
  gB_inter_tot->SetMarkerSize(markerSizeCMS[1]);

  //forward-rapidity, 1.6 < |y| < 2.4
  double bFraction_FWD[kNbpTFWD] = {0.057, 0.087, 0.113, 0.139, 0.160, 0.177, 0.235, 0.374};
  double bFrac_Stat_FWD[kNbpTFWD] = {0.021, 0.014, 0.013, 0.014, 0.014, 0.012, 0.016, 0.031};
  double bFrac_Syst_FWD[kNbpTFWD] = {0.042, 0.022, 0.020, 0.010, 0.013, 0.012, 0.012, 0.008};
  double bFrac_tot_FWD[kNbpTFWD];
  double pT_FWD[kNbpTFWD] = {0.79, 1.60, 2.35, 3.10, 3.96, 5.35, 7.86, 13.11};
  double pT_FWD_Err[kNbpTFWD] = {0.29, 0.21, 0.22, 0.21, 0.29, 0.57, 0.97, 3.23};
  double pTRange_FWD[kNbpTFWD+1] = {0., 1.25, 2., 2.75, 3.5, 4.5, 6.5, 10., 30.};
  double pTL_FWD[kNbpTFWD], pTR_FWD[kNbpTFWD];
  double errPTSyst_FWD[kNbpTFWD];
  for(int ipT = 0; ipT < kNbpTFWD; ipT++){
    // pTL_FWD[ipT] = 0.;
    // pTR_FWD[ipT] = 0.;
    pT_FWD_Err[ipT] = 0.; //don't show the RMS of the data
    pTL_FWD[ipT] = pT_FWD_Err[ipT];
    pTR_FWD[ipT] = pT_FWD_Err[ipT];
    errPTSyst_FWD[ipT] = 0.1;
    bFrac_tot_FWD[ipT] = sqrt(pow(bFrac_Stat_FWD[ipT],2.) + pow(bFrac_Syst_FWD[ipT],2.));
  }
  gB_FWD = new TGraphAsymmErrors(kNbpTFWD, pT_FWD, bFraction_FWD, pTL_FWD, pTR_FWD, bFrac_Stat_FWD, bFrac_Stat_FWD);
  gB_FWD->SetMarkerStyle(markerCMS[2]);
  gB_FWD->SetMarkerColor(colourCMS[2]);
  gB_FWD->SetLineColor(colourCMS[2]);
  gB_FWD->SetMarkerSize(markerSizeCMS[2]);
  //  gB_FWD->SetLineWidth(2.);
  gB_FWD_syst = new TGraphErrors(kNbpTFWD, pT_FWD, bFraction_FWD, errPTSyst_FWD, bFrac_Syst_FWD);
  gB_FWD_syst->SetLineColor(colourCMS[2]);
  gB_FWD_syst->SetFillStyle(0);
  gB_FWD_tot = new TGraphErrors(kNbpTFWD, pT_FWD, bFraction_FWD, pT_FWD_Err, bFrac_tot_FWD);
  gB_FWD_tot->SetLineColor(colourCMS[2]);
  //  gB_FWD_tot->SetLineWidth(2.);
  gB_FWD_tot->SetMarkerColor(colourCMS[2]);
  gB_FWD_tot->SetMarkerStyle(markerCMS[2]);
  gB_FWD_tot->SetMarkerSize(markerSizeCMS[2]);
}

void LoadCDF(){

  //================================
  //Phys. Rev. D71 (2005) 032001
  //================================
  int const kNbPoints = 26;
  double bFraction[kNbPoints] = {0.094, 0.092, 0.085, 0.100,
				   0.091, 0.101, 0.099, 0.109,
				   0.112, 0.113, 0.133, 0.116,
				   0.126, 0.131, 0.147, 0.141,
				   0.156, 0.169, 0.182, 0.208,
				   0.227, 0.250, 0.279, 0.337,
				   0.397, 0.464};
  double bFrac_Stat[kNbPoints] = {0.010, 0.006, 0.006, 0.005,
				   0.005, 0.005, 0.005, 0.005,
				   0.005, 0.005, 0.005, 0.005,
				   0.006, 0.006, 0.007, 0.005,
				   0.006, 0.007, 0.007, 0.006,
				   0.009, 0.011, 0.012, 0.019,
				   0.025, 0.045};
  double bFrac_Syst[kNbPoints] = {0.012, 0.010, 0.009, 0.011,
				   0.010, 0.009, 0.008, 0.007,
				   0.008, 0.007, 0.007, 0.007,
				   0.007, 0.007, 0.008, 0.006,
				   0.007, 0.007, 0.008, 0.009,
				   0.007, 0.008, 0.008, 0.009,
				   0.009, 0.014};
  double pTMid[kNbPoints] = {1.38, 1.63, 1.87, 2.13, 2.38, 2.62, 
			      2.87, 3.12, 3.38, 3.62, 3.87, 4.12, 
			      4.38, 4.62, 4.88, 5.24, 5.75, 6.24, 
			      6.74, 7.45, 8.46, 9.46, 10.8, 12.8, 
			      15.2, 18.3};
  double bFrac_Tot[kNbPoints];
  double errPT[kNbPoints];
  double errPTSyst[kNbPoints];
  for(int ipT = 0; ipT < kNbPoints; ipT++){
    errPT[ipT] = 0.;
    errPTSyst[ipT] = 0.07;
    bFrac_Tot[ipT] = sqrt(pow(bFrac_Stat[ipT],2.) + pow(bFrac_Syst[ipT],2.));
  }
  gB_CDF = new TGraphErrors(kNbPoints, pTMid, bFraction, errPT, bFrac_Stat);
  gB_CDF->SetMarkerStyle(21);
  gB_CDF->SetMarkerColor(kGray+2);
  gB_CDF->SetLineColor(kGray+2);
  gB_CDF_syst = new TGraphErrors(kNbPoints, pTMid, bFraction, errPTSyst, bFrac_Syst);
  // gB_CDF_syst->SetFillColor(11);
  gB_CDF_syst->SetFillColor(5);
  gB_CDF_tot = new TGraphErrors(kNbPoints, pTMid, bFraction, errPT, bFrac_Tot);
  gB_CDF_tot->SetMarkerColor(kGray+2);
  gB_CDF_tot->SetLineColor(kGray+2);
  gB_CDF_tot->SetMarkerStyle(21);

}


void LoadPHENIX()
{
  double raa_035[] = {0.436,  0.437,   0.395,   0.316, 0.792};
  double stat_035[] = {0.046, 0.044, 0.056, 0.096, 0.324};
  double syst_035[] = {0.066, 0.066, 0.059, 0.047, 0.119};

  double pt[]={0.5,1.5,2.5,3.5,4.5};
  double raa[]= { 0.152, 0.207, 0.352, 0.323, 0.499};
  double stat_updown[]={0.016, 0.016 , 0.024, 0.036, 0.071 };
  double syst_uncorrelated_updown[]={ 0.018 , 0.011,0.035, 0.035 ,0.067 };
  double syst_correlated_updown_plus[]={ 0.022,0.021,0.045, 0.042 , 0.067};
  double syst_correlated_updown_minus[]={ 0.022 , 0.021, 0.045 ,0.042,0.067};
  double syst_global[]={0.015, 0.020, 0.035, 0.032,0.049};

  double err0[] = {0.0, 0.0, 0.0, 0.0, 0.0};
  double err[] = {0.50, 0.50, 0.50, 0.50, 0.50};

  for (int i=0;i<5;++i) {
    stat_updown[i] = sqrt(stat_updown[i]*stat_updown[i] + syst_uncorrelated_updown[i]*syst_uncorrelated_updown[i]);
  }

  pgPhenix_pt_y1222         = new TGraphErrors(5, 
					   pt, raa,
					   err0, stat_updown
					   );
  pgPhenix_pt_y1222_box  = new TGraphAsymmErrors(5, 
						 pt, raa,
						 err, err,
						 syst_correlated_updown_minus, syst_correlated_updown_plus
						 );

  pgPhenix_pt_y035         = new TGraphErrors(5, 
					      pt, raa_035,
					      err0, stat_035
					      );
  pgPhenix_pt_y035_box  = new TGraphErrors(5, 
					   pt, raa_035,
					   err, syst_035
					   );

  return;
}

void LoadSTAR()
{
  double raa[] = {0.67,  0.67,   0.54,   1.12, 0.81};
  double stat[] = {0.126, 0.095, 0.097, 0.355, 0.31};

  double pt[]={3.5,4.5,5.,6.5,7.5};

  double err0[] = {0.0, 0.0, 0.0, 0.0, 0.0};

  pgStar_pt         = new TGraphErrors(5, 
				       pt, raa,
				       err0, stat
				       );

  return;
}
