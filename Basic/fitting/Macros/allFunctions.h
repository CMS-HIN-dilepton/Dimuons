#ifndef allFunctions_h
#define allFunctions_h

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TROOT.h"
#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooCBShape.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooWorkspace.h"
#include "RooChebychev.h"
#include "RooPolynomial.h"
#include "RooExponential.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooAddPdf.h"
#include "RooConstVar.h"
#include "RooArgList.h"
#include "RooHist.h"
#include "TH1F.h"

#include <iostream>

using namespace std;
using namespace RooFit;


typedef struct StartEnd {
  int Start, End;
} StartEnd;
typedef struct MinMax {
  double Min, Max;
} MinMax;
typedef struct DiMuonPar {
  MinMax M, Pt, AbsRap;
} DiMuonPar;
typedef struct SiMuonPar {
  MinMax Pt, Eta;
} SiMuonPar;
typedef struct InputOpt {
  TString    plotDir; 
  TString    lumi;
  bool       isData, isPbPb;
  int        oniaMode;
  StartEnd   Centrality, RunNb;
  DiMuonPar  dMuon;
  SiMuonPar  sMuon;
} InputOpt;

struct ParticleMass { double JPsi, Psi2S, Y1S, Y2S, Y3S, Z; };
ParticleMass Mass = {3.096, 3.686, 9.460, 10.023, 10.355, 91.188};
const double binw   = 0.1;    //bin width of the histogram

#endif // #ifndef allFunctions_h
