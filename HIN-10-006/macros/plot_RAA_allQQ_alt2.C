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
TGraphErrors *pgPhenix_y1222;
TGraphAsymmErrors *pgPhenix_y1222_box;

// barrel
TGraphErrors *pgPhenix_y035;
TGraphAsymmErrors *pgPhenix_y035_box;

void load_PHENIX();

void plot_cent_allQQ(bool isPaper=false, bool plotPhenix=true)
{
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  //!!! merge first two centralities!!!

  // incl. and prompt J/psi
  // 0-100, 0-20, 20-30,30-40, 40-50, 50-100
  double promptTAA[] = {5.6625, 23.1857, 14.4777, 8.7830, 5.0892, 2.7484, 0.4682};
  double promptTAAError[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  double promptNpart[] = {113.0518, 308.3853, 187.1470, 129.9835, 86.2622, 22.0709};
  double promptNpartError[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  //  double promptNpartError[] = {1.682743428, 8.29009935, 9.57718473, 7.28379756, 5.5459031, 4.25551871, 0.91201189};

  double promptNcoll[] = {362.4007, 1483.8875, 926.5725, 562.1105, 325.7105, 175.8948, 29.9663};
  double promptNcollError[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double promptrelCentBinWidth[] = {10.0, 1.0, 1.0, 1.0, 1.0, 1.0, 5.0};


  // Upsilon
  // 0-100, 0-10, 10-20, 20-100
  double upsTAA[] = {5.6625, 23.1857, 14.4777, 2.3702};
  double upsTAAError[] = {0.0, 0.0, 0.0, 0.0};

  // double upsNpart[] = {113.0518, 355.3528, 261.4178, 64.2184};
  // double upsNpartError[] = {0.0, 0.0, 0.0, 0.0};
  //  double upsNpartError[] = {1.682743428, 8.29009935, 9.57718473, 1.38470625};

  // 0-100, 0-20, 20-100
  double upsNpart[] = {113.0518, 308.3853, 64.2184};
  double upsNpartError[] = {0.0, 0.0, 0.0};

  double upsNcoll[] = {362.4007, 1483.8875, 926.5725, 151.6934};
  double upsNcollError[] = {0.0, 0.0, 0.0, 0.0};
  double upsrelCentBinWidth[] = {10.0, 1.0, 1.0, 8.0};

  // non-prompt J/psi
  // 0-100, 0-20, 20-100
  double nonPromptTAA[] = {5.6625, 18.8317, 2.3702};
  double nonPromptTAAError[] = {0.0, 0.0, 0.0};

  double nonPromptNpart[] = {113.0518, 308.3853, 64.2184};
  double nonPromptNpartError[] = {0.0, 0.0, 0.0};
  //  double nonPromptNpartError[] = {1.682743428, 6.33340774, 1.38470625};

  double nonPromptNcoll[] = {362.4007, 1205.2300, 151.6934};
  double nonPromptNcollError[] = {0.0, 0.0, 0.0};
  double nonPromptrelCentBinWidth[] = {10.0, 2.0, 8.0};

  double promptSystX[] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};
  double nonPromptSystX[] = {10.0, 10.0, 10.0, 10.0, 10.0};
  double upsSystX[] = {10.0, 10.0, 10.0, 10.0, 10.0};


  // RAA
  double ppPromptJpsi = 226.0;
  double ppPromptJpsiError = 20;

  double inclJpsiRAA[6] = {0.306, 0.230, 0.311, 0.482, 0.671, 0.595};
  double promptJpsiRAA[6] = {0.296, 0.204, 0.362, 0.493, 0.644, 0.593};

  double inclJpsiRAA2[3] = {0.306, 0.230, 0.448};
  double promptJpsiRAA2[3] = {0.296, 0.204, 0.472};
  double nonPromptJpsiRAA[3] = {0.368, 0.361, 0.367};
  //  double upsRAA[1] = {0.68};
  //  double upsRAA[4] = {0.6612, 0.5057, 0.9386, 0.7547};
  double upsRAA[3] = {0.617, 0.595, 0.659};

  double inclJpsiRAAError[6] = {0.021, 0.023, 0.045, 0.070, 0.107, 0.103};
  double promptJpsiRAAError[6] = {0.025, 0.028, 0.056, 0.083, 0.127, 0.119};
  double nonPromptJpsiRAAError[3] = {0.066, 0.079, 0.095};
  double inclJpsiRAA2Error[3] = {0.026, 0.023, 0.035};
  double promptJpsiRAA2Error[3] = {0.029, 0.028, 0.053};
  //  double upsRAAError[4] = {0.1186, 0.1515, 0.2376, 0.1858};
  double upsRAAError[3] = {0.110, 0.124, 0.132};

  double inclJpsiRAASyst[6] = {0.028, 0.020, 0.030, 0.058, 0.099, 0.118};
  double promptJpsiRAASyst[6] = {0.026, 0.016, 0.035, 0.057, 0.090, 0.112};
  double nonPromptJpsiRAASyst[3] = {0.028, 0.025, 0.036};
  double inclJpsiRAA2Syst[3] = {0.028, 0.020, 0.054};
  double promptJpsiRAA2Syst[3] = {0.026, 0.016, 0.055};
  //  double upsRAASyst[4] = {0.1038, 0.0880, 0.1453, 0.1283};
  double upsRAASyst[3] = {0.096, 0.104, 0.111};


  TGraphErrors *gInclJpsiRAA = new TGraphErrors(6, promptNpart, inclJpsiRAA, promptNpartError, inclJpsiRAAError);
  TGraphErrors *gPromptJpsiRAA = new TGraphErrors(6, promptNpart, promptJpsiRAA, promptNpartError, promptJpsiRAAError);
  TGraphErrors *gNonPromptJpsiRAA = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiRAA, nonPromptNpartError, nonPromptJpsiRAAError);
  TGraphErrors *gPromptJpsiRAA2 = new TGraphErrors(3, nonPromptNpart, promptJpsiRAA2, nonPromptNpartError, promptJpsiRAA2Error);
  TGraphErrors *gUpsRAA = new TGraphErrors(3, upsNpart, upsRAA, upsNpartError, upsRAAError);

  TGraphErrors *gInclJpsiRAAMB = new TGraphErrors(1, promptNpart, inclJpsiRAA, promptNpartError, inclJpsiRAAError);
  TGraphErrors *gPromptJpsiRAAMB = new TGraphErrors(1, promptNpart, promptJpsiRAA, promptNpartError, promptJpsiRAAError);
  TGraphErrors *gNonPromptJpsiRAAMB = new TGraphErrors(1, nonPromptNpart, nonPromptJpsiRAA, nonPromptNpartError, nonPromptJpsiRAAError);
  TGraphErrors *gUpsRAAMB = new TGraphErrors(1, upsNpart, upsRAA, upsNpartError, upsRAAError);


  TGraphErrors *gInclJpsiRAASyst = new TGraphErrors(6, promptNpart, inclJpsiRAA, promptSystX, inclJpsiRAASyst);
  TGraphErrors *gPromptJpsiRAASyst = new TGraphErrors(6, promptNpart, promptJpsiRAA, promptSystX, promptJpsiRAASyst);
  TGraphErrors *gNonPromptJpsiRAASyst = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiRAA, nonPromptSystX, nonPromptJpsiRAASyst);
  TGraphErrors *gPromptJpsiRAA2Syst = new TGraphErrors(3, nonPromptNpart, promptJpsiRAA2, nonPromptSystX, promptJpsiRAA2Syst);
  TGraphErrors *gUpsRAASyst = new TGraphErrors(3, upsNpart, upsRAA, upsSystX, upsRAASyst);

  TGraphErrors *gInclJpsiRAAMBSyst = new TGraphErrors(1, promptNpart, inclJpsiRAA, promptSystX, inclJpsiRAASyst);
  TGraphErrors *gPromptJpsiRAAMBSyst = new TGraphErrors(1, promptNpart, promptJpsiRAA, promptSystX, promptJpsiRAASyst);
  TGraphErrors *gNonPromptJpsiRAAMBSyst = new TGraphErrors(1, nonPromptNpart, nonPromptJpsiRAA, nonPromptSystX, nonPromptJpsiRAASyst);
  TGraphErrors *gUpsRAAMBSyst = new TGraphErrors(1, upsNpart, upsRAA, upsSystX, upsRAASyst);


  TGraphErrors *gInclJpsiRAAP = new TGraphErrors(6, promptNpart, inclJpsiRAA, promptNpartError, promptNpartError);
  TGraphErrors *gPromptJpsiRAAP = new TGraphErrors(6, promptNpart, promptJpsiRAA, promptNpartError, promptNpartError);
  TGraphErrors *gNonPromptJpsiRAAP = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiRAA, nonPromptNpartError, nonPromptNpartError);
  TGraphErrors *gPromptJpsiRAA2P = new TGraphErrors(3, nonPromptNpart, promptJpsiRAA2, nonPromptNpartError, nonPromptNpartError);
  TGraphErrors *gUpsRAAP = new TGraphErrors(3, upsNpart, upsRAA, upsNpartError, upsNpartError);

  TGraphErrors *gInclJpsiRAAMBP = new TGraphErrors(1, promptNpart, inclJpsiRAA, promptNpartError, promptNpartError);
  TGraphErrors *gPromptJpsiRAAMBP = new TGraphErrors(1, promptNpart, promptJpsiRAA, promptNpartError, promptNpartError);
  TGraphErrors *gNonPromptJpsiRAAMBP = new TGraphErrors(1, nonPromptNpart, nonPromptJpsiRAA, nonPromptNpartError, nonPromptNpartError);
  TGraphErrors *gUpsRAAMBP = new TGraphErrors(1, upsNpart, upsRAA, upsNpartError, upsNpartError);


  gInclJpsiRAA->RemovePoint(0);
  gPromptJpsiRAA->RemovePoint(0);
  gNonPromptJpsiRAA->RemovePoint(0);
  gPromptJpsiRAA2->RemovePoint(0);
  gUpsRAA->RemovePoint(0);

  gNonPromptJpsiRAA->RemovePoint(1);
  gUpsRAA->RemovePoint(1);

  gInclJpsiRAAP->RemovePoint(0);
  gPromptJpsiRAAP->RemovePoint(0);
  gNonPromptJpsiRAAP->RemovePoint(0);
  gPromptJpsiRAA2P->RemovePoint(0);
  gUpsRAAP->RemovePoint(0);

  gNonPromptJpsiRAAP->RemovePoint(1);
  gUpsRAAP->RemovePoint(1);

  gInclJpsiRAASyst->RemovePoint(0);
  gPromptJpsiRAASyst->RemovePoint(0);
  gNonPromptJpsiRAASyst->RemovePoint(0);
  gPromptJpsiRAA2Syst->RemovePoint(0);
  gUpsRAASyst->RemovePoint(0);

  gNonPromptJpsiRAASyst->RemovePoint(1);
  gUpsRAASyst->RemovePoint(1);

  gInclJpsiRAA->SetName("gInclJpsiRAA");
  gPromptJpsiRAA->SetName("gPromptJpsiRAA");
  gNonPromptJpsiRAA->SetName("gNonPromptJpsiRAA");
  gPromptJpsiRAA2->SetName("gPromptJpsiRAA2");

  gInclJpsiRAAP->SetName("gInclJpsiRAAP");
  gPromptJpsiRAAP->SetName("gPromptJpsiRAAP");
  gNonPromptJpsiRAAP->SetName("gNonPromptJpsiRAAP");
  gPromptJpsiRAA2P->SetName("gPromptJpsiRAA2P");

  gInclJpsiRAAMB->SetName("gInclJpsiRAAMB");
  gPromptJpsiRAAMB->SetName("gPromptJpsiRAAMB");
  gNonPromptJpsiRAAMB->SetName("gNonPromptJpsiRAAMB");

  gInclJpsiRAAMBP->SetName("gInclJpsiRAAMBP");
  gPromptJpsiRAAMBP->SetName("gPromptJpsiRAAMBP");
  gNonPromptJpsiRAAMBP->SetName("gNonPromptJpsiRAAMBP");

  gUpsRAA->SetName("gUpsRAA");
  gUpsRAAP->SetName("gUpsRAAP");
  gUpsRAAMB->SetName("gUpsRAAMB");
  gUpsRAAMBP->SetName("gUpsRAAMBP");

  gInclJpsiRAASyst->SetName("gInclJpsiRAASyst");
  gPromptJpsiRAASyst->SetName("gPromptJpsiRAASyst");
  gNonPromptJpsiRAASyst->SetName("gNonPromptJpsiRAASyst");
  gPromptJpsiRAA2Syst->SetName("gPromptJpsiRAA2Syst");

  gInclJpsiRAAMBSyst->SetName("gInclJpsiRAAMBSyst");
  gPromptJpsiRAAMBSyst->SetName("gPromptJpsiRAAMBSyst");
  gNonPromptJpsiRAAMBSyst->SetName("gNonPromptJpsiRAAMBSyst");

  gUpsRAASyst->SetName("gUpsRAASyst");
  gUpsRAAMBSyst->SetName("gUpsRAAMBSyst");


  gInclJpsiRAASyst->SetFillColor(kAzure-9);
  gPromptJpsiRAASyst->SetFillColor(kRed-9);
  gNonPromptJpsiRAASyst->SetFillColor(kOrange-9);
  gPromptJpsiRAA2Syst->SetFillColor(kRed-9);
  gUpsRAASyst->SetFillColor(kGreen-9);

  gInclJpsiRAAMBSyst->SetFillStyle(0);
  gPromptJpsiRAAMBSyst->SetFillStyle(0);
  gNonPromptJpsiRAAMBSyst->SetFillStyle(0);
  gUpsRAAMBSyst->SetFillStyle(0);

  gInclJpsiRAAMBSyst->SetLineWidth(3);
  gPromptJpsiRAAMBSyst->SetLineWidth(3);
  gNonPromptJpsiRAAMBSyst->SetLineWidth(3);
  gUpsRAAMBSyst->SetLineWidth(3);

  gInclJpsiRAAMBSyst->SetLineColor(kAzure-9);
  gPromptJpsiRAAMBSyst->SetLineColor(kRed-9);
  gNonPromptJpsiRAAMBSyst->SetLineColor(kOrange-9);
  gUpsRAAMBSyst->SetLineColor(kGreen-9);

  if (isPaper) {
    gInclJpsiRAA->SetMarkerColor(kBlue+1);
    gPromptJpsiRAA->SetMarkerColor(kRed+2);
    gPromptJpsiRAA2->SetMarkerColor(kRed+2);
  }
  else {
    gInclJpsiRAA->SetMarkerColor(kBlue);
    gPromptJpsiRAA->SetMarkerColor(kRed);
    gPromptJpsiRAA2->SetMarkerColor(kRed);
  }
    gNonPromptJpsiRAA->SetMarkerColor(kOrange+2);
  gUpsRAA->SetMarkerColor(kGreen+2);

  if (isPaper) {
    gInclJpsiRAAMB->SetMarkerColor(kBlue+1);
    gPromptJpsiRAAMB->SetMarkerColor(kRed+2);
  }
  else {
    gInclJpsiRAAMB->SetMarkerColor(kBlue);
    gPromptJpsiRAAMB->SetMarkerColor(kRed);
  }
  gNonPromptJpsiRAAMB->SetMarkerColor(kOrange+2);
  gUpsRAAMB->SetMarkerColor(kGreen+2);

  gInclJpsiRAA->SetMarkerStyle(20);
  gPromptJpsiRAA->SetMarkerStyle(21);
  gNonPromptJpsiRAA->SetMarkerStyle(29);
  gPromptJpsiRAA2->SetMarkerStyle(21);
  gUpsRAA->SetMarkerStyle(33);

  gInclJpsiRAAMB->SetMarkerStyle(24);
  gPromptJpsiRAAMB->SetMarkerStyle(25);
  gNonPromptJpsiRAAMB->SetMarkerStyle(30);
  gUpsRAAMB->SetMarkerStyle(27);

  // gInclJpsiRAAMBP->SetMarkerStyle(24);
  // gPromptJpsiRAAMBP->SetMarkerStyle(25);
  // gNonPromptJpsiRAAMBP->SetMarkerStyle(30);
  // gUpsRAAMBP->SetMarkerStyle(27);

  gInclJpsiRAAP->SetMarkerStyle(24);
  gPromptJpsiRAAP->SetMarkerStyle(25);
  gNonPromptJpsiRAAP->SetMarkerStyle(30);
  gPromptJpsiRAA2P->SetMarkerStyle(25);
  gUpsRAAP->SetMarkerStyle(27);

  gInclJpsiRAAP->SetMarkerColor(kBlack);
  gPromptJpsiRAAP->SetMarkerColor(kBlack);
  gNonPromptJpsiRAAP->SetMarkerColor(kBlack);
  gPromptJpsiRAA2P->SetMarkerColor(kBlack);
  gUpsRAAP->SetMarkerColor(kBlack);

  gInclJpsiRAA->SetMarkerSize(1.2);
  gPromptJpsiRAA->SetMarkerSize(1.2);
  gNonPromptJpsiRAA->SetMarkerSize(2.0);
  gPromptJpsiRAA2->SetMarkerSize(1.2);
  gUpsRAA->SetMarkerSize(2.0);

  gInclJpsiRAAP->SetMarkerSize(1.2);
  gPromptJpsiRAAP->SetMarkerSize(1.2);
  gNonPromptJpsiRAAP->SetMarkerSize(2.0);
  gPromptJpsiRAA2P->SetMarkerSize(1.2);
  gUpsRAAP->SetMarkerSize(2.0);


  // gInclJpsiRAAMBP->SetMarkerColor(kBlack);
  // gPromptJpsiRAAMBP->SetMarkerColor(kBlack);
  // gNonPromptJpsiRAAMBP->SetMarkerColor(kBlack);
  // gUpsRAAMBP->SetMarkerColor(kBlack);

  gInclJpsiRAAMB->SetMarkerSize(1.2);
  gPromptJpsiRAAMB->SetMarkerSize(1.2);
  gNonPromptJpsiRAAMB->SetMarkerSize(2.0);
  gUpsRAAMB->SetMarkerSize(2.0);

  // gInclJpsiRAAMBP->SetMarkerSize(1.2);
  // gPromptJpsiRAAMBP->SetMarkerSize(1.2);
  // gNonPromptJpsiRAAMBP->SetMarkerSize(2.0);
  // gUpsRAAMBP->SetMarkerSize(2.0);


  TF1 *f4 = new TF1("f4","1",0,400);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  TBox *lumi;
  TBox *lumiPhenix;
  if (plotPhenix) {
    lumi = new TBox(0.0,0.93,20.0,1.07);
    lumi->SetFillColor(kGray+1);

    lumiPhenix = new TBox(380.0,0.908,399.0,1.092);
    lumiPhenix->SetFillStyle(0);
    lumiPhenix->SetLineWidth(2);
    lumiPhenix->SetLineColor(kBlue);
  }
  else
    lumi = new TBox(0.0,0.93,20.0,1.07);
    lumi->SetFillColor(kGray+1);

  TCanvas *c1 = new TCanvas("c1","c1");

  f4->Draw();
  lumi->Draw();
  if (plotPhenix)
    lumiPhenix->Draw("2");
  f4->Draw("same");

  double ppg068RAA_mid[] = {0.37, 0.57, 0.66, 0.74};
  double ppg068RAA_midError[] = {0.04, 0.06, 0.07, 0.12};
  double ppg068RAA_midSyst[] = {0.06, 0.08, 0.10, 0.11};
  double ppg068Npart_mid[] = {279.9, 140.0, 60.0, 14.5};
  double ppg068Npart_midError[] = {4.0, 5.0, 4.0, 2.5};
  double ppg068SystX[] = {10.0, 10.0, 10.0, 10.0};

  TGraphErrors *gPPG068RAA_mid = new TGraphErrors(4, ppg068Npart_mid, ppg068RAA_mid, ppg068Npart_midError, ppg068RAA_midError);
  TGraphErrors *gPPG068RAA_midSyst = new TGraphErrors(4, ppg068Npart_mid, ppg068RAA_mid, ppg068SystX, ppg068RAA_midSyst);

  gPPG068RAA_midSyst->SetFillColor(kGray);
  gPPG068RAA_mid->SetMarkerStyle(24);
  gPPG068RAA_mid->SetMarkerColor(kBlack);


  double ppg068RAA_fwd[] = {0.16, 0.27, 0.3, 0.38, 0.6, 0.75};
  double ppg068RAA_fwdError[] = {0.05, 0.035, 0.035, 0.038, 0.032, 0.049};
  double ppg068RAA_fwdSyst[] = {0.035, 0.022, 0.02, 0.023, 0.017, 0.032};
  double ppg068Npart_fwd[] = {325.2, 234.6, 166.6, 114.2, 60.0, 14.5};
  double ppg068Npart_fwdError[] = {3.3, 4.7, 5.4, 4.4, 4.0, 2.5};
  double ppg068SystX_fwd[] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0};



  TGraphErrors *gPPG068RAA_fwd = new TGraphErrors(6, ppg068Npart_fwd, ppg068RAA_fwd, ppg068Npart_fwdError, ppg068RAA_fwdError);
  TGraphErrors *gPPG068RAA_fwdSyst = new TGraphErrors(6, ppg068Npart_fwd, ppg068RAA_fwd, ppg068SystX, ppg068RAA_fwdSyst);

  gPPG068RAA_fwdSyst->SetFillColor(kOrange-9);
  gPPG068RAA_fwd->SetMarkerStyle(20);
  gPPG068RAA_fwd->SetMarkerColor(kOrange+10);

  // gPPG068RAA_midSyst->Draw("2");
  // gPPG068RAA_mid->Draw("P");
  
  // gPPG068RAA_fwdSyst->Draw("2");
  // gPPG068RAA_fwd->Draw("P");

  load_PHENIX();
  pgPhenix_y1222->SetMarkerStyle(24);
  pgPhenix_y1222_box->SetMarkerStyle(24);
  pgPhenix_y035->SetMarkerStyle(25);
  pgPhenix_y035_box->SetMarkerStyle(25);

  pgPhenix_y1222->SetMarkerColor(kBlue);
  pgPhenix_y1222->SetLineColor(kBlue);
  pgPhenix_y035->SetMarkerColor(kBlack);
  pgPhenix_y035->SetLineColor(kBlack);

  pgPhenix_y1222_box->SetLineColor(kBlue);
  pgPhenix_y035_box->SetLineColor(kBlack);
  pgPhenix_y035_box->SetFillStyle(0);
  pgPhenix_y1222_box->SetFillStyle(0);

  pgPhenix_y1222->SetMarkerSize(1.0);
  pgPhenix_y035->SetMarkerSize(1.0);


  //  gInclJpsiRAASyst->Draw("2");
  gPromptJpsiRAASyst->Draw("2");
  //  gNonPromptJpsiRAASyst->Draw("2");
  //  gUpsRAASyst->Draw("2");

  if (plotPhenix) {
    pgPhenix_y1222_box->Draw("2");
    pgPhenix_y035_box->Draw("2");
    pgPhenix_y1222->Draw("P");
    pgPhenix_y035->Draw("P");
  }  

   // gInclJpsiRAA->Draw("P");
   // gInclJpsiRAAP->Draw("P");
  gPromptJpsiRAA->Draw("P");
  gPromptJpsiRAAP->Draw("P");

  TLegend *leg1a;
  if (plotPhenix) {
    //    leg1a = new TLegend(0.44,0.45,0.77,0.65);
    leg1a = new TLegend(0.49,0.45,0.82,0.65);
    leg1a->SetFillStyle(0);
    leg1a->SetFillColor(0);
    leg1a->SetBorderSize(0);
    leg1a->SetMargin(0.2);
    leg1a->SetTextSize(0.035);
  }
    //leg1 = new TLegend(0.37,0.618,0.90,0.75);
  TLegend *leg1 = new TLegend(0.37,0.72,0.83,0.78);
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->SetMargin(0.2);
  leg1->SetTextSize(0.04);

  //    leg1->AddEntry(gInclJpsiRAA,"Inclusive J/#psi","P");
  leg1->AddEntry(gPromptJpsiRAA,"Prompt J/#psi","P");
  if (plotPhenix) {
    leg1a->SetHeader("AuAu  #sqrt{s_{NN}} = 200 GeV");
    leg1a->AddEntry(pgPhenix_y035,"PHENIX: |y|<0.35","P"); // AuAu  #sqrt{s_{NN}} = 200 GeV
    leg1a->AddEntry(pgPhenix_y1222,"PHENIX: 1.2<|y|<2.2","P");
    leg1a->AddEntry(pgPhenix_y1222,"(both arXiv:1103.6269)","");
    leg1a->Draw();
  }
  leg1->Draw();


  TLatex *pre = new TLatex(125,1.375,"CMS Preliminary");
  pre->SetTextFont(42);
  pre->SetTextSize(0.05);
  pre->Draw();

  TLatex *l1 = new TLatex(125,1.25,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1->SetTextFont(42);
  l1->SetTextSize(0.05);
  l1->Draw();

  TLatex *ly = new TLatex(20.0,0.16875,"0.0 < |y| < 2.4");
  ly->Draw();
  TLatex *lpt = new TLatex(20.0,0.075,"6.5 < p_{T} < 30.0 GeV/c");
  lpt->Draw();

  gPad->RedrawAxis();

  //  gNonPromptJpsiRAA->Draw("P");
  //  gNonPromptJpsiRAAP->Draw("P");
  // gUpsRAA->Draw("P");
  // gUpsRAAP->Draw("P");

  // gInclJpsiRAAMBSyst->Draw("2");
  // gPromptJpsiRAAMBSyst->Draw("2");
  // gNonPromptJpsiRAAMBSyst->Draw("2");
  //   gUpsRAAMBSyst->Draw("2");

  // gInclJpsiRAAMB->Draw("P");
  // gPromptJpsiRAAMB->Draw("P");
  // gNonPromptJpsiRAAMB->Draw("P");

  //   gUpsRAAMB->Draw("P");
  
  //  c1->SaveAs("PromptJpsi_RAA_comp_phenix.pdf");
  //  c1->SaveAs("PromptNonPromptJpsi_RAA.pdf");
  //  c1->SaveAs("PromptNonPromptJpsi_Ups_RAA.pdf");
  //   c1->SaveAs("AllJpsi_Ups_RAA.pdf");
  //   c1->SaveAs("AllJpsi_RAA.pdf");
  //  c1->SaveAs("InclPromptJpsi_RAA.pdf");
  if (plotPhenix) {
    c1->SaveAs("PromptJpsi_RAA_PHENIX.pdf");
    c1->SaveAs("PromptJpsi_RAA_PHENIX.png");
  }
  else {
    c1->SaveAs("PromptJpsi_RAA.pdf");
    c1->SaveAs("PromptJpsi_RAA.png");
    //    c1->SaveAs("InclPromptJpsi_RAA.pdf");
  //  c1->SaveAs("AllJpsi_Ups_MB_RAA.pdf");
  }

  TCanvas *c2 = new TCanvas("c2","c2");
  f4->Draw();
  lumi->Draw();
  f4->Draw("same");

  gPromptJpsiRAASyst->Draw("2");
  //  gPromptJpsiRAA2Syst->Draw("2");
  gNonPromptJpsiRAASyst->Draw("2");
  gUpsRAASyst->Draw("2");
  
  // gPromptJpsiRAAMBSyst->Draw("2");
  // gNonPromptJpsiRAAMBSyst->Draw("2");
  // gUpsRAAMBSyst->Draw("2");

  gPromptJpsiRAA->Draw("P");
  gPromptJpsiRAAP->Draw("P");
  // gPromptJpsiRAA2->Draw("P");
  // gPromptJpsiRAA2P->Draw("P");
  gNonPromptJpsiRAA->Draw("P");
  gNonPromptJpsiRAAP->Draw("P");
  gUpsRAA->Draw("P");
  gUpsRAAP->Draw("P");

  // gPromptJpsiRAAMB->Draw("P");
  // gNonPromptJpsiRAAMB->Draw("P");
  // gUpsRAAMB->Draw("P");

  //  TLegend *leg2 = new TLegend(0.37,0.67,0.83,0.74);
  TLegend *leg2 = new TLegend(0.37,0.61,0.83,0.81);
  // leg2->SetFillStyle(1001);
  // leg2->SetFillColor(10);
  leg2->SetFillStyle(0);
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetMargin(0.2);
  leg2->SetTextSize(0.04);
  // leg2->SetNColumns(2);
  // leg2->SetColumnSeparation(0.2);

  leg2->AddEntry(gPromptJpsiRAA2,"Prompt J/#psi","P");
  //  leg2->AddEntry(gPromptJpsiRAAMB,"(0-100%)","P");
  leg2->AddEntry(gNonPromptJpsiRAA,"Non-prompt J/#psi","P");
  //  leg2->AddEntry(gNonPromptJpsiRAAMB,"(0-100%)","P");
  leg2->AddEntry(gUpsRAA,"#Upsilon(1S)","P");
  //  leg2->AddEntry(gUpsRAAMB,"(0-100%)","P");
  leg2->Draw();


  pre->Draw();
  l1->Draw();
  ly->Draw();


  TLatex *lptJpsi = new TLatex(20.0,0.075,"6.5 < p_{T}^{J/#psi} < 30.0 GeV/c");
  TLatex *lptUps = new TLatex(220.0,0.075,"0.0 < p_{T}^{#Upsilon} < 20.0 GeV/c");
  lptJpsi->Draw();
  lptUps->Draw();

  TLatex *l020 = new TLatex(300, 0.8,"0-20%");
  TLatex *l2030 = new TLatex(180, 0.8,"20-30%");
  TLatex *l3040 = new TLatex(120, 0.8,"30-40%");
  TLatex *l4050 = new TLatex(80, 0.8,"40-50%");
  TLatex *l5000 = new TLatex(20, 0.8,"50-100%");

  // l020->Draw();
  // l2030->Draw();
  // l3040->Draw();
  // l4050->Draw();
  // l5000->Draw();

  gPad->RedrawAxis();

  //  c2->SaveAs("NonPromptJpsi_RAA.pdf");
  c2->SaveAs("PromptNonPromptJpsi_Ups_RAA2.pdf");
  c2->SaveAs("PromptNonPromptJpsi_Ups_RAA2.png");

  /*
  TCanvas *c3 = new TCanvas("c3","c3");
  f4->Draw();
  lumi->Draw();
  f4->Draw("same");

  gPromptJpsiRAAMBSyst->Draw("2");
  gNonPromptJpsiRAAMBSyst->Draw("2");
  gUpsRAAMBSyst->Draw("2");
  

  gPromptJpsiRAAMB->Draw("P");
  gPromptJpsiRAAMBP->Draw("P");
  gNonPromptJpsiRAAMB->Draw("P");
  gNonPromptJpsiRAAMBP->Draw("P");
  gUpsRAAMB->Draw("P");
  gUpsRAAMBP->Draw("P");

  TLegend *leg3 = new TLegend(0.45,0.44,0.90,0.64);
  // leg3->SetFillStyle(1001);
  // leg3->SetFillColor(10);
  leg3->SetFillStyle(0);
  leg3->SetFillColor(0);
  leg3->SetBorderSize(0);
  leg3->SetMargin(0.2);
  leg3->SetTextSize(0.04);

  leg3->AddEntry(gPromptJpsiRAAMB,"Prompt J/#psi","P");
  leg3->AddEntry(gNonPromptJpsiRAAMB,"Non-prompt J/#psi","P");
  leg3->AddEntry(gUpsRAAMB,"#Upsilon(1S)","P");
  leg3->Draw();


  pre->Draw();
  l1->Draw();
  ly->Draw();
  lptJpsi->Draw();
  lptUps->Draw();


  gPad->RedrawAxis();

  c3->SaveAs("PromptNonPromptJpsi_Ups_MB_RAA.pdf");
  */

  return;
}

