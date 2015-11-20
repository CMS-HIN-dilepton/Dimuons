#include "Macros/allFunctions.h"

#include "TLorentzVector.h"
#include "TClonesArray.h"

using namespace RooFit;

void makeWorkspace2015(RooWorkspace& ws, const TString FileName, struct InputOpt opt){
  double binw=0.05;
 
  cout<< "You're in makeWorkspace2015!" << endl;
  std::string finput(FileName);
  cout << finput << endl;
  //  TFile f(finput,"read");
  TFile *f = new TFile(finput.c_str());
  TTree* theTree       = (TTree*)gROOT->FindObject("myTree"); // OS --- all mass
  cout << theTree << endl;
  //  //RooWorkspace* ws = new RooWorkspace("ws","DiMuon Mass Fitting");
  //ws.var("invariantMass");	
  RooRealVar* mass       = new RooRealVar("invariantMass","#mu#mu mass", opt.dMuon.M.Min, opt.dMuon.M.Max, "GeV/c^{2}");	
  //  ws.import(*mass);
  RooRealVar* dimuPt     = new RooRealVar("dimuPt","p_{T}(#DiMuon)",0,60,"GeV/c");
  RooRealVar* dimuRapidity = new RooRealVar("dimuRapidity",  "dimuRapidity",-2.4, 2.4);
  RooRealVar* vProb      = new RooRealVar("vProb",  "vProb"  ,0.01,1.00);
  RooRealVar* QQsign     = new RooRealVar("QQsign",  "QQsign"  ,-1,5);
  RooRealVar* Centrality = new RooRealVar("Centrality","Centrality",0,200);
  RooRealVar* RunNb      = new RooRealVar("RunNb","RunNb",0,350000);
  RooRealVar* muPlusPt   = new RooRealVar("muPlusPt","muPlusPt", 0, 1000);
  RooRealVar* muMinusPt  = new RooRealVar("muMinusPt","muMinusPt", 0, 1000);
  RooRealVar* muPlusEta  = new RooRealVar("muPlusEta","muPlusEta", -2.4, 2.4);
  RooRealVar* muMinusEta = new RooRealVar("muMinusEta","muMinusEta", -2.4, 2.4);
  RooDataSet* data0, *data;
  RooArgSet cols(*mass,*dimuPt,*dimuRapidity,*Centrality,*muPlusPt,*muMinusPt,*muPlusEta,*muMinusEta, *RunNb);
  data0 = new RooDataSet("data","data",cols); 

  // import the tree to the RooDataSet
  UInt_t          runNb;
  Int_t           centrality;
  ULong64_t       HLTriggers;
  Int_t           Reco_QQ_size;
  Int_t           Reco_QQ_sign[99];   //[Reco_QQ_size]
  TClonesArray    *Reco_QQ_4mom;
  TClonesArray    *Reco_QQ_mupl_4mom;
  TClonesArray    *Reco_QQ_mumi_4mom;
  ULong64_t       Reco_QQ_trig[99];   //[Reco_QQ_size]
  Float_t         Reco_QQ_VtxProb[99];   //[Reco_QQ_size]

  TBranch        *b_runNb;   //!
  TBranch        *b_centrality;   //!
  TBranch        *b_HLTriggers;   //!
  TBranch        *b_Reco_QQ_size;   //!
  TBranch        *b_Reco_QQ_sign;   //!
  TBranch        *b_Reco_QQ_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_4mom;   //!
  TBranch        *b_Reco_QQ_trig;   //!
  TBranch        *b_Reco_QQ_VtxProb;   //!

  Reco_QQ_4mom = 0;
  Reco_QQ_mupl_4mom = 0;
  Reco_QQ_mumi_4mom = 0;
  theTree->SetBranchAddress("runNb", &runNb, &b_runNb);
  theTree->SetBranchAddress("Centrality", &centrality, &b_centrality);
  theTree->SetBranchAddress("HLTriggers", &HLTriggers, &b_HLTriggers);
  theTree->SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
  theTree->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
  theTree->GetBranch("Reco_QQ_4mom")->SetAutoDelete(kFALSE);
  theTree->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
  theTree->GetBranch("Reco_QQ_mupl_4mom")->SetAutoDelete(kFALSE);
  theTree->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom, &b_Reco_QQ_mupl_4mom);
  theTree->GetBranch("Reco_QQ_mumi_4mom")->SetAutoDelete(kFALSE);
  theTree->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom, &b_Reco_QQ_mumi_4mom);
  theTree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig, &b_Reco_QQ_trig);
  theTree->SetBranchAddress("Reco_QQ_VtxProb", Reco_QQ_VtxProb, &b_Reco_QQ_VtxProb);

   if (theTree == 0) return;

   Long64_t nentries = theTree->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // Long64_t ientry = LoadTree(jentry);
      // if (ientry < 0) break;
      nb = theTree->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      for (int i=0; i<Reco_QQ_size; i++) {
         TLorentzVector *qq4mom = (TLorentzVector*) Reco_QQ_4mom->At(i);
         TLorentzVector *qq4mupl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(i);
         TLorentzVector *qq4mumi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(i);
         mass->setVal(qq4mom->M());
         dimuPt->setVal(qq4mom->Pt());
         dimuRapidity->setVal(qq4mom->Rapidity());
         vProb->setVal(Reco_QQ_VtxProb[i]);
         QQsign->setVal(Reco_QQ_sign[i]);
         Centrality->setVal(centrality);
         muPlusPt->setVal(qq4mupl->Pt());
         muMinusPt->setVal(qq4mumi->Pt());
         muPlusEta->setVal(qq4mupl->Eta());
         muMinusEta->setVal(qq4mumi->Eta());
         RunNb->setVal(runNb);

         data0->add(cols);
      }
   }


  TString cut_ap(Form("(%.2f<invariantMass && invariantMass<%.2f) &&" 
                      "(%d<=Centrality && Centrality<%d) &&" 
                      "(%.2f<muPlusEta && muPlusEta < %.2f) &&" 
                      "(%.2f<muMinusEta && muMinusEta < %.2f) &&" 
                      "(%.2f<dimuPt && dimuPt<%.2f) &&"
                      "(abs(dimuRapidity)>%.2f && abs(dimuRapidity)<%.2f)  &&"
                      "(muPlusPt > %.2f && muMinusPt > %.2f) &&"
                      "(%d<=RunNb && RunNb<=%d)",
                      opt.dMuon.M.Min, opt.dMuon.M.Max,
                      opt.Centrality.Start, opt.Centrality.End,
                      opt.sMuon.Eta.Min, opt.sMuon.Eta.Max,
                      opt.sMuon.Eta.Min, opt.sMuon.Eta.Max,
                      opt.dMuon.Pt.Min, opt.dMuon.Pt.Max,
                      opt.dMuon.AbsRap.Min, opt.dMuon.AbsRap.Max,
                      opt.sMuon.Pt.Min, opt.sMuon.Pt.Min,
                      opt.RunNb.Start, opt.RunNb.End));
  cout << cut_ap << endl;
  data =  ( RooDataSet*) data0->reduce(Cut(cut_ap));
  ws.import(*data);
  data->Print();
  f->Close();
}
