#include "Macros/allFunctions.h"

#include "Macros/makeWorkspace2015.C"
#include "Macros/buildModelUpsi2015.C"
#include "Macros/buildModelJpsi2015.C"

#include "Macros/drawPlot.C"

void SetOptions(InputOpt* opt, bool isData = true, bool isPbPb = false, int oniamode = 1, bool inExcStat = false) {

  opt->isData    = isData;
  opt->isPbPb    = isPbPb;
  opt->isJPsi    = oniamode==1 ? true : false;
  opt->inExcStat = inExcStat;

  opt->plotDir   = "Plots";
  opt->lumi      = opt->isPbPb ? "PbPb ?.? #mub^{-1}" : "pp ?.? pb^{-1}" ;
  opt->dMuon.M.Min = opt->isJPsi ? 2.6 : 7.0;
  
  opt->dMuon.M.Max = opt->isJPsi ? 3.5 : 14.0;
  opt->dMuon.Pt.Min = 0.0;
  opt->dMuon.Pt.Max = 30.0;
  opt->dMuon.AbsRap.Min = 0.0;
  opt->dMuon.AbsRap.Max = 2.4;
  
  opt->sMuon.Pt.Min  = 3.0;
  opt->sMuon.Pt.Max  = 100.0;
  opt->sMuon.Eta.Min = -2.4;
  opt->sMuon.Eta.Max = 2.4;
  
  opt->Centrality.Start = opt->isPbPb ? 0 : -1;
  opt->Centrality.End   = 200;
  opt->RunNb.Start      = 0;
  opt->RunNb.End        = 192000;
  
  return;
};

void fit2015(
             const TString FileName = "/afs/cern.ch/user/e/echapon/workspace/public/RunPrep2015/oniatree_upsi1S_5TeV_3.8T.root", 
             bool isData    = true,     // isData = false for MC, true for Data
             bool isPbPb    = false,    // isPbPb = false for pp, true for PbPb
             int  oniamode  = 2,        // oniamode-> 2: Upsilon and 1: J/Psi
             bool inExcStat = false     // if inExcStat is true, then the excited states are fitted
             ) {
    
  InputOpt opt;
  SetOptions(&opt, isData, isPbPb, oniamode, inExcStat); 
  
  TString colltag = opt.isPbPb ? "PbPb" : "pp";
  TString  nSigName = opt.isJPsi ? "N_{J/#psi}" : "N_{#varUpsilon(1S)}" ;
  cout << opt.dMuon.M.Min << " " << opt.dMuon.M.Max << endl;

  int sigModel, bkgModel;  
  if (isData) {
    if (oniamode==1){
      sigModel = 1;
      bkgModel = 2;
    } else {
      sigModel = inExcStat ? 1 : 3; // gaussian   
      bkgModel = 3;
    }      
  } else {
    if (oniamode==1){
      sigModel = 4; // gaussian   
      bkgModel = 2;
    } else {
      sigModel = inExcStat ? 1 : 3; // gaussian   
      bkgModel = 3;
    }
  }

  RooWorkspace myws;
  makeWorkspace2015(myws, FileName, opt);

  if (opt.isJPsi) buildModelJpsi2015(myws, sigModel, bkgModel);
  else buildModelUpsi2015(myws, sigModel, bkgModel);

  RooRealVar* mass      =(RooRealVar*) myws.var("invariantMass"); //
  RooDataSet* data0_fit =(RooDataSet*) myws.data("data");
  RooAbsPdf*  pdf       =(RooAbsPdf*)  myws.pdf("pdf");
  
  RooFitResult* fitObject = pdf->fitTo(*data0_fit,Save(),Hesse(kTRUE),Extended(kTRUE)); // Fit

  Double_t baseNll = fitObject->minNll();
  RooRealVar *nSig   =(RooRealVar*) myws.var(nSigName);

  RooArgSet allvars = myws.allVars();
  int npars = allvars.getSize() ;
  int nbins = 40; //ceil((opt.dMuon->M->Max - opt.dMuon->M->Min)/binw);
  RooPlot* frame = mass->frame(Bins(nbins),Range(opt.dMuon.M.Min, opt.dMuon.M.Max));  
  data0_fit->plotOn(frame); // data drawn first for pdf object to pick the proper normalisation
  pdf->plotOn(frame,Name("thePdf"));
  
  drawPlot(frame, pdf, nSig->getVal(), opt);
}
