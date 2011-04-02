// -*- C++ -*-
//
// Package:    HiOniaAnalyzer
// Class:      HiOniaAnalyzer
// 
/**\class HiOniaAnalyzer HiOniaAnalyzer.cc UserCode/tdahms/HiAnalysis/HiOnia/plugins/HiOniaAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Torsten Dahms,40 4-A32,+41227671635,
//         Created:  Mon Nov 29 03:13:35 CET 2010
// $Id: HiOniaAnalyzer.cc,v 1.17 2011/03/17 11:18:13 tdahms Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include <TTree.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

#include "HiAnalysis/HiOnia/interface/MyCommonHistoManager.h"

//
// class declaration
//

class HiOniaAnalyzer : public edm::EDAnalyzer {
public:
  explicit HiOniaAnalyzer(const edm::ParameterSet&);
  ~HiOniaAnalyzer();
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  
  void InitEvent();
  void InitTree();

  void makeCuts(int sign) ;
  bool checkCuts(const pat::CompositeCandidate* cand, const pat::Muon* muon1,  const pat::Muon* muon2, bool(HiOniaAnalyzer::* callFunc1)(const pat::Muon*), bool(HiOniaAnalyzer::* callFunc2)(const pat::Muon*)); 

  void fillGenInfo();

  void fillRecoMuons(int theCentralityBin);
  bool isMuonInAccept(const pat::Muon* aMuon);

  pair< unsigned int, const pat::CompositeCandidate* > theBestQQ(int sign);
  double CorrectMass(const reco::Muon& mu1,const reco::Muon& mu2, int mode);

  bool selGlobalMuon(const pat::Muon* aMuon);
  bool selTrackerMuon(const pat::Muon* aMuon);

  void fillRecoHistos(int lastSign);
  void fillRecoJpsi(int iSign, int count, std::string trigName, std::string centName);
  void fillHistosAndDS(unsigned int theCat, const pat::CompositeCandidate* aJpsiCand);

  void fillTreeMuon(const pat::Muon* muon, int iType, int trigBits);
  void fillTreeJpsi(int iSign, int count);

  void checkTriggers(const pat::CompositeCandidate* aJpsiCand);

  TLorentzVector lorentzMomentum(const reco::Candidate::LorentzVector& p);
  // ----------member data ---------------------------
  enum StatBins {
    BIN_nEvents = 0,
    BIN_HLT_HIL1DoubleMuOpen = 1,
    BIN_HLT_HIL2DoubleMu0 = 2,
    BIN_HLT_HIL2DoubleMu3 = 3,
    BIN_HLT_HIL2Mu20 = 4,
    BIN_HLT_HIL2Mu3 = 5,
    BIN_HLT_HIL2Mu5Tight = 6
  };

  enum dimuonCategories {
    GlbGlb = 0,
    GlbTrk = 1,
    TrkTrk = 2,
    GlbCal = 3,
    TrkCal = 4,
    CalCal = 5
  };

  std::vector<std::string> theRegions;
  std::vector<std::string> theCentralities;
  std::vector<std::string> theTriggerNames;
  std::vector<std::string> theSign;

  float etaMin;
  float etaMax;

  // TFile
  TFile* fOut;

  // TTree
  TTree* myTree;

  TClonesArray* Reco_mu_4mom;
  TClonesArray* Reco_mu_3vec;
  TClonesArray* Reco_QQ_4mom;
  TClonesArray* Reco_QQ_mupl_4mom;
  TClonesArray* Reco_QQ_mumi_4mom;

  TClonesArray* Gen_mu_4mom;
  TClonesArray* Gen_mu_3vec;
  TClonesArray* Gen_QQ_4mom;
  TClonesArray* Gen_QQ_mupl_4mom;
  TClonesArray* Gen_QQ_mumi_4mom;

  static const int Max_QQ_size = 100;
  static const int Max_mu_size = 100;

  int Gen_QQ_size; // number of generated Onia
  int Gen_QQ_type[100]; // Onia type: prompt, non-prompt, unmatched
  //  float Gen_QQ_ctau[100];    // ctau: flight time
  
  
  int Gen_mu_size; // number of generated muons
  int Gen_mu_charge[100]; // muon charge
  int Gen_mu_type[100]; // muon type: prompt, non-prompt, unmatched

  int Reco_QQ_size;       // Number of reconstructed Onia 
  int Reco_QQ_type[100];   // Onia category: GG, GT, TT
  int Reco_QQ_sign[100];   /* Mu Mu combinations sign:
			     0 = +/- (signal)
			     1 = +/+
			     2 = -/- 
			  */
  int Reco_QQ_trig[100];      // Vector of trigger bits matched to the Onia
  float Reco_QQ_VtxProb[100]; // chi2 probability of vertex fitting 
  float Reco_QQ_ctau[100];    // ctau: flight time
  float Reco_QQ_ctauErr[100]; // error on ctau
  float Reco_QQ_ctauTrue[100];    // true ctau

  int Reco_mu_size;           // Number of reconstructed muons
  int Reco_mu_trig[100];      // Vector of trigger bits matched to the muons
  float Reco_mu_ptErr[100];   // Vector of err on pt of muons
  float Reco_mu_phiErr[100];  // Vector of err on phi of muons
  float Reco_mu_etaErr[100];  // Vector of err on eta of muons
  float Reco_mu_d0[100];      // Vector of d0 of muons
  float Reco_mu_d0err[100];   // Vector of d0err of muons
  float Reco_mu_dz[100];      // Vector of dz of muons
  float Reco_mu_dzerr[100];   // Vector of dzerr of muons
  int Reco_mu_charge[100];  // Vector of charge of muons
  int Reco_mu_type[100];  // Vector of type of muon (global=0, tracker=1, calo=2)  
  float Reco_mu_normChi2[100];   // Vector of chi2/ndof of muons
  int Reco_mu_nhitsCSC[100];    // Vector of number of valid hits of muons
  int Reco_mu_nhitsDT[100];    // Vector of number of valid hits of muons
  int Reco_mu_nhitsTrack[100];    // Vector of number of valid hits of muons
  float Reco_mu_caloComp[100];    // Vector of calorimeter compatibilities
  float Reco_mu_segmComp[100];    // Vector of muon segment compatibilities 
  float Reco_mu_iso[100];    // Vector of isolations (NOW ONLY SUMPt OF TRACKS) 
  int Reco_mu_nhitsStrip[100];  // Vectors of strip/pixel hits
  int Reco_mu_nhitsPixB[100];
  int Reco_mu_nhitsPixE[100];
  int Reco_mu_nhitsPix1Hit[100];
  int Reco_mu_nhitsPix1HitBE[100];


  // histos
  TH1F* hGoodMuonsNoTrig;
  TH1F* hGoodMuons;
  TH1F* hL1DoubleMuOpen;
  TH1F* hL2DoubleMu3;
  TH1F* hL2Mu20;

  MyCommonHistoManager* myRecoGlbMuonHistos;
  MyCommonHistoManager* myRecoTrkMuonHistos;

  MyCommonHistoManager* myRecoJpsiHistos;
  MyCommonHistoManager* myRecoJpsiGlbGlbHistos;
  MyCommonHistoManager* myRecoJpsiGlbTrkHistos;
  MyCommonHistoManager* myRecoJpsiTrkTrkHistos;

  // event counters
  TH1F* hStats;

  // centrality
  TH1F *hCent;

  // number of primary vertices
  TH1F* hPileUp;

  // z vertex distribution
  TH1F* hZVtx;

  // centrality
  CentralityProvider* centrality_;
  int centBin;
  int theCentralityBin;

  // handles
  edm::Handle<pat::CompositeCandidateCollection> collJpsi;
  edm::Handle<pat::MuonCollection> collMuon;
  edm::Handle<pat::MuonCollection> collMuonNoTrig;

  edm::Handle<reco::GenParticleCollection> collGenParticles;

  // data members
  edm::InputTag       _patMuon;
  edm::InputTag       _patMuonNoTrig;
  edm::InputTag       _patJpsi;
  edm::InputTag       _genParticle;
  edm::InputTag       _thePVs;
  std::string         _histfilename;
  std::string         _datasetname;

  std::vector<double> _ptbinranges;
  std::vector<double> _etabinranges;
  std::vector<double> _centralityranges;
  bool           _onlythebest;
  bool           _applycuts;
  bool           _storeefficiency;
  bool           _useBS;
  bool           _useRapidity;
  bool           _removeSignal;
  bool           _removeMuons;
  bool           _storeSs;
  bool           _combineCategories;
  bool           _fillRooDataSet;
  bool           _fillTree;
  bool           _theMinimumFlag;
  bool           _fillSingleMuons;
  bool           _isMC;
  bool           _isPromptMC;

  int _oniaPDG;

  std::vector<unsigned int>                     _thePassedCats[3];
  std::vector<const pat::CompositeCandidate*>   _thePassedCands[3];

  std::vector<uint32_t>   _thePassedPFPhotons;
  std::vector<uint32_t>   _thePassedPFPhotonsForPi0Rejection;

  // number of events
  unsigned int nEvents;
  unsigned int passedCandidates;

  unsigned int runNb;
  unsigned int eventNb;
  unsigned int lumiSection;

  // limits 
  float JpsiMassMin;
  float JpsiMassMax;
  float JpsiMassMinSide;
  float JpsiMassMaxSide;
  float JpsiCtMin;
  float JpsiCtMax;
  float JpsiPtMin;           // SET BY 
  float JpsiPtMax;           // DEFINITION
  float JpsiRapMin;          // OF BIN
  float JpsiRapMax;          // LIMITS 

  math::XYZPoint RefVtx;
  float zVtx;
  float nPV;

 // Triger stuff
  // PUT HERE THE *LAST FILTERS* OF THE BITS YOU LIKE
  static const unsigned int sNTRIGGERS = 10;
  unsigned int NTRIGGERS;
  // MC 8E29
  bool isTriggerMatched[sNTRIGGERS];
  std::string HLTLastFilters[sNTRIGGERS];
  bool alreadyFilled[sNTRIGGERS];
  int HLTriggers;

  const edm::ParameterSet _iConfig;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HiOniaAnalyzer::HiOniaAnalyzer(const edm::ParameterSet& iConfig):
  _patMuon(iConfig.getParameter<edm::InputTag>("srcMuon")),
  _patMuonNoTrig(iConfig.getParameter<edm::InputTag>("srcMuonNoTrig")),
  _patJpsi(iConfig.getParameter<edm::InputTag>("src")),
  _genParticle(iConfig.getParameter<edm::InputTag>("genParticles")),
  _thePVs(iConfig.getParameter<edm::InputTag>("primaryVertexTag")),
  _histfilename(iConfig.getParameter<std::string>("histFileName")),		
  _datasetname(iConfig.getParameter<std::string>("dataSetName")),		
  _ptbinranges(iConfig.getParameter< std::vector<double> >("pTBinRanges")),	
  _etabinranges(iConfig.getParameter< std::vector<double> >("etaBinRanges")),
  _centralityranges(iConfig.getParameter< std::vector<double> >("centralityRanges")),	
  _onlythebest(iConfig.getParameter<bool>("onlyTheBest")),		
  _applycuts(iConfig.getParameter<bool>("applyCuts")),			
  _storeefficiency(iConfig.getParameter<bool>("storeEfficiency")),	
  _useBS(iConfig.getParameter<bool>("useBeamSpot")),
  _useRapidity(iConfig.getParameter<bool>("useRapidity")),
  _removeSignal(iConfig.getUntrackedParameter<bool>("removeSignalEvents",false)),
  _removeMuons(iConfig.getUntrackedParameter<bool>("removeTrueMuons",false)),
  _storeSs(iConfig.getUntrackedParameter<bool>("storeSameSign",false)),
  _combineCategories(iConfig.getParameter<bool>("combineCategories")),
  _fillRooDataSet(iConfig.getParameter<bool>("fillRooDataSet")),  
  _fillTree(iConfig.getParameter<bool>("fillTree")),  
  _theMinimumFlag(iConfig.getParameter<bool>("minimumFlag")),  
  _fillSingleMuons(iConfig.getParameter<bool>("fillSingleMuons")),
  _isMC(iConfig.getUntrackedParameter<bool>("isMC",false) ),
  _isPromptMC(iConfig.getUntrackedParameter<bool>("isPromptMC",true) ),
  _oniaPDG(iConfig.getParameter<int>("oniaPDG")),
  NTRIGGERS(iConfig.getParameter<uint32_t>("NumberOfTriggers")),
  _iConfig(iConfig)
{
   //now do what ever initialization is needed
  nEvents = 0;
  passedCandidates = 0;
  centrality_ = 0;

  theRegions.push_back("All");
  theRegions.push_back("Barrel");
  theRegions.push_back("EndCap");

  std::stringstream centLabel;
  for (unsigned int iCent=0; iCent<_centralityranges.size(); ++iCent) {
    if (iCent==0)
      centLabel << "00" << _centralityranges.at(iCent);
    else
      centLabel << _centralityranges.at(iCent-1) << _centralityranges.at(iCent);

    theCentralities.push_back(centLabel.str());
    centLabel.str("");
  }
  theCentralities.push_back("MinBias");

  theSign.push_back("pm");
  if (_storeSs) {
    theSign.push_back("pp");
    theSign.push_back("mm");
  }

  isTriggerMatched[0]=true; // first entry 'hardcoded' true to accept "all" events
  for (unsigned int iTr = 1; iTr<NTRIGGERS; ++iTr) {
    isTriggerMatched[iTr] = false;
  }

  HLTLastFilters[0] = "";
  HLTLastFilters[1] = "hltHIDoubleMuLevel1PathL1OpenFiltered";  // BIT HLT_HIL1DoubleMuOpen
  HLTLastFilters[2] = "hltHIL2DoubleMu0L2Filtered";             // BIT HLT_HIL2DoubleMu0
  HLTLastFilters[3] = "hltHIL2DoubleMu3L2Filtered";             // BIT HLT_HIL2DoubleMu3
  HLTLastFilters[4] = "hltHIL2Mu20L2Filtered";                  // BIT HLT_HIL2Mu20
  HLTLastFilters[5] = "hltHIL2Mu3L2Filtered";                   // BIT HLT_HIL2Mu3
  HLTLastFilters[6] = "hltHIL2Mu5TightFiltered";                // BIT HLT_HIL2Mu5Tight
  // dummy names for now, there are no filters for these triggers, plain pass through
  // need to add a different matching for these
  HLTLastFilters[7] = "HLT_HIL1SingleMu3";                  // BIT HLT_HIL1SingleMu3
  HLTLastFilters[8] = "HLT_HIL1SingleMu5";                  // BIT HLT_HIL1SingleMu5
  HLTLastFilters[9] = "HLT_HIL1SingleMu7";                  // BIT HLT_HIL1SingleMu7

  theTriggerNames.push_back("NoTrigger");
  theTriggerNames.push_back("HLT_HIL1DoubleMuOpen");
  theTriggerNames.push_back("HLT_HIL2DoubleMu0");
  theTriggerNames.push_back("HLT_HIL2DoubleMu3");
  theTriggerNames.push_back("HLT_HIL2Mu20");
  theTriggerNames.push_back("HLT_HIL2Mu3");
  theTriggerNames.push_back("HLT_HIL2Mu5Tight");
  // not used yet
  theTriggerNames.push_back("HLT_HIL1SingleMu3");
  theTriggerNames.push_back("HLT_HIL1SingleMu5");
  theTriggerNames.push_back("HLT_HIL1SingleMu7");

  etaMax = 2.4;

  JpsiMassMin = 2.6;
  JpsiMassMax = 3.5;
  JpsiMassMinSide = 0.;
  JpsiMassMaxSide = 12.0;
  JpsiCtMin = -1.0;
  JpsiCtMax = 3.5;

  JpsiPtMin = _ptbinranges[0];
  std::cout << "Pt min = " << JpsiPtMin << std::endl;
  JpsiPtMax = _ptbinranges[_ptbinranges.size()-1];
  std::cout << "Pt max = " << JpsiPtMax << std::endl;

     
  JpsiRapMin = _etabinranges[0];
  std::cout << "Rap min = " << JpsiRapMin << std::endl;
  JpsiRapMax = _etabinranges[_etabinranges.size()-1];
  std::cout << "Rap max = " << JpsiRapMax << std::endl;
  
}


