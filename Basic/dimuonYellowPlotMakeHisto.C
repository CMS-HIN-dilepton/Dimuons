//
// dimuonYellowPlotMakeHisto.C
// CMS Heavy-Ion dilepton group
// Modifications: MCBS, Nov 2015
// Macro that will:
// 1) Open a file containing trees with CMS dimuons, onia trees.
// 2) Book a histogram of the dimuon invariant mass
// 3) Write to a file
// The drawing with all the bells and whistles is left to a follow-up macro
// Needs dimuonYellowPlotDrawHisto.C for the code to do the drawing.
// The goal is for this macro to be run only when the data need to be made into
// a histogram, as this is time consuming.  The macro to draw can then be
// invoked as many times as needed to make plots look as desired, without the
// overhead of the call to the tree->Draw command that can take many minutes.
// 


void dimuonYellowPlotMakeHisto(TString histoFileName="dimuonMassYellowPlotHistos.root") {
  cout << "dimuonYellowPlotMakeHisto: Starting macro dimuonYellowPlotMakeHisto" << endl;
  
  //
  // Open the appropriate input file
  //
  TFile *inFile;
  inFile = new TFile("pPbData_2nd_PromptReco-v1_GR_P_V43D_pileupRej_newAccCut_tot.root","READ");
 
  
  // The histograms
  // sse variable size bins, multiplying the bin limit of a previous bin by a scale factor.
  // This will make the bins span about the same width in a log-x scale.
  
  cout << "dimuonYellowPlot: creating bin limit array..." << endl;

  double bins[100000];
  bins[0] = 0.2;
  int nBins = 0;
  double massBinScaleFactor = 1.015; // Typical values: //1.04; //1.015;
  // The for loop should give enough bins to span up to mass of about 200 GeV/c^2.
  for (int i=1; bins[i-1]<200; i++) {
    bins[i] = bins[i-1]*1.015; 
    nBins++;
  }

  
  cout << "dimuonYellowPlot: booking mass histogram with ";
  cout << nBins << " mass bins, mass limits: ";
  cout << bins[0] << " - " << bins[nBins] << endl;
  
  TH1F* massHistoUnlikeSign = new TH1F("massHistoUnlikeSign","massHistoUnlikeSign;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);

  // TH1F* massHistoLikeSign = new TH1F("massHistoLikeSign","massHistoLikeSign;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
  // TH1F* massHistoSubtracted = new TH1F("massHistoSubtracted","massHistoSubtracted;m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})",nBins,bins);
  // TH2F* h3 = new TH2F("h3","h3;#eta_{#mu^{+}} - #eta_{#mu^{-}};#mu^{+}#mu^{-} mass",500,-5.0,5.0,nBins,bins);

  cout << "dimuonYellowPlot: Draw histograms from tree..." << endl;

  myTree->Draw("Reco_QQ_4mom.M()>>massHistoUnlikeSign","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","");

  // Output file creation and writing of histogram
  cout << "dimuonYellowPlotMakeHisto: Open output file... " << histoFileName << endl;
  TFile* outFile = new TFile(histoFileName,"RECREATE");
  
  cout << "Writing histogram with " << massHistoUnlikeSign->GetEntries() << " dimuons " << endl;
  massHistoUnlikeSign->Write();
  outFile->Close();
  return;
}
