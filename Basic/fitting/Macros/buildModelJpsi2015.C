#include "Macros/allFunctions.h"
#include "RooAddPdf.h"

using namespace RooFit;

void buildModelJpsi2015(RooWorkspace& w, int sigModel, int bkgModel){
  // C r e a t e   m o d e l  
  int nt=100000;

  // Signal Model
  RooRealVar *nSig1 = new RooRealVar("N_{J/#psi}","nSig1",0,nt*10);
  RooRealVar *mass   = (RooRealVar*) w.var("invariantMass");

  RooRealVar *meanSig1   = new RooRealVar("m_{J/#psi}","m_{J/#psi}",Mass.JPsi,Mass.JPsi-0.046,Mass.JPsi+0.046);
  RooRealVar *sigmaSig1  = new RooRealVar("#sigma_{J/#psi}","#sigma_{J/#psi}",0.1, 0.02, 0.2); // 
  RooRealVar *sigmaSig2  = new RooRealVar("#sigma_{CB}","#sigma_{CB}",0.03, 0.001, 0.06); // 

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

  RooAbsPdf* sigPDF = NULL;   
  RooRealVar *coeffGaus = new RooRealVar("coeffGaus","coeffGaus",0.1, 0.0,1.0*100.);

  switch(sigModel) 
    {    
    case 1: 
      // Gaussian 
      sigPDF  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSig1, *sigmaSig1); //signalG1
      cout << "you're fitting 1 signal peak with a Gaussian function"<< endl;
      break;    
    case 2: 
      // Crystal Ball
      sigPDF = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSig1, *sigmaSig1); //signalG1
      cout << "you're fitting 1 signal peak with a Crystal Ball function"<< endl;
      break;
    case 3:
      // Currently used in JPsi analysis
      // Sum of gaussian 1 and crystall ball 2 with wide n 
      signalG1    = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSig1, *sigmaSig1); //signalG1
      signalCB2WN = new RooCBShape("signalCB2WN","FSR cb 1s", *mass, *meanSig1, *sigmaSig2, *alpha, *nW);
      sigPDF    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB2WN), *coeffGaus);
      cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
      break;
    case 4:
      // Sum of gaussian 1 and a crystall ball
      signalG1  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSig1, *sigmaSig1);
      signalCB1 = new RooCBShape("signalCB1", "FSR cb 1s", *mass, *meanSig1, *sigmaSig1, *alpha, *n);
      sigPDF    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB1), *coeffGaus);
      cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
      break;
    case 5:
      // Sum of gaussian 1 and a crystall ball
      signalG1  = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSig1, *sigmaSig1);
      signalCB2 = new RooCBShape("signalCB2", "FSR cb 1s", *mass, *meanSig1, *sigmaSig2, *alpha, *n);
      sigPDF    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB2), *coeffGaus);
      cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
      break;
    case 6:
      // Sum of gaussian 1 and crystall ball with wide n
      signalG1    = new RooGaussian("signalG1","Gaussian Sig1", *mass, *meanSig1, *sigmaSig1);
      signalCB1WN = new RooCBShape("signalCB1WN","FSR cb 1s", *mass, *meanSig1, *sigmaSig1, *alpha, *nW);
      sigPDF    = new RooAddPdf ("sigPDF", "sigPDF", RooArgList(*signalG1, *signalCB1WN), *coeffGaus);
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
  RooAbsPdf  *pdf  = new RooAddPdf ("pdf","total p.d.f.", RooArgList(*sigPDF,*bkgPDF), RooArgList(*nSig1,*nBkg));

  w.import(*pdf);
  w.Print();

}