HiOniaAnalyzer::~HiOniaAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HiOniaAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //   using namespace edm;
  InitEvent();

  nEvents++;
  hStats->Fill(BIN_nEvents);
   
  runNb = iEvent.id().run();
  eventNb = iEvent.id().event();
  lumiSection = iEvent.luminosityBlock();
  
  edm::Handle<reco::VertexCollection> privtxs;
  iEvent.getByLabel(_thePVs, privtxs);
  reco::VertexCollection::const_iterator privtx;

  nPV = privtxs->size();
   
  if ( privtxs->begin() != privtxs->end() ) {
    privtx=privtxs->begin();
    RefVtx = privtx->position();
  } else {
    RefVtx.SetXYZ(0.,0.,0.);
  }

  zVtx = RefVtx.Z();

  hZVtx->Fill(zVtx);
  if (fabs(zVtx) > _iConfig.getParameter< double > ("maxAbsZ")) return;
  hPileUp->Fill(nPV);

  if(!centrality_) centrality_ = new CentralityProvider(iSetup);
  centrality_->newEvent(iEvent,iSetup); // make sure you do this first in every event
  centBin = centrality_->getBin();
  hCent->Fill(centBin);

  for (unsigned int iCent=0; iCent<_centralityranges.size(); ++iCent) {
    if (centBin<_centralityranges.at(iCent)/2.5) {
      theCentralityBin=iCent;
      break;
    }
  }

  iEvent.getByLabel(_patJpsi,collJpsi); 
  iEvent.getByLabel(_patMuon,collMuon);
  iEvent.getByLabel(_patMuonNoTrig,collMuonNoTrig);

  if (_isMC) {
    iEvent.getByLabel(_genParticle,collGenParticles);
    fillGenInfo();
  }
  
  // APPLY CUTS
  int lastSign = 0;
  makeCuts(0);
  if (_storeSs) {
    makeCuts(1);
    makeCuts(2);
    lastSign = 2;
  }

  if (_fillSingleMuons)
    fillRecoMuons(theCentralityBin);

  fillRecoHistos(lastSign);

  if (_fillTree)
    myTree->Fill();

  return;
}

