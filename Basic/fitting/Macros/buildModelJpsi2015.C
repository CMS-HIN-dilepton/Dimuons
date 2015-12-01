#include "allFunctions.h"
#include "RooAddPdf.h"
#include "RooFormulaVar.h"
#include "RooConstVar.h"

using namespace RooFit;

void buildModelJpsi2015(RooWorkspace& w, int sigModel, int bkgModel,bool fitExcited){
  // C r e a t e   m o d e l  
  
  bool doRatio = true;
  int nt=100000;

  // Signal Model
  RooRealVar *nSigJPSI = new RooRealVar("N_{J/#psi}","N_{J/#psi}",0,nt*10);
  RooRealVar *nSigPSI2S = new RooRealVar("N_{#psi(2S)}","N_{#psi(2S)}",0,nt*10);
  if(!fitExcited){nSigPSI2S=NULL;}
  RooRealVar *mass = (RooRealVar*) w.var("invariantMass");

  RooRealVar *f2Svs1S   = NULL;
  if(doRatio && fitExcited){
    f2Svs1S   = new RooRealVar("R_{#frac{2S}{1S}}","f2Svs1S",0.26,-0.1,1.0);
    RooFormulaVar *tmp1 = new RooFormulaVar("N_{ #varUpsilon(2S)}","@0*@1", RooArgList(*nSigJPSI,*f2Svs1S));
    f2Svs1S->setConstant(kFALSE);
    nSigPSI2S = (RooRealVar*)tmp1;
  }

  RooRealVar *meanSigJPSI   = new RooRealVar("m_{J/#psi}","m_{J/#psi}",Mass.JPsi,Mass.JPsi-0.046,Mass.JPsi+0.046);
  RooConstVar *rat = new RooConstVar("rat", "rat", Mass.Psi2S/Mass.JPsi);
  RooFormulaVar *meanSigPSI2S = new RooFormulaVar("m_{#psi(2S)}","@0*@1",RooArgList(*meanSigJPSI,*rat));
  RooRealVar *sigmaSigJPSI  = new RooRealVar("#sigma_{J/#psi}","#sigma_{J/#psi}",0.1, 0.02, 0.2); // 
  RooFormulaVar *sigmaSigPSI2S  = new RooFormulaVar("#sigma_{#psi(2S)}","@0*@1",RooArgList(*sigmaSigJPSI,*rat)); // 
  RooRealVar *sigmaSigJPSI2  = new RooRealVar("#sigma_{CB}","#sigma_{CB}",0.03, 0.001, 0.06); // 

  RooRealVar *alpha = new RooRealVar("#alpha_{CB}","tail shift",1., 0., 30.*100.);  
  RooRealVar *n  = new RooRealVar("n_{CB}","power order",5., 1., 30.*100.);  
  RooRealVar *nW = new RooRealVar("n_{CB}","power order",5., 1., 50.*100.); 
   
  //////// Candidates for signal
  // Normal gaussians
  RooGaussian* signalG1  = NULL;
  RooCBShape*  signalCB1 = NULL;
  RooCBShape*  signalCB2 = NULL;
  RooCBShape*  signalCB1WN = NULL;
  RooCBShape*  signalCB2WN = NULL;

  RooAbsPdf* sigJPSI  = NULL;   
  RooAbsPdf* sigPSI2S = NULL; 
  RooRealVar *coeffGaus = new RooRealVar("coeffGaus","coeffGaus",0.1, 0.0,1.0*100.);

  switch(sigModel) 
    {    
    case 1: 
      // Gaussian for JPsi and Psi(2S)
      sigJPSI  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI); //signalG1
      sigPSI2S  = new RooGaussian("signalG2","Gaussian Sig2", *mass, *meanSigPSI2S, *sigmaSigPSI2S); //signalG2
      cout << "you're fitting 2 signal peaks with Gaussian functions"<< endl;
      break;  
    case 2: 
       // Gaussian for JPsi and Psi(2S)
      signalG1  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI); //signalG1
      signalCB2WN = new RooCBShape("signalCB2WN","FSR cb 1s", *mass, *meanSigJPSI, *sigmaSigJPSI2, *alpha, *nW);
      sigJPSI    = new RooAddPdf ("sigJPSI", "sigJPSI", RooArgList(*signalG1, *signalCB2WN), *coeffGaus);
      sigPSI2S  = new RooGaussian("signalG2","Gaussian Sig2", *mass, *meanSigPSI2S, *sigmaSigPSI2S); //signalG2
      cout << "you're fitting 2 signal peaks with Gaussian functions"<< endl;
      break;   
    case 3: 
      // Gaussian for JPsi
      sigJPSI  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI); //signalG1
      cout << "you're fitting 1 signal peak with a Gaussian function"<< endl;
      break;    
    case 4: 
      // Crystal Ball for JPsi
      sigJPSI = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI); //signalG1
      cout << "you're fitting 1 signal peak with a Crystal Ball function"<< endl;
      break;
    case 5:
      // Currently used in JPsi analysis
      // Sum of gaussian 1 and crystall ball 2 with wide n  for JPsi
      signalG1    = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI); //signalG1
      signalCB2WN = new RooCBShape("signalCB2WN","FSR cb 1s", *mass, *meanSigJPSI, *sigmaSigJPSI2, *alpha, *nW);
      sigJPSI    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB2WN), *coeffGaus);
      cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
      break;
    case 6:
      // Sum of gaussian 1 and a crystall ball for JPsi
      signalG1  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI);
      signalCB1 = new RooCBShape("signalCB1", "FSR cb 1s", *mass, *meanSigJPSI, *sigmaSigJPSI, *alpha, *n);
      sigJPSI    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB1), *coeffGaus);
      cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
      break;
    case 7:
      // Sum of gaussian 1 and a crystall ball for JPsi
      signalG1  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI);
      signalCB2 = new RooCBShape("signalCB2", "FSR cb 1s", *mass, *meanSigJPSI, *sigmaSigJPSI2, *alpha, *n);
      sigJPSI    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB2), *coeffGaus);
      cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
      break;
    case 8:
      // Sum of gaussian 1 and crystall ball with wide n for JPsi
      signalG1    = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSigJPSI, *sigmaSigJPSI);
      signalCB1WN = new RooCBShape("signalCB1WN","FSR cb 1s", *mass, *meanSigJPSI, *sigmaSigJPSI, *alpha, *nW);
      sigJPSI    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB1WN), *coeffGaus);
      cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
      break;
    default :
      cout<<"Donno what you are talking about! Pick another fit option for signal!"<<endl;
      break;
  }
   
  // Background Model  
  RooRealVar *nBkg   = new RooRealVar("N_{bkg}","nbkgd",0,nt);
  RooRealVar *coeffPoll = new RooRealVar("#lambda","coeffPoll",-0.05, -5., 5.*100.);
  RooRealVar *coeffExp  = new RooRealVar("#lambda","coeffExp",0.1, -3., 1.*100.);
  RooAbsPdf* bkgPDF = NULL;

  switch(bkgModel) {    
    case 1: 
      // 1st order polynomial
      bkgPDF = new RooPolynomial("bkgPoll", "bkgPoll", *mass, *coeffPoll);
      cout << "you're fitting the background with a 1st order polynomial"<< endl;
      break;
    case 2:
      // Exponential
      bkgPDF = new RooExponential("bkgExp", "bkgExp", *mass, *coeffExp);
      cout << "you're fitting the background with an exponential"<< endl;
      break;
    default :
      cout<<"Donno what you are talking about! Pick another fit option for background!"<<endl;
      break;
  }

  // Total PDF = Signal + Background
  RooAbsPdf  *pdf  = new RooAddPdf ("pdf","total p.d.f.", RooArgList(*sigJPSI, *sigPSI2S, *bkgPDF), RooArgList(*nSigJPSI, *nSigPSI2S, *nBkg));

  w.import(*pdf);
  w.Print();

}
