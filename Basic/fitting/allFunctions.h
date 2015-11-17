#ifndef allFunctions_h
#define allFunctions_h

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooCBShape.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooWorkspace.h"
#include "RooChebychev.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooAddPdf.h"
#include "RooConstVar.h"
#include "RooArgList.h"

#include <iostream>

using namespace std;
using namespace RooFit;

const double binw   = 0.1;    //bin width of the histogram
const double MJpsi = 3.096;
const double Mpsi2S = 3.686;
const double M1S = 9.460;   //upsilon 1S pgd mass value
const double M2S = 10.023;  //upsilon 2S pgd mass value
const double M3S = 10.355;  //upsilon 3S pgd mass value


#endif // #ifndef allFunctions_h
