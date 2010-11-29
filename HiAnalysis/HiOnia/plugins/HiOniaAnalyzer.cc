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
// $Id: HiOniaAnalyzer.cc,v 1.1 2010/11/29 16:50:30 tdahms Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <vector>
#include <utility>

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
  
  
  void Init();

  void makeCuts(int sign) ;
  bool checkCuts(const pat::CompositeCandidate* cand, const pat::Muon* muon1,  const pat::Muon* muon2, bool(HiOniaAnalyzer::* callFunc1)(const pat::Muon*), bool(HiOniaAnalyzer::* callFunc2)(const pat::Muon*)); 

  void fillRecoMuons(int theCentralityBin);
  bool isMuonInAccept(const pat::Muon* aMuon);

  pair< unsigned int, const pat::CompositeCandidate* > theBestQQ(int sign);
  double CorrectMass(const reco::Muon& mu1,const reco::Muon& mu2, int mode);

  bool selGlobalMuon(const pat::Muon* aMuon);
  bool selTrackerMuon(const pat::Muon* aMuon);

  void fillRecoHistos(int lastSign);
  void fillRecoJpsi(int iSign, int count, std::string trigName, std::string centName);
  void fillHistosAndDS(unsigned int theCat, const pat::CompositeCandidate* aJpsiCand);
  void checkTriggers(const pat::CompositeCandidate* aJpsiCand);

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

  // histos
  TH1F *hGoodMuonsNoTrig;
  TH1F *hGoodMuons;
  TH1F *hL1DoubleMuOpen;
  TH1F *hL2DoubleMu3;
  TH1F *hL2Mu20;

  MyCommonHistoManager *myRecoGlbMuonHistos;
  MyCommonHistoManager *myRecoTrkMuonHistos;
  MyCommonHistoManager *myRecoCalMuonHistos;

  MyCommonHistoManager *myRecoJpsiHistos;
  MyCommonHistoManager *myRecoJpsiGlbGlbHistos;
  MyCommonHistoManager *myRecoJpsiGlbTrkHistos;
  MyCommonHistoManager *myRecoJpsiTrkTrkHistos;

  // event counters
  TH1F *hStats;

  // number of primary vertices
  TH1F *hPileUp;

  // z vertex distribution
  TH1F *hZVtx;

  // centrality
  CentralityProvider * centrality_;
  int theCentralityBin;

  // handles
  edm::Handle<pat::CompositeCandidateCollection> collJpsi;
  edm::Handle<pat::MuonCollection> collMuon;
  edm::Handle<pat::MuonCollection> collMuonNoTrig;

  // data members
  edm::InputTag       _patMuon;
  edm::InputTag       _patMuonNoTrig;
  edm::InputTag       _patJpsi;
  edm::InputTag       _thePVs;
  std::string         _histfilename;
  std::string         _datasetname;

  std::vector<double> _ptbinranges;
  std::vector<double> _etabinranges;
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
  bool           _fillSingleMuons;

  std::vector<unsigned int>                     _thePassedCats[3];
  std::vector<const pat::CompositeCandidate*>   _thePassedCands[3];

  std::vector<uint32_t>   _thePassedPFPhotons;
  std::vector<uint32_t>   _thePassedPFPhotonsForPi0Rejection;

  // number of events
  unsigned int nEvents;
  unsigned int passedCandidates;

  // limits 
  float JpsiMassMin;
  float JpsiMassMax;
  float JpsiMassMinSide;
  float JpsiMassMaxSide;
  float JpsiCtMin;
  float JpsiCtMax;
  float JpsiPtMin;           // SET BY 
  float JpsiPtMax;           // DEFINITION
  float JpsiEtaMin;          // OF BIN
  float JpsiEtaMax;          // LIMITS 

  math::XYZPoint RefVtx;
  float nPV;

 // Triger stuff
  // PUT HERE THE *LAST FILTERS* OF THE BITS YOU LIKE
  static const unsigned int sNTRIGGERS = 7;
  unsigned int NTRIGGERS;
  // MC 8E29
  bool isTriggerMatched[sNTRIGGERS];
  std::string HLTLastFilters[sNTRIGGERS];
  bool alreadyFilled[sNTRIGGERS];

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
  _thePVs(iConfig.getParameter<edm::InputTag>("primaryVertexTag")),
  _histfilename(iConfig.getParameter<std::string>("histFileName")),		
  _datasetname(iConfig.getParameter<std::string>("dataSetName")),		
  _ptbinranges(iConfig.getParameter< std::vector<double> >("pTBinRanges")),	
  _etabinranges(iConfig.getParameter< std::vector<double> >("etaBinRanges")),
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
  _fillSingleMuons(iConfig.getParameter<bool>("fillSingleMuons")),  
  NTRIGGERS(iConfig.getParameter<uint32_t>("NumberOfTriggers")),
  _iConfig(iConfig)
{
   //now do what ever initialization is needed
  nEvents = 0;
  passedCandidates = 0;

  theRegions.push_back("All");
  theRegions.push_back("Barrel");
  theRegions.push_back("EndCap");

  theCentralities.push_back("MinBias");
  theCentralities.push_back("0010");
  theCentralities.push_back("1020");
  theCentralities.push_back("2040");
  theCentralities.push_back("40100");

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
  HLTLastFilters[4] = "hltHIL2Mu20L2Filtered";                  // BIN_HLT_HIL2Mu20
  HLTLastFilters[5] = "hltHIL2Mu3L2Filtered";                   // BIN_HLT_HIL2Mu3
  HLTLastFilters[6] = "hltHIL2Mu5TightFiltered";                // BIN_HLT_HIL2Mu5Tight

  theTriggerNames.push_back("NoTrigger");
  theTriggerNames.push_back("HLT_HIL1DoubleMuOpen");
  theTriggerNames.push_back("HLT_HIL2DoubleMu0");
  theTriggerNames.push_back("HLT_HIL2DoubleMu3");
  theTriggerNames.push_back("HLT_HIL2Mu20");
  theTriggerNames.push_back("HLT_HIL2Mu3");
  theTriggerNames.push_back("HLT_HIL2Mu5Tight");

  etaMax = 2.4;

  JpsiMassMin = 2.6;
  JpsiMassMax = 3.5;
  JpsiMassMinSide = 0.;
  JpsiMassMaxSide = 12.0;
  JpsiCtMin = -1.0;
  JpsiCtMax = 3.5;

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
   Init();

   nEvents++;
   hStats->Fill(BIN_nEvents);

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

  hZVtx->Fill(RefVtx.Z());
  if (fabs(RefVtx.Z()) > _iConfig.getParameter< double > ("maxAbsZ")) return;
  hPileUp->Fill(nPV);

  if(!centrality_) centrality_ = new CentralityProvider(iSetup);
//   centrality_->newEvent(iEvent,iSetup); // make sure you do this first in every event
//   double c = centrality_->centralityValue();
//   int bin = centrality_->getBin();
//   std::cout << "centrality bin: " << bin << " value: " << c << std::endl;
  theCentralityBin=0;

  iEvent.getByLabel(_patJpsi,collJpsi); 
  iEvent.getByLabel(_patMuon,collMuon);
  iEvent.getByLabel(_patMuonNoTrig,collMuonNoTrig);



  // APPLY CUTS
  int lastSign = 0;
  makeCuts(0);
  if (_storeSs) {
    makeCuts(1);
    makeCuts(2);
    lastSign = 2;
  }

  if (_fillSingleMuons)
    fillRecoMuons(0); // theCentralityBin

  fillRecoHistos(lastSign);

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
	const pat::CompositeCandidate *aJpsiCand = _thePassedCands[iSign].at(count); 

	checkTriggers(aJpsiCand);
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
HiOniaAnalyzer::fillRecoJpsi(int iSign, int count, std::string trigName, std::string centName) {
  const pat::CompositeCandidate *aJpsiCand = _thePassedCands[iSign].at(count);
  std::string theLabel =  trigName + "_" + centName + "_" + theSign.at(iSign);

  bool isBarrel = false;
  if (aJpsiCand->rapidity() < 1.2) isBarrel = true;

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
      fabs(aJpsiCand->rapidity()) >= JpsiEtaMin && fabs(aJpsiCand->rapidity()) < JpsiEtaMax) {
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
	/*	
	// for the moment consider only Glb+Glb pairs
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
       (!_applycuts || cand->userFloat("vProb") > 0.001) )
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
	  ((fabs(aMuon->eta()) < 1.3 && aMuon->pt() >= 3.3) ||
	   (fabs(aMuon->eta()) >= 1.3 && fabs(aMuon->eta()) < 2.2 && aMuon->p() >= 2.9) ||
	   (fabs(aMuon->eta()) >= 2.2 && aMuon->pt() >= 0.8)));
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

  return (isMuonInAccept(aMuon) &&
	  iTrack->found() > 11 &&
	  gTrack->chi2()/gTrack->ndof() < 10.0 &&
	  q.numberOfValidMuonHits() > 0 &&
	  iTrack->chi2()/iTrack->ndof() < 4.0 &&
	  aMuon->muonID("TrackerMuonArbitrated") &&
	  aMuon->muonID("TMLastStationAngTight") &&
	  p.pixelLayersWithMeasurement() > 1 &&
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
HiOniaAnalyzer::Init()
{
  for (unsigned int iTr=1;iTr<NTRIGGERS;++iTr) {
    alreadyFilled[iTr]=false;
  }

  _thePassedCats[0].clear();      _thePassedCands[0].clear();
  _thePassedCats[1].clear();      _thePassedCands[1].clear();
  _thePassedCats[2].clear();      _thePassedCands[2].clear();

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
      if (muon->eta() < 1.2) isBarrel = true;

      if (muon->isGlobalMuon() &&
	  selGlobalMuon(muon)) {
	std::string theLabel = theTriggerNames.at(0) + "_" + theCentralities.at(iCent);

	myRecoGlbMuonHistos->Fill(muon, "All_"+theLabel);
	if (isBarrel)
	  myRecoGlbMuonHistos->Fill(muon, "Barrel_"+theLabel);
	else
	  myRecoGlbMuonHistos->Fill(muon, "EndCap_"+theLabel);
	
	nGoodMuons++;

	for (unsigned int iTrig=1; iTrig<NTRIGGERS; ++iTrig) {
	  const pat::TriggerObjectStandAloneCollection muHLTMatches = muon->triggerObjectMatchesByFilter(  HLTLastFilters[iTrig] );

	  if (muHLTMatches.size() > 0) {
	    std::string theLabel = theTriggerNames.at(iTrig) + "_" + theCentralities.at(iCent);

	    myRecoGlbMuonHistos->Fill(muon, "All_"+theLabel);
	    if (isBarrel)
	      myRecoGlbMuonHistos->Fill(muon, "Barrel_"+theLabel);
	    else
	      myRecoGlbMuonHistos->Fill(muon, "EndCap_"+theLabel);

	    if (iTrig==1) nL1DoubleMuOpenMuons++;
	    if (iTrig==3) nL2DoubleMu3Muons++;
	    if (iTrig==4) nL2Mu20Muons++;
	  }
	}
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

// ------------ method called once each job just before starting event loop  ------------
void 
HiOniaAnalyzer::beginJob()
{
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
  myRecoTrkMuonHistos = new MyCommonHistoManager("TrackerMuon");

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
    for (unsigned int j=0; j<theTriggerNames.size(); ++j) {
      for (unsigned int k=0; k<theCentralities.size(); ++k) {

	std::string theAppendix = theRegions.at(i) ;
	theAppendix += "_" + theTriggerNames.at(j);
	theAppendix += "_" + theCentralities.at(k);

	// muons
	myRecoGlbMuonHistos->Add(theAppendix);
	myRecoTrkMuonHistos->Add(theAppendix);
	
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

  TFile *fOut = new TFile(_histfilename.c_str(), "RECREATE");
  fOut->cd();
  hGoodMuonsNoTrig->Write();
  hGoodMuons->Write();
  hL1DoubleMuOpen->Write();
  hL2DoubleMu3->Write();
  hL2Mu20->Write();

  // muons
  myRecoGlbMuonHistos->Write(fOut);
  myRecoTrkMuonHistos->Write(fOut);

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

//define this as a plug-in
DEFINE_FWK_MODULE(HiOniaAnalyzer);