void
HiOniaAnalyzer::fillRecoHistos(int lastSign) {

  // BEST J/PSI? 
  if (_onlythebest) {  // yes, fill simply the best (possibly same-sign)

    for (int iSign = 0; iSign <= lastSign; ++iSign) {
      pair< unsigned int, const pat::CompositeCandidate* > theBest = theBestQQ(iSign);
      if (theBest.first < 10) fillHistosAndDS(theBest.first, theBest.second);
    }

  } else {   // no, fill all candidates passing cuts (possibly same-sign)
   
    for (int iSign = 0; iSign <= lastSign; ++iSign) {
      for( unsigned int count = 0; count < _thePassedCands[iSign].size(); count++) { 
	const pat::CompositeCandidate* aJpsiCand = _thePassedCands[iSign].at(count); 

	checkTriggers(aJpsiCand);
	if (_fillTree)
	  fillTreeJpsi(iSign, count);

	for (unsigned int iTr=0; iTr<NTRIGGERS; ++iTr) {
	  if (isTriggerMatched[iTr]) {
	    fillRecoJpsi(iSign,count,theTriggerNames.at(iTr), theCentralities.at(theCentralityBin));
	  }
	}
      }
    }
  }

  return;
}

void
HiOniaAnalyzer::fillTreeMuon(const pat::Muon* muon, int iType, int trigBits) {
  if (Reco_mu_size >= Max_mu_size) {
    std::cout << "Too many muons: " << Reco_mu_size << std::endl;
    std::cout << "Maximum allowed: " << Max_mu_size << std::endl;
    return;
  }

  Reco_mu_charge[Reco_mu_size] = muon->charge();
  Reco_mu_type[Reco_mu_size] = iType;
  
  TLorentzVector vMuon = lorentzMomentum(muon->p4());
  new((*Reco_mu_4mom)[Reco_mu_size])TLorentzVector(vMuon);

  Reco_mu_trig[Reco_mu_size] = trigBits;

  Reco_mu_size++;
  return;
}

