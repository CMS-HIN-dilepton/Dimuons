#include "TLatex.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TAxis.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TH1I.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TMath.h"
#include "TVector3.h"
#include "TString.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "TObjArray.h"
#include "TGaxis.h"

void PlotEff_Quarkonia()   
{
    gROOT->Macro("rootlogon.C");
    //gROOT->ProcessLine(".x rootlogon.C");
    //gROOT->ProcessLine(".x ../../rootlogon.C");
  //gROOT->SetStyle("Plain");
  //gStyle->SetPalette(1);
  //gStyle->SetFrameBorderMode(0);
  //gStyle->SetFrameFillColor(0);
  //gStyle->SetCanvasColor(0);
  //gStyle->SetTitleFillColor(0);
  //gStyle->SetStatColor(0);
  //gStyle->SetPadBorderSize(0);
  //gStyle->SetCanvasBorderSize(0);
  //gStyle->SetOptTitle(0); // at least most of the time
  //gStyle->SetOptStat(0); 
// most of the time, sometimes "nemriou" might be useful to display name, 
  //number of entries, mean, rms, integral, overflow and underflow
  //gStyle->SetOptFit(0); // set to 1 only if you want to display fit results
  //==================================== Define Histograms==================================

 
  //gPromptJpsi->SetMarkerStyle(21);
  //gNonPromptJpsi->SetMarkerStyle(29);
  //gUps->SetMarkerStyle(33);
  


  
  //gPromptJpsi->SetMarkerColor(kRed);
  //gNonPromptJpsi->SetMarkerColor(kOrange+2);
  //gPromptJpsi->SetMarkerColor(kRed+2);
  //gUpsRAA->SetMarkerColor(kGreen+2);
 


  //gPromptJpsi->SetMarkerSize(2.0);
  //gNonPromptJpsi->SetMarkerSize(2.0);
  //gUps->SetMarkerSize(2.0);

  //  double VcentEff[5] =             {0, 0.206, 0.283, 0.292, 0.292};//AllRap all pt     
  //double VcentEffErr[5] =          {0, 0.061, 0.051, 0.037, 0.037};
 
  //TGraphErrors *gr = new TGraphErrors(10,x,VcentEff,ex,VcentEffErr);
  //gr->SetTitle("TGraphErrors Example");
  //gr->SetMarkerColor(2);
  //gr->SetMarkerStyle(21);
  //new TCanvas;
  //gr->Draw("AP");
   
  double xerror=0;
  double Npart[10] = {355.3528, 261.4178, 187.1470, 129.9835, 86.2622, 60.8310, 27.082, 6.0005};

  //Pt03    25.8%   1.1%
  //Pt36    29.0%   0.5%
  //Pt69    36.6%   0.4%
  //Pt912   47.9%   0.4%
  //Pt1215  56.9%   1.3%
  //Pt1530  54.6%   0.8%
  
  Int_t n = 6;
 
  double x[6],y[6],ex[6],ey[6];
  
  x[0]  =1.5;
  x[1]  =4.5;
  x[2]  =7.5;
  x[3]  =10.5;
  x[4]  =13.5;
  x[5]  =22.5;
  
  y[0]  =0.25;
  y[1]  =0.29;
  y[2]  =0.366;
  y[3]  =0.479;
  y[4]  =0.569;
  y[5]  =0.546;

  ex[0]  =0.0;
  ex[1]  =0.0;
  ex[2]  =0.0;
  ex[3]  =0.0;
  ex[4]  =0.0;
  ex[5]  =0.0;
 
  ey[0]  =0.011;
  ey[1]  =0.005;
  ey[2]  =0.004;
  ey[3]  =0.004;
  ey[4]  =0.013;
  ey[5]  =0.008;
  
    
  TGraphErrors *gr = new TGraphErrors(n,x,y,ex,ey);
  gr->SetTitle("TGraphErrors Example");
  gr->SetMarkerColor(1);
  gr->SetMarkerStyle(21);
  gr->SetMarkerSize(1.2);
 
  //new TCanvas; 
  //gr->Draw("AP");

  TFile *fil0 =new TFile("EffPlotRootFiles/EffJPsi_Pt_NW.root","R");
  TGraphErrors *JPsiPt_NW = (TGraphErrors*)fil0->Get("Graph");
  JPsiPt_NW->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  JPsiPt_NW->GetXaxis()->CenterTitle(kTRUE);
  JPsiPt_NW->GetYaxis()->SetTitle("Efficiency ");
  JPsiPt_NW->SetMarkerColor(1);
  JPsiPt_NW->SetMarkerStyle(21);


  //===========prompt jpsi===========================================//
  TFile *fil1 =new TFile("EffPlotRootFiles/EffJPsi_Pt_Plot.root","R");
  TGraphErrors *JPsiPt = (TGraphErrors*)fil1->Get("Graph");
  JPsiPt->SetName("JPsiPt");
  for (int i=0; i<JPsiPt->GetN();++i){
    double ye=JPsiPt->GetErrorY(i);
    if(xerror==0) JPsiPt->SetPointError(i,0,ye);
  }

  JPsiPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  JPsiPt->GetXaxis()->CenterTitle(kTRUE);
  JPsiPt->GetYaxis()->SetTitle("Efficiency ");
  JPsiPt->SetMarkerColor(kRed+2);
  JPsiPt->SetMarkerStyle(21);
  JPsiPt->SetMarkerSize(1.2);

  TGraphErrors *JPsiPtP = (TGraphErrors*) JPsiPt->Clone();
  JPsiPtP->SetMarkerColor(kBlack);
  JPsiPtP->SetMarkerStyle(25);
  JPsiPtP->SetMarkerSize(1.2);

  TFile *fil2 =new TFile("EffPlotRootFiles/EffJPsi_Rap_Plot.root","R");
  TGraphErrors *JPsiRap = (TGraphErrors*)fil2->Get("Graph");
  JPsiRap->SetName("JPsiRap");
  for (int i=0; i<JPsiRap->GetN();++i){
    double ye=JPsiRap->GetErrorY(i);
    if(xerror==0) JPsiRap->SetPointError(i,0,ye);
  }

  JPsiRap->SetMarkerColor(kRed+2);
  JPsiRap->SetMarkerStyle(21);
  JPsiRap->SetMarkerSize(1.2);

  TGraphErrors *JPsiRapP = (TGraphErrors*) JPsiRap->Clone();
  JPsiRapP->SetMarkerColor(kBlack);
  JPsiRapP->SetMarkerStyle(25);
  JPsiRapP->SetMarkerSize(1.2);

  TFile *fil3 =new TFile("EffPlotRootFiles/EffJPsi_Cent_Plot.root","R");
  TGraphErrors *JPsiCent = (TGraphErrors*)fil3->Get("Graph");
  JPsiCent->SetName("JPsiCent");
  double x1, y1,ye1;
  for (int i=0; i<JPsiCent->GetN();++i){
    JPsiCent->GetPoint(i,x1,y1);
    JPsiCent->SetPoint(i,Npart[i],y1);
    double xerror1=TMath::Abs((Npart[i+1]-Npart[i]))/2;
    ye1=JPsiCent->GetErrorY(i);
    JPsiCent->SetPointError(i,xerror1,ye1);
    if(xerror==0) JPsiCent->SetPointError(i,0,ye1);
  }
  
  JPsiCent->SetMarkerColor(kRed+2);
  JPsiCent->SetMarkerStyle(21);
  JPsiCent->SetMarkerSize(1.2);

  TGraphErrors *JPsiCentP = (TGraphErrors*) JPsiCent->Clone();
  JPsiCentP->SetMarkerColor(kBlack);
  JPsiCentP->SetMarkerStyle(25);
  JPsiCentP->SetMarkerSize(1.2);

  //new TCanvas;
  //JPsiCent->Draw("AP");
  //===========non prompt jpsi =========================================//
  TFile *fil4 =new TFile("EffPlotRootFiles/EffNPJPsi_Pt_Plot.root","R");
  TGraphErrors *NPJPsiPt = (TGraphErrors*)fil4->Get("Graph");
  NPJPsiPt->SetName("NPJPsiPt");
  for (int i=0; i<NPJPsiPt->GetN();++i){
    double ye=NPJPsiPt->GetErrorY(i);
    if(xerror==0) NPJPsiPt->SetPointError(i,0,ye);
  }

  NPJPsiPt->SetMarkerColor(kOrange+2); 
  //NPJPsiPt->SetLineColor(4);
  NPJPsiPt->SetMarkerSize(2.0);
  NPJPsiPt->SetMarkerStyle(29);

  TGraphErrors *NPJPsiPtP = (TGraphErrors*) NPJPsiPt->Clone();
  NPJPsiPtP->SetMarkerColor(kBlack);
  NPJPsiPtP->SetMarkerStyle(30);
  NPJPsiPtP->SetMarkerSize(2.0);

  TFile *fil5 =new TFile("EffPlotRootFiles/EffNPJPsi_Rap_Plot.root","R");
  TGraphErrors *NPJPsiRap = (TGraphErrors*)fil5->Get("Graph");
  NPJPsiRap->SetName("NPJPsiRap");
  for (int i=0; i<NPJPsiRap->GetN();++i){
    double ye=NPJPsiRap->GetErrorY(i);
    if(xerror==0) NPJPsiRap->SetPointError(i,0,ye);
  }
  //NPJPsiRap->SetLineColor(4);
  NPJPsiRap->SetMarkerColor(kOrange+2);
  NPJPsiRap->SetMarkerSize(2.0);
  NPJPsiRap->SetMarkerStyle(29);

  TGraphErrors *NPJPsiRapP = (TGraphErrors*) NPJPsiRap->Clone();
  NPJPsiRapP->SetMarkerColor(kBlack);
  NPJPsiRapP->SetMarkerStyle(30);
  NPJPsiRapP->SetMarkerSize(2.0);

  TFile *fil6 =new TFile("EffPlotRootFiles/EffNPJPsi_Cent_Plot.root","R");
  TGraphErrors *NPJPsiCent = (TGraphErrors*)fil6->Get("Graph");
  NPJPsiCent->SetName("NPJPsiCent");
  double x2, y2,ye2;
  for (int i=0; i<NPJPsiCent->GetN();++i){
    NPJPsiCent->GetPoint(i,x2,y2);
    NPJPsiCent->SetPoint(i,Npart[i],y2);
    double xerror2=TMath::Abs((Npart[i+1]-Npart[i]))/2;
    ye2=NPJPsiCent->GetErrorY(i);
    NPJPsiCent->SetPointError(i,xerror2,ye2);
    if(xerror==0) NPJPsiCent->SetPointError(i,0,ye2);
  }
  //NPJPsiCent->SetLineColor(4);
  NPJPsiCent->SetMarkerColor(kOrange+2);
  NPJPsiCent->SetMarkerSize(2.0);
  NPJPsiCent->SetMarkerStyle(29);

  TGraphErrors *NPJPsiCentP = (TGraphErrors*) NPJPsiCent->Clone();
  NPJPsiCentP->SetMarkerColor(kBlack);
  NPJPsiCentP->SetMarkerStyle(30);
  NPJPsiCentP->SetMarkerSize(2.0);

  //===============================upsilon=============================//
  TFile *fil7 =new TFile("EffPlotRootFiles/EffUpsilon_Pt_Plot.root","R");
  TGraphErrors *UpsilonPt = (TGraphErrors*)fil7->Get("Graph");
  UpsilonPt->SetName("UpsilonPt"); 
  for (int i=0; i<UpsilonPt->GetN();++i){
    double ye=UpsilonPt->GetErrorY(i);
    if(xerror==0) UpsilonPt->SetPointError(i,0,ye);
  }
  //UpsilonPt->SetLineColor(2);
  UpsilonPt->SetMarkerColor(kGreen+2);
  UpsilonPt->SetMarkerStyle(33);
  UpsilonPt->SetMarkerSize(2.0);
  UpsilonPt->GetXaxis()->CenterTitle(kTRUE);
  UpsilonPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  UpsilonPt->GetYaxis()->SetTitle("Efficiency ");

  TGraphErrors *UpsilonPtP = (TGraphErrors*) UpsilonPt->Clone();
  UpsilonPtP->SetMarkerColor(kBlack);
  UpsilonPtP->SetMarkerStyle(27);
  UpsilonPtP->SetMarkerSize(2.0);


  TFile *fil8 =new TFile("EffPlotRootFiles/EffUpsilon_Rap_Plot.root","R");
  TGraphErrors *UpsilonRap = (TGraphErrors*)fil8->Get("Graph");
  UpsilonRap->SetName("UpsilonRap"); 
  for (int i=0; i<UpsilonRap->GetN();++i){
    double ye=UpsilonRap->GetErrorY(i);
    if(xerror==0) UpsilonRap->SetPointError(i,0,ye);
  }
  // UpsilonRap->SetLineColor(2);
  UpsilonRap->SetMarkerColor(kGreen+2);
  UpsilonRap->SetMarkerStyle(33);
  UpsilonRap->SetMarkerSize(2.0);
  UpsilonRap->GetXaxis()->CenterTitle(kTRUE);
  UpsilonRap->GetXaxis()->SetTitle("y");
  UpsilonRap->GetYaxis()->SetTitle("Efficiency ");
  UpsilonRap->GetXaxis()->SetRangeUser(-2.5,2.5);

  TGraphErrors *UpsilonRapP = (TGraphErrors*) UpsilonRap->Clone();
  UpsilonRapP->SetMarkerColor(kBlack);
  UpsilonRapP->SetMarkerStyle(27);
  UpsilonRapP->SetMarkerSize(2.0);

  TFile *fil9 =new TFile("EffPlotRootFiles/EffUpsilon_Cent_Plot.root","R");
  TGraphErrors *UpsilonCent = (TGraphErrors*)fil9->Get("Graph");
  UpsilonCent->SetName("UpsilonCent"); 
  double x3, y3,ye3;
  for (int i=0; i<UpsilonCent->GetN();++i){
    UpsilonCent->GetPoint(i,x3,y3);
    UpsilonCent->SetPoint(i,Npart[i],y3);
    double xerror3=TMath::Abs((Npart[i+1]-Npart[i]))/2;
    ye3=UpsilonCent->GetErrorY(i);
    UpsilonCent->SetPointError(i,xerror3,ye3);
    if(xerror==0) UpsilonCent->SetPointError(i,0,ye3);
  }
 
  UpsilonCent->SetMarkerColor(kGreen+2);
  UpsilonCent->SetMarkerStyle(33);
  UpsilonCent->SetMarkerSize(2.0);
  UpsilonCent->GetXaxis()->CenterTitle(kTRUE);
  UpsilonCent->GetXaxis()->SetTitle("N_{part}");
  UpsilonCent->GetXaxis()->SetTitleOffset(1.0);
  UpsilonCent->GetYaxis()->SetTitle("Efficiency ");
  UpsilonCent->GetYaxis()->SetRangeUser(0,1);

  TGraphErrors *UpsilonCentP = (TGraphErrors*) UpsilonCent->Clone();
  UpsilonCentP->SetMarkerColor(kBlack);
  UpsilonCentP->SetMarkerStyle(27);
  UpsilonCentP->SetMarkerSize(2.0);


  TFile *outfile =new TFile("QuarkoniaEff.root","Recreate");


  TLegend *lcat1;
  lcat1 = new TLegend(0.5654362,0.6818182,0.8557047,0.8706294);
  //lcat1 = new TLegend(//0.5654362,0.6818182,0.8557047,0.8706294);
  //lcat1 = new TLegend(//0.4648241,0.6414835,0.9660804,0.8241758,NULL,"brNDC");
  //lcat1 = new TLegend(.42, .78, .84, .93);
  lcat1->SetBorderSize(0);
  lcat1->SetFillStyle(0);
  lcat1->SetFillColor(0);
  lcat1->SetTextSize(0.04);
  lcat1->SetMargin(0.2);
  //lcat1->AddEntry(UpsilonPt,"PYTHIA+EvtGen+HYDJET(Bass)", "");
  //lcat1->AddEntry(UpsilonPt,"#Upsilon(1S)", "P");
  lcat1->AddEntry(UpsilonPt,"#varUpsilon(1S)", "P");
  lcat1->AddEntry(JPsiPt,"Prompt J/#psi", "P");
  lcat1->AddEntry(NPJPsiPt,"Non-prompt J/#psi", "P");
 
 
 // Category _1 
  TLegend *lcat2;
  // lcat2 = new TLegend(.11, .83, .46, .95);
  lcat2 = new TLegend(0.5654362,0.6818182,0.8557047,0.8706294);
  //lcat2 = new TLegend(.05, .82, .39, .95);
  lcat2->SetBorderSize(0);
  lcat2->SetFillStyle(0);
  lcat2->SetFillColor(0);
  lcat2->SetTextSize(0.04);
  lcat2->SetMargin(0.2);
  lcat2->AddEntry(UpsilonPt,"CMS Simulation", " ");
  lcat2->AddEntry(UpsilonPt,"PbPb  #sqrt{s_{NN}} = 2.76 TeV", "");



  TLegend *lcat3;
  lcat3 = new TLegend(0.5654362,0.6818182,0.8557047,0.8706294);
  //lcat3 = new TLegend(.60, .81, .90, .91);
  lcat3->SetBorderSize(0);
  lcat3->SetFillStyle(0);
  lcat3->SetFillColor(0);
  lcat3->SetTextSize(0.04);
  lcat3->SetMargin(0.2);
  lcat3->AddEntry( gr,"Mihee", "P");
  lcat3->AddEntry(JPsiPt_NW,"Vineet", "P");
 
 
  TLegend *lcat4;
  lcat4 = new TLegend(0.5553691,0.7443357,0.8456376,0.8636364);
  //lcat4 = new TLegend(.48, .78, .90, .94);
  //lcat4 = new TLegend(.42, .77, .84, .93);
  lcat4->SetFillStyle(0);
  lcat4->SetFillColor(0);
  lcat4->SetBorderSize(0);
  lcat4->SetTextSize(0.04);
  lcat4->SetMargin(0.2);

  lcat4->AddEntry(JPsiPt,"Prompt J/#psi", "P");
  lcat4->AddEntry(NPJPsiPt,"Non-prompt J/#psi", "P");
  
  TLegend *lcat5;
  lcat5 = new TLegend(0.4354362,0.6818182,0.7257047,0.8706294);
  //lcat5 = new TLegend(.42, .77, .84, .93);
  lcat5->SetBorderSize(0);
  lcat5->SetFillStyle(0);
  lcat5->SetFillColor(0);
  lcat5->SetMargin(0.2);
  lcat5->SetTextSize(0.04);
  lcat5->AddEntry(gr,"Prompt J/#psi w/o p_{T} weight", "P"); 
  lcat5->AddEntry(JPsiPt,"Prompt J/#psi", "P");
  lcat5->AddEntry(NPJPsiPt,"Non-prompt J/#psi", "P");

  TLegend *lcat6;
  lcat6 = new TLegend(0.5654362,0.7218182,0.8557047,0.9206294);
  //lcat6 = new TLegend(.42, .77, .84, .93);
  lcat6->SetBorderSize(0);
  lcat6->SetFillStyle(0);
  lcat6->SetFillColor(0);
  lcat6->SetMargin(0.2);
  lcat6->SetTextSize(0.04);
  lcat6->AddEntry(UpsilonPt,"#varUpsilon(1S)", "P");



  TLatex *lt1 = new TLatex();
  lt1->SetNDC();
  lt1->SetTextAlign(12);
  lt1->SetTextFont(42);

  lt1->SetTextSize(0.05);
//  lt1->drawlatex(0.47,0.6,"cms simulation");
//  lt1->drawlatex(0.47,0.53,"pbpb  #sqrt{s_{nn}} = 2.76 tev");

  // Drawing

  //new TCanvas; 
  TCanvas *c1 = new TCanvas("c1","c1");
  UpsilonPt->Draw("AP");
  UpsilonPtP->Draw("Psame");
  
  JPsiPt->Draw("Psame");
  JPsiPtP->Draw("Psame");
  
  NPJPsiPt->Draw("Psame");
  NPJPsiPtP->Draw("Psame");
  
  lcat1->Draw("same");
  //lcat2->Draw("same"); 
  lt1->DrawLatex(0.45,0.285,"CMS Simulation");
  lt1->DrawLatex(0.45,0.215,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.47,0.9,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.38,0.9,"PYTHIA+EvtGen+HYDJET(Bass)");

  gPad->RedrawAxis();
  gPad->Print("figures/QuarkoniaEff_Pt.png");
  gPad->Print("figures/QuarkoniaEff_Pt.pdf");
  gPad->Print("figures/QuarkoniaEff_Pt.gif");
 
  //new TCanvas; 
  JPsiPt->Draw("AP");
  JPsiPtP->Draw("Psame");

  NPJPsiPt->Draw("Psame");
  NPJPsiPtP->Draw("Psame");

  lcat4->Draw("same");
  //lcat2->Draw("same"); 

  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.45,0.285,"CMS Simulation");
  lt1->DrawLatex(0.45,0.215,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.47,0.9,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.38,0.9,"PYTHIA+EvtGen+HYDJET(Bass)");
  gPad->RedrawAxis();
  gPad->Print("figures/JPsiEff_Pt.png");
  gPad->Print("figures/JPsiEff_Pt.pdf");
  gPad->Print("figures/JPsiEff_Pt.gif");

  UpsilonPt->Draw("AP");
  UpsilonPtP->Draw("Psame");

  lcat6->Draw("same");
  //lcat2->Draw("same"); 

  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.45,0.285,"CMS Simulation");
  lt1->DrawLatex(0.45,0.215,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.47,0.9,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.38,0.9,"PYTHIA+EvtGen+HYDJET(Bass)");
  gPad->RedrawAxis();
  gPad->Print("figures/UpsilonEff_Pt.png");
  gPad->Print("figures/UpsilonEff_Pt.pdf");
  gPad->Print("figures/UpsilonEff_Pt.gif");


  //new TCanvas; 
  JPsiPt->Draw("AP");
  JPsiPtP->Draw("Psame");

  NPJPsiPt->Draw("Psame");
  NPJPsiPtP->Draw("Psame");

  gr->Draw("Psame");
  lcat5->Draw("same");
  //lcat2->Draw("same"); 
  
  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.45,0.285,"CMS Simulation");
  lt1->DrawLatex(0.45,0.215,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.47,0.9,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.38,0.9,"PYTHIA+EvtGen+HYDJET(Bass)");
  gPad->RedrawAxis();
  gPad->Print("figures/JPsiEff_Pt_1.png");
  gPad->Print("figures/JPsiEff_Pt_1.pdf");
  gPad->Print("figures/JPsiEff_Pt_1.gif");

 

  //new TCanvas; 
  UpsilonRap->Draw("AP");
  UpsilonRapP->Draw("Psame");

  JPsiRap->Draw("Psame");
  JPsiRapP->Draw("Psame");

  NPJPsiRap->Draw("Psame");
  NPJPsiRapP->Draw("Psame");

  lcat1->Draw("same");
  //lcat2->Draw("same"); 
  
  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.45,0.285,"CMS Simulation");
  lt1->DrawLatex(0.45,0.215,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.47,0.9,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.38,0.9,"PYTHIA+EvtGen+HYDJET(Bass)");
  gPad->RedrawAxis();
  gPad->Print("figures/QuarkoniaEff_Rap.png");
  gPad->Print("figures/QuarkoniaEff_Rap.pdf");
  gPad->Print("figures/QuarkoniaEff_Rap.gif");

  //new TCanvas; 
  UpsilonCent->Draw("AP");
  UpsilonCentP->Draw("Psame");

  //JPsiCent->Draw("AP");
  JPsiCent->Draw("Psame");
  JPsiCentP->Draw("Psame");

  NPJPsiCent->Draw("Psame");
  NPJPsiCentP->Draw("Psame");

  lcat1->Draw("same");
  //lcat2->Draw("same");

  lt1->SetTextSize(0.05);
  lt1->DrawLatex(0.45,0.285,"CMS Simulation");
  lt1->DrawLatex(0.45,0.215,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.47,0.9,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.38,0.9,"PYTHIA+EvtGen+HYDJET(Bass)");
  gPad->RedrawAxis();
  gPad->Print("figures/QuarkoniaEff_Cent.png");
  gPad->Print("figures/QuarkoniaEff_Cent.pdf");
  gPad->Print("figures/QuarkoniaEff_Cent.gif");

  
  //new TCanvas; 
  //JPsiPt_NW->Draw("AP");
  //gr->Draw("Psame");
  //lcat3->Draw("same");
  //gPad->Print("figures/Comp_Pt.pdf");

  UpsilonPt->Write();
  UpsilonRap->Write();
  UpsilonCent->Write();

  JPsiPt->Write();
  JPsiRap->Write();
  JPsiCent->Write();

  NPJPsiPt->Write();
  NPJPsiRap->Write();
  NPJPsiCent->Write();

  outfile->Write();
  outfile->Close();

}



//Vineet's efficiencies(from April 14th)
//double VptEff[4] =               {0, 0.209, 0.278, 0.436};//allcent all rap  
//double VptEffErr[4] =            {0, 0.047, 0.039, 0.018};
//double VcentEff[5] =             {0, 0.206, 0.283, 0.292, 0.292};//AllRap all pt     
//double VcentEffErr[5] =          {0, 0.061, 0.051, 0.037, 0.037};
//double VcentEff_hiPt[5] =        {0, 0.232, 0.367, 0.355, 0.405};//all rap [6.5-30Pt]    
//double VcentEffErr_hiPt[5] =     {0, 0.064, 0.047, 0.037, 0.028};
//double VrapEff[6] =              {0, 0.244, 0.270, 0.316, 0.270, 0.244};//all cent 
//double VrapEffErr[6] =           {0, 0.040, 0.057, 0.041, 0.057, 0.040};
//double VoverAll = (0.274)*100;
//double VoverAllErr = (0.028)*100;
