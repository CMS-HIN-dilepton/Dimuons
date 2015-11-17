#include "allFunctions.h"
#include "RooAbsPdf.h"
#include "RooCBShape.h"

using namespace RooFit;

void buildModelJpsi2015(RooWorkspace& w, int signalModel, int bkgdModel){
   // C r e a t e   m o d e l  
   int nt=100000;
   // cout << "you're building a model for the quarkonium resonance of mass = "<< M1S <<" GeV/c^{2},"endl;
   RooRealVar *nsig1f   = new RooRealVar("N_{J/#psi}","nsig1",0,nt*10);
   RooRealVar* mass       = (RooRealVar*) w.var("invariantMass");

   RooRealVar  *mean = new RooRealVar("m_{J/#psi}","J/#psi mean",MJpsi,MJpsi-0.2,MJpsi+0.2);
   // scale mean and resolution by mass ratio
   RooFormulaVar *mean1 = new RooFormulaVar("mean1","@0",RooArgList(*mean));

   // //detector resolution ?? where is this coming from?
   RooRealVar    *sigma  = new RooRealVar("#sigma_{CB1}","#sigma_{CB1}",0.01,0.1); // 
   RooFormulaVar *sigma1 = new RooFormulaVar("sigma1","@0"   ,RooArgList(*sigma));
   RooRealVar *alpha  = new RooRealVar("#alpha_{CB}","tail shift",0.1,10);    // MC 5tev 1S pol2 
   RooRealVar *npow   = new RooRealVar("n_{CB}","power order",0.1,10);    // MC 5tev 1S pol2 
   RooRealVar *sigmaFraction = new RooRealVar("sigmaFraction","Sigma Fraction",0.,1.);
   // scale the sigmaGaus with sigma1S*scale=sigmaGaus now.
   RooRealVar    *scaleWidth = new RooRealVar("#sigma_{CB2}/#sigma_{CB1}","scaleWidth",1.,2.5);
   RooFormulaVar *sigmaGaus = new RooFormulaVar("sigmaGaus","@0*@1", RooArgList(*sigma1,*scaleWidth));
   RooGaussian* gauss = new RooGaussian("gaus","gaus",
         *nsig1f,
         *mass,    //mean
         *sigmaGaus); //sigma
   RooGaussian* gauss1 = new RooGaussian("gaus1","gaus1",
         *nsig1f,
         *mean,    //mean
         *sigma); //sigma

   // declarations
   RooAbsPdf  *cb1S_1   = NULL;
   RooAbsPdf  *cb1S_2   = NULL;
   RooAbsPdf  *sig1S   = NULL;

   switch(signalModel){    
      case 1: //crystal boule
         sig1S   = new RooCBShape("cb1S_1", "FSR cb 1s",
               *mass,*mean1,*sigma,*alpha,*npow);
         cout << "you're fitting each signal peak with a Crystal Ball function"<< endl;
         break;
      case 2: //Gaussein
         sig1S  = new RooGaussian ("g1", "gaus 1s",
               *mass,*mean1,*sigma);
         cout << "you're fitting 1 signal peak with a Gaussian function"<< endl;
         break;
      case 3: //Gaussein + crystal boule
         cb1S_1    = new RooCBShape ("cb1S_1", "FSR cb 1s",
               *mass,*mean1,*sigma,*alpha,*npow);
         sig1S  = new RooAddPdf ("cbg", "cbgaus 1s",
               RooArgList(*gauss,*cb1S_1),*sigmaFraction);
         cout << "you're fitting 1 signal peak with a sum of a Gaussian and a Crystal Ball function"<< endl;
         break;
      case 4: //crystal boules
         cb1S_1    = new RooCBShape ("cb1S_1", "FSR cb 1s",
               *mass,*mean1,*sigma,*alpha,*npow);

         cb1S_2    = new RooCBShape ("cb1S_2", "FSR cb 1s",
               *mass,*mean1,*sigmaGaus,*alpha,*npow);
         sig1S  = new RooAddPdf  ("cbcb","1S mass pdf",
               RooArgList(*cb1S_1,*cb1S_2),*sigmaFraction);
         cout << "you're fitting each signal peak with a Double Crystal Ball function"<< endl;
         break;

      case 5: //deux Gausseins
         sig1S  = new RooAddPdf ("cb1S_1", "cbgaus 1s",
               RooArgList(*gauss,*gauss1),*sigmaFraction);
         cout << "you're fitting each signal peak with a Double Gaussian function"<< endl;
         break;
   }
   // bkg Chebychev
   RooRealVar *nbkgd   = new RooRealVar("n_{Bkgd}","nbkgd",0,nt);
   RooRealVar *bkg_a1  = new RooRealVar("a1_bkg", "bkg_{a1}", 0, -5, 5);
   RooRealVar *bkg_a2  = new RooRealVar("a2_Bkg", "bkg_{a2}", 0, -2, 2);
   RooRealVar *bkg_a3  = new RooRealVar("a3_Bkg", "bkg_{a3}", 0, -0.9, 2);

   //  likesign
   //RooRealVar *nLikesignbkgd = new RooRealVar("NLikesignBkg","nlikesignbkgd",nt*0.75,0,10*nt);
   // *************************************************** bkgModel

   RooRealVar turnOn("turnOn","turnOn", 0,10);
   RooRealVar width("width","width", 0.1,10);// MB 2.63
   RooRealVar decay("decay","decay",0,10);// MB: 3.39


   width.setConstant(false);
   decay.setConstant(false);
   turnOn.setConstant(false);


   alpha->setConstant(false);
   npow->setConstant(false);
   sigma->setConstant(false);
   scaleWidth->setConstant(false);
   sigmaFraction->setConstant(false);

   //thisPdf: form of the bkg pdf
   //pdf_combinedbkgd; // total bkg pdf. usually form*normalization  (so that you can do extended ML fits)

   RooAbsPdf *pdf_combinedbkgd = NULL;
   RooRealVar *fPol   = new RooRealVar("F_{pol}","fraction of polynomial distribution",0.0,1);
   RooAbsPdf *ChebPdf = NULL;
   RooAbsPdf *ErrPdf = NULL;
   RooAbsPdf* ExpPdf = NULL;

   switch (bkgdModel) 
   {
      case 3 : //use error function to fit the OS directly
         bkg_a3->setConstant(true);
         pdf_combinedbkgd            = new  RooGenericPdf("bkgPdf","bkgPdf",
               "exp(-@0/decay)*(TMath::Erf((@0-turnOn)/width)+1)",
               RooArgList(*mass,turnOn,width,decay));
         break;

      case 4 : //use pol 2+ErfExp to fit the OS directly

         ChebPdf  = new RooChebychev("ChebPdf","ChebPdf",
               *mass, RooArgList(*bkg_a1,*bkg_a2));
         ErrPdf     = new  RooGenericPdf("ErrPdf","ErrPdf",
               "exp(-@0/decay)*(TMath::Erf((@0-turnOn)/width)+1)",
               RooArgList(*mass,turnOn,width,decay));
         pdf_combinedbkgd   = new RooAddPdf ("bkgPdf","total combined background pdf",
               RooArgList(*ChebPdf,*ErrPdf),
               RooArgList(*fPol));

         break;
      case 5 : //use ( error function + polynomial 1) to fit the OS directly

         bkg_a3->setConstant(true);
         bkg_a2->setConstant(true);
         ChebPdf  = new RooChebychev("ChebPdf","ChebPdf",
               *mass, RooArgList(*bkg_a1,*bkg_a2,*bkg_a3));
         ErrPdf     = new  RooGenericPdf("ErrPdf","ErrPdf",
               "exp(-@0/decay)*(TMath::Erf((@0-turnOn)/width)+1)",
               RooArgList(*mass,turnOn,width,decay));
         pdf_combinedbkgd   = new RooAddPdf ("bkgPdf","total combined background pdf",
               RooArgList(*ChebPdf,*ErrPdf),
               RooArgList(*fPol));
         break;
      case 6: // NOT WORKING
         ChebPdf  = new RooChebychev("ChebPdf","ChebPdf",
               *mass, RooArgList(*bkg_a1,*bkg_a2));
         ExpPdf     = new  RooGenericPdf("ExpPdf","ExpPdf",
               "exp(-@0/decay)",
               RooArgList(*mass,decay));
         pdf_combinedbkgd  =  new RooAddPdf ("bkgPdf","total combined background pdf",
               RooArgList(*ChebPdf,*ExpPdf),
               RooArgList(*fPol));
         break;
      default :
         cout<<"Donno what you are talking about! Pick another fit option!"<<endl;
         break;
   }

   //###### the nominal fit with default pdf 

   // can remove the double crystal ball in pbpb: just commenting out and copying an appropriate version
   RooAbsPdf  *pdf             = new RooAddPdf ("pdf","total p.d.f.",
         RooArgList(*sig1S,*pdf_combinedbkgd),
         RooArgList(*nsig1f,*nbkgd));

   w.import(*pdf);
   w.Print();
}