void
HiOniaAnalyzer::fillTreeJpsi(int iSign, int count) {
  if (Reco_QQ_size >= Max_QQ_size) {
    std::cout << "Too many dimuons: " << Reco_QQ_size << std::endl;
    std::cout << "Maximum allowed: " << Max_QQ_size << std::endl;
    return;
  }

  const pat::CompositeCandidate* aJpsiCand = _thePassedCands[iSign].at(count);
  const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon1"));
  const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon2"));


  int trigBits=0;
  for (unsigned int iTr=1; iTr<NTRIGGERS; ++iTr) {
    if (isTriggerMatched[iTr])
      trigBits += pow(2,iTr-1);
  }


  Reco_QQ_sign[Reco_QQ_size] = iSign;
  Reco_QQ_type[Reco_QQ_size] = _thePassedCats[iSign].at(count);

  Reco_QQ_trig[Reco_QQ_size] = trigBits;

  TLorentzVector vMuon1 = lorentzMomentum(muon1->p4());
  TLorentzVector vMuon2 = lorentzMomentum(muon2->p4());

  if (muon1->charge() > muon2->charge()) {
    new((*Reco_QQ_mupl_4mom)[Reco_QQ_size])TLorentzVector(vMuon1);
    new((*Reco_QQ_mumi_4mom)[Reco_QQ_size])TLorentzVector(vMuon2);
  }
  else {
    new((*Reco_QQ_mupl_4mom)[Reco_QQ_size])TLorentzVector(vMuon2);
    new((*Reco_QQ_mumi_4mom)[Reco_QQ_size])TLorentzVector(vMuon1);
  }
  
  TLorentzVector vJpsi = lorentzMomentum(aJpsiCand->p4());
  new((*Reco_QQ_4mom)[Reco_QQ_size])TLorentzVector(vJpsi);

  if (_useBS) {
    Reco_QQ_ctau[Reco_QQ_size] = 10.0*aJpsiCand->userFloat("ppdlBS");
    Reco_QQ_ctauErr[Reco_QQ_size] = 10.0*aJpsiCand->userFloat("ppdlErrBS");
  }
  else {
    Reco_QQ_ctau[Reco_QQ_size] = 10.0*aJpsiCand->userFloat("ppdlPV");
    Reco_QQ_ctauErr[Reco_QQ_size] = 10.0*aJpsiCand->userFloat("ppdlErrPV");
  }
  Reco_QQ_ctauTrue[Reco_QQ_size] = 10.*aJpsiCand->userFloat("ppdlTrue");

  Reco_QQ_VtxProb[Reco_QQ_size] = aJpsiCand->userFloat("vProb");

  Reco_QQ_size++;
  return;
}

void
HiOniaAnalyzer::fillRecoJpsi(int iSign, int count, std::string trigName, std::string centName) {
  pat::CompositeCandidate* aJpsiCand = _thePassedCands[iSign].at(count)->clone();
  aJpsiCand->addUserInt("centBin",centBin);

  std::string theLabel =  trigName + "_" + centName + "_" + theSign.at(iSign);

  bool isBarrel = false;
  if ( fabs(aJpsiCand->rapidity()) < 1.2) isBarrel = true;

  double theCtau;
  float theCtauErr; 

  if (_useBS) {
    theCtau = 10.0*aJpsiCand->userFloat("ppdlBS");
    theCtauErr = 10.*aJpsiCand->userFloat("ppdlErrBS");
  }
  else {
    theCtau = 10.0*aJpsiCand->userFloat("ppdlPV");
    theCtauErr = 10.*aJpsiCand->userFloat("ppdlErrPV");
  }

  if (iSign==0 &&
      aJpsiCand->mass() >= JpsiMassMin && aJpsiCand->mass() < JpsiMassMax && 
      theCtau >= JpsiCtMin && theCtau < JpsiCtMax && 
      aJpsiCand->pt() >= JpsiPtMin && aJpsiCand->pt() < JpsiPtMax && 
      fabs(aJpsiCand->rapidity()) >= JpsiRapMin && fabs(aJpsiCand->rapidity()) < JpsiRapMax) {
    passedCandidates++;
  }

  if (_combineCategories && _thePassedCats[iSign].at(count)<=GlbGlb) { // for the moment consider Glb+Glb only
    myRecoJpsiHistos->Fill(aJpsiCand, "All_"+ theLabel);
    if (isBarrel)
      myRecoJpsiHistos->Fill(aJpsiCand, "Barrel_"+ theLabel);
    else
      myRecoJpsiHistos->Fill(aJpsiCand, "EndCap_"+ theLabel);
  }
  else {
    switch (_thePassedCats[iSign].at(count)) {
    case GlbGlb:
      myRecoJpsiGlbGlbHistos->Fill(aJpsiCand, "All_"+ theLabel);
      if (isBarrel)
	myRecoJpsiGlbGlbHistos->Fill(aJpsiCand, "Barrel_"+ theLabel);
      else
	myRecoJpsiGlbGlbHistos->Fill(aJpsiCand, "EndCap_"+ theLabel);
      break;
    case GlbTrk:
      myRecoJpsiGlbTrkHistos->Fill(aJpsiCand, "All_"+ theLabel);
      if (isBarrel)
	myRecoJpsiGlbTrkHistos->Fill(aJpsiCand, "Barrel_"+ theLabel);
      else
	myRecoJpsiGlbTrkHistos->Fill(aJpsiCand, "EndCap_"+ theLabel);
      break;
    case TrkTrk:
      myRecoJpsiTrkTrkHistos->Fill(aJpsiCand, "All_"+ theLabel);
      if (isBarrel)
	myRecoJpsiTrkTrkHistos->Fill(aJpsiCand, "Barrel_"+ theLabel);
      else
	myRecoJpsiTrkTrkHistos->Fill(aJpsiCand, "EndCap_"+ theLabel);
      break;
    default:
      break;
    }
  }

  fillHistosAndDS(_thePassedCats[iSign].at(count), aJpsiCand); 

  return;
}

void
HiOniaAnalyzer::fillHistosAndDS(unsigned int theCat, const pat::CompositeCandidate* aJpsiCand) {

  return;
}