void load_PHENIX()
{
  const int nBins_phenix_y1222                        = 17;
  const char* cbins_phenix_y1222[nBins_phenix_y1222]  = {"[0,5]","[5,10]","[10,15]","[15,20]","[20,25]","[25,30]","[30,35]",
							  "[35,40]","[40,45]","[45,50]","[50,55]","[55,60]","[60,65]","[65,70]",
							  "[70,75]","[75,80]","[80,92]"};
  
  double adNpart_phenix_y1222[nBins_phenix_y1222]     = {350.8, 301.7, 255.7, 216.4, 182.4, 152.7, 126.8,
							 104.2,  84.6,  67.7,  53.2,  41.0,  30.8,  22.6, 
							 16.1,  11.2,   5.6};
  double adNpart_phenix_y1222_err[nBins_phenix_y1222] = {3.1,   4.7,   5.4,   5.6,   5.7,   5.9,   5.9, 
							 5.8,   5.6,   5.4,   5.0,   4.5,   3.9,   3.4, 
							 2.8,   2.2,   0.8};
  double adNpart_phenix_y1222_err0[nBins_phenix_y1222] = {0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
							  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
							  0.0,   0.0,   0.0};
  
  double adRaa_phenix_y1222[nBins_phenix_y1222]       = {0.17, 0.16, 0.20, 0.25, 0.25, 0.35, 0.35,
							 0.41, 0.52, 0.49, 0.54, 0.80, 0.68, 0.72,
							 0.91, 1.03, 1.20};
  double adRaa_phenix_y1222_errA[nBins_phenix_y1222]  = {0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04,
							 0.04, 0.04, 0.04, 0.04, 0.05, 0.06, 0.06, 
							 0.08, 0.11, 0.10};
  double adRaa_phenix_y1222_errBp[nBins_phenix_y1222] = {0.02, 0.02, 0.03, 0.04, 0.03, 0.04, 0.04,
							 0.06, 0.07, 0.07, 0.09, 0.14, 0.13, 0.15,
							 0.21, 0.26, 0.23};
  double adRaa_phenix_y1222_errBm[nBins_phenix_y1222] = {0.04, 0.02, 0.03, 0.04, 0.03, 0.04, 0.04,
							 0.06, 0.07, 0.07, 0.09, 0.14, 0.13, 0.15,
							 0.21, 0.26, 0.23};
  double adRaa_phenix_y1222_errC[nBins_phenix_y1222]  = {0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09,
							 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09,
							 0.09, 0.09, 0.09};

  const int nBins_phenix_y035                       = 8;
  const char* cbins_phenix_y035[nBins_phenix_y035]    = {"[0,5]","[5,10]","[10,15]","[15,20]","[20,30]","[30,40]","[40,60]","[60,93]"};
  
  double adNpart_phenix_y035[nBins_phenix_y035]     = {351.4, 299.0, 253.9, 215.3, 166.6, 114.2, 58.4, 14.5};
  double adNpart_phenix_y035_err[nBins_phenix_y035] = {  2.9,   3.8,   4.3,   5.3,   5.4,   4.4,  4.0,  2.5};
  double adNpart_phenix_y035_err0[nBins_phenix_y035]= {  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0, 0.0};
  double adRaa_phenix_y035[nBins_phenix_y035]       = {0.26, 0.34, 0.36, 0.45, 0.58, 0.58, 0.65, 0.74};
  double adRaa_phenix_y035_errA[nBins_phenix_y035]  = {0.05, 0.06, 0.06, 0.07, 0.07, 0.08, 0.07, 0.12};
  double adRaa_phenix_y035_errBp[nBins_phenix_y035] = {0.04, 0.05, 0.05, 0.07, 0.08, 0.08, 0.10, 0.21};
  double adRaa_phenix_y035_errBm[nBins_phenix_y035] = {0.04, 0.05, 0.05, 0.07, 0.08, 0.08, 0.10, 0.21};
  double adRaa_phenix_y035_errC[nBins_phenix_y035]  = {0.03, 0.04, 0.04, 0.05, 0.07, 0.07, 0.08, 0.09};

  // define the graphs:
  // forward
  pgPhenix_y1222           = new TGraphErrors(nBins_phenix_y1222, 
					      adNpart_phenix_y1222,      adRaa_phenix_y1222,
					      adNpart_phenix_y1222_err0, adRaa_phenix_y1222_errA
					      );
  pgPhenix_y1222_box  = new TGraphAsymmErrors(nBins_phenix_y1222, 
					      adNpart_phenix_y1222,      adRaa_phenix_y1222,
					      adNpart_phenix_y1222_err,  adNpart_phenix_y1222_err,
					      adRaa_phenix_y1222_errBm,  adRaa_phenix_y1222_errBp
					      );
  
  // barrel
  pgPhenix_y035            = new TGraphErrors(nBins_phenix_y035, 
					      adNpart_phenix_y035,      adRaa_phenix_y035,
					      adNpart_phenix_y035_err0, adRaa_phenix_y035_errA
					      );
  pgPhenix_y035_box   = new TGraphAsymmErrors(nBins_phenix_y035, 
					      adNpart_phenix_y035,      adRaa_phenix_y035,
					      adNpart_phenix_y035_err,  adNpart_phenix_y035_err,
					      adRaa_phenix_y035_errBm,  adRaa_phenix_y035_errBp
					      );

}
