#include "allFunctions.h"

#include "makeWorkspace2015.C"
#include "buildModelUpsi2015.C"
#include "buildModelJpsi2015.C"

double massmin = 7; // actually modified later depending on oniamode
double massmax = 11.5; // actually modified later depending on oniamode
double singlemuon_ptmin = 3;
double singlemuon_etamin = -2.4;
double singlemuon_etamax = 2.4;
double dimu_ptmin = 0;
double dimu_ptmax = 30;
double dimu_absymin = 0;
double dimu_absymax = 2.4;
int centralitymin = 0;
int centralitymax = 200;


void fit2015(const char *filename="/afs/cern.ch/user/e/echapon/workspace/public/RunPrep2015/fitting_utils/dimuonTree_MCtest_upsi1S_5TeV_3.8T_RunMCtest_trigBit2_allTriggers1.root", bool ispbpb=false, int oniamode=2, bool isdata=false) {
   // ispbpb = false for pp, true for PbPb
   // oniamode = 1 for J/psi, 2 for upsilon
   // isdata = false for MC, true for data

   TString colltag = "PbPb";

   if (!ispbpb) {
      centralitymin=-1;
      colltag = "pp";
   }

   TString nsigname = "";
   if (oniamode==1) {
      nsigname = "N_{J/#psi}";
      massmin = 2.5;
      massmax = 4;
   }
   else if (oniamode==2) {
      nsigname = "N_{ #varUpsilon(1S)}";
      massmin = 7;
      massmax = 14;
   }
   else {cout << "Error, oniamode should be 1 (jpsi) or 2 (upsilon)" << endl; return;}

   cout << massmin << " " << massmax << endl;

   RooWorkspace myws;
   makeWorkspace2015(myws, 
         filename,
         massmin,massmax,
         singlemuon_etamin,singlemuon_etamax,
         singlemuon_ptmin,
         dimu_absymin,dimu_absymax, 
         dimu_ptmin,dimu_ptmax, 
         centralitymin,centralitymax );

   if (oniamode==1) buildModelJpsi2015(myws, 2, 3);
   else if (oniamode==2) buildModelUpsi2015(myws, 2, 3);

   RooRealVar* mass =(RooRealVar*) myws.var("invariantMass"); //
   RooDataSet* data0_fit =(RooDataSet*) myws.data("data");
   RooAbsPdf* pdf = (RooAbsPdf*) myws.pdf("pdf");
   RooFitResult* fitObject = pdf->fitTo(*data0_fit,Save(),Hesse(kTRUE),Extended(kTRUE)); // Fit
   pdf->Print();
   Double_t baseNll = fitObject->minNll();
   // RooMinuit m(*nll);

   RooRealVar *nsig1f   =(RooRealVar*) myws.var(nsigname);

   RooArgSet allvars = myws.allVars();
   int npars= allvars.getSize() ;
   int nbins = 40;//ceil((massmax-massmin)/binw);
   RooPlot* frame = mass->frame(Bins(nbins),Range(massmin,massmax));  
   data0_fit->plotOn(frame);// data drawn first for pdf object to pick the proper normalisation
   pdf->plotOn(frame,Name("thePdf"));

   frame->Draw();
}