void
HiOniaAnalyzer::checkTriggers(const pat::CompositeCandidate* aJpsiCand) {
  const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon1"));
  const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon2"));

  // Trigger passed
  for (unsigned int iTr = 1; iTr<NTRIGGERS; ++iTr) {
    const pat::TriggerObjectStandAloneCollection mu1HLTMatches = muon1->triggerObjectMatchesByFilter( HLTLastFilters[iTr] );
    const pat::TriggerObjectStandAloneCollection mu2HLTMatches = muon2->triggerObjectMatchesByFilter( HLTLastFilters[iTr] );

    if (iTr>6) {
      const pat::TriggerObjectStandAloneCollection mu1HLTMatches = muon1->triggerObjectMatchesByPath( HLTLastFilters[iTr] );
      const pat::TriggerObjectStandAloneCollection mu2HLTMatches = muon2->triggerObjectMatchesByPath( HLTLastFilters[iTr] );
    }

    bool pass1 = mu1HLTMatches.size() > 0;
    bool pass2 = mu2HLTMatches.size() > 0;
    if (iTr > 4) {  // single triggers here
      isTriggerMatched[iTr] = pass1 || pass2;
    } else {        // double triggers here
      isTriggerMatched[iTr] = pass1 && pass2;
    }
  }

  for (unsigned int iTr=1;iTr<NTRIGGERS;++iTr) {
    if (isTriggerMatched[iTr]) {
      // fill event counting histogram only once per event, also if several muons fired trigger
      if (alreadyFilled[iTr]) continue;
      hStats->Fill(iTr);
      HLTriggers += pow(2,iTr-1);
      alreadyFilled[iTr]=true;
    }
  }

  return;
}

void
HiOniaAnalyzer::makeCuts(int sign) {

  if (collJpsi.isValid()) {
    for(std::vector<pat::CompositeCandidate>::const_iterator it=collJpsi->begin();
	it!=collJpsi->end(); ++it) {
      
      const pat::CompositeCandidate* cand = &(*it);	
      if (fabs(cand->rapidity()) >= etaMax) continue;

      const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
      const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));

      if (fabs(muon1->rapidity()) >= etaMax ||
	  fabs(muon2->rapidity()) >= etaMax) continue;
      
      bool thisSign = ( (sign == 0 && muon1->charge() + muon2->charge() == 0) || 
			(sign == 1 && muon1->charge() + muon2->charge() == 2) || 
			(sign == 2 && muon1->charge() + muon2->charge() == -2) );

      if (thisSign) {
	
	// global + global?
	if (checkCuts(cand,muon1,muon2,&HiOniaAnalyzer::selGlobalMuon,&HiOniaAnalyzer::selGlobalMuon)){
	  _thePassedCats[sign].push_back(GlbGlb);  _thePassedCands[sign].push_back(cand);
	  continue;
	}
	// for the moment consider only Glb+Glb pairs
	/*
	// global + tracker? (x2)    
	if (checkCuts(cand,muon1,muon2,&HiOniaAnalyzer::selGlobalMuon,&HiOniaAnalyzer::selTrackerMuon)){
	  _thePassedCats[sign].push_back(GlbTrk);  _thePassedCands[sign].push_back(cand);
	  continue;
	}

	if (checkCuts(cand,muon2,muon1,&HiOniaAnalyzer::selGlobalMuon,&HiOniaAnalyzer::selTrackerMuon)){
	  _thePassedCats[sign].push_back(GlbTrk);  _thePassedCands[sign].push_back(cand);
	  continue;
	}

	// tracker + tracker?  
	if (checkCuts(cand,muon1,muon2,&HiOniaAnalyzer::selTrackerMuon,&HiOniaAnalyzer::selTrackerMuon)){
	  _thePassedCats[sign].push_back(TrkTrk);  _thePassedCands[sign].push_back(cand);
	  continue;
	}
	*/
      }
    }
  }
  
  return;
}


bool
HiOniaAnalyzer::checkCuts(const pat::CompositeCandidate* cand, const pat::Muon* muon1,  const pat::Muon* muon2, bool(HiOniaAnalyzer::* callFunc1)(const pat::Muon*), bool(HiOniaAnalyzer::* callFunc2)(const pat::Muon*)) {
  if ( (  (this->*callFunc1)(muon1) &&  (this->*callFunc2)(muon2) ) &&
       (!_applycuts || cand->userFloat("vProb") > 0.01) )
    return true;
  else
    return false;
}


pair< unsigned int, const pat::CompositeCandidate* > 
HiOniaAnalyzer::theBestQQ(int sign) {

  unsigned int theBestCat = 99;
  const pat::CompositeCandidate* theBestCand = new pat::CompositeCandidate();

  for( unsigned int i = 0; i < _thePassedCands[sign].size(); i++) { 
    if (_thePassedCats[sign].at(i) < theBestCat) {
      theBestCat = _thePassedCats[sign].at(i);
      theBestCand = _thePassedCands[sign].at(i);
    }
  }

  pair< unsigned int, const pat::CompositeCandidate* > result = make_pair(theBestCat, theBestCand );
  return result;
}

bool
HiOniaAnalyzer::isMuonInAccept(const pat::Muon* aMuon) {
  return (fabs(aMuon->eta()) < 2.4 &&
	  ((fabs(aMuon->eta()) < 1.0 && aMuon->pt() >= 3.4) ||
	   (1.0 <= fabs(aMuon->eta()) && fabs(aMuon->eta()) < 1.5 && aMuon->pt() >= 5.8-2.4*fabs(aMuon->eta())) ||
	   (1.5 <= fabs(aMuon->eta()) && aMuon->pt() >= 3.3667-7.0/9.0*fabs(aMuon->eta()))));
}

bool
HiOniaAnalyzer::selGlobalMuon(const pat::Muon* aMuon) {
  
  if(!aMuon->isGlobalMuon())
    return false;

  if(!aMuon->isTrackerMuon())
    return false;
  
  if(!_applycuts)
    return true;

  reco::TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  reco::TrackRef gTrack = aMuon->globalTrack();
  const reco::HitPattern& q = gTrack->hitPattern();
  /* Z analysis cuts
  return (isMuonInAccept(aMuon) &&
	  iTrack->found() > 10 &&
	  gTrack->chi2()/gTrack->ndof() < 10.0 &&
	  q.numberOfValidMuonHits() > 0 &&
	  iTrack->chi2()/iTrack->ndof() < 4.0 &&
	  iTrack->ptError()/iTrack->pt() <= 0.1 &&
	  fabs(iTrack->dxy(RefVtx)) < 0.03 &&
	  fabs(iTrack->dz(RefVtx)) < 0.150 );
  */
  // J/psi tuned as of 2011-03-18
  return (isMuonInAccept(aMuon) &&
	  iTrack->found() > 10 &&
	  gTrack->chi2()/gTrack->ndof() < 6.0 &&
	  q.numberOfValidMuonHits() > 6 &&
	  iTrack->chi2()/iTrack->ndof() < 4.0 &&
 	  aMuon->muonID("TrackerMuonArbitrated") &&
// 	  aMuon->muonID("TMLastStationAngTight") &&
	  p.pixelLayersWithMeasurement() > 0 &&
	  fabs(iTrack->dxy(RefVtx)) < 3.0 &&
	  fabs(iTrack->dz(RefVtx)) < 15.0 );
}


