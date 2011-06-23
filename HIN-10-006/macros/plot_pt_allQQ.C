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


void LoadCMS();
void LoadCDF();
void LoadPythia();

TGraphAsymmErrors *gB_mid, *gB_inter, *gB_FWD;
TGraphErrors *gB_mid_syst, *gB_inter_syst, *gB_FWD_syst;
TGraphErrors *gB_mid_tot, *gB_inter_tot, *gB_FWD_tot;
TGraphErrors *gB_CDF_syst, *gB_CDF, *gB_CDF_tot;

TH1F *hPromptJpsiPtPythia;
TH1F *hNonPromptJpsiPtPythia;
TH1F *hUpsPtPythia;

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

void plot_pt_allQQ(bool isPaper=false)
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
  

  double inclJpsiPt[] = {2.05, 0.40}; // this is dN/dy, need to divide by Delta pT:2.5, 20
  double inclJpsiPtError[] = {0.15, 0.04};
  double inclJpsiPtSyst[] = {0.32, 0.06};
  
  double promptJpsiPt[] = {1.56, 0.27}; // this is dN/dy, need to divide by Delta pT
  double promptJpsiPtError[] = {0.14, 0.03};
  double promptJpsiPtSyst[] = {0.24, 0.04};
  
  double nonPromptJpsiPt[] = {0.60}; // this is dN/dy, need to divide by Delta pT: 6.5, 23.5
  double nonPromptJpsiPtError[] = {0.09};
  double nonPromptJpsiPtSyst[] = {0.09};

  double upsPt[] = {0.293, 0.093, 0.066};  // this is dN/dy, need to divide by Delta pT: 6.5, 2.5, 10
  double upsPtError[] = {0.057, 0.027, 0.016};
  double upsPtSyst[] = {0.053, 0.017, 0.012};
  
  double ptSystX[] = {0.75, 0.75, 0.75};

  inclJpsiPt[0]/=2.5;
  inclJpsiPt[1]/=20.0;

  inclJpsiPtError[0]/=2.5;
  inclJpsiPtError[1]/=20.0;

  inclJpsiPtSyst[0]/=2.5;
  inclJpsiPtSyst[1]/=20.0;

  promptJpsiPt[0]/=2.5;
  promptJpsiPt[1]/=20.0;

  promptJpsiPtError[0]/=2.5;
  promptJpsiPtError[1]/=20.0;

  promptJpsiPtSyst[0]/=2.5;
  promptJpsiPtSyst[1]/=20.0;

  nonPromptJpsiPt[0]/=23.5;
  nonPromptJpsiPtError[0]/=23.5;
  nonPromptJpsiPtSyst[0]/=23.5;

  upsPt[0]/=6.5;
  upsPt[1]/=2.5;
  upsPt[2]/=10.0;

  upsPtError[0]/=6.5;
  upsPtError[1]/=2.5;
  upsPtError[2]/=10.0;

  upsPtSyst[0]/=6.5;
  upsPtSyst[1]/=2.5;
  upsPtSyst[2]/=10.0;


  TGraphErrors *gInclJpsiPt = new TGraphErrors(2, ptBins, inclJpsiPt, ptError, inclJpsiPtError);
  TGraphErrors *gPromptJpsiPt = new TGraphErrors(2, ptBins, promptJpsiPt, ptError, promptJpsiPtError);
  TGraphErrors *gNonPromptJpsiPt = new TGraphErrors(1, ptBinsCoarse, nonPromptJpsiPt, ptErrorCoarse, nonPromptJpsiPtError);
  TGraphErrors *gUpsPt = new TGraphErrors(3, upsPtBins, upsPt, upsPtBinsError, upsPtError);

  TGraphErrors *gInclJpsiPtSyst = new TGraphErrors(2, ptBins, inclJpsiPt, ptSystX, inclJpsiPtSyst);
  TGraphErrors *gPromptJpsiPtSyst = new TGraphErrors(2, ptBins, promptJpsiPt, ptSystX, promptJpsiPtSyst);
  TGraphErrors *gNonPromptJpsiPtSyst = new TGraphErrors(1, ptBinsCoarse, nonPromptJpsiPt, ptSystX, nonPromptJpsiPtSyst);
  TGraphErrors *gUpsPtSyst = new TGraphErrors(3, upsPtBins, upsPt, ptSystX, upsPtSyst);

  TGraphAsymmErrors *gInclJpsiPtP = new TGraphAsymmErrors(2, ptBins, inclJpsiPt, ptBinsLow, ptBinsUp, ptError, ptError);
  TGraphAsymmErrors *gPromptJpsiPtP = new TGraphAsymmErrors(2, ptBins, promptJpsiPt, ptBinsLow, ptBinsUp,  ptError, ptError);
  TGraphAsymmErrors *gNonPromptJpsiPtP = new TGraphAsymmErrors(1, ptBinsCoarse, nonPromptJpsiPt, ptBinsCoarseLow, ptBinsCoarseUp, ptErrorCoarse, ptErrorCoarse);
  TGraphAsymmErrors *gUpsPtP = new TGraphAsymmErrors(3, upsPtBins, upsPt, upsPtBinsLow, upsPtBinsUp, upsPtBinsError, upsPtBinsError);

  gInclJpsiPtSyst->SetFillColor(kAzure-9);
  gPromptJpsiPtSyst->SetFillColor(kRed-9);
  gNonPromptJpsiPtSyst->SetFillColor(kOrange-9);
  gUpsPtSyst->SetFillColor(kGreen-9);

  if (isPaper) {
    gInclJpsiPt->SetMarkerColor(kBlue+1);
    gPromptJpsiPt->SetMarkerColor(kRed+2);
  }
  else {
    gInclJpsiPt->SetMarkerColor(kBlue);
    gPromptJpsiPt->SetMarkerColor(kRed);
  }
  gNonPromptJpsiPt->SetMarkerColor(kOrange+2);
  gUpsPt->SetMarkerColor(kGreen+2);

  gInclJpsiPt->SetMarkerStyle(20);
  gPromptJpsiPt->SetMarkerStyle(21);
  gNonPromptJpsiPt->SetMarkerStyle(29);
  gUpsPt->SetMarkerStyle(33);

  gInclJpsiPt->SetMarkerSize(1.2);
  gPromptJpsiPt->SetMarkerSize(1.2);
  gNonPromptJpsiPt->SetMarkerSize(2.0);
  gUpsPt->SetMarkerSize(2.0);

  gInclJpsiPtP->SetMarkerStyle(24);
  gPromptJpsiPtP->SetMarkerStyle(25);
  gNonPromptJpsiPtP->SetMarkerStyle(30);
  gUpsPtP->SetMarkerStyle(27);

  gInclJpsiPtP->SetMarkerSize(1.2);
  gPromptJpsiPtP->SetMarkerSize(1.2);
  gNonPromptJpsiPtP->SetMarkerSize(2.0);
  gUpsPtP->SetMarkerSize(2.0);
  
  if (isPaper) {
    gInclJpsiPtP->SetLineColor(kBlue+1);
    gPromptJpsiPtP->SetLineColor(kRed+2);
  }
  else {
    gInclJpsiPtP->SetLineColor(kBlue);
    gPromptJpsiPtP->SetLineColor(kRed);
  }

  gNonPromptJpsiPtP->SetLineColor(kOrange+2);
  gUpsPtP->SetLineColor(kGreen+2);

  gInclJpsiPtP->SetLineWidth(2);
  gPromptJpsiPtP->SetLineWidth(2);
  gNonPromptJpsiPtP->SetLineWidth(2);
  gUpsPtP->SetLineWidth(2);

  double xbins[] = {0.0, 6.5, 10.0, 30.0};
  double xbins2[] = {0.0, 6.5, 10.0, 20.0};
  // pythia curves
  hPromptJpsiPtPythia = new TH1F("hPromptJpsiPtPythia","hPromptJpsiPtPythia",3,xbins);
  hNonPromptJpsiPtPythia = new TH1F("hNonPromptJpsiPtPythia","hNonPromptJpsiPtPythia",3,xbins);
  hUpsPtPythia = new TH1F("hUpsPtPythia","hUpsPtPythia",3,xbins2);

  LoadPythia();

  double norm = hPromptJpsiPtPythia->Integral(1,4);// include overflow for total xsection normalization

  hPromptJpsiPtPythia->Scale(0.181*5.93e-2*100.74e3/norm); // esp1*acc(|y|<2.4)*BR*xsection
  //  hPromptJpsiPtPythia->Scale(0.181*5.93e-2*100.74e3/norm); // esp1*acc(|y|<2.4)*BR*xsection
  hPromptJpsiPtPythia->SetBinContent(1, hPromptJpsiPtPythia->GetBinContent(1)/(4.8*6.5));
  hPromptJpsiPtPythia->SetBinContent(2, hPromptJpsiPtPythia->GetBinContent(2)/(4.8*3.5));
  hPromptJpsiPtPythia->SetBinContent(3, hPromptJpsiPtPythia->GetBinContent(3)/(4.8*20));

  norm = hNonPromptJpsiPtPythia->Integral(1,4);
  hNonPromptJpsiPtPythia->Scale(41.3*4.85*0.0593/norm);//100.0/107106.0*0.751*5.93e-2*4.13e7
  hNonPromptJpsiPtPythia->SetBinContent(1, hNonPromptJpsiPtPythia->GetBinContent(1)/(4.8*6.5));
  hNonPromptJpsiPtPythia->SetBinContent(2, hNonPromptJpsiPtPythia->GetBinContent(2)/(4.8*3.5));
  hNonPromptJpsiPtPythia->SetBinContent(3, hNonPromptJpsiPtPythia->GetBinContent(3)/(4.8*20));

  norm = hUpsPtPythia->Integral(1,4);
  hUpsPtPythia->Scale(1.36*0.192*0.0248e+3/norm);//100.0/600.0*2.48e-2*9.332e4
  hUpsPtPythia->SetBinContent(1, hUpsPtPythia->GetBinContent(1)/(4.8*6.5));
  hUpsPtPythia->SetBinContent(2, hUpsPtPythia->GetBinContent(2)/(4.8*3.5));
  hUpsPtPythia->SetBinContent(3, hUpsPtPythia->GetBinContent(3)/(4.8*10));

  hPromptJpsiPtPythia->SetLineColor(kRed);
  hNonPromptJpsiPtPythia->SetLineColor(kOrange+2);
  hUpsPtPythia->SetLineColor(kGreen+2);

  hPromptJpsiPtPythia->SetLineWidth(3);
  hNonPromptJpsiPtPythia->SetLineWidth(3);
  hUpsPtPythia->SetLineWidth(3);


  // pp interpolation shape
  TH1F *hpp = new TH1F("hpp","hpp",3,xbins);
  TH1F *hppLow = new TH1F("hppLow","hppLow",3,xbins);
  TH1F *hppUp = new TH1F("hppUp","hppUp",3,xbins);

  float n,pt ;
  float reach = 30 ;
  n = 3.91 ; pt = 2.38 ;
  TF1* spect2 = new TF1("Spect2","x/([0]*pow(1+[1]*[1]*x*x/[0]/[0],[2]))",0,reach);
  spect2->SetParameters(pt, TMath::Gamma(1.5) * TMath::Gamma(n-1.5) / TMath::Gamma(n-1) ,n);
  
  n = 3.81 ; pt = 2.67 ;
  TF1* spect1 = new TF1("Spect1","x/([0]*pow(1+[1]*[1]*x*x/[0]/[0],[2]))",0,reach);
  spect1->SetParameters(pt, TMath::Gamma(1.5) * TMath::Gamma(n-1.5) / TMath::Gamma(n-1) ,n);
  spect1->SetLineColor(3);
  
  n = 4.41 ; pt = 2.26 ;
  TF1* spect3 = new TF1("Spect3","x/([0]*pow(1+[1]*[1]*x*x/[0]/[0],[2]))",0,reach);
  spect3->SetParameters(pt, TMath::Gamma(1.5) * TMath::Gamma(n-1.5) / TMath::Gamma(n-1) ,n);
  spect3->SetLineColor(2);

  hpp->FillRandom("Spect2",1e6);
  hppUp->FillRandom("Spect1",1e6);
  hppLow->FillRandom("Spect3",1e6);

  hpp->Scale(226/1e6);
  hppUp->Scale(226/1e6);
  hppLow->Scale(226/1e6);
  hpp->SetBinContent(1,hpp->GetBinContent(1)/6.5);
  hpp->SetBinContent(2,hpp->GetBinContent(2)/3.5);
  hpp->SetBinContent(3,hpp->GetBinContent(3)/20.0);

  hppLow->SetBinContent(1,hppLow->GetBinContent(1)/6.5);
  hppLow->SetBinContent(2,hppLow->GetBinContent(2)/3.5);
  hppLow->SetBinContent(3,hppLow->GetBinContent(3)/20.0);

  hppUp->SetBinContent(1,hppUp->GetBinContent(1)/6.5);
  hppUp->SetBinContent(2,hppUp->GetBinContent(2)/3.5);
  hppUp->SetBinContent(3,hppUp->GetBinContent(3)/20.0);

  double x,y;
  gPromptJpsiPt->GetPoint(0,x,y);
  //  double norm = y/hpp->GetBinContent(2);
  // hpp->Scale(norm);
  // hppUp->Scale(norm);
  // hppLow->Scale(norm);


  hppUp->SetFillColor(kGray);
  hppLow->SetFillColor(10);
  hppUp->SetFillStyle(1001);
  hppLow->SetFillStyle(1001);

  hppUp->SetLineStyle(2);
  hppLow->SetLineStyle(2);

  hppUp->SetLineWidth(1);
  hppLow->SetLineWidth(1);
  hpp->SetLineWidth(1);

  TF1 *f1 = new TF1("f1","100",0,30);
  f1->SetLineWidth(1);
  f1->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f1->GetYaxis()->SetTitle("1/T_{AA} d^{2}N/dydp_{T} (nb c/GeV)");
  f1->GetYaxis()->SetTitleOffset(1.15);
  f1->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->SetLogy();

  f1->Draw();
  f1->GetYaxis()->SetRangeUser(4.0e-3, 40.0);

  hppUp->Draw("histsame");
  hppLow->Draw("histsame");
  hpp->Draw("histsame");
  //  hPromptJpsiPtPythia->Draw("histsame");
  gInclJpsiPtSyst->Draw("2");
  gPromptJpsiPtSyst->Draw("2");
  gInclJpsiPt->Draw("PZ");
  gPromptJpsiPt->Draw("PZ");
  gInclJpsiPtP->Draw("PX");
  gPromptJpsiPtP->Draw("PX");
  // gInclJpsiPtP->Draw("||");
  // gPromptJpsiPtP->Draw("||");

  //  TLegend *leg1 = new TLegend(0.22,0.56,0.90,0.76);
  //  TLegend *leg1 = new TLegend(0.50,0.54,0.90,0.79);
  TLegend *leg1 = new TLegend(0.50,0.54,0.90,0.79);
  leg1->SetFillStyle(0);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(0);
  leg1->SetMargin(0.2);
  leg1->SetTextSize(0.04);
  leg1->AddEntry(gInclJpsiPt,"Inclusive J/#psi","P");
  leg1->AddEntry(gPromptJpsiPt,"Prompt J/#psi","P");
  //  leg1->AddEntry(hPromptJpsiPtPythia,"PYTHIA: Prompt J/#psi","L");
  leg1->AddEntry(hppUp,"pp interpolation","F");
  leg1->AddEntry(hppUp,"(arXiv:1103.2394)","");
  leg1->Draw();


  TLatex *pre = new TLatex(10,18.0,"CMS Preliminary");
  pre->SetTextSize(0.05);
  pre->Draw();

  TLatex *l1 = new TLatex(10,9,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1->SetTextSize(0.05);
  l1->Draw();


  TLatex *lcent1 = new TLatex(2.3,0.012,"0-100%");
  lcent1->Draw();
  TLatex *lpt1 = new TLatex(2.3,0.006,"0.0 < |y| < 2.4");
  lpt1->Draw();

  gPad->RedrawAxis();

  TF1 *f2 = new TF1("f2","100",0,30);
  f2->SetLineWidth(1);
  f2->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f2->GetYaxis()->SetTitle("1/T_{AA} d^{2}N/dydp_{T} (nb c/GeV)");
  f2->GetYaxis()->SetTitleOffset(1.15);
  f2->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c2 = new TCanvas("c2","c2");
  c2->SetLogy();

  f2->Draw();
  f2->GetYaxis()->SetRangeUser(4.0e-3, 40.0);
  //  hNonPromptJpsiPtPythia->Draw("histsame");
  gPromptJpsiPtSyst->Draw("2");
  gNonPromptJpsiPtSyst->Draw("2");
  gPromptJpsiPt->Draw("PZ");
  gNonPromptJpsiPt->Draw("PZ");
  gPromptJpsiPtP->Draw("PX");
  gNonPromptJpsiPtP->Draw("PX");
  // gPromptJpsiPtP->Draw("||");
  // gNonPromptJpsiPtP->Draw("||");

  TLegend *leg2 = new TLegend(0.60,0.63,0.90,0.76);
  //  TLegend *leg2 = new TLegend(0.41,0.69,0.92,0.87);
  leg2->SetFillStyle(0);
  leg2->SetFillColor(kWhite);
  leg2->SetBorderSize(0);
  leg2->SetMargin(0.2);
  leg2->SetTextSize(0.04);
  leg2->AddEntry(gPromptJpsiPt,"Prompt J/#psi","P");
  leg2->AddEntry(gNonPromptJpsiPt,"Non-prompt J/#psi","P"); // drop it
  leg2->Draw();

  pre->Draw();
  l1->Draw();

  lcent1->Draw();
  lpt1->Draw();

  gPad->RedrawAxis();

  TF1 *f3 = new TF1("f3","100",0,20);
  f3->SetLineWidth(1);
  f3->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f3->GetYaxis()->SetTitle("1/T_{AA} d^{2}N/dydp_{T} (nb c/GeV)");
  f3->GetYaxis()->SetTitleOffset(1.15);
  f3->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c3 = new TCanvas("c3","c3");
  c3->SetLogy();

  f3->Draw();
  f3->GetYaxis()->SetRangeUser(2.0e-3, 0.2);
  //  hUpsPtPythia->Draw("histsame");
  //  gPromptJpsiPtSyst->Draw("2");
  gUpsPtSyst->Draw("2");
  //  gPromptJpsiPt->Draw("PZ");
  gUpsPt->Draw("PZ");
  // gPromptJpsiPtP->Draw("PX");
  // gPromptJpsiPtP->Draw("||");
  gUpsPtP->Draw("PX");
  //  gUpsPtP->Draw("||");

  TLegend *leg3 = new TLegend(0.60,0.73,0.90,0.79);
  //  TLegend *leg3 = new TLegend(0.41,0.69,0.92,0.87);
  leg3->SetFillStyle(0);
  leg3->SetFillColor(kWhite);
  leg3->SetBorderSize(0);
  leg3->SetMargin(0.2);
  leg3->SetTextSize(0.04);
  //  leg3->AddEntry(gPromptJpsiPt,"Prompt J/#psi","P"); // show 1S alone?
  leg3->AddEntry(gUpsPt,"#Upsilon(1S)","P");
  leg3->Draw();


  // TLatex *pre2 = new TLatex(6.6667,9.0,"CMS Preliminary");
  // pre2->SetTextSize(0.05);
  // pre2->Draw();

  // TLatex *l2 = new TLatex(6.66667,4.5,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  // l2->SetTextSize(0.05);
  // l2->Draw();

  // TLatex *lcent2 = new TLatex(1.5333,0.006,"0-100%");
  // lcent2->Draw();
  // TLatex *lpt2 = new TLatex(1.5333,0.003,"0.0 < |y| < 2.4");
  // lpt2->Draw();
  // lcent2->Draw();
  // lpt2->Draw();

  TLatex *pre2 = new TLatex(6.6667,0.1335,"CMS Preliminary");
  pre2->SetTextSize(0.05);
  pre2->Draw();

  TLatex *l2 = new TLatex(6.66667,0.095,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l2->SetTextSize(0.05);
  l2->Draw();

  TLatex *lcent2 = new TLatex(1.5333,0.00343,"0-100%");
  lcent2->Draw();
  TLatex *lpt2 = new TLatex(1.5333,0.00243,"0.0 < |y| < 2.4");
  lpt2->Draw();
  lcent2->Draw();
  lpt2->Draw();

  gPad->RedrawAxis();

  c1->SaveAs("inclJpsi_pt.pdf");
  c2->SaveAs("nonpromptJpsi_pt.pdf");
  c3->SaveAs("upsilon_pt.pdf");

  c1->SaveAs("inclJpsi_pt.png");
  c2->SaveAs("nonpromptJpsi_pt.png");
  c3->SaveAs("upsilon_pt.png");
  //  return;

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

  gBfractionPt->SetMarkerSize(1.8);
  gBfractionCoarsePt->SetMarkerSize(1.8);

  gBfractionPtP->SetMarkerSize(1.8);
  gBfractionCoarsePtP->SetMarkerSize(1.8);

  if (isPaper) {
    gBfractionPt->SetMarkerColor(kRed+2);
    gBfractionCoarsePt->SetMarkerColor(kBlue+1);
    gBfractionPtP->SetLineColor(kRed+2);
    gBfractionCoarsePtP->SetLineColor(kBlue+1);
  }
  else {
    gBfractionPt->SetMarkerColor(kRed);
    gBfractionCoarsePt->SetMarkerColor(kBlue);
    gBfractionPtP->SetLineColor(kRed);
    gBfractionCoarsePtP->SetLineColor(kBlue);
  }

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

  gB_mid_tot->Draw("PZ");
  gB_inter_tot->Draw("PZ");
  gB_FWD_tot->Draw("PZ");
  gB_CDF_tot->Draw("PZ");

  gBfractionPt->Draw("PZ");
  gBfractionCoarsePt->Draw("PZ");

  gBfractionPtP->Draw("PX");
  gBfractionCoarsePtP->Draw("PX");
  // gBfractionPtP->Draw("||");
  // gBfractionCoarsePtP->Draw("||");

  TLegend *leg4 = new TLegend(0.10,0.65,0.70,0.95);
  leg4->SetFillStyle(0);
  leg4->SetFillColor(kWhite);
  leg4->SetBorderSize(0);
  leg4->SetMargin(0.2);
  leg4->SetTextSize(0.04);
  leg4->AddEntry(gBfractionPt,"CMS PbPb  #sqrt{s_{NN}} = 2.76 TeV (|y|<2.4)","P"); // add pt info
  leg4->AddEntry(gBfractionCoarsePt,"CMS PbPb  #sqrt{s_{NN}} = 2.76 TeV (1.6<|y|<2.4)","P");
  leg4->AddEntry(gB_FWD_tot, "CMS pp  #sqrt{s} = 7 TeV (1.6 < |y| < 2.4)", "p");
  leg4->AddEntry(gB_inter_tot, "CMS pp  #sqrt{s} = 7 TeV (1.2 < |y| < 1.6)", "p");
  leg4->AddEntry(gB_mid_tot, "CMS pp  #sqrt{s} = 7 TeV (|y| < 1.2)", "p");
  leg4->AddEntry(gB_CDF,"CDF p#bar{p}  #sqrt{s} = 1.96 TeV (|y|<0.6)","P");
  leg4->Draw();

  TLatex *l4 = new TLatex(16,0.90,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l4->SetTextSize(0.05);
  //  l4->Draw();

  TLatex *pre4 = new TLatex(16,0.05,"CMS Preliminary");
  pre4->SetTextSize(0.05);
  pre4->Draw();

  gPad->RedrawAxis();

  c4->SaveAs("bFraction_pt.pdf");
  c4->SaveAs("bFraction_pt.png");

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


void LoadPythia() {
  TFile *infPythia = new TFile("acceptancestudy_20110517.root","READ");
  int max = ((TH2F*) gROOT->FindObject("promptDen1YPt"))->GetNbinsX();
  TH1D *pJpsiPt = ((TH2F*) gROOT->FindObject("promptDen1YPt"))->ProjectionY("pJpsiPt",1,max,"e");
  max = ((TH2F*) gROOT->FindObject("nonPromptDen1YPt"))->GetNbinsX();
  TH1D *npJpsiPt = ((TH2F*) gROOT->FindObject("nonPromptDen1YPt"))->ProjectionY("npJpsiPt",1,max,"e");
  max = ((TH2F*) gROOT->FindObject("UpsDen1YPt"))->GetNbinsX();
  TH1D *upsPt = ((TH2F*) gROOT->FindObject("UpsDen1YPt"))->ProjectionY("upsPt",1,max,"e");

  double epsilon=0.0001;
  int min = pJpsiPt->GetXaxis()->FindBin(0.0+epsilon);
  max = pJpsiPt->GetXaxis()->FindBin(6.5-epsilon);
  hPromptJpsiPtPythia->SetBinContent(1,pJpsiPt->Integral(min,max));
  min = pJpsiPt->GetXaxis()->FindBin(6.5+epsilon);
  max = pJpsiPt->GetXaxis()->FindBin(10.0-epsilon);
  hPromptJpsiPtPythia->SetBinContent(2,pJpsiPt->Integral(min,max));
  min = pJpsiPt->GetXaxis()->FindBin(10.0+epsilon);
  max = pJpsiPt->GetXaxis()->FindBin(30.0-epsilon);
  hPromptJpsiPtPythia->SetBinContent(3,pJpsiPt->Integral(min,max));
  min = pJpsiPt->GetXaxis()->FindBin(30.0+epsilon);
  max = pJpsiPt->GetXaxis()->FindBin(3000.0);
  hPromptJpsiPtPythia->SetBinContent(4,pJpsiPt->Integral(min,max));

  min = npJpsiPt->GetXaxis()->FindBin(0.0+epsilon);
  max = npJpsiPt->GetXaxis()->FindBin(6.5-epsilon);
  hNonPromptJpsiPtPythia->SetBinContent(1,npJpsiPt->Integral(min,max));
  min = npJpsiPt->GetXaxis()->FindBin(6.5+epsilon);
  max = npJpsiPt->GetXaxis()->FindBin(10.0-epsilon);
  hNonPromptJpsiPtPythia->SetBinContent(2,npJpsiPt->Integral(min,max));
  min = npJpsiPt->GetXaxis()->FindBin(10.0+epsilon);
  max = npJpsiPt->GetXaxis()->FindBin(30.0-epsilon);
  hNonPromptJpsiPtPythia->SetBinContent(3,npJpsiPt->Integral(min,max));
  min = npJpsiPt->GetXaxis()->FindBin(30.0+epsilon);
  max = npJpsiPt->GetXaxis()->FindBin(3000.0);
  hNonPromptJpsiPtPythia->SetBinContent(4,npJpsiPt->Integral(min,max));

  min = upsPt->GetXaxis()->FindBin(0.0+epsilon);
  max = upsPt->GetXaxis()->FindBin(6.5-epsilon);
  hUpsPtPythia->SetBinContent(1,upsPt->Integral(min,max));
  min = upsPt->GetXaxis()->FindBin(6.5+epsilon);
  max = upsPt->GetXaxis()->FindBin(10.0-epsilon);
  hUpsPtPythia->SetBinContent(2,upsPt->Integral(min,max));
  min = upsPt->GetXaxis()->FindBin(10.0+epsilon);
  max = upsPt->GetXaxis()->FindBin(20.0-epsilon);
  hUpsPtPythia->SetBinContent(3,upsPt->Integral(min,max));
  min = upsPt->GetXaxis()->FindBin(20.0+epsilon);
  max = upsPt->GetXaxis()->FindBin(3000.0);
  hUpsPtPythia->SetBinContent(4,upsPt->Integral(min,max));

  return;
}
