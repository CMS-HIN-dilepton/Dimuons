#include "Macros/allFunctions.h"

#include "Macros/makeWorkspace2015.C"
#include "Macros/buildModelUpsi2015.C"
#include "Macros/buildModelJpsi2015.C"

#include "Macros/drawPlot.C"

void SetOptions(InputOpt* opt, bool isData = true, bool isPbPb = false, int oniamode = 1,bool inExcStat= false) {

  opt->isData    = isData;
  opt->isPbPb    = isPbPb;
  opt->oniaMode  = oniamode;

  opt->plotDir   = "Plots";
  opt->lumi      = opt->isPbPb ? "PbPb" : "pp" ;

  opt->dMuon.Pt.Min = 0.0;
  opt->dMuon.Pt.Max = 100000.0;
  opt->dMuon.AbsRap.Min = 0.0;
  opt->dMuon.AbsRap.Max = 2.4;
  opt->sMuon.Pt.Max  = 100000.0;
  opt->sMuon.Eta.Min = -2.4;
  opt->sMuon.Eta.Max = 2.4;

  if (opt->oniaMode==1){  
    opt->dMuon.M.Min = 2.6;
    opt->dMuon.M.Max = inExcStat ? 4.0 : 3.5; 
    opt->sMuon.Pt.Min  = 3.0;
  } else if (opt->oniaMode==2){
    opt->dMuon.M.Min = 7.0;
    opt->dMuon.M.Max = 14.0; 
    opt->sMuon.Pt.Min  = 4.0;
  } else if (opt->oniaMode==3){
    opt->dMuon.M.Min = 70.0;
    opt->dMuon.M.Max = 110.0; 
    opt->sMuon.Pt.Min  = 10.0;
  }
  
  opt->Centrality.Start = opt->isPbPb ? 0 : -1;
  opt->Centrality.End   = 200;
  opt->RunNb.Start      = 262163;
  opt->RunNb.End        = 262328;
  
  return;
};

void fit2015(
             const TString FileName ="/afs/cern.ch/user/a/anstahll/public/pp502TeV/OniaTree_pp502TeV_Run262163.root", 
             int  oniamode  = 2,        // oniamode-> 3: Z,  2: Upsilon and 1: J/Psi
             bool isData    = true,     // isData = false for MC, true for Data
             bool isPbPb    = false,    // isPbPb = false for pp, true for PbPb
	     bool doFit = true,
             bool inExcStat = true      // if inExcStat is true, then the excited states are fitted
             ) {
    
  InputOpt opt;
  SetOptions(&opt, isData, isPbPb, oniamode,inExcStat); 
  
  RooWorkspace myws;
  makeWorkspace2015(myws, FileName, opt);

  RooRealVar* mass      = (RooRealVar*) myws.var("invariantMass"); 
  RooDataSet* dataOS_fit = (RooDataSet*) myws.data("dataOS");
  RooDataSet* dataSS_fit = (RooDataSet*) myws.data("dataSS");
  RooAbsPdf*  pdf = NULL;

  int nbins = 1; //ceil((opt.dMuon->M->Max - opt.dMuon->M->Min)/binw);
  if (oniamode==1){
    nbins = 140; 
  } else if (oniamode==2) {
    nbins = 70; 
  } else if (oniamode==3) {
    nbins = 40;
  } 

  if (doFit) {
    int sigModel=0, bkgModel=0;  
    if (isData) {
      if (oniamode==1){
        sigModel = inExcStat ? 1 : 2;
        bkgModel = 1;
      } else {
        sigModel = inExcStat ? 2 : 3; // gaussian   
        bkgModel = 2;
      }      
    } else {
      if (oniamode==1){
        sigModel = inExcStat ? 1 : 2; // gaussian   
        bkgModel = 2;
      } else {
        sigModel = inExcStat ? 2 : 3; // gaussian   
        bkgModel = 3;
      }
    }

    if (opt.oniaMode==1) buildModelJpsi2015(myws, sigModel, bkgModel,inExcStat);
    else if (opt.oniaMode==2) buildModelUpsi2015(myws, sigModel, bkgModel,inExcStat);

    pdf       =(RooAbsPdf*)  myws.pdf("pdf");
    RooFitResult* fitObject = pdf->fitTo(*dataOS_fit,Save(),Hesse(kTRUE),Extended(kTRUE)); // Fit
  }

  RooPlot* frame = mass->frame(Bins(nbins),Range(opt.dMuon.M.Min, opt.dMuon.M.Max));  
  dataOS_fit->plotOn(frame, Name("dataOS_FIT"), MarkerColor(kBlue), LineColor(kBlue), MarkerSize(1.2));
  dataSS_fit->plotOn(frame, Name("dataSS_FIT"), MarkerColor(kRed), LineColor(kRed), MarkerSize(1.2)); 
  if (doFit) {pdf->plotOn(frame,Name("thePdf"),Normalization(dataOS_fit->sumEntries(),RooAbsReal::NumEvent));}
  
  drawPlot(frame, pdf, opt, doFit,inExcStat);
}