bool 
HiOniaAnalyzer::selTrackerMuon(const pat::Muon* aMuon) {
  
  if(!aMuon->isTrackerMuon())
    return false;

  if(!_applycuts)
    return true;

  reco::TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  return (isMuonInAccept(aMuon) &&
	  iTrack->found() > 11 &&
	  iTrack->chi2()/iTrack->ndof() < 4.0 &&
	  aMuon->muonID("TrackerMuonArbitrated") &&
	  aMuon->muonID("TMLastStationAngTight") &&
	  p.pixelLayersWithMeasurement() > 1 &&
	  fabs(iTrack->dxy(RefVtx)) < 3.0 &&
	  fabs(iTrack->dz(RefVtx)) < 15.0 );
}


void
HiOniaAnalyzer::InitEvent()
{
  for (unsigned int iTr=1;iTr<NTRIGGERS;++iTr) {
    alreadyFilled[iTr]=false;
  }
  HLTriggers = 0;

  _thePassedCats[0].clear();      _thePassedCands[0].clear();
  _thePassedCats[1].clear();      _thePassedCands[1].clear();
  _thePassedCats[2].clear();      _thePassedCands[2].clear();

  Reco_QQ_size = 0;
  Reco_mu_size = 0;

  Gen_QQ_size = 0;
  Gen_mu_size = 0;

  Reco_QQ_4mom->Clear();
  Reco_QQ_mupl_4mom->Clear();
  Reco_QQ_mumi_4mom->Clear();
  Reco_mu_4mom->Clear();
  Reco_mu_3vec->Clear();

  if (_isMC) {
    Gen_QQ_4mom->Clear();
    Gen_QQ_mupl_4mom->Clear();
    Gen_QQ_mumi_4mom->Clear();
    Gen_mu_4mom->Clear();
    Gen_mu_3vec->Clear();
  }

  return;
}



void
HiOniaAnalyzer::fillGenInfo()
{
  if (Gen_QQ_size >= Max_QQ_size) {
    std::cout << "Too many dimuons: " << Gen_QQ_size << std::endl;
    std::cout << "Maximum allowed: " << Max_QQ_size << std::endl;
    return;
  }

  if (Gen_mu_size >= Max_mu_size) {
    std::cout << "Too many muons: " << Gen_mu_size << std::endl;
    std::cout << "Maximum allowed: " << Max_mu_size << std::endl;
    return;
  }
  
  if (collGenParticles.isValid()) {
    for(std::vector<reco::GenParticle>::const_iterator it=collGenParticles->begin();
	it!=collGenParticles->end();++it) {
      const reco::GenParticle* gen = &(*it);

      if (abs(gen->pdgId()) == _oniaPDG  && gen->status() == 2) {
	Gen_QQ_type[Gen_QQ_size] = _isPromptMC ? 0 : 1; // prompt: 0, non-prompt: 1
	
	TLorentzVector vJpsi = lorentzMomentum(gen->p4());
	new((*Gen_QQ_4mom)[Gen_QQ_size])TLorentzVector(vJpsi);


	if (gen->numberOfDaughters() == 2) {
	  const reco::Candidate* genMuon1 = gen->daughter(0);
	  const reco::Candidate* genMuon2 = gen->daughter(1);
	  if ( abs(genMuon1->pdgId()) == 13 &&
	       abs(genMuon2->pdgId()) == 13 &&
	       genMuon1->status() == 1 &&
	       genMuon2->status() == 1 ) {

	    TLorentzVector vMuon1 = lorentzMomentum(genMuon1->p4());
	    TLorentzVector vMuon2 = lorentzMomentum(genMuon2->p4());
	    
	    if (genMuon1->charge() > genMuon2->charge()) {
	      new((*Gen_QQ_mupl_4mom)[Gen_QQ_size])TLorentzVector(vMuon1);
	      new((*Gen_QQ_mumi_4mom)[Gen_QQ_size])TLorentzVector(vMuon2);
	    }
	    else {
	      new((*Gen_QQ_mupl_4mom)[Gen_QQ_size])TLorentzVector(vMuon2);
	      new((*Gen_QQ_mumi_4mom)[Gen_QQ_size])TLorentzVector(vMuon1);
	    }
	  }
	}
	Gen_QQ_size++;
      }

      if (abs(gen->pdgId()) == 13  && gen->status() == 1) {
	Gen_mu_type[Gen_mu_size] = _isPromptMC ? 0 : 1; // prompt: 0, non-prompt: 1
	Gen_mu_charge[Gen_mu_size] = gen->charge();

	TLorentzVector vMuon = lorentzMomentum(gen->p4());
	new((*Gen_mu_4mom)[Gen_mu_size])TLorentzVector(vMuon);

	Gen_mu_size++;
      }

    }
  }
  
  return;
}

void
HiOniaAnalyzer::fillRecoMuons(int iCent)
{
  int nL1DoubleMuOpenMuons=0;
  int nL2DoubleMu3Muons=0;
  int nL2Mu20Muons=0;
  int nGoodMuons=0;
  int nGoodMuonsNoTrig=0;

  if (collMuonNoTrig.isValid()) {
    for(std::vector<pat::Muon>::const_iterator it=collMuonNoTrig->begin();
	it!=collMuonNoTrig->end();++it) {
      const pat::Muon* muon = &(*it);

      if (muon->isGlobalMuon() &&
	  selGlobalMuon(muon))
	nGoodMuonsNoTrig++;
    }
  }

  if (collMuon.isValid()) {
    for(vector<pat::Muon>::const_iterator it=collMuon->begin();
	it!=collMuon->end();++it) {
      const pat::Muon* muon = &(*it);

      bool isBarrel = false;
      if ( fabs(muon->eta() < 1.2) ) isBarrel = true;

      if (muon->isGlobalMuon() &&
	  selGlobalMuon(muon)) {
	std::string theLabel = theTriggerNames.at(0) + "_" + theCentralities.at(iCent);

	myRecoGlbMuonHistos->Fill(muon, "All_"+theLabel);
	if (isBarrel)
	  myRecoGlbMuonHistos->Fill(muon, "Barrel_"+theLabel);
	else
	  myRecoGlbMuonHistos->Fill(muon, "EndCap_"+theLabel);
	
	nGoodMuons++;

	int trigBits=0;
	for (unsigned int iTr=1; iTr<NTRIGGERS; ++iTr) {
	  const pat::TriggerObjectStandAloneCollection muHLTMatches = muon->triggerObjectMatchesByFilter(  HLTLastFilters[iTr] );

	  if (muHLTMatches.size() > 0) {
	    std::string theLabel = theTriggerNames.at(iTr) + "_" + theCentralities.at(iCent);

	    myRecoGlbMuonHistos->Fill(muon, "All_"+theLabel);
	    if (isBarrel)
	      myRecoGlbMuonHistos->Fill(muon, "Barrel_"+theLabel);
	    else
	      myRecoGlbMuonHistos->Fill(muon, "EndCap_"+theLabel);

	    trigBits += pow(2,iTr-1);

	    if (iTr==1) nL1DoubleMuOpenMuons++;
	    if (iTr==3) nL2DoubleMu3Muons++;
	    if (iTr==4) nL2Mu20Muons++;
	  }
	}
	if (_fillTree)
	  fillTreeMuon(muon, 0, trigBits);
      }
    }
  }
  
  hGoodMuonsNoTrig->Fill(nGoodMuonsNoTrig);
  hGoodMuons->Fill(nGoodMuons);
  hL1DoubleMuOpen->Fill(nL1DoubleMuOpenMuons);
  hL2DoubleMu3->Fill(nL2DoubleMu3Muons);
  hL2Mu20->Fill(nL2Mu20Muons);

  return;
}

