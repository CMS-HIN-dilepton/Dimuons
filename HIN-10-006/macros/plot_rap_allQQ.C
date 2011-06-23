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

void LoadPythia();

TH1F *hPromptJpsiEtaPythia;
TH1F *hNonPromptJpsiEtaPythia;
TH1F *hUpsEtaPythia;


void plot_rap_allQQ(bool isPaper=false)
{
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(5);
  
  double etaBins[] = {0.0,1.4,2.0}; // 6.5--30
  double etaBinsM[] = {0.0,-1.4,-2.0}; // 6.5--30
  double etaError[] = {0.0,0.0,0.0};
  double etaBinsLow[] = {0.0,0.2,0.4};
  double etaBinsUp[] = {1.2,0.2,0.4};
  
  double upsEtaBins[] = {0.0, 1.8};
  double upsEtaBinsM[] = {0.0, -1.8};
  double upsEtaBinsError[] = {0.0, 0.0};
  double upsEtaBinsLow[] = {0.0, 0.6};
  double upsEtaBinsUp[] = {1.2, 0.6};


  double inclJpsiEta[] = {2.76, 2.29, 2.22}; // this is acc corrected to 6.5--30
  double inclJpsiEtaError[] = {0.26, 0.28, 0.21};
  double inclJpsiEtaSyst[] = {0.44, 0.35, 0.33};
  
  double promptJpsiEta[] = {2.12, 1.71, 1.83}; // this is acc corrected to 6.5--30 (2--30)
  double promptJpsiEtaError[] = {0.23, 0.26, 0.20};
  double promptJpsiEtaSyst[] = {0.34, 0.26, 0.28};
  
  double upsEta[] = {0.496, 0.498};  // this is acc corrected to 0-20
  double upsEtaError[] = {0.091, 0.097};
  double upsEtaSyst[] = {0.091, 0.092};

  
  double etaSystX[] = {0.15, 0.15, 0.15};


  TGraphErrors *gInclJpsiEta = new TGraphErrors(3, etaBins, inclJpsiEta, etaError, inclJpsiEtaError);
  TGraphErrors *gPromptJpsiEta = new TGraphErrors(3, etaBins, promptJpsiEta, etaError, promptJpsiEtaError);
  TGraphErrors *gUpsEta = new TGraphErrors(2, upsEtaBins, upsEta, upsEtaBinsError, upsEtaError);

  TGraphErrors *gInclJpsiEtaSyst = new TGraphErrors(3, etaBins, inclJpsiEta, etaSystX, inclJpsiEtaSyst);
  TGraphErrors *gPromptJpsiEtaSyst = new TGraphErrors(3, etaBins, promptJpsiEta, etaSystX, promptJpsiEtaSyst);
  TGraphErrors *gUpsEtaSyst = new TGraphErrors(2, upsEtaBins, upsEta, etaSystX, upsEtaSyst);

  TGraphAsymmErrors *gInclJpsiEtaP = new TGraphAsymmErrors(3, etaBins, inclJpsiEta, etaBinsLow, etaBinsUp, etaError, etaError);
  TGraphAsymmErrors *gPromptJpsiEtaP = new TGraphAsymmErrors(3, etaBins, promptJpsiEta, etaBinsLow, etaBinsUp, etaError, etaError);
  TGraphAsymmErrors *gUpsEtaP = new TGraphAsymmErrors(2, upsEtaBins, upsEta, upsEtaBinsLow, upsEtaBinsUp, upsEtaBinsError, upsEtaBinsError);


  TGraphErrors *gInclJpsiEtaM = new TGraphErrors(3, etaBinsM, inclJpsiEta, etaError, inclJpsiEtaError);
  TGraphErrors *gPromptJpsiEtaM = new TGraphErrors(3, etaBinsM, promptJpsiEta, etaError, promptJpsiEtaError);
  TGraphErrors *gUpsEtaM = new TGraphErrors(2, upsEtaBinsM, upsEta, upsEtaBinsError, upsEtaError);

  TGraphErrors *gInclJpsiEtaSystM = new TGraphErrors(3, etaBinsM, inclJpsiEta, etaSystX, inclJpsiEtaSyst);
  TGraphErrors *gPromptJpsiEtaSystM = new TGraphErrors(3, etaBinsM, promptJpsiEta, etaSystX, promptJpsiEtaSyst);
  TGraphErrors *gUpsEtaSystM = new TGraphErrors(2, upsEtaBinsM, upsEta, etaSystX, upsEtaSyst);

  gInclJpsiEtaM->RemovePoint(0);
  gPromptJpsiEtaM->RemovePoint(0);
  gUpsEtaM->RemovePoint(0);

  gInclJpsiEtaSystM->RemovePoint(0);
  gPromptJpsiEtaSystM->RemovePoint(0);
  gUpsEtaSystM->RemovePoint(0);

  gInclJpsiEtaSyst->SetFillColor(kAzure-9);
  gPromptJpsiEtaSyst->SetFillColor(kRed-9);
  gUpsEtaSyst->SetFillColor(kGreen-9);

  gInclJpsiEtaSystM->SetLineColor(kAzure-9);
  gPromptJpsiEtaSystM->SetLineColor(kRed-9);
  gUpsEtaSystM->SetLineColor(kGreen-9);

  gInclJpsiEtaSystM->SetLineWidth(3);
  gPromptJpsiEtaSystM->SetLineWidth(3);
  gUpsEtaSystM->SetLineWidth(3);

  gInclJpsiEtaSystM->SetFillStyle(0);
  gPromptJpsiEtaSystM->SetFillStyle(0);
  gUpsEtaSystM->SetFillStyle(0);

  if (isPaper) {
    gInclJpsiEta->SetMarkerColor(kBlue+1);
    gPromptJpsiEta->SetMarkerColor(kRed+2);
    gInclJpsiEtaM->SetMarkerColor(kBlue+1);
    gPromptJpsiEtaM->SetMarkerColor(kRed+2);
  }
  else {
    gInclJpsiEta->SetMarkerColor(kBlue);
    gPromptJpsiEta->SetMarkerColor(kRed);
    gInclJpsiEtaM->SetMarkerColor(kBlue);
    gPromptJpsiEtaM->SetMarkerColor(kRed);
  }
  gUpsEta->SetMarkerColor(kGreen+2);
  gUpsEtaM->SetMarkerColor(kGreen+2);

  gInclJpsiEta->SetMarkerStyle(20);
  gPromptJpsiEta->SetMarkerStyle(21);
  gUpsEta->SetMarkerStyle(33);

  gInclJpsiEtaM->SetMarkerStyle(24);
  gPromptJpsiEtaM->SetMarkerStyle(25);
  gUpsEtaM->SetMarkerStyle(27);

  gInclJpsiEta->SetMarkerSize(1.2);
  gPromptJpsiEta->SetMarkerSize(1.2);
  gUpsEta->SetMarkerSize(2.0);

  gInclJpsiEtaM->SetMarkerSize(1.2);
  gPromptJpsiEtaM->SetMarkerSize(1.2);
  gUpsEtaM->SetMarkerSize(2.0);

  gInclJpsiEtaP->SetMarkerStyle(24);
  gPromptJpsiEtaP->SetMarkerStyle(25);
  gUpsEtaP->SetMarkerStyle(27);

  gInclJpsiEtaP->SetMarkerSize(1.2);
  gPromptJpsiEtaP->SetMarkerSize(1.2);
  gUpsEtaP->SetMarkerSize(2.0);

  if (isPaper) {
    gInclJpsiEtaP->SetLineColor(kBlue+1);
    gPromptJpsiEtaP->SetLineColor(kRed+2);
  }
  else {
    gInclJpsiEtaP->SetLineColor(kBlue);
    gPromptJpsiEtaP->SetLineColor(kRed);
  }
  gUpsEtaP->SetLineColor(kGreen+2);

  gInclJpsiEtaP->SetLineWidth(2);
  gPromptJpsiEtaP->SetLineWidth(2);
  gUpsEtaP->SetLineWidth(2);


  double xbins[] = {-2.4,-1.6,-1.2, 1.2, 1.6, 2.4};
  double xbins2[] = {-2.4,-1.2, 1.2, 2.4};
  // pythia curves
  hPromptJpsiEtaPythia = new TH1F("hPromptJpsiEtaPythia","hPromptJpsiEtaPythia",5,xbins);
  hNonPromptJpsiEtaPythia = new TH1F("hNonPromptJpsiEtaPythia","hNonPromptJpsiEtaPythia",5,xbins);
  hUpsEtaPythia = new TH1F("hUpsEtaPythia","hUpsEtaPythia",3,xbins2);

  LoadPythia();

  double norm = hPromptJpsiEtaPythia->Integral(0,4);// include overflow for total xsection normalization
  hPromptJpsiEtaPythia->Scale(0.0551604*0.2683*0.181*5.93e-2*100.74e3/norm); // esp1*acc(|y|<2.4)*BR*xsection
  hPromptJpsiEtaPythia->SetBinContent(1, hPromptJpsiEtaPythia->GetBinContent(1)/0.8); // we fill + and - independently
  hPromptJpsiEtaPythia->SetBinContent(2, hPromptJpsiEtaPythia->GetBinContent(2)/0.4);
  hPromptJpsiEtaPythia->SetBinContent(3, hPromptJpsiEtaPythia->GetBinContent(3)/2.4);
  hPromptJpsiEtaPythia->SetBinContent(4, hPromptJpsiEtaPythia->GetBinContent(4)/0.4);
  hPromptJpsiEtaPythia->SetBinContent(5, hPromptJpsiEtaPythia->GetBinContent(5)/0.8);

  norm = hNonPromptJpsiEtaPythia->Integral(0,4);
  hNonPromptJpsiEtaPythia->Scale(0.128519*41.3*4.85*0.0593/norm); // 100.0/107106.0*0.751*5.93e-2*4.13e7
  hNonPromptJpsiEtaPythia->SetBinContent(1, hNonPromptJpsiEtaPythia->GetBinContent(1)/0.8);
  hNonPromptJpsiEtaPythia->SetBinContent(2, hNonPromptJpsiEtaPythia->GetBinContent(2)/0.4);
  hNonPromptJpsiEtaPythia->SetBinContent(3, hNonPromptJpsiEtaPythia->GetBinContent(3)/2.4);
  hNonPromptJpsiEtaPythia->SetBinContent(4, hNonPromptJpsiEtaPythia->GetBinContent(4)/0.4);
  hNonPromptJpsiEtaPythia->SetBinContent(5, hNonPromptJpsiEtaPythia->GetBinContent(5)/0.8);

  norm = hUpsEtaPythia->Integral(0,4);
  hUpsEtaPythia->Scale(1.36*0.192*0.0248e+3/norm);//100.0/600.0*2.48e-2*9.332e4
  hUpsEtaPythia->SetBinContent(1, hUpsEtaPythia->GetBinContent(1)/1.2);
  hUpsEtaPythia->SetBinContent(2, hUpsEtaPythia->GetBinContent(2)/2.4);
  hUpsEtaPythia->SetBinContent(3, hUpsEtaPythia->GetBinContent(3)/1.2); // we fill + and - independently

  hPromptJpsiEtaPythia->SetLineColor(kRed);
  hNonPromptJpsiEtaPythia->SetLineColor(kOrange+2);
  hUpsEtaPythia->SetLineColor(kGreen+2);

  hPromptJpsiEtaPythia->SetLineWidth(3);
  hNonPromptJpsiEtaPythia->SetLineWidth(3);
  hUpsEtaPythia->SetLineWidth(3);

  
  TF1 *f1 = new TF1("f1","100",-2.5,2.5);
  f1->SetLineWidth(1);
  f1->GetXaxis()->SetTitle("y");
  f1->GetYaxis()->SetTitle("1/T_{AA} dN/dy (nb)");
  f1->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c1 = new TCanvas("c1","c1");

  f1->Draw();
  f1->GetYaxis()->SetRangeUser(0.0, 5.0);

  //  hPromptJpsiEtaPythia->Draw("histsame");
  gInclJpsiEtaSyst->Draw("2");
  gPromptJpsiEtaSyst->Draw("2");
  gInclJpsiEtaSystM->Draw("2");
  gPromptJpsiEtaSystM->Draw("2");
  gInclJpsiEta->Draw("PZ");
  gPromptJpsiEta->Draw("PZ");
  gInclJpsiEtaM->Draw("PZ");
  gPromptJpsiEtaM->Draw("PZ");
  gInclJpsiEtaP->Draw("PX");
  gPromptJpsiEtaP->Draw("PX");
  // gInclJpsiEtaP->Draw("||");
  // gPromptJpsiEtaP->Draw("||");

  TLegend *leg1 = new TLegend(0.60,0.61,0.95,0.74);
  //  TLegend *leg1 = new TLegend(0.41,0.69,0.92,0.87);
  leg1->SetFillStyle(0);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(0);
  leg1->SetMargin(0.2);
  leg1->SetTextSize(0.04);
  leg1->AddEntry(gInclJpsiEta,"Inclusive J/#psi","P"); // add pt bin
  leg1->AddEntry(gPromptJpsiEta,"Prompt J/#psi","P");
  //  leg1->AddEntry(hPromptJpsiEtaPythia,"PYTHIA: Prompt J/#psi","L");
  leg1->Draw();


  TLatex *pre = new TLatex(-1.0,4.583,"CMS Preliminary");
  pre->SetTextSize(0.05);
  pre->Draw();

  TLatex *l1 = new TLatex(-1.0,4.16667,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  //  TLatex *l1 = new TLatex(0.78125,4.16667,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1->SetTextSize(0.05);
  l1->Draw();

  TLatex *lcent1 = new TLatex(-2.2,0.6,"0-100%");
  //  TLatex *lcent1 = new TLatex(0.1,0.6,"0-100%");
  lcent1->Draw();
  TLatex *lpt1 = new TLatex(-2.2,0.3,"6.5 < p_{T} < 30.0 GeV/c");
  lpt1->Draw();

  gPad->RedrawAxis();

  TF1 *f3 = new TF1("f3","10000",-2.5,2.5);
  f3->SetLineWidth(1);
  f3->GetXaxis()->SetTitle("y");
  f3->GetYaxis()->SetTitle("1/T_{AA} dN/dy (nb)");
  f3->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c3 = new TCanvas("c3","c3");
  f3->GetYaxis()->SetRangeUser(0.0, 1.0);

  f3->Draw();
  //  hUpsEtaPythia->Draw("histsame");
  //  gPromptJpsiEta->Draw("P");
  gUpsEtaSyst->Draw("2");
  gUpsEtaSystM->Draw("2");
  gUpsEta->Draw("PZ");
  gUpsEtaM->Draw("PZ");
  gUpsEtaP->Draw("PX");
  //  gUpsEtaP->Draw("||");

  TLegend *leg3 = new TLegend(0.60,0.68,0.95,0.74);
  //  TLegend *leg3 = new TLegend(0.41,0.69,0.92,0.87);
  leg3->SetFillStyle(0);
  leg3->SetFillColor(kWhite);
  leg3->SetBorderSize(0);
  leg3->SetMargin(0.2);
  leg3->SetTextSize(0.04);
  //  leg3->AddEntry(gPromptJpsiEta,"Prompt J/#psi (0-100%)","P");
  leg3->AddEntry(gUpsEta,"#Upsilon(1S)","P");
  leg3->Draw();

  TLatex *pre3 = new TLatex(-1.0,0.9166,"CMS Preliminary");
  pre3->SetTextSize(0.05);
  pre3->Draw();

  TLatex *l3 = new TLatex(-1.0,0.833333,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l3->SetTextSize(0.05);
  l3->Draw();
  

  TLatex *lcent2 = new TLatex(-2.2,0.12,"0-100%");
  lcent2->Draw();
  TLatex *lpt2 = new TLatex(-2.2,0.06,"0.0 < p_{T} < 20.0 GeV/c");
  lpt2->Draw();


  gPad->RedrawAxis();

  c1->SaveAs("inclJpsi_eta.pdf");
  c3->SaveAs("upsilon_eta.pdf"); 
  c1->SaveAs("inclJpsi_eta.png");
  c3->SaveAs("upsilon_eta.png");
  return;

/*
  double bfractionEta[] = {0.079, 0.207, 0.252};  // 0--2.4
  double bfractionEtaError[] = {0.041, 0.040, 0.052};

  double bfractionCoarseEta[] = {0.102, 0.153}; // 1.6--2.4
  double bfractionCoarseEtaError[] = {0.045, 0.055};
  

  TGraphErrors *gBfractionEta = new TGraphErrors(3, etaBins, bfractionEta, etaError, bfractionEtaError);
  TGraphErrors *gBfractionCoarseEta = new TGraphErrors(2, etaBinsCoarse, bfractionCoarseEta, etaErrorCoarse, bfractionCoarseEtaError);

  gBfractionEta->SetName("gBfractionEta");
  gBfractionCoarseEta->SetName("gBfractionCoarseEta");

  gBfractionEta->SetMarkerStyle(20);
  gBfractionCoarseEta->SetMarkerStyle(33);

  gBfractionEta->SetMarkerSize(1.2);
  gBfractionCoarseEta->SetMarkerSize(1.5);

  gBfractionEta->SetMarkerColor(kRed);
  gBfractionCoarseEta->SetMarkerColor(kBlack);


  TF1 *f4 = new TF1("f4","1",0,2.4);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("y");
  f4->GetYaxis()->SetTitle("B-fraction");
  f4->GetYaxis()->SetTitleOffset(1.2);

  f4->GetYaxis()->SetRangeUser(0.0,1.0);

  TCanvas *c4 = new TCanvas("c4","c4");
  c4->SetRightMargin(rmargin);
  c4->SetTopMargin(tmargin);

  f4->Draw();

  gB_mid_tot->Draw("p");
  gB_inter_tot->Draw("p");
  gB_FWD_tot->Draw("p");

  gB_CDF_tot->Draw("p");

  gBfractionEta->Draw("P");
  gBfractionCoarseEta->Draw("P");


  TLegend *leg4 = new TLegend(0.10,0.60,0.70,0.95);
  leg4->SetFillStyle(0);
  leg4->SetFillColor(kWhite);
  leg4->SetBorderSize(0);
  leg4->AddEntry(gBfractionEta,"B-fraction (|y|<2.4)","P");
  leg4->AddEntry(gBfractionCoarseEta,"B-fraction (1.6<|y|<2.4)","P");
  leg4->Draw();

  TLatex *l4 = new TLatex(16,0.05,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l4->SetTextSize(0.05);
  l4->Draw();

  c4->SaveAs("bFraction_eta.pdf");
*/
  return;
}


void LoadPythia() {
  TFile *infPythia = new TFile("acceptancestudy_20110517.root","READ");
  double epsilon=0.0001;
  int minPt = ((TH2F*) gROOT->FindObject("promptDen1YPt"))->GetYaxis()->FindBin(6.5+epsilon);
  int maxPt = ((TH2F*) gROOT->FindObject("promptDen1YPt"))->GetNbinsY();
  int max = ((TH2F*) gROOT->FindObject("promptDen1YPt"))->GetNbinsY();
  //  cout <<  ((TH2F*) gROOT->FindObject("promptDen1YPt"))->Integral(1,max,minPt,maxPt)/((TH2F*) gROOT->FindObject("promptDen1YPt"))->Integral(1,max,1,maxPt) << endl;
  TH1D *pJpsiEta = ((TH2F*) gROOT->FindObject("promptDen1YPt"))->ProjectionX("pJpsiEta",1,max,"e");

  max = ((TH2F*) gROOT->FindObject("nonPromptDen1YPt"))->GetNbinsY();
  minPt = ((TH2F*) gROOT->FindObject("nonPromptDen1YPt"))->GetYaxis()->FindBin(6.5+epsilon);
  maxPt = ((TH2F*) gROOT->FindObject("nonPromptDen1YPt"))->GetNbinsY();
  //  cout <<  ((TH2F*) gROOT->FindObject("nonPromptDen1YPt"))->Integral(1,max,minPt,maxPt)/((TH2F*) gROOT->FindObject("promptDen1YPt"))->Integral(1,max,1,maxPt) << endl;
  TH1D *npJpsiEta = ((TH2F*) gROOT->FindObject("nonPromptDen1YPt"))->ProjectionX("npJpsiEta",1,max,"e");
  max = ((TH2F*) gROOT->FindObject("UpsDen1YPt"))->GetNbinsY();
  TH1D *upsEta = ((TH2F*) gROOT->FindObject("UpsDen1YPt"))->ProjectionX("upsEta",1,max,"e");

  int min = pJpsiEta->GetXaxis()->FindBin(-1.2+epsilon);
  max = pJpsiEta->GetXaxis()->FindBin(1.2-epsilon);
  hPromptJpsiEtaPythia->SetBinContent(3,pJpsiEta->Integral(min,max));

  max = pJpsiEta->GetXaxis()->FindBin(-1.2-epsilon);
  min = pJpsiEta->GetXaxis()->FindBin(-1.6+epsilon);
  hPromptJpsiEtaPythia->SetBinContent(2,pJpsiEta->Integral(min,max));
  min = pJpsiEta->GetXaxis()->FindBin(1.2+epsilon);
  max = pJpsiEta->GetXaxis()->FindBin(1.6-epsilon);
  hPromptJpsiEtaPythia->SetBinContent(4,pJpsiEta->Integral(min,max));

  max = pJpsiEta->GetXaxis()->FindBin(-1.6-epsilon);
  min = pJpsiEta->GetXaxis()->FindBin(-2.4+epsilon);
  hPromptJpsiEtaPythia->SetBinContent(1,pJpsiEta->Integral(min,max));
  min = pJpsiEta->GetXaxis()->FindBin(1.6+epsilon);
  max = pJpsiEta->GetXaxis()->FindBin(2.4-epsilon);
  hPromptJpsiEtaPythia->SetBinContent(5,pJpsiEta->Integral(min,max));

  max = pJpsiEta->GetXaxis()->FindBin(-2.4-epsilon);
  min = pJpsiEta->GetXaxis()->FindBin(-30.0);
  hPromptJpsiEtaPythia->SetBinContent(0,pJpsiEta->Integral(min,max));
  min = pJpsiEta->GetXaxis()->FindBin(2.4+epsilon);
  max = pJpsiEta->GetXaxis()->FindBin(30.0);
  hPromptJpsiEtaPythia->SetBinContent(6,pJpsiEta->Integral(min,max));

  min = npJpsiEta->GetXaxis()->FindBin(1.2+epsilon);
  max = npJpsiEta->GetXaxis()->FindBin(1.2-epsilon);
  hNonPromptJpsiEtaPythia->SetBinContent(3,npJpsiEta->Integral(min,max));

  max = npJpsiEta->GetXaxis()->FindBin(-1.2-epsilon);
  min = npJpsiEta->GetXaxis()->FindBin(-1.6+epsilon);
  hNonPromptJpsiEtaPythia->SetBinContent(2,npJpsiEta->Integral(min,max));
  min = npJpsiEta->GetXaxis()->FindBin(1.2+epsilon);
  max = npJpsiEta->GetXaxis()->FindBin(1.6-epsilon);
  hNonPromptJpsiEtaPythia->SetBinContent(4,npJpsiEta->Integral(min,max));

  max = npJpsiEta->GetXaxis()->FindBin(-1.6-epsilon);
  min = npJpsiEta->GetXaxis()->FindBin(-2.4+epsilon);
  hNonPromptJpsiEtaPythia->SetBinContent(1,npJpsiEta->Integral(min,max));
  min = npJpsiEta->GetXaxis()->FindBin(1.6+epsilon);
  max = npJpsiEta->GetXaxis()->FindBin(2.4-epsilon);
  hNonPromptJpsiEtaPythia->SetBinContent(5,npJpsiEta->Integral(min,max));

  max = npJpsiEta->GetXaxis()->FindBin(-2.4-epsilon);
  min = npJpsiEta->GetXaxis()->FindBin(-30.0);
  hNonPromptJpsiEtaPythia->SetBinContent(0,npJpsiEta->Integral(min,max));
  min = npJpsiEta->GetXaxis()->FindBin(2.4+epsilon);
  max = npJpsiEta->GetXaxis()->FindBin(30.0);
  hNonPromptJpsiEtaPythia->SetBinContent(6,npJpsiEta->Integral(min,max));

  min = upsEta->GetXaxis()->FindBin(-1.2+epsilon);
  max = upsEta->GetXaxis()->FindBin(1.2-epsilon);
  hUpsEtaPythia->SetBinContent(2,upsEta->Integral(min,max));

  max = upsEta->GetXaxis()->FindBin(-1.2-epsilon);
  min = upsEta->GetXaxis()->FindBin(-2.4+epsilon);
  hUpsEtaPythia->SetBinContent(1,upsEta->Integral(min,max));
  min = upsEta->GetXaxis()->FindBin(1.2+epsilon);
  max = upsEta->GetXaxis()->FindBin(2.4-epsilon);
  hUpsEtaPythia->SetBinContent(3,upsEta->Integral(min,max));

  max = upsEta->GetXaxis()->FindBin(-2.4-epsilon);
  min = upsEta->GetXaxis()->FindBin(-30.0+epsilon);
  hUpsEtaPythia->SetBinContent(0,upsEta->Integral(min,max));
  min = upsEta->GetXaxis()->FindBin(2.4+epsilon);
  max = upsEta->GetXaxis()->FindBin(30.0-epsilon);
  hUpsEtaPythia->SetBinContent(4,upsEta->Integral(min,max));

  return;
}
