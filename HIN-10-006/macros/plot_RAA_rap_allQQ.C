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


void plot_RAA_rap_allQQ(bool isPaper=false, bool plotPhenix=false)
{
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(5);
  
  double etaBins[] = {0.0,1.4,2.0}; // 6.5--30
  double etaBinsM[] = {0.0,-1.4,-2.0}; // 6.5--30
  double etaError[] = {0.0,0.0,0.0,0.0};
  double etaBinsLow[] = {0.0,0.2,0.4};
  double etaBinsUp[] = {1.2,0.2,0.4};
  
  double upsEtaBins[] = {0.0, 1.8};
  double upsEtaBinsM[] = {0.0, -1.8};
  double upsEtaBinsError[] = {0.0, 0.0};
  double upsEtaBinsLow[] = {0.0, 0.6};
  double upsEtaBinsUp[] = {1.2, 0.6};


  double inclJpsiEta[] = {0.286, 0.255, 0.394}; // this is acc corrected to 6.5--30
  double inclJpsiEtaError[] = {0.035, 0.037, 0.040};
  double inclJpsiEtaSyst[] = {0.023, 0.023, 0.036};
  
  double promptJpsiEta[] = {0.284, 0.254, 0.418}; // this is acc corrected to 6.5--30 (2--30)
  double promptJpsiEtaError[] = {0.040, 0.044, 0.051};
  double promptJpsiEtaSyst[] = {0.023, 0.022, 0.028};
  
  double upsEta[] = { 0.523, 0.826}; // this is acc corrected to 6.5--30 (2--30)
  double upsEtaError[] = {0.121, 0.239};
  double upsEtaSyst[] = {0.082, 0.129};
  
  double etaSystX[] = {0.15, 0.15, 0.15, 0.15};


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

  gInclJpsiEtaSystM->SetFillStyle(0);
  gPromptJpsiEtaSystM->SetFillStyle(0);
  gUpsEtaSystM->SetFillStyle(0);

  gInclJpsiEtaSystM->SetLineWidth(3);
  gPromptJpsiEtaSystM->SetLineWidth(3);
  gUpsEtaSystM->SetLineWidth(3);

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

  gInclJpsiEtaP->SetMarkerStyle(24);
  gPromptJpsiEtaP->SetMarkerStyle(25);
  gUpsEtaP->SetMarkerStyle(27);

  gInclJpsiEta->SetMarkerSize(1.2);
  gPromptJpsiEta->SetMarkerSize(1.2);
  gUpsEta->SetMarkerSize(2.0);

  gInclJpsiEtaM->SetMarkerSize(1.2);
  gPromptJpsiEtaM->SetMarkerSize(1.2);
  gUpsEtaM->SetMarkerSize(2.0);

  gInclJpsiEtaP->SetMarkerSize(1.2);
  gPromptJpsiEtaP->SetMarkerSize(1.2);
  gUpsEtaP->SetMarkerSize(2.0);

  // gInclJpsiEtaP->SetLineColor(kBlue+1);
  // gPromptJpsiEtaP->SetLineColor(kRed+2);
  //  gUpsEtaP->SetLineColor(kGreen+2);

  // gInclJpsiEtaP->SetLineWidth(2);
  // gPromptJpsiEtaP->SetLineWidth(2);
  //  gUpsEtaP->SetLineWidth(2);

  
  TF1 *f1 = new TF1("f1","1",-2.5,2.5);
  f1->SetLineWidth(1);
  f1->GetXaxis()->SetTitle("y");
  f1->GetYaxis()->SetTitle("R_{AA}");
  f1->GetYaxis()->SetRangeUser(0.0,1.5);
  f1->GetXaxis()->CenterTitle(kTRUE);

  TBox *lumi = new TBox(-2.5,0.93,-2.25,1.07);
  lumi->SetFillColor(kGray+1);

  TBox *lumiPhenix = new TBox(2.25,0.88,2.5,1.12);
  lumiPhenix->SetFillStyle(0);
  lumiPhenix->SetLineWidth(2);
  lumiPhenix->SetLineColor(kBlue);

  double phenix_RAA_x035[] = {0.0};
  double phenix_RAA_y035[] = {0.425};
  double phenix_RAA_y035_err[] = {0.025};
  double phenix_RAA_y035_box[] = {0.051};

  TGraphErrors *pgPhenix_y035 = new TGraphErrors(1, phenix_RAA_x035, phenix_RAA_y035, etaError, phenix_RAA_y035_err );
  TGraphErrors *pgPhenix_y035_box = new TGraphErrors(1, phenix_RAA_x035, phenix_RAA_y035, etaSystX, phenix_RAA_y035_box);

  pgPhenix_y035->SetMarkerStyle(25);
  pgPhenix_y035_box->SetMarkerStyle(25);

  pgPhenix_y035->SetMarkerColor(kBlack);
  pgPhenix_y035->SetLineColor(kBlack);

  pgPhenix_y035_box->SetLineColor(kBlack);
  pgPhenix_y035_box->SetFillStyle(0);

  pgPhenix_y035->SetMarkerSize(1.0);

  double phenix_RAA_x1222[] = {-1.95,-1.45,1.45, 1.95};
  double phenix_RAA_y1222[] = {0.316, 0.354, 0.282, 0.312};
  double phenix_RAA_y1222_err[4];
  phenix_RAA_y1222_err[0] = sqrt(0.034*0.034+0.035*0.035);
  phenix_RAA_y1222_err[1] = sqrt(0.029*0.029+0.026*0.026);
  phenix_RAA_y1222_err[2] = sqrt(0.033*0.033+0.024*0.024);
  phenix_RAA_y1222_err[3] = sqrt(0.052*0.052+0.049*0.049);

  double phenix_RAA_y1222_boxUp[] = {0.053, 0.059, 0.047, 0.052};
  double phenix_RAA_y1222_boxLow[] = {0.050, 0.057, 0.045, 0.050};

  TGraphErrors *pgPhenix_y1222 = new TGraphErrors(4, phenix_RAA_x1222, phenix_RAA_y1222, etaError, phenix_RAA_y1222_err );
  TGraphAsymmErrors *pgPhenix_y1222_box = new TGraphAsymmErrors(4, phenix_RAA_x1222, phenix_RAA_y1222, etaSystX, etaSystX, phenix_RAA_y1222_boxLow, phenix_RAA_y1222_boxUp);

  pgPhenix_y1222->SetMarkerStyle(24);
  pgPhenix_y1222_box->SetMarkerStyle(24);

  pgPhenix_y1222->SetMarkerColor(kBlue);
  pgPhenix_y1222->SetLineColor(kBlack);

  pgPhenix_y1222_box->SetLineColor(kBlue);
  pgPhenix_y1222_box->SetFillStyle(0);

  pgPhenix_y1222->SetMarkerSize(1.0);


  TCanvas *c1 = new TCanvas("c1","c1");

  f1->Draw();
  lumi->Draw();
  gPromptJpsiEtaSyst->Draw("2");
  gPromptJpsiEtaSystM->Draw("2");

  if (plotPhenix) {
    lumiPhenix->Draw();
    pgPhenix_y035_box->Draw("2");
    pgPhenix_y035->Draw("PZ");
    pgPhenix_y1222_box->Draw("2");
    pgPhenix_y1222->Draw("PZ");
  }

  f1->Draw("same");
  //  gInclJpsiEtaSyst->Draw("2");
  //  gInclJpsiEta->Draw("PZ");
  gPromptJpsiEta->Draw("PZ");
  //  gInclJpsiEtaP->Draw("PX");
  gPromptJpsiEtaM->Draw("PZ");
  gPromptJpsiEtaP->Draw("PX");
  // gInclJpsiEtaP->Draw("||");
  // gPromptJpsiEtaP->Draw("||");

  TLegend *leg1a;
  if (plotPhenix) {
    leg1a = new TLegend(0.42,0.45,0.75,0.65);
    leg1a->SetFillStyle(0);
    leg1a->SetFillColor(0);
    leg1a->SetBorderSize(0);
    leg1a->SetMargin(0.2);
    leg1a->SetTextSize(0.035);
  }

  TLegend *leg1 = new TLegend(0.45,0.68,0.90,0.74);
  //  TLegend *leg1 = new TLegend(0.41,0.69,0.92,0.87);
  leg1->SetFillStyle(0);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(0);
  leg1->SetMargin(0.2);
  leg1->SetTextSize(0.04);
  //  leg1->AddEntry(gInclJpsiEta,"Inclusive J/#psi (0-100%)","P"); // add pt bin
  leg1->AddEntry(gPromptJpsiEta,"Prompt J/#psi","P");
  leg1->Draw();

  if (plotPhenix) {
    leg1a->SetHeader("AuAu  #sqrt{s_{NN}} = 200 GeV");
    leg1a->AddEntry(pgPhenix_y035,"PHENIX: |y|<0.35","P");
    leg1a->AddEntry(pgPhenix_y1222,"PHENIX: 1.2<|y|<2.2","P");
    leg1a->AddEntry(pgPhenix_y035,"(both PRL 98, 232301 (2007))","");
    leg1a->Draw();
  }


  TLatex *pre = new TLatex(-1.0,1.375,"CMS Preliminary");//0.78125
  pre->SetTextSize(0.05);
  pre->Draw();

  TLatex *l1 = new TLatex(-1.0,1.25,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1->SetTextSize(0.05);
  l1->Draw();

  TLatex *lcent1 = new TLatex(-2.25,0.16875,"0-100%");
  lcent1->Draw();
  TLatex *lpt1 = new TLatex(-2.25,0.075,"6.5 < p_{T} < 30.0 GeV/c");
  lpt1->Draw();


  gPad->RedrawAxis();
  
  TF1 *f3 = new TF1("f3","10000",-2.5,2.5);
  f3->SetLineWidth(1);
  f3->GetXaxis()->SetTitle("y");
  f3->GetYaxis()->SetTitle("1/T_{AA} dN/dy (nb)");
  f3->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c3 = new TCanvas("c3","c3");
  f3->GetYaxis()->SetRangeUser(0.0, 1.5);

  f1->Draw();
  lumi->Draw();
  f1->Draw("same");
  //  gPromptJpsiEta->Draw("P");
  gUpsEtaSyst->Draw("2");
  gUpsEtaSystM->Draw("2");
  gUpsEtaM->Draw("PZ");
  gUpsEta->Draw("PZ");
  gUpsEtaP->Draw("PX");
  //  gUpsEtaP->Draw("||");

  TLegend *leg3 = new TLegend(0.45,0.68,0.90,0.74);
  //  TLegend *leg3 = new TLegend(0.41,0.69,0.92,0.87);
  leg3->SetFillStyle(0);
  leg3->SetFillColor(kWhite);
  leg3->SetBorderSize(0);
  leg3->SetMargin(0.2);
  leg3->SetTextSize(0.04);
  //  leg3->AddEntry(gPromptJpsiEta,"Prompt J/#psi (0-100%)","P");
  leg3->AddEntry(gUpsEta,"#Upsilon(1S)","P");
  leg3->Draw();

  TLatex *pre3 = new TLatex(-1,1.375,"CMS Preliminary");
  pre3->SetTextSize(0.05);
  pre3->Draw();

  TLatex *l3 = new TLatex(-1.0,1.25,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l3->SetTextSize(0.05);
  l3->Draw();
  gPad->RedrawAxis();
  
  lcent1->Draw();
  TLatex *lpt2 = new TLatex(-2.25,0.075,"0.0 < p_{T} < 20.0 GeV/c");
  lpt2->Draw();

  if (plotPhenix) {
    c1->SaveAs("PromptJpsi_RAA_eta_PHENIX.pdf");
    c1->SaveAs("PromptJpsi_RAA_eta_PHENIX.png");
  }
  else {
    c1->SaveAs("PromptJpsi_RAA_eta.pdf");
    c1->SaveAs("PromptJpsi_RAA_eta.png");
  }
  c3->SaveAs("upsilon_RAA_eta.pdf");
  c3->SaveAs("upsilon_RAA_eta.png");
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
  f4->GetXaxis()->SetTitle("|y|");
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