void
HiOniaAnalyzer::InitTree()
{
  Reco_mu_4mom = new TClonesArray("TLorentzVector", 100);
  Reco_mu_3vec = new TClonesArray("TVector3", 100);
  Reco_QQ_4mom = new TClonesArray("TLorentzVector",10);
  Reco_QQ_mupl_4mom = new TClonesArray("TLorentzVector",10);
  Reco_QQ_mumi_4mom = new TClonesArray("TLorentzVector",10);

  if (_isMC) {
    Gen_mu_4mom = new TClonesArray("TLorentzVector", 2);
    Gen_mu_3vec = new TClonesArray("TVector3", 2);
    Gen_QQ_4mom = new TClonesArray("TLorentzVector", 2);
    Gen_QQ_mupl_4mom = new TClonesArray("TLorentzVector", 2);
    Gen_QQ_mumi_4mom = new TClonesArray("TLorentzVector", 2);
  }

  myTree = new TTree("myTree","My TTree of dimuons");
  
  myTree->Branch("eventNb", &eventNb,   "eventNb/i");
  myTree->Branch("runNb",   &runNb,     "runNb/i");
  myTree->Branch("LS",      &lumiSection, "LS/i"); 
  myTree->Branch("zVtx",    &zVtx,        "zVtx/F"); 
  myTree->Branch("HLTriggers", &HLTriggers, "HLTriggers/I");
  myTree->Branch("Centrality", &centBin, "Centrality/I");

  myTree->Branch("Reco_QQ_size", &Reco_QQ_size,  "Reco_QQ_size/I");
  myTree->Branch("Reco_QQ_type", Reco_QQ_type,   "Reco_QQ_type[Reco_QQ_size]/I");
  myTree->Branch("Reco_QQ_sign", Reco_QQ_sign,   "Reco_QQ_sign[Reco_QQ_size]/I");
  myTree->Branch("Reco_QQ_4mom", "TClonesArray", &Reco_QQ_4mom, 32000, 0);
  myTree->Branch("Reco_QQ_mupl_4mom", "TClonesArray", &Reco_QQ_mupl_4mom, 32000, 0);
  myTree->Branch("Reco_QQ_mumi_4mom", "TClonesArray", &Reco_QQ_mumi_4mom, 32000, 0);
  myTree->Branch("Reco_QQ_trig", Reco_QQ_trig,   "Reco_QQ_trig[Reco_QQ_size]/I");
  myTree->Branch("Reco_QQ_ctau", Reco_QQ_ctau,   "Reco_QQ_ctau[Reco_QQ_size]/F");
  myTree->Branch("Reco_QQ_ctauErr", Reco_QQ_ctauErr,   "Reco_QQ_ctauErr[Reco_QQ_size]/F");
  myTree->Branch("Reco_QQ_ctauTrue", Reco_QQ_ctauTrue,   "Reco_QQ_ctauTrue[Reco_QQ_size]/F");
  myTree->Branch("Reco_QQ_VtxProb", Reco_QQ_VtxProb,   "Reco_QQ_VtxProb[Reco_QQ_size]/F");

  myTree->Branch("Reco_mu_size", &Reco_mu_size,  "Reco_mu_size/I");
  myTree->Branch("Reco_mu_type", Reco_mu_type,   "Reco_mu_type[Reco_mu_size]/I");
  myTree->Branch("Reco_mu_charge", Reco_mu_charge,   "Reco_mu_charge[Reco_mu_size]/I");
  myTree->Branch("Reco_mu_4mom", "TClonesArray", &Reco_mu_4mom, 32000, 0);
  //  myTree->Branch("Reco_mu_3vec", "TClonesArray", &Reco_mu_3vec, 32000, 0);
  myTree->Branch("Reco_mu_trig", Reco_mu_trig,   "Reco_mu_trig[Reco_mu_size]/I");

  if (_isMC) {
    myTree->Branch("Gen_QQ_size",      &Gen_QQ_size,    "Gen_QQ_size/I");
    myTree->Branch("Gen_QQ_type",      Gen_QQ_type,    "Gen_QQ_type[Gen_QQ_size]/I");
    myTree->Branch("Gen_QQ_4mom",      "TClonesArray", &Gen_QQ_4mom, 32000, 0);
    //    myTree->Branch("Gen_QQ_ctau",      Gen_QQ_ctau,    "Gen_QQ_ctau[Gen_QQ_size]/F");
    myTree->Branch("Gen_QQ_mupl_4mom", "TClonesArray", &Gen_QQ_mupl_4mom, 32000, 0);
    myTree->Branch("Gen_QQ_mumi_4mom", "TClonesArray", &Gen_QQ_mumi_4mom, 32000, 0);

    myTree->Branch("Gen_mu_size",   &Gen_mu_size,  "Gen_mu_size/I");
    myTree->Branch("Gen_mu_type",   Gen_mu_type,   "Gen_mu_type[Gen_mu_size]/I");
    myTree->Branch("Gen_mu_charge", Gen_mu_charge, "Gen_mu_charge[Gen_mu_size]/I");
    myTree->Branch("Gen_mu_4mom",   "TClonesArray", &Gen_mu_4mom, 32000, 0);
    //    myTree->Branch("Gen_mu_3vec",   "TClonesArray", &Gen_QQ_mumi_4mom, 32000, 0);
  }

  if (!_theMinimumFlag) {
    myTree->Branch("Reco_mu_phiErr",   Reco_mu_phiErr,  "Reco_mu_phiErr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_etaErr",   Reco_mu_etaErr,  "Reco_mu_etaErr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_ptErr",    Reco_mu_ptErr,   "Reco_mu_ptErr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_d0",       Reco_mu_d0,      "Reco_mu_d0[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_d0err",    Reco_mu_d0err,   "Reco_mu_d0err[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dz",       Reco_mu_dz,      "Reco_mu_dz[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dzerr",    Reco_mu_dzerr,   "Reco_mu_dzerr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_normChi2",     Reco_mu_normChi2,    "Reco_mu_normChi2[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_nhitsTrack",    Reco_mu_nhitsTrack,   "Reco_mu_nhitsTrack[Reco_mu_size]/I");      
    myTree->Branch("Reco_mu_nhitsStrip",    Reco_mu_nhitsStrip,   "Reco_mu_nhitsStrip[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nhitsPixB",    Reco_mu_nhitsPixB,   "Reco_mu_nhitsPixB[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nhitsPixE",    Reco_mu_nhitsPixE,   "Reco_mu_nhitsPixE[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nhitsPix1Hit",    Reco_mu_nhitsPix1Hit,   "Reco_mu_nhitsPix1Hit[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nhitsPix1HitBE",    Reco_mu_nhitsPix1HitBE,   "Reco_mu_nhitsPix1HitBE[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nhitsDT",    Reco_mu_nhitsDT,   "Reco_mu_nhitsDT[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nhitsCSC",    Reco_mu_nhitsCSC,   "Reco_mu_nhitsCSC[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_caloComp",   Reco_mu_caloComp,  "Reco_mu_caloComp[Reco_mu_size]/F"); 
    myTree->Branch("Reco_mu_segmComp",   Reco_mu_segmComp,  "Reco_mu_segmComp[Reco_mu_size]/F"); 
    myTree->Branch("Reco_mu_iso",   Reco_mu_iso,  "Reco_mu_iso[Reco_mu_size]/F");  
  }


}

