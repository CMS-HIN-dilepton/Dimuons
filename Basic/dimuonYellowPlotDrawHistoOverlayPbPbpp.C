//
// dimuonYellowPlotDrawHisto.C
// Draw the dimuon invariant mass from CMS data
// Uses the output from 

#include "tdrstyle.C"
#include "CMS_lumi.C"

dimuonYellowPlotDrawHistoOverlayPbPbpp(){
  cout << "dimuonYellowPlot: Starting macro dimuonYellowPlot" << endl;
  cout << "dimuonYellowPlot: Setting styles..." << endl;

  // Henceforth, this macro will use only CMS TDR style macro.
  // Margins, fonts, can be handled by adjusting this
  // style macro.  Leave some of the parameters that are typically fiddled with below

  setTDRStyle();
  tdrStyle->SetPadTopMargin(0.05); // default 0.08
  tdrStyle->SetPadBottomMargin(0.11); // default 0.12
  tdrStyle->SetPadLeftMargin(0.10); // default 0.16 
  tdrStyle->SetPadRightMargin(0.02); // default 0.04

  //
  // Note: the title offests and sizes need to be changed directly on the histograms
  // if they were created with different values.
  // Changing them here affect newly created histograms only. Histograms written to file
  // need to be changed each time.
  //
  
  tdrStyle->SetTitleYOffset(1.12); // default 1.25
  

  //
  // Open the file, this should be the file created from the dimuonYellowPlotMakeHisto.C
  // macro.
  // and create a pointer to the histogram.
  // If the pointer is not created properly, some things will work, but the
  // scaling code won't work, the part inside the for-loop will not recognize the
  // name of the histogram as a variable.
  //

  TString inFileName1 = "dimuonMassYellowPlotHistos262163_262277.root";
  TString inFileName2 = "dimuonMassYellowPlotHistosPbPbExpressPt1_5Rap1_6Add.root";
  cout << "dimuonYellowPlot: Opening histogram file " << inFileName2 << ", " << inFileName2 << endl;

  TFile *inf = new TFile(inFileName1,"READ");
  TFile *inf2 = new TFile(inFileName2,"READ");
  inf->cd();
  TH1F* massHistoUnlikeSignPP = (TH1F*) inf->Get("massHistoUnlikeSign");
  if (massHistoUnlikeSignPP==0x0) { 
    cout << "Histogram not found in the input file. Fatal error, can't continue! Bailing out! Agh!" << endl;
    return;
  }
  cout << "pp   : " <<  massHistoUnlikeSignPP->GetName() << ", entries: " << massHistoUnlikeSignPP->GetEntries() << endl;
  inf2->cd();
  TH1F* massHistoUnlikeSignPbPb = (TH1F*) inf2->Get("massHistoUnlikeSign");
  cout << "PbPb : " << massHistoUnlikeSignPbPb->GetName() << ", entries: " << massHistoUnlikeSignPbPb->GetEntries() << endl;
  
  
  // Set up the styles of the histogram
  massHistoUnlikeSignPP->GetYaxis()->SetTitleOffset(1.2);
  massHistoUnlikeSignPP->GetYaxis()->SetTitleSize(0.04);
  massHistoUnlikeSignPP->GetXaxis()->SetTitleSize(0.045);

  // Set up the Canvas
  TCanvas* yellowPlot = new TCanvas("yellowPlot","yellowPlot",500,500);
  yellowPlot->SetLogx();
  yellowPlot->SetLogy();


  // Draw histograms
  cout << "dimuonYellowPlot Drawing histogram " << endl;
  massHistoUnlikeSignPP->GetXaxis()->SetRangeUser(2,2e2);
  massHistoUnlikeSignPP->GetYaxis()->SetRangeUser(1e-1,4e6);
  massHistoUnlikeSignPP->GetXaxis()->SetTitleFont(42);
  massHistoUnlikeSignPP->GetYaxis()->SetTitleFont(42);

  massHistoUnlikeSignPbPb->SetFillColor(kYellow);
  massHistoUnlikeSignPbPb->SetLineWidth(2);
  massHistoUnlikeSignPbPb->SetLineColor(kBlue+1);
  
  massHistoUnlikeSignPP->SetFillColor(kTeal-5);
  //massHistoUnlikeSignPP->SetFillStyle(2);
  massHistoUnlikeSignPP->SetLineWidth(2);
  massHistoUnlikeSignPP->SetLineColor(kGreen+3);

  massHistoUnlikeSignPP->Draw("");
  massHistoUnlikeSignPbPb->Draw("same");
  
  // The second histogram can cover the axes if it has a fill color.
  // Add the following line so that the axes and tick marks are visible.
  gPad->RedrawAxis();

  cout << "dimuonYellowPlot: Scaling histograms..." << endl;

  
  // Scale by bin width, needs to be done bin by bin because bin widths are different.
  for (int i=1; i<=massHistoUnlikeSignPP->GetNbinsX(); i++) {
    massHistoUnlikeSignPP->SetBinContent(i, massHistoUnlikeSignPP->GetBinContent(i)/massHistoUnlikeSignPP->GetBinWidth(i));
    massHistoUnlikeSignPbPb->SetBinContent(i, massHistoUnlikeSignPbPb->GetBinContent(i)/massHistoUnlikeSignPbPb->GetBinWidth(i));
  }

  double PbPbArbitraryScalingFactor = massHistoUnlikeSignPP->GetBinContent(412)/massHistoUnlikeSignPbPb->GetBinContent(412); // bin 412 should be at mass~91, i.e. the Z^0 mass.
  //massHistoUnlikeSignPbPb->Scale(PbPbArbitraryScalingFactor/3.);


  //
  // Options to be used with the CMS_lumi macro
  //
  
  writeExtraText = true;       // if extra text
  extraText      = "Preliminary";  // default extra text is "Preliminary"
  lumi_PbPb2011 = "PbPb 166 #mub^{-1}  pp 5.4 pb^{-1}";
  //
  // iPeriod options: 99 for pPb 5.02 TeV, 101 for PbPb 2011, 102 for pp 2013
  //
  int iPeriod      = 99;     
  lumiTextOffset   = 0.3; // default 0.28

  //lumi_5TeV = "pp, ~20 pb^{-1} lumi";
  lumi_5TeV = "pp [262163_262277], PbPb [262548-262811], Online Express Stream";

  // Call the CMS_lumi macro to draw:
  // CMS preliminary, aligned on the right and justified (iPos=33, third argument)
  // integrated luminosity (drawn on top left, out of frame, or use lumiTextOffset)
  // center of mass energy (drawn on top right, out of frame, or use lumiTextOffset)
  
  CMS_lumi(yellowPlot,iPeriod,33);

  // Note, the code for putting the CMS logo is only used
  // in the style macro if one does not write CMS preliminary.
  // For writing both, one needs to put one of them by hand.
  // This is a copy-paste of the code to insert the CMS logo, changing it to
  // a color logo. Needs to have the CMS-Color.gif file in the same directory.
  float H = gPad->GetWh();
  float W = gPad->GetWw();
  float l = gPad->GetLeftMargin();
  float t = gPad->GetTopMargin();
  float r = gPad->GetRightMargin();
  float b = gPad->GetBottomMargin();
  float posX_ =   l + 0.025*(1-l-r)*W/H;
  float posY_ = 1-t - 0.025*(1-t-b);
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
  

  // Booking the TLatex class and set the parameters
  // CMS Preliminary (not used anymore) left as a dummy
  // Each of the peaks is done with a call to DrawLatex, which
  // makes a copy of the TLatex class style, so the style should
  // be set with the first TLatex instance, and that style will 
  // be inherited by each of the copies.

  TLatex* latex = new TLatex(0.62,0.88,"CMS Preliminary");
  latex->SetNDC();
  latex->SetTextColor(kBlack);
  latex->SetTextFont(42);
  //latex->DrawLatex(0.24,0.88,"#rho, #omega");
  //latex->DrawLatex(0.32,0.88,"#phi");
  
  latexJpsi = latex->DrawLatex(0.20,0.78,"J/#psi");
  
  latexPsiPrime = latex->DrawLatex(0.22,0.65,"#psi(2S)");

  latexUpsi = latex->DrawLatex(0.40,0.65,"#varUpsilon(1,2,3S)");

  latexZ0 = latex->DrawLatex(0.8,0.5,"Z");

  latexPtCut = latex->DrawLatex(0.25,0.20,"p_{T}^{#mu} > 4 GeV/c");

  TLegend* leg = new TLegend(0.75,0.65,0.95,0.80);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->AddEntry(massHistoUnlikeSignPP ,"pp","F");
  leg->AddEntry(massHistoUnlikeSignPbPb,"PbPb","F");
  leg->Draw();

  return;
}
