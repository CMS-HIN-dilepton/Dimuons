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
#include "TPaveStats.h"
#include "TLatex.h"
#include "TLegend.h"

void plot_cent_allQQ(bool isPaper=false)
{
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  // incl. and prompt J/psi
  // 0-100, 0-10, 10-20, 20-30, 30-40, 40-50, 50-100
  double promptTAA[] = {5.6625, 23.1857, 14.4777, 8.7830, 5.0892, 2.7484, 0.4682};
  double promptTAAError[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  double promptNpart[] = {113.0518, 355.3528, 261.4178, 187.1470, 129.9835, 86.2622, 22.0709};
  double promptNpartError[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  double promptNcoll[] = {362.4007, 1483.8875, 926.5725, 562.1105, 325.7105, 175.8948, 29.9663};
  double promptNcollError[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double promptrelCentBinWidth[] = {10.0, 1.0, 1.0, 1.0, 1.0, 1.0, 5.0};


  // Upsilon
  // 0-100, 0-10, 10-20, 20-100
  double upsTAA[] = {5.6625, 23.1857, 14.4777, 2.3702};
  double upsTAAError[] = {0.0, 0.0, 0.0, 0.0};

  double upsNpart[] = {113.0518, 355.3528, 261.4178, 64.2184};
  double upsNpartError[] = {0.0, 0.0, 0.0, 0.0};

  double upsNcoll[] = {362.4007, 1483.8875, 926.5725, 151.6934};
  double upsNcollError[] = {0.0, 0.0, 0.0, 0.0};
  double upsrelCentBinWidth[] = {10.0, 1.0, 1.0, 8.0};

  // non-prompt J/psi
  // 0-100, 0-20, 20-100
  double nonPromptTAA[] = {5.6625, 18.8317, 2.3702};
  double nonPromptTAAError[] = {0.0, 0.0, 0.0};

  double nonPromptNpart[] = {113.0518, 308.3853, 64.2184};
  double nonPromptNpartError[] = {0.0, 0.0, 0.0};

  double nonPromptNcoll[] = {362.4007, 1205.2300, 151.6934};
  double nonPromptNcollError[] = {0.0, 0.0, 0.0};
  double nonPromptrelCentBinWidth[] = {10.0, 2.0, 8.0};

  double promptSystX[] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};
  double nonPromptSystX[] = {10.0, 10.0, 10.0, 10.0, 10.0};
  double upsSystX[] = {10.0, 10.0, 10.0, 10.0, 10.0};

  
  double inclJpsiCent[] = {2.41, 1.78, 1.93, 2.37, 3.73, 5.23, 4.67};
  double inclJpsiCentError[] = {0.15, 0.20, 0.24, 0.33, 0.53, 0.81, 0.80};
  double inclJpsiCentSyst[] = {0.37, 0.28, 0.30, 0.37, 0.62, 0.93, 0.97};  

  double promptJpsiCent[] = {1.79, 1.18, 1.29, 2.18, 2.97, 3.88, 3.58};
  double promptJpsiCentError[] = {0.13, 0.17, 0.21, 0.33, 0.49, 0.75, 0.70};
  double promptJpsiCentSyst[] = {0.28, 0.18, 0.20, 0.34, 0.50, 0.69, 0.74};
    

  double inclJpsiCent2[] = {2.41, 1.84, 3.46};
  double inclJpsiCent2Error[] = {0.15, 0.15, 0.26};
  double inclJpsiCent2Syst[] = {0.37, 0.28, 0.57};

  double promptJpsiCent2[] = {1.79, 1.23, 2.85};
  double promptJpsiCent2Error[] = {0.13, 0.14, 0.25};
  double promptJpsiCent2Syst[] = {0.28, 0.18, 0.47};

  double nonPromptJpsiCent[] = {0.60, 0.59, 0.60};
  double nonPromptJpsiCentError[] = {0.09, 0.12, 0.14};
  double nonPromptJpsiCentSyst[] = {0.09, 0.10, 0.10};
  
  double upsCent[] = {0.485, 0.347, 0.643, 0.517};
  double upsCentError[] = {0.066, 0.096, 0.144, 0.101};
  double upsCentSyst[] = {0.089, 0.069, 0.118, 0.101};

  double upsCent2[] = {0.485, 0.468, 0.517};
  double upsCent2Error[] = {0.066, 0.081, 0.101};
  double upsCent2Syst[] = {0.089, 0.094, 0.101};

  TGraphErrors *gInclJpsiCent = new TGraphErrors(7, promptNpart, inclJpsiCent, promptNpartError, inclJpsiCentError);
  TGraphErrors *gPromptJpsiCent = new TGraphErrors(7, promptNpart, promptJpsiCent, promptNpartError, promptJpsiCentError);
  TGraphErrors *gNonPromptJpsiCent = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiCent, nonPromptNpartError, nonPromptJpsiCentError);
  TGraphErrors *gUpsCent = new TGraphErrors(4, upsNpart, upsCent, upsNpartError, upsCentError);

  TGraphErrors *gInclJpsiCentSyst = new TGraphErrors(7, promptNpart, inclJpsiCent, promptSystX, inclJpsiCentSyst);
  TGraphErrors *gPromptJpsiCentSyst = new TGraphErrors(7, promptNpart, promptJpsiCent, promptSystX, promptJpsiCentSyst);
  TGraphErrors *gNonPromptJpsiCentSyst = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiCent, nonPromptSystX, nonPromptJpsiCentSyst);
  TGraphErrors *gUpsCentSyst = new TGraphErrors(4, upsNpart, upsCent, upsSystX, upsCentSyst);

  TGraphErrors *gInclJpsiCentP = new TGraphErrors(7, promptNpart, inclJpsiCent, promptNpartError, promptNpartError);
  TGraphErrors *gPromptJpsiCentP = new TGraphErrors(7, promptNpart, promptJpsiCent, promptNpartError, promptNpartError);
  TGraphErrors *gNonPromptJpsiCentP = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiCent, nonPromptNpartError, nonPromptNpartError);
  TGraphErrors *gUpsCentP = new TGraphErrors(4, upsNpart, upsCent, upsNpartError, upsNpartError);


  TGraphErrors *gInclJpsiMB = new TGraphErrors(1, promptNpart, inclJpsiCent, promptNpartError, inclJpsiCentError);
  TGraphErrors *gPromptJpsiMB = new TGraphErrors(1, promptNpart, promptJpsiCent, promptNpartError, promptJpsiCentError);
  TGraphErrors *gNonPromptJpsiMB = new TGraphErrors(1, nonPromptNpart, nonPromptJpsiCent, nonPromptNpartError, nonPromptJpsiCentError);
  TGraphErrors *gUpsMB = new TGraphErrors(1, upsNpart, upsCent, upsNpartError, upsCentError);

  TGraphErrors *gInclJpsiMBSyst = new TGraphErrors(1, promptNpart, inclJpsiCent, promptSystX, inclJpsiCentSyst);
  TGraphErrors *gPromptJpsiMBSyst = new TGraphErrors(1, promptNpart, promptJpsiCent, promptSystX, promptJpsiCentSyst);
  TGraphErrors *gNonPromptJpsiMBSyst = new TGraphErrors(1, nonPromptNpart, nonPromptJpsiCent, nonPromptSystX, nonPromptJpsiCentSyst);
  TGraphErrors *gUpsMBSyst = new TGraphErrors(1, upsNpart, upsCent, upsSystX, upsCentSyst);  


  TGraphErrors *gInclJpsiCent2 = new TGraphErrors(3, nonPromptNpart, inclJpsiCent2, nonPromptNpartError, inclJpsiCent2Error);
  TGraphErrors *gPromptJpsiCent2 = new TGraphErrors(3, nonPromptNpart, promptJpsiCent2, nonPromptNpartError, promptJpsiCent2Error);
  TGraphErrors *gInclJpsiCent2Syst = new TGraphErrors(3, nonPromptNpart, inclJpsiCent2, nonPromptSystX, inclJpsiCent2Syst);
  TGraphErrors *gPromptJpsiCent2Syst = new TGraphErrors(3, nonPromptNpart, promptJpsiCent2, nonPromptSystX, promptJpsiCent2Syst);


  // Remove MB point
  gInclJpsiCent->RemovePoint(0);
  gPromptJpsiCent->RemovePoint(0);
  gNonPromptJpsiCent->RemovePoint(0);
  gUpsCent->RemovePoint(0);

  gInclJpsiCentP->RemovePoint(0);
  gPromptJpsiCentP->RemovePoint(0);
  gNonPromptJpsiCentP->RemovePoint(0);
  gUpsCentP->RemovePoint(0);

  gInclJpsiCentSyst->RemovePoint(0);
  gPromptJpsiCentSyst->RemovePoint(0);
  gNonPromptJpsiCentSyst->RemovePoint(0);
  gUpsCentSyst->RemovePoint(0);

  gInclJpsiCent2->RemovePoint(0);
  gPromptJpsiCent2->RemovePoint(0);
  gInclJpsiCent2Syst->RemovePoint(0);
  gPromptJpsiCent2Syst->RemovePoint(0);


  gInclJpsiCent->SetName("gInclJpsiCent");
  gPromptJpsiCent->SetName("gPromptJpsiCent");
  gNonPromptJpsiCent->SetName("gNonPromptJpsiCent");

  gInclJpsiMB->SetName("gInclJpsiMB");
  gPromptJpsiMB->SetName("gPromptJpsiMB");
  gNonPromptJpsiMB->SetName("gNonPromptJpsiMB");

  gUpsCent->SetName("gUpsCent");
  gUpsMB->SetName("gUpsMB");

  gInclJpsiCent2->SetName("gInclJpsiCent2");
  gPromptJpsiCent2->SetName("gPromptJpsiCent2");

  gInclJpsiCentP->SetName("gInclJpsiCentP");
  gPromptJpsiCentP->SetName("gPromptJpsiCentP");
  gNonPromptJpsiCentP->SetName("gNonPromptJpsiCentP");
  gUpsCentP->SetName("gUpsCentP");

  gInclJpsiCentSyst->SetName("gInclJpsiCentSyst");
  gPromptJpsiCentSyst->SetName("gPromptJpsiCentSyst");
  gNonPromptJpsiCentSyst->SetName("gNonPromptJpsiCentSyst");

  gInclJpsiMBSyst->SetName("gInclJpsiMBSyst");
  gPromptJpsiMBSyst->SetName("gPromptJpsiMBSyst");
  gNonPromptJpsiMBSyst->SetName("gNonPromptJpsiMBSyst");

  gUpsCentSyst->SetName("gUpsCentSyst");
  gUpsMBSyst->SetName("gUpsMBSyst");

  gInclJpsiCent2Syst->SetName("gInclJpsiCent2Syst");
  gPromptJpsiCent2Syst->SetName("gPromptJpsiCent2Syst");

  // gInclJpsiCentSyst->SetFillColor(kGray+1);
  // gPromptJpsiCentSyst->SetFillColor(kAzure-9);
  // gNonPromptJpsiCentSyst->SetFillColor(kRed-10);
  // gUpsCentSyst->SetFillColor(kGreen-10);
  // gInclJpsiMBSyst->SetFillColor(kGray+1);
  // gPromptJpsiMBSyst->SetFillColor(kAzure-9);
  // gNonPromptJpsiMBSyst->SetFillColor(kRed-10);
  // gUpsMBSyst->SetFillColor(kGreen-10);
  // gInclJpsiCent->SetMarkerColor(kBlack);
  // gPromptJpsiCent->SetMarkerColor(kBlue);
  // gNonPromptJpsiCent->SetMarkerColor(kRed);
  // gUpsCent->SetMarkerColor(kGreen+2);

  gInclJpsiCentSyst->SetFillColor(kAzure-9);
  gPromptJpsiCentSyst->SetFillColor(kRed-9);
  gNonPromptJpsiCentSyst->SetFillColor(kOrange-9);
  gUpsCentSyst->SetFillColor(kGreen-9);

  gInclJpsiMBSyst->SetFillStyle(0);
  gPromptJpsiMBSyst->SetFillStyle(0);
  gNonPromptJpsiMBSyst->SetFillStyle(0);
  gUpsMBSyst->SetFillStyle(0);

  gInclJpsiMBSyst->SetLineWidth(3);
  gPromptJpsiMBSyst->SetLineWidth(3);
  gNonPromptJpsiMBSyst->SetLineWidth(3);
  gUpsMBSyst->SetLineWidth(3);

  gInclJpsiMBSyst->SetLineColor(kAzure-9);
  gPromptJpsiMBSyst->SetLineColor(kRed-9);
  gNonPromptJpsiMBSyst->SetLineColor(kOrange-9);
  gUpsMBSyst->SetLineColor(kGreen-9);

  if (isPaper) {
    gInclJpsiCent->SetMarkerColor(kBlue+1);
    gPromptJpsiCent->SetMarkerColor(kRed+2);
  }
  else {
    gInclJpsiCent->SetMarkerColor(kBlue);
    gPromptJpsiCent->SetMarkerColor(kRed);
  }
  gNonPromptJpsiCent->SetMarkerColor(kOrange+2);
  gUpsCent->SetMarkerColor(kGreen+2);

  gInclJpsiCent->SetMarkerStyle(20);
  gPromptJpsiCent->SetMarkerStyle(21);
  gNonPromptJpsiCent->SetMarkerStyle(29);
  gUpsCent->SetMarkerStyle(33);

  if (isPaper) {
    gInclJpsiMB->SetMarkerColor(kBlue+1);
    gPromptJpsiMB->SetMarkerColor(kRed+2);
  }
  else {
    gInclJpsiMB->SetMarkerColor(kBlue);
    gPromptJpsiMB->SetMarkerColor(kRed);
  }

  gNonPromptJpsiMB->SetMarkerColor(kOrange+2);
  gUpsMB->SetMarkerColor(kGreen+2);

  gInclJpsiMB->SetMarkerStyle(24);
  gPromptJpsiMB->SetMarkerStyle(25);
  gNonPromptJpsiMB->SetMarkerStyle(30);
  gUpsMB->SetMarkerStyle(27);

  gInclJpsiCent->SetMarkerSize(1.2);
  gInclJpsiMB->SetMarkerSize(1.2);

  gPromptJpsiCent->SetMarkerSize(1.2);
  gPromptJpsiMB->SetMarkerSize(1.2);

  gNonPromptJpsiCent->SetMarkerSize(2.0);
  gNonPromptJpsiMB->SetMarkerSize(2.0);

  gUpsCent->SetMarkerSize(2.0);
  gUpsMB->SetMarkerSize(2.0);

  gInclJpsiCentP->SetMarkerStyle(24);
  gPromptJpsiCentP->SetMarkerStyle(25);
  gNonPromptJpsiCentP->SetMarkerStyle(30);
  gUpsCentP->SetMarkerStyle(27);

  gInclJpsiCentP->SetMarkerColor(kBlack);
  gPromptJpsiCentP->SetMarkerColor(kBlack);
  gNonPromptJpsiCentP->SetMarkerColor(kBlack);
  gUpsCentP->SetMarkerColor(kBlack);

  gInclJpsiCentP->SetMarkerSize(1.2);
  gPromptJpsiCentP->SetMarkerSize(1.2);
  gNonPromptJpsiCentP->SetMarkerSize(2.0);
  gUpsCentP->SetMarkerSize(2.0);


  TH1F *f1 = new TH1F("f1","f1",1,0,400);
  TH1F *f1l = new TH1F("f1l","f1l",1,0,400);
  f1->SetBinContent(1,9.2);
  f1l->SetBinContent(1,3.8);
  f1->SetBinError(1,0.0);
  f1l->SetBinError(1,0.0);

  f1->SetLineWidth(1);
  f1->SetLineStyle(1);
  f1->SetFillColor(kGray);
  f1->SetFillStyle(1001);
  f1l->SetLineWidth(1);
  f1l->SetLineStyle(1);
  f1l->SetFillColor(10);
  f1l->SetFillStyle(1001);
  f1->GetXaxis()->SetTitle("N_{part}");
  f1->GetYaxis()->SetTitle("1/T_{AA} dN/dy (nb)");
  f1->GetXaxis()->CenterTitle(kTRUE);
  f1->GetYaxis()->SetRangeUser(0.0, 12.0);

  TCanvas *c1 = new TCanvas("c1","c1");

  f1->Draw("HIST2");
  f1l->Draw("HIST2same");
  gInclJpsiCentSyst->Draw("2");
  gPromptJpsiCentSyst->Draw("2");
  gInclJpsiCent->Draw("P");
  gPromptJpsiCent->Draw("P");
  gInclJpsiCentP->Draw("P");
  gPromptJpsiCentP->Draw("P");

  TLegend *leg1 = new TLegend(0.22,0.56,0.90,0.76);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(10);
  leg1->SetBorderSize(0);
  leg1->SetMargin(0.2);
  leg1->SetTextSize(0.04);

  leg1->AddEntry(gInclJpsiCent,"Inclusive J/#psi","P");
  leg1->AddEntry(gPromptJpsiCent,"Prompt J/#psi","P");
  leg1->AddEntry(f1,"pp interpolation (arXiv:1103.2394)","F");
  leg1->Draw();


  TLatex *pre = new TLatex(125,11.0,"CMS Preliminary");
  pre->SetTextFont(42);
  pre->SetTextSize(0.05);
  pre->Draw();

  TLatex *l1 = new TLatex(125,10.0,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1->SetTextFont(42);
  l1->SetTextSize(0.05);
  l1->Draw();

  // TLatex *ly = new TLatex(200.0,5.25,"0.0 < |y| < 2.4");
  // ly->Draw();
  // TLatex *lpt = new TLatex(200.0,4.5,"6.5 < p_{T} < 30.0 GeV/c");
  // lpt->Draw();


  TLatex *ly = new TLatex(20.0,1.35,"0.0 < |y| < 2.4");
  ly->Draw();
  TLatex *lpt = new TLatex(20.0,0.6,"6.5 < p_{T} < 30.0 GeV/c");
  lpt->Draw();

  gPad->RedrawAxis();

  TCanvas *c1a = new TCanvas("c1a","c1a");

  TH1F *f1a = new TH1F("f1a","f1a",1,0,400);
  TH1F *f1al = new TH1F("f1al","f1al",1,0,400);
  f1a->SetBinContent(1,9.2*0.23); // MB high pT b-fraction
  f1al->SetBinContent(1,3.8*0.23); // MB high pT b-fraction
  f1a->SetBinError(1,0.0);
  f1al->SetBinError(1,0.0);

  f1a->SetLineWidth(1);
  f1a->SetLineStyle(1);
  f1a->SetFillColor(kGray);
  f1a->SetFillStyle(1001);
  f1al->SetLineWidth(1);
  f1al->SetLineStyle(1);
  f1al->SetFillColor(10);
  f1al->SetFillStyle(1001);
  f1a->GetXaxis()->SetTitle("N_{part}");
  f1a->GetYaxis()->SetTitle("1/T_{AA} dN/dy (nb)");
  f1a->GetXaxis()->CenterTitle(kTRUE);
  f1a->GetYaxis()->SetRangeUser(0.0,2.76);

  f1a->Draw("HIST2");
  f1al->Draw("HIST2same");
  gNonPromptJpsiCentSyst->Draw("2");
  gNonPromptJpsiCent->Draw("P");
  gNonPromptJpsiCentP->Draw("P");
  gNonPromptJpsiMBSyst->Draw("2");
  gNonPromptJpsiMB->Draw("P");


  TLegend *leg1a = new TLegend(0.22,0.56,0.90,0.69);
  leg1a->SetFillStyle(1001);
  leg1a->SetFillColor(kWhite);
  leg1a->SetBorderSize(0);
  leg1a->SetMargin(0.2);
  leg1a->SetTextSize(0.04);
  leg1a->SetNColumns(2);
  leg1a->SetColumnSeparation(-0.78);

  // leg1a->AddEntry(gNonPromptJpsiCent,"Non-prompt J/#psi (0-20%, 20-100%)","P");
  leg1a->AddEntry(gNonPromptJpsiCent,"Non-prompt J/#psi","P");
  leg1a->AddEntry(gNonPromptJpsiMB,"(0-100%)","P");
  //  leg1a->AddEntry(gNonPromptJpsiMB,"Non-prompt J/#psi (0-100%)","P");
  leg1a->AddEntry(f1a,"pp interpolation (arXiv:1103.2394)","F");
  leg1a->Draw();

  TLatex *prea = new TLatex(125,2.53,"CMS Preliminary");
  prea->SetTextFont(42);
  prea->SetTextSize(0.05);
  prea->Draw();

  TLatex *l1a = new TLatex(125,2.30,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1a->SetTextSize(0.05);
  l1a->Draw();

  // TLatex *l1a = new TLatex(125,2.30,"L_{int} = 7.28");
  // l1a->SetTextSize(0.05);
  // l1a->Draw();

  // TLatex *lya = new TLatex(200.0,1.2075,"0.0 < |y| < 2.4");
  // lya->Draw();
  // TLatex *lpta = new TLatex(200.0,1.035,"6.5 < p_{T} < 30.0 GeV/c");
  // lpta->Draw();
  TLatex *lya = new TLatex(20.0,0.3105,"0.0 < |y| < 2.4");
  lya->Draw();
  TLatex *lpta = new TLatex(20.0,0.138,"6.5 < p_{T} < 30.0 GeV/c");
  lpta->Draw();

  gPad->RedrawAxis();
  
  TCanvas *c1b = new TCanvas("c1b","c1b");
  c1b->SetLogy();
  // TF1 *f1b = new TF1("f1b","0.753",0,400);
  // f1b->SetLineWidth(1);
  // f1b->SetLineStyle(2);
  // f1b->GetXaxis()->SetTitle("N_{part}");
  // f1b->GetYaxis()->SetTitle("1/T_{AA} dN/dy (pb)");
  // f1b->GetXaxis()->CenterTitle(kTRUE);

  // f1b->Draw();
  // f1b->GetYaxis()->SetRangeUser(0.0,1.6);

  TH1F *f1b = new TH1F("f1b","f1b",1,0,400);
  TH1F *f1bl = new TH1F("f1bl","f1bl",1,0,400);
  f1b->SetBinContent(1,0.753+sqrt(0.029*0.029+0.072*0.072));
  f1bl->SetBinContent(1,0.753-sqrt(0.029*0.029+0.072*0.072));
  f1b->SetBinError(1,0.0);
  f1bl->SetBinError(1,0.0);

  f1b->SetLineWidth(1);
  f1b->SetLineStyle(1);
  f1b->SetFillColor(kGray);
  f1b->SetFillStyle(1001);
  f1bl->SetLineWidth(2);
  f1bl->SetLineStyle(3);
  f1bl->SetFillColor(10);
  f1bl->SetFillStyle(1001);
  f1b->GetXaxis()->SetTitle("N_{part}");
  f1b->GetYaxis()->SetTitle("1/T_{AA} dN/dy (pb)");
  f1b->GetXaxis()->CenterTitle(kTRUE);
  f1b->GetYaxis()->SetRangeUser(0.0,1.8);



  TH1F *f2b = new TH1F("f2b","f2b",1,0,400);
  TH1F *f2bl = new TH1F("f2bl","f2bl",1,0,400);
  f2b->SetBinContent(1,7.37 + (0.13*0.13 + 0.61*0.61));
  f2bl->SetBinContent(1,7.37 - sqrt(0.13*0.13 + 0.42*0.42));
  f2b->SetBinError(1,0.0);
  f2bl->SetBinError(1,0.0);

  f2b->SetLineWidth(2);
  f2b->SetLineStyle(2);
  f2b->SetFillColor(kGray);
  f2b->SetFillStyle(1001);
  f2bl->SetLineWidth(1);
  f2bl->SetLineStyle(1);
  f2bl->SetFillColor(10);
  f2bl->SetFillStyle(1001);
  f2b->GetXaxis()->SetTitle("N_{part}");
  f2b->GetYaxis()->SetTitle("1/T_{AA} dN/dy (pb)");
  f2b->GetXaxis()->CenterTitle(kTRUE);
  f2b->GetYaxis()->SetRangeUser(0.1,20);


  f2b->Draw("HIST2");
  // f2bl->Draw("HIST2same");
  //  f1b->Draw("HIST2");
  f1bl->Draw("HIST2same");

  gUpsCentSyst->Draw("2");
  gUpsCent->Draw("P");
  gUpsCentP->Draw("P");
  gUpsMBSyst->Draw("2");
  gUpsMB->Draw("P");

  TLegend *leg1b = new TLegend(0.22,0.51,0.90,0.76);
  leg1b->SetFillStyle(1001);
  leg1b->SetFillColor(kWhite);
  leg1b->SetBorderSize(0);
  leg1b->SetMargin(0.2);
  leg1b->SetTextSize(0.04);
  leg1b->SetNColumns(2);
  leg1b->SetColumnSeparation(-1.25);
  leg1b->AddEntry(gUpsCent,"#Upsilon(1S)","P");
  leg1b->AddEntry(gUpsMB,"(0-100%)","P");
  leg1b->AddEntry(f1b,"CDF p#bar{p}  #sqrt{s} = 1.8 TeV","L");
  leg1b->AddEntry(f1b," ","");
  leg1b->AddEntry(f1b,"(PRL 75, 4358 (1995))","");
  leg1b->AddEntry(f2b," ","");
  leg1b->AddEntry(f2b,"CMS pp  #sqrt{s} = 7 TeV","L");
  leg1b->AddEntry(f2b," ","");
  leg1b->AddEntry(f2b,"(arXiv:1012.5545)","");
  leg1b->Draw();

  //  TLatex *preb = new TLatex(125,9.1667,"CMS Preliminary");
  TLatex *preb = new TLatex(125,12.816,"CMS Preliminary");
  preb->SetTextFont(42);
  preb->SetTextSize(0.05);
  preb->Draw();

  //  TLatex *l1b = new TLatex(125,8.889,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  TLatex *l1b = new TLatex(125,8.752,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l1b->SetTextSize(0.05);
  l1b->Draw();

  TLatex *lyb = new TLatex(20.0,0.185,"0.0 < |y| < 2.4");
  //  TLatex *lyb = new TLatex(20.0,0.2025,"0.0 < |y| < 2.4");
  lyb->Draw();
  TLatex *lptb = new TLatex(20.0,0.130,"0.0 < p_{T} < 20.0 GeV/c");
  //  TLatex *lptb = new TLatex(20.0,0.09,"0.0 < p_{T} < 20.0 GeV/c");
  lptb->Draw();

  gPad->RedrawAxis();

  c1->SaveAs("inclJpsi_cent.pdf");
  c1a->SaveAs("nonpromptJpsi_cent.pdf");
  c1b->SaveAs("upsilon_cent.pdf");

  c1->SaveAs("inclJpsi_cent.png");
  c1a->SaveAs("nonpromptJpsi_cent.png");
  c1b->SaveAs("upsilon_cent.png");



  double bfractionCent[] = {0.2550, 0.3337, 0.3268, 0.0786, 0.2003, 0.2550, 0.2317};
  double bfractionCentError[] = {0.0351, 0.0723, 0.0833, 0.0541, 0.0738, 0.0966, 0.0885};
  double bfractionCentSyst[] = {0.0510, 0.0667, 0.0654, 0.0157, 0.0401, 0.0510, 0.0463}; // fixme: update syst. uncertainties

  double bfractionCoarseCent[] = {0.2550, 0.3289, 0.1757};
  double bfractionCoarseCentError[] = {0.0351, 0.0583, 0.0401};
  double bfractionCoarseCentSyst[] = {0.0510, 0.0658, 0.0351}; // fixme: update syst. uncertainties
  
  
  TGraphErrors *gBfractionCent = new TGraphErrors(7, promptNpart, bfractionCent, promptNpartError, bfractionCentError);

  TGraphErrors *gBfractionCentSyst = new TGraphErrors(7, promptNpart, bfractionCent, promptSystX, bfractionCentSyst);

  TGraphErrors *gBfractionCoarseCent = new TGraphErrors(3, nonPromptNpart, bfractionCoarseCent, nonPromptNpartError, bfractionCoarseCentError);

  TGraphErrors *gBfractionCoarseCentSyst = new TGraphErrors(3, nonPromptNpart, bfractionCoarseCent, nonPromptSystX, bfractionCoarseCentSyst);

  TGraphErrors *gBfractionCoarseCentP = new TGraphErrors(3, nonPromptNpart, bfractionCoarseCent, nonPromptNpartError, nonPromptNpartError);

  TGraphErrors *gBfractionMB = new TGraphErrors(1, promptNpart, bfractionCent, promptNpartError, bfractionCentError);

  TGraphErrors *gBfractionMBSyst = new TGraphErrors(1, promptNpart, bfractionCent, promptSystX, bfractionCentSyst);


  gBfractionCent->RemovePoint(0);
  gBfractionCoarseCent->RemovePoint(0);
  gBfractionCoarseCentP->RemovePoint(0);

  gBfractionCentSyst->RemovePoint(0);
  gBfractionCoarseCentSyst->RemovePoint(0);

  gBfractionCent->SetName("gBfractionCent");
  gBfractionMB->SetName("gBfractionMB");

  gBfractionCoarseCent->SetName("gBfractionCoarseCent");
  gBfractionCoarseCentP->SetName("gBfractionCoarseCentP");


  gBfractionCentSyst->SetName("gBfractionCentSyst");
  gBfractionMBSyst->SetName("gBfractionMBSyst");

  gBfractionCoarseCentSyst->SetName("gBfractionCoarseCentSyst");


  gBfractionCentSyst->SetFillColor(kAzure-9);
  gBfractionMBSyst->SetFillColor(kAzure-9);

  gBfractionCoarseCentSyst->SetFillColor(kAzure-9);

  gBfractionCentSyst->SetFillColor(kRed-9);
  gBfractionMBSyst->SetFillColor(kRed-9);
  gBfractionCoarseCentSyst->SetFillColor(kRed-9);


  if (isPaper) {
    gBfractionCent->SetMarkerColor(kRed+2);
    gBfractionMB->SetMarkerColor(kRed+2);
    gBfractionCoarseCent->SetMarkerColor(kRed+2);
  }
  else {
    gBfractionCent->SetMarkerColor(kRed);
    gBfractionMB->SetMarkerColor(kRed);
    gBfractionCoarseCent->SetMarkerColor(kRed);
  }

  gBfractionCent->SetMarkerStyle(20);
  gBfractionMB->SetMarkerStyle(24);
  gBfractionCoarseCent->SetMarkerStyle(20);

  gBfractionCent->SetMarkerSize(1.2);
  gBfractionMB->SetMarkerSize(1.2);
  gBfractionCoarseCent->SetMarkerSize(1.2);

  gBfractionCoarseCentP->SetMarkerStyle(24);
  gBfractionCoarseCentP->SetMarkerSize(1.2);


  TF1 *f2 = new TF1("f2","1",0,400);
  f2->SetLineWidth(1);
  f2->GetXaxis()->SetTitle("N_{part}");
  f2->GetYaxis()->SetTitle("B-fraction");
  f2->GetYaxis()->SetRangeUser(0.0,0.8);
  f2->GetXaxis()->CenterTitle(kTRUE);

  TCanvas *c2 = new TCanvas("c2","c2");
  f2->Draw();
  gBfractionCoarseCentSyst->Draw("2");
  gBfractionCoarseCent->Draw("P");
  gBfractionCoarseCentP->Draw("P");


  TLegend *leg2 = new TLegend(0.22,0.62,0.90,0.69);
  leg2->SetFillStyle(0);
  leg2->SetFillColor(kWhite);
  leg2->SetBorderSize(0);
  leg2->SetMargin(0.2);
  leg2->SetTextSize(0.04);
  leg2->AddEntry(gBfractionCoarseCent,"B-fraction","P");
  leg2->Draw();

  TLatex *pre2 = new TLatex(125,0.733336,"CMS Preliminary");
  pre2->SetTextFont(42);
  pre2->SetTextSize(0.05);
  pre2->Draw();

  TLatex *l2 = new TLatex(125,0.66664,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  l2->SetTextSize(0.05);
  l2->Draw();

  TLatex *ly2 = new TLatex(20.0,0.090,"0.0 < |y| < 2.4");
  ly2->Draw();
  TLatex *lpt2 = new TLatex(20.0,0.04,"0.0 < p_{T} < 20.0 GeV/c");
  lpt2->Draw();

  gPad->RedrawAxis();
 
  c2->SaveAs("bFraction_cent.pdf");
  c2->SaveAs("bFraction_cent.png");
  return;

  TF1 *f3 = new TF1("f3","220",0,400);
  f3->GetXaxis()->SetTitle("N_{part}");
  f3->GetYaxis()->SetTitle("1/T_{AA} dN/dy (nb)");

  f3->GetYaxis()->SetRangeUser(0.0,1.0);
  f3->GetXaxis()->CenterTitle(kTRUE);


  double ppNpart[] = {2.0};
  double ppNpartError[] = {0.0};
  double ppNpartError2[] = {1.0};
  double ppCDF[] = {2.325};
  //  double ppCDF[] = {0.753};
  double ppCDFError[] = {0.029};
  double ppCDFSyst[] = {0.072};

  TGraphErrors *gUpsCDF = new TGraphErrors(1, ppNpart, ppCDF, ppNpartError, ppCDFError);
  TGraphErrors *gUpsCDFsyst = new TGraphErrors(1, ppNpart, ppCDF, ppNpartError2, ppCDFSyst);

  gUpsCDF->SetName("gUpsCDF");
  gUpsCDFsyst->SetName("gUpsCDFsyst");

  gUpsCDF->SetMarkerStyle(20);
  gUpsCDFsyst->SetFillColor(kGray);

  TCanvas *c3 = new TCanvas("c3","c3");
  // c3->SetRightMargin(rmargin);
  // c3->SetTopMargin(tmargin);

  f3->Draw();
  gUpsCent->Draw("P");
  gUpsMB->Draw("P");
  gUpsCDFsyst->Draw("2");
  gUpsCDF->Draw("P");

  // RAA
  double ppPromptJpsi = 226.0;
  double ppPromptJpsiError = 20;

  double inclJpsiRAA[5];
  double promptJpsiRAA[5];
  double nonPromptJpsiRAA[3];
  double upsRAA[4];

  double inclJpsiRAAError[5];
  double promptJpsiRAAError[5];
  double nonPromptJpsiRAAError[3];
  double upsRAAError[4];

  double inclJpsiRAASyst[5];
  double promptJpsiRAASyst[5];
  double nonPromptJpsiRAASyst[3];
  double upsRAASyst[4];

  for (int i=0; i<5; ++i) {
    inclJpsiRAA[i] = inclJpsiCent[i]/ppPromptJpsi;
    promptJpsiRAA[i] = promptJpsiCent[i]/ppPromptJpsi;

    inclJpsiRAAError[i] = inclJpsiRAA[i]*inclJpsiCentError[i]/inclJpsiCent[i];
    promptJpsiRAAError[i] = promptJpsiRAA[i]*promptJpsiCentError[i]/promptJpsiCent[i];

    inclJpsiRAASyst[i] = inclJpsiRAA[i]*inclJpsiCentSyst[i]/inclJpsiCent[i];
    promptJpsiRAASyst[i] = promptJpsiRAA[i]*promptJpsiCentSyst[i]/promptJpsiCent[i];
  }

  for (int i=0; i<3; ++i) {
    nonPromptJpsiRAA[i] = nonPromptJpsiCent[i]/(bfractionCent[i]*ppPromptJpsi);
    nonPromptJpsiRAAError[i] = nonPromptJpsiRAA[i]*nonPromptJpsiCentError[i]/nonPromptJpsiCent[i];
    nonPromptJpsiRAASyst[i] = nonPromptJpsiRAA[i]*nonPromptJpsiCentSyst[i]/nonPromptJpsiCent[i];
  }

  for (int i=0; i<4; ++i) {
    upsRAA[i] = upsCent[i]/ppCDF[0];
    upsRAAError[i] = upsRAA[i]*upsCentError[i]/upsCent[i];
    upsRAASyst[i] = upsRAA[i]*upsCentSyst[i]/upsCent[i];
  }

  TGraphErrors *gInclJpsiRAA = new TGraphErrors(5, promptNpart, inclJpsiRAA, promptNpartError, inclJpsiRAAError);
  TGraphErrors *gPromptJpsiRAA = new TGraphErrors(5, promptNpart, promptJpsiRAA, promptNpartError, promptJpsiRAAError);
  TGraphErrors *gNonPromptJpsiRAA = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiRAA, nonPromptNpartError, nonPromptJpsiRAAError);
  TGraphErrors *gUpsRAA = new TGraphErrors(4, upsNpart, upsRAA, upsNpartError, upsRAAError);

  TGraphErrors *gInclJpsiRAAMB = new TGraphErrors(1, promptNpart, inclJpsiRAA, promptNpartError, inclJpsiRAAError);
  TGraphErrors *gPromptJpsiRAAMB = new TGraphErrors(1, promptNpart, promptJpsiRAA, promptNpartError, promptJpsiRAAError);
  TGraphErrors *gNonPromptJpsiRAAMB = new TGraphErrors(1, nonPromptNpart, nonPromptJpsiRAA, nonPromptNpartError, nonPromptJpsiRAAError);
  TGraphErrors *gUpsRAAMB = new TGraphErrors(1, upsNpart, upsRAA, upsNpartError, upsRAAError);


  TGraphErrors *gInclJpsiRAASyst = new TGraphErrors(5, promptNpart, inclJpsiRAA, promptSystX, inclJpsiRAASyst);
  TGraphErrors *gPromptJpsiRAASyst = new TGraphErrors(5, promptNpart, promptJpsiRAA, promptSystX, promptJpsiRAASyst);
  TGraphErrors *gNonPromptJpsiRAASyst = new TGraphErrors(3, nonPromptNpart, nonPromptJpsiRAA, nonPromptSystX, nonPromptJpsiRAASyst);
  TGraphErrors *gUpsRAASyst = new TGraphErrors(4, upsNpart, upsRAA, upsSystX, upsRAASyst);

  TGraphErrors *gInclJpsiRAAMBSyst = new TGraphErrors(1, promptNpart, inclJpsiRAA, promptSystX, inclJpsiRAASyst);
  TGraphErrors *gPromptJpsiRAAMBSyst = new TGraphErrors(1, promptNpart, promptJpsiRAA, promptSystX, promptJpsiRAASyst);
  TGraphErrors *gNonPromptJpsiRAAMBSyst = new TGraphErrors(1, nonPromptNpart, nonPromptJpsiRAA, nonPromptSystX, nonPromptJpsiRAASyst);
  TGraphErrors *gUpsRAAMBSyst = new TGraphErrors(1, upsNpart, upsRAA, upsSystX, upsRAASyst);


  gInclJpsiRAA->RemovePoint(0);
  gPromptJpsiRAA->RemovePoint(0);
  gNonPromptJpsiRAA->RemovePoint(0);
  gUpsRAA->RemovePoint(0);

  gInclJpsiRAASyst->RemovePoint(0);
  gPromptJpsiRAASyst->RemovePoint(0);
  gNonPromptJpsiRAASyst->RemovePoint(0);
  gUpsRAASyst->RemovePoint(0);

  gInclJpsiRAA->SetName("gInclJpsiRAA");
  gPromptJpsiRAA->SetName("gPromptJpsiRAA");
  gNonPromptJpsiRAA->SetName("gNonPromptJpsiRAA");

  gInclJpsiRAAMB->SetName("gInclJpsiRAAMB");
  gPromptJpsiRAAMB->SetName("gPromptJpsiRAAMB");
  gNonPromptJpsiRAAMB->SetName("gNonPromptJpsiRAAMB");

  gUpsRAA->SetName("gUpsRAASyst");
  gUpsRAAMB->SetName("gUpsRAAMBSyst");

  gInclJpsiRAASyst->SetName("gInclJpsiRAASyst");
  gPromptJpsiRAASyst->SetName("gPromptJpsiRAASyst");
  gNonPromptJpsiRAASyst->SetName("gNonPromptJpsiRAASyst");

  gInclJpsiRAAMBSyst->SetName("gInclJpsiRAAMBSyst");
  gPromptJpsiRAAMBSyst->SetName("gPromptJpsiRAAMBSyst");
  gNonPromptJpsiRAAMBSyst->SetName("gNonPromptJpsiRAAMBSyst");

  gUpsRAASyst->SetName("gUpsRAASyst");
  gUpsRAAMBSyst->SetName("gUpsRAAMBSyst");


  gInclJpsiRAASyst->SetFillColor(kGray+1);
  gPromptJpsiRAASyst->SetFillColor(kAzure-9);
  gNonPromptJpsiRAASyst->SetFillColor(kRed-10);
  gUpsRAASyst->SetFillColor(kGreen-10);

  gInclJpsiRAAMBSyst->SetFillColor(kGray+1);
  gPromptJpsiRAAMBSyst->SetFillColor(kAzure-9);
  gNonPromptJpsiRAAMBSyst->SetFillColor(kRed-10);
  gUpsRAAMBSyst->SetFillColor(kGreen-10);


  gInclJpsiRAA->SetMarkerColor(kBlack);
  gPromptJpsiRAA->SetMarkerColor(kBlue);
  gNonPromptJpsiRAA->SetMarkerColor(kRed);
  gUpsRAA->SetMarkerColor(kGreen+2);

  gInclJpsiRAAMB->SetMarkerColor(kBlack);
  gPromptJpsiRAAMB->SetMarkerColor(kBlue);
  gNonPromptJpsiRAAMB->SetMarkerColor(kRed);
  gUpsRAAMB->SetMarkerColor(kGreen+2);

  gInclJpsiRAA->SetMarkerStyle(20);
  gPromptJpsiRAA->SetMarkerStyle(21);
  gNonPromptJpsiRAA->SetMarkerStyle(22);
  gUpsRAA->SetMarkerStyle(23);

  gInclJpsiRAAMB->SetMarkerStyle(24);
  gPromptJpsiRAAMB->SetMarkerStyle(25);
  gNonPromptJpsiRAAMB->SetMarkerStyle(26);
  gUpsRAAMB->SetMarkerStyle(27);


  TF1 *f4 = new TF1("f4","1",0,400);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.0);
  f4->GetXaxis()->CenterTitle(kTRUE);


  TCanvas *c4 = new TCanvas("c4","c4");
  // c4->SetRightMargin(rmargin);
  // c4->SetTopMargin(tmargin);
  c4->SetTickx();
  c4->SetTicky();

  f4->Draw();

  gInclJpsiRAASyst->Draw("2");
  gPromptJpsiRAASyst->Draw("2");
  gNonPromptJpsiRAASyst->Draw("2");
  gUpsRAASyst->Draw("2");


  gInclJpsiRAA->Draw("P");
  gPromptJpsiRAA->Draw("P");
  gNonPromptJpsiRAA->Draw("P");
  gUpsRAA->Draw("P");

  gInclJpsiRAAMBSyst->Draw("2");
  gPromptJpsiRAAMBSyst->Draw("2");
  gNonPromptJpsiRAAMBSyst->Draw("2");
  gUpsRAAMBSyst->Draw("2");

  gInclJpsiRAAMB->Draw("P");
  gPromptJpsiRAAMB->Draw("P");
  gNonPromptJpsiRAAMB->Draw("P");
  gUpsRAAMB->Draw("P");


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

  gPPG068RAA_midSyst->Draw("2");
  gPPG068RAA_mid->Draw("P");

  gPPG068RAA_fwdSyst->Draw("2");
  gPPG068RAA_fwd->Draw("P");

  return;
}