// ------------ method called once each job just before starting event loop  ------------
void 
HiOniaAnalyzer::beginJob()
{
  fOut = new TFile(_histfilename.c_str(), "RECREATE");
  InitTree();

  // book histos
  hGoodMuonsNoTrig = new TH1F("hGoodMuonsNoTrig","hGoodMuonsNoTrig",10,0,10);
  hGoodMuons = new TH1F("hGoodMuons","hGoodMuons",10,0,10);
  hL1DoubleMuOpen = new TH1F("hL1DoubleMuOpen","hL1DoubleMuOpen",10,0,10);
  hL2DoubleMu3    = new TH1F("hL1DoubleMu3","hL2DoubleMu3",10,0,10);
  hL2Mu20         = new TH1F("hL2Mu20","hL1Mu20",10,0,10);
  
  hGoodMuonsNoTrig->Sumw2();
  hGoodMuons->Sumw2();
  hL1DoubleMuOpen->Sumw2();
  hL2DoubleMu3->Sumw2();
  hL2Mu20->Sumw2();

  // muons
  myRecoGlbMuonHistos = new MyCommonHistoManager("GlobalMuon");
  //  myRecoTrkMuonHistos = new MyCommonHistoManager("TrackerMuon");

  // J/psi
  if (_combineCategories) {
    myRecoJpsiHistos = new MyCommonHistoManager("RecoJpsi");
  }
  else {
    myRecoJpsiGlbGlbHistos = new MyCommonHistoManager("GlbGlbJpsi");
    myRecoJpsiGlbTrkHistos = new MyCommonHistoManager("GlbTrkJpsi");
    myRecoJpsiTrkTrkHistos = new MyCommonHistoManager("TrkTrkJpsi");
  }
  
  for (unsigned int i=0; i<theRegions.size(); ++i) {
    for (unsigned int j=0; j<NTRIGGERS; ++j) {
      for (unsigned int k=0; k<theCentralities.size(); ++k) {

	std::string theAppendix = theRegions.at(i) ;
	theAppendix += "_" + theTriggerNames.at(j);
	theAppendix += "_" + theCentralities.at(k);

	// muons
	myRecoGlbMuonHistos->Add(theAppendix);
	//	myRecoTrkMuonHistos->Add(theAppendix);
	
	myRecoGlbMuonHistos->GetHistograms(theAppendix)->SetMassBinning(1,0.10,0.11);
	myRecoGlbMuonHistos->GetHistograms(theAppendix)->SetPtBinning(200,0.0,100.0);

	for (unsigned int l=0; l<theSign.size(); ++l) {
	  // J/psi
	  if (_combineCategories) {
	    myRecoJpsiHistos->Add(theAppendix + "_" + theSign.at(l));
	  }
	  else {
	    myRecoJpsiGlbGlbHistos->Add(theAppendix + "_" + theSign.at(l));
	    myRecoJpsiGlbTrkHistos->Add(theAppendix + "_" + theSign.at(l));
	    myRecoJpsiTrkTrkHistos->Add(theAppendix + "_" + theSign.at(l));
	  }
	}
      }
    }
  }

  myRecoGlbMuonHistos->Print();

  hStats = new TH1F("hStats","hStats;;Number of Events",20,0,20);
  hStats->GetXaxis()->SetBinLabel(1,"All");
  hStats->GetXaxis()->SetBinLabel(2,"HLT_HIL1DoubleMuOpen");
  hStats->GetXaxis()->SetBinLabel(3,"HLT_HIL2DoubleMu0");
  hStats->GetXaxis()->SetBinLabel(4,"HLT_HIL2DoubleMu3");
  hStats->GetXaxis()->SetBinLabel(4,"HLT_HIL2Mu20");
  hStats->GetXaxis()->SetBinLabel(4,"HLT_HIL2Mu3");
  hStats->GetXaxis()->SetBinLabel(4,"HLT_HIL2Mu5Tight");
  hStats->Sumw2();

  hCent = new TH1F("hCent","hCent;centrality bin;Number of Events",40,0,40);
  hCent->Sumw2();

  hPileUp = new TH1F("hPileUp","Number of Primary Vertices;n_{PV};counts", 50, 0, 50);
  hPileUp->Sumw2();

  hZVtx = new TH1F("hZVtx","Primary z-vertex distribution;z_{vtx} [cm];counts", 120, -30, 30);
  hZVtx->Sumw2();

  return;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HiOniaAnalyzer::endJob() {
  std::cout << "Total number of events = " << nEvents << std::endl;
  std::cout << "Total number of passed candidates = " << passedCandidates << std::endl;

  fOut->cd();
  hStats->Write();
  hCent->Write();
  hPileUp->Write();
  hZVtx->Write();

  if (_fillTree)
    myTree->Write();

  hGoodMuonsNoTrig->Write();
  hGoodMuons->Write();
  hL1DoubleMuOpen->Write();
  hL2DoubleMu3->Write();
  hL2Mu20->Write();

  // muons
  myRecoGlbMuonHistos->Write(fOut);
  //  myRecoTrkMuonHistos->Write(fOut);

  // J/psi
  if (_combineCategories) {
    myRecoJpsiHistos->Write(fOut);
  }
  else {
    myRecoJpsiGlbGlbHistos->Write(fOut);
    myRecoJpsiGlbTrkHistos->Write(fOut);
    myRecoJpsiTrkTrkHistos->Write(fOut);
  }


  return;
}

TLorentzVector
HiOniaAnalyzer::lorentzMomentum(const reco::Candidate::LorentzVector& p) {
  TLorentzVector res;
  res.SetPtEtaPhiM(p.pt(), p.eta(), p.phi(), p.mass());

  return res;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiOniaAnalyzer);
