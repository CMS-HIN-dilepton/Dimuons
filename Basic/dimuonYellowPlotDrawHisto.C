//
// dimuonYellowPlotDrawHisto.C
// Draw the dimuon invariant mass from CMS data
// Uses the output from dimuonYellowPlotMakeHisto.C
// i.e. this is the second step, one should run dimuonYellowPlotMakeHisto.C 
// first.

#include "CMS/tdrstyle.C"
#include "CMS/CMS_lumi.C"

dimuonYellowPlotDrawHisto(){
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
  
  // 2015 pp data, express stream
  //TString inFileName = "dimuonMassYellowPlotHistos262163_262277.root";
  
  // 2015 PbPb Data, express stream
  //TString inFileName = "dimuonMassYellowPlotHistosPbPbExpressAdd.root";
  //TString inFileName = "dimuonMassYellowPlotHistosPbPbExpressPt1_5Rap1_6Add.root";
  TString inFileName = "dummy.root";
  cout << "dimuonYellowPlot: Opening histogram file " << inFileName << endl;

  TFile *inf = new TFile(inFileName,"READ");
  TH1F* massHistoUnlikeSign = (TH1F*) inf->Get("massHistoUnlikeSign");
  if (massHistoUnlikeSign==0x0) { 
    cout << "Histogram not found in the input file. Fatal error, can't continue! Bailing out! Agh!" << endl;
    return;
  }
  cout << massHistoUnlikeSign->GetName() << ", " << massHistoUnlikeSign->GetNbinsX() << endl;
  
  // Set up the styles of the histogram
  massHistoUnlikeSign->GetYaxis()->SetTitleOffset(1.2);
  massHistoUnlikeSign->GetYaxis()->SetTitleSize(0.04);
  massHistoUnlikeSign->GetXaxis()->SetTitleSize(0.045);
  //massHistoUnlikeSign->Rebin(4);

  // Set up the Canvas
  TCanvas* yellowPlot = new TCanvas("yellowPlot","yellowPlot",500,500);
  yellowPlot->SetLogx();
  yellowPlot->SetLogy();
  cout << massHistoUnlikeSign->GetName() << ", " << massHistoUnlikeSign->GetNbinsX() << endl;

  // Draw histograms
  cout << "dimuonYellowPlot Drawing histogram " << endl;
  massHistoUnlikeSign->GetXaxis()->SetRangeUser(2,2e2);
  massHistoUnlikeSign->GetYaxis()->SetRangeUser(2e-1,3e5);
  massHistoUnlikeSign->GetXaxis()->SetTitleFont(42);
  massHistoUnlikeSign->GetYaxis()->SetTitleFont(42);
  massHistoUnlikeSign->SetFillColor(kYellow);
  massHistoUnlikeSign->SetLineWidth(2);
  massHistoUnlikeSign->SetLineColor(kBlue+1);
  massHistoUnlikeSign->Draw(""); 
  //  massHistoLikeSign->Draw("same");


  cout << "dimuonYellowPlot: Scaling histograms..." << endl;

  // Scale by bin width, needs to be done bin by bin because bin widths are different.
  for (int i=1; i<=massHistoUnlikeSign->GetNbinsX(); i++) {
    massHistoUnlikeSign->SetBinContent(i, massHistoUnlikeSign->GetBinContent(i)/massHistoUnlikeSign->GetBinWidth(i));
    //massHistoLikeSign->SetBinContent(i, massHistoUnlikeSign->GetBinContent(i)/massHistoUnlikeSign->GetBinWidth(i));
  }


  //
  // Options to be used with the CMS_lumi macro
  //
  
  writeExtraText = true;       // if extra text
  extraText      = "Preliminary";  // default extra text is "Preliminary"
  
  //
  // iPeriod options: 99 for pPb 5.02 TeV, 101 for PbPb 2011, 102 for pp 2013
  //
  // iPeriod 99 will give sqrt(s) = 5.02 TeV, but the collision and luminosity need
  // to be changed.

  int iPeriod      = 99;     
  lumiTextOffset   = 0.3; // default 0.28
  
  //lumi_5TeV = "pp, ~20 pb^{-1} lumi";
  lumi_5TeV = "PbPb, Online Express Stream";

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
  
  latexPsiPrime = latex->DrawLatex(0.22,0.6,"#psi(2S)");

  latexUpsi = latex->DrawLatex(0.40,0.6,"#varUpsilon(1,2,3S)");

  latexZ0 = latex->DrawLatex(0.8,0.5,"Z");

  latexPtCut = latex->DrawLatex(0.25,0.20,"#splitline{p_{T}^{#mu} > 3.5 GeV/c}{|y|<1.6}");

  return;
}
