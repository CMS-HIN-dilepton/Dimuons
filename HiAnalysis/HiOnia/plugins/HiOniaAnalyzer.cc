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
// $Id: HiOniaAnalyzer.cc,v 1.23.2.9 2013/01/21 11:30:39 tdahms Exp $
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
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

#include "HiAnalysis/HiOnia/interface/MyCommonHistoManager.h"

// adding Event Plane by dmoon 
#include "DataFormats/HeavyIonEvent/interface/EvtPlane.h"

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
  void hltReport(const edm::Event &iEvent ,const edm::EventSetup& iSetup);

  void beginRun(const edm::Run &, const edm::EventSetup &);  

  TLorentzVector lorentzMomentum(const reco::Candidate::LorentzVector& p);
  // ----------member data ---------------------------
  enum StatBins {
    BIN_nEvents = 0,
    BIN_HLT_PAL1DoubleMuOpen = 1,
    BIN_HLT_PAL1DoubleMu0_HighQ = 2,
    BIN_HLT_PAL2DoubleMu3 = 3,
    BIN_HLT_PAMu3 = 4,
    BIN_HLT_PAMu7 = 5,
    BIN_HLT_PAMu12 = 6,
    BIN_HLT_PAPixelTrackMultiplicity100_L2DoubleMu3 = 7
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

  HLTConfigProvider hltConfig;
  bool hltConfigInit;

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
  float Reco_QQ_ctauTrue[100];// true ctau
  float Reco_QQ_dca[100];
  float Reco_QQ_MassErr[100];

  bool Reco_QQ_mupl_TrkMuArb[100];      // Vector of TrackerMuonArbitrated for plus muon
  bool Reco_QQ_mumi_TrkMuArb[100];      // Vector of TrackerMuonArbitrated for minus muon
  bool Reco_QQ_mupl_TMOneStaTight[100]; // Vector of TMOneStationTight for plus muon
  bool Reco_QQ_mumi_TMOneStaTight[100]; // Vector of TMOneStationTight for minus muon

  int Reco_QQ_mupl_nMuValHits[100];  // Number of valid muon hits in plus sta muons
  int Reco_QQ_mumi_nMuValHits[100];  // Number of valid muon hits in minus sta muons
  int Reco_QQ_mupl_nTrkHits[100];  // track hits plus global muons
  int Reco_QQ_mumi_nTrkHits[100];  // track hits minus global muons
  int Reco_QQ_mupl_nPixWMea[100];  // pixel layers with measurement for plus inner track muons
  int Reco_QQ_mumi_nPixWMea[100];  // pixel layers with measurement for minus inner track muons
  int Reco_QQ_mupl_nTrkWMea[100];  // track layers with measurement for plus inner track muons
  int Reco_QQ_mumi_nTrkWMea[100];  // track layers with measurement for minus inner track muons
  int Reco_QQ_mupl_StationsMatched[100];  // number of stations matched for plus inner track muons
  int Reco_QQ_mumi_StationsMatched[100];  // number of stations matched for minus inner track muons
  float Reco_QQ_mupl_normChi2_inner[100];  // chi2/ndof for plus inner track muons
  float Reco_QQ_mumi_normChi2_inner[100];  // chi2/ndof for minus inner track muons
  float Reco_QQ_mupl_normChi2_global[100];  // chi2/ndof for plus global muons
  float Reco_QQ_mumi_normChi2_global[100];  // chi2/ndof for minus global muons
  float Reco_QQ_mupl_dxy[100];  // dxy for plus inner track muons
  float Reco_QQ_mumi_dxy[100];  // dxy for minus inner track muons
  float Reco_QQ_mupl_dxyErr[100];  // dxy error for plus inner track muons
  float Reco_QQ_mumi_dxyErr[100];  // dxy error for minus inner track muons
  float Reco_QQ_mupl_dz[100];  // dz for plus inner track muons
  float Reco_QQ_mumi_dz[100];  // dz for minus inner track muons
  float Reco_QQ_mupl_dzErr[100];  // dz error for plus inner track muons
  float Reco_QQ_mumi_dzErr[100];  // dz error for minus inner track muons
  float Reco_QQ_mupl_pt_inner[100];  // pT for plus inner track muons
  float Reco_QQ_mumi_pt_inner[100];  // pT for minus inner track muons
  float Reco_QQ_mupl_pt_global[100];  // pT for plus global muons
  float Reco_QQ_mumi_pt_global[100];  // pT for minus global muons
  float Reco_QQ_mupl_ptErr_inner[100];  // pT error for plus inner track muons
  float Reco_QQ_mumi_ptErr_inner[100];  // pT error for minus inner track muons
  float Reco_QQ_mupl_ptErr_global[100];  // pT error for plus global muons
  float Reco_QQ_mumi_ptErr_global[100];  // pT error for minus global muons

  int Reco_mu_size;           // Number of reconstructed muons
  int Reco_mu_trig[100];      // Vector of trigger bits matched to the muons
  int Reco_mu_charge[100];  // Vector of charge of muons
  int Reco_mu_type[100];  // Vector of type of muon (global=0, tracker=1, calo=2)  

  bool Reco_mu_TrkMuArb[100];      // Vector of TrackerMuonArbitrated
  bool Reco_mu_TMOneStaTight[100]; // Vector of TMOneStationTight

  int Reco_mu_nMuValHits[100];  // Number of valid muon hits in sta muons
  int Reco_mu_nTrkHits[100];  // track hits global muons
  int Reco_mu_nPixWMea[100];  // pixel layers with measurement for inner track muons
  int Reco_mu_nTrkWMea[100];  // track layers with measurement for inner track muons
  int Reco_mu_StationsMatched[100];  // number of stations matched for inner track muons
  float Reco_mu_normChi2_inner[100];  // chi2/ndof for inner track muons
  float Reco_mu_normChi2_global[100];  // chi2/ndof for global muons
  float Reco_mu_dxy[100];  // dxy for inner track muons
  float Reco_mu_dxyErr[100];  // dxy error for inner track muons
  float Reco_mu_dz[100];  // dz for inner track muons
  float Reco_mu_dzErr[100];  // dz error for inner track muons
  float Reco_mu_pt_inner[100];  // pT for inner track muons
  float Reco_mu_pt_global[100];  // pT for global muons
  float Reco_mu_ptErr_inner[100];  // pT error for inner track muons
  float Reco_mu_ptErr_global[100];  // pT error for global muons
  
  /*
    float Reco_mu_ptErr[100];   // Vector of err on pt of muons
    float Reco_mu_phiErr[100];  // Vector of err on phi of muons
    float Reco_mu_etaErr[100];  // Vector of err on eta of muons
    float Reco_mu_d0[100];      // Vector of d0 of muons
    float Reco_mu_d0err[100];   // Vector of d0err of muons
    float Reco_mu_dz[100];      // Vector of dz of muons
    float Reco_mu_dzerr[100];   // Vector of dzerr of muons
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
  */
  int muType; // type of muon (global=0, tracker=1, calo=2, none=-1) 

  // Event Plane variables
  int nEP;
  int nNfEP;
  float rpAng[100];
  float rpCos[100];
  float rpSin[100];
  float NfRpAng[100];
  float NfRpCos[100];
  float NfRpSin[100];

  // histos
  TH1F* hGoodMuonsNoTrig;
  TH1F* hGoodMuons;
  TH1F* hL1DoubleMuOpen;
  TH1F* hL2DoubleMu3;
  TH1F* hL3Mu12;

  MyCommonHistoManager* myRecoMuonHistos;
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

  edm::Handle<edm::TriggerResults> collTriggerResults;

  // data members
  edm::InputTag       _patMuon;
  edm::InputTag       _patMuonNoTrig;
  edm::InputTag       _patJpsi;
  edm::InputTag       _genParticle;
  edm::InputTag       _thePVs;
  edm::InputTag       _tagTriggerResults;
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
  bool           _fillHistos;
  bool           _theMinimumFlag;
  bool           _fillSingleMuons;
  bool           _isHI;
  bool           _isPA;
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

  std::map<std::string, int> mapTriggerNameToIntFired_;
  std::map<std::string, int> mapTriggerNameToPrescaleFac_;

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
  _tagTriggerResults(iConfig.getParameter<edm::InputTag>("triggerResultsLabel")),
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
  _fillHistos(iConfig.getParameter<bool>("fillHistos")),
  _theMinimumFlag(iConfig.getParameter<bool>("minimumFlag")),  
  _fillSingleMuons(iConfig.getParameter<bool>("fillSingleMuons")),
  _isHI(iConfig.getUntrackedParameter<bool>("isHI",false) ),
  _isPA(iConfig.getUntrackedParameter<bool>("isPA",true) ),
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
  HLTLastFilters[1] = "hltL1fL1sPAL1DoubleMuOpenL1Filtered0";       // BIT HLT_PAL1DoubleMuOpen
  HLTLastFilters[2] = "hltL1fL1sPAL1DoubleMu0HighQL1FilteredHighQ"; // BIT HLT_PAL1DoubleMu0_HighQ
  HLTLastFilters[3] = "hltL2fL1sPAL2DoubleMu3L2Filtered3";          // BIT HLT_PAL2DoubleMu3
  HLTLastFilters[4] = "hltL3fL2sMu3L3Filtered3";                    // BIT HLT_PAMu3
  HLTLastFilters[5] = "hltL3fL2sMu7L3Filtered7";                    // BIT HLT_PAMu7
  HLTLastFilters[6] = "hltL3fL2sMu12L3Filtered12";                  // BIT HLT_PAMu12
  HLTLastFilters[7] = "hltL2fL1sPAL2DoubleMu3L2Filtered3";          // BIT HLT_PAL2DoubleMu3

  theTriggerNames.push_back("NoTrigger");
  theTriggerNames.push_back("HLT_PAL1DoubleMuOpen_v1");
  theTriggerNames.push_back("HLT_PAL1DoubleMu0_HighQ_v1");
  theTriggerNames.push_back("HLT_PAL2DoubleMu3_v1");
  theTriggerNames.push_back("HLT_PAMu3_v1");
  theTriggerNames.push_back("HLT_PAMu7_v1");
  theTriggerNames.push_back("HLT_PAMu12_v1");
  theTriggerNames.push_back("HLT_PAPixelTrackMultiplicity100_L2DoubleMu3_v1");

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
  

  for(std::vector<std::string>::iterator it = theTriggerNames.begin(); it != theTriggerNames.end(); ++it){
      mapTriggerNameToIntFired_[*it] = -9999;
  }
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

  this->hltReport(iEvent, iSetup);

  for (unsigned int iTr = 1 ; iTr < theTriggerNames.size() ; iTr++) {
    if (mapTriggerNameToIntFired_[theTriggerNames.at(iTr)] == 3) {
      HLTriggers += pow(2,iTr-1);
      hStats->Fill(iTr); // event info
    }
  }

  if (_isHI || _isPA) {
    if(!centrality_) centrality_ = new CentralityProvider(iSetup);
    centrality_->newEvent(iEvent,iSetup); // make sure you do this first in every event
    centBin = centrality_->getBin();
    hCent->Fill(centBin);

    for (unsigned int iCent=0; iCent<_centralityranges.size(); ++iCent) {
      if ( (_isHI && centBin<_centralityranges.at(iCent)/2.5) ||
	   (_isPA && centBin<_centralityranges.at(iCent)) ) {
	theCentralityBin=iCent;
	break;
      }
    }
  }
  else 
    theCentralityBin=0;

  if (_isHI) {
    edm::Handle<reco::EvtPlaneCollection> FlatEvtPlanes;
    edm::Handle<reco::EvtPlaneCollection> NoFlatEvtPlanes;
    iEvent.getByLabel("hiEvtPlaneFlat",FlatEvtPlanes);
    iEvent.getByLabel("hiEvtPlane","recoLevel",NoFlatEvtPlanes);

    if(FlatEvtPlanes.isValid()) {
      for (reco::EvtPlaneCollection::const_iterator rp = FlatEvtPlanes->begin(); rp!=FlatEvtPlanes->end(); rp++) {
	rpAng[nEP] = rp->angle();
	rpSin[nEP] = rp->sumSin();
	rpCos[nEP] = rp->sumCos();
	nEP++;
      }
    }
    else if (!_isMC) 
      cout << "Warning! Can't get flattened hiEvtPlane product!" << endl;


    if(NoFlatEvtPlanes.isValid()){
      for (reco::EvtPlaneCollection::const_iterator rp = NoFlatEvtPlanes->begin();rp !=NoFlatEvtPlanes->end(); rp++) {
	NfRpAng[nNfEP] = rp->angle();
	NfRpSin[nNfEP] = rp->sumSin();
	NfRpCos[nNfEP] = rp->sumCos();
	nNfEP++;
      }
    }
    else if (!_isMC)
      cout << "Warning! Can't get hiEvtPlane product!" << endl;
  }

  iEvent.getByLabel(_patJpsi,collJpsi); 
  iEvent.getByLabel(_patMuon,collMuon);
  iEvent.getByLabel(_patMuonNoTrig,collMuonNoTrig);

  if (_isMC) {
    iEvent.getByLabel(_genParticle,collGenParticles);
    this->fillGenInfo();
  }
  
  // APPLY CUTS
  int lastSign = 0;
  this->makeCuts(0);
  if (_storeSs) {
    this->makeCuts(1);
    this->makeCuts(2);
    lastSign = 2;
  }

  if (_fillSingleMuons)
    this->fillRecoMuons(theCentralityBin);

  this->fillRecoHistos(lastSign);

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
      if (theBest.first < 10) this->fillHistosAndDS(theBest.first, theBest.second);
    }

  } else {   // no, fill all candidates passing cuts (possibly same-sign)
   
    for (int iSign = 0; iSign <= lastSign; ++iSign) {
      for( unsigned int count = 0; count < _thePassedCands[iSign].size(); count++) { 
	const pat::CompositeCandidate* aJpsiCand = _thePassedCands[iSign].at(count); 

	this->checkTriggers(aJpsiCand);
	if (_fillTree)
	  this->fillTreeJpsi(iSign, count);

	for (unsigned int iTr=0; iTr<NTRIGGERS; ++iTr) {
	  if (isTriggerMatched[iTr]) {
	    this->fillRecoJpsi(iSign,count,theTriggerNames.at(iTr), theCentralities.at(theCentralityBin));
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

  reco::TrackRef iTrack = muon->innerTrack();
  
  if (!_theMinimumFlag) {
    Reco_mu_TrkMuArb[Reco_mu_size] = muon->muonID("TrackerMuonArbitrated");
    Reco_mu_TMOneStaTight[Reco_mu_size] = muon->muonID("TMOneStationTight");
    Reco_mu_nTrkHits[Reco_mu_size] = iTrack->found();
    Reco_mu_normChi2_inner[Reco_mu_size] = iTrack->normalizedChi2();
    Reco_mu_nPixWMea[Reco_mu_size] = iTrack->hitPattern().pixelLayersWithMeasurement();
    Reco_mu_nTrkWMea[Reco_mu_size] = iTrack->hitPattern().trackerLayersWithMeasurement();
    Reco_mu_StationsMatched[Reco_mu_size] = muon->numberOfMatchedStations();
    Reco_mu_dxy[Reco_mu_size] = iTrack->dxy(RefVtx);
    Reco_mu_dxyErr[Reco_mu_size] = iTrack->dxyError();
    Reco_mu_dz[Reco_mu_size] = iTrack->dz(RefVtx);
    Reco_mu_dzErr[Reco_mu_size] = iTrack->dzError();
    Reco_mu_pt_inner[Reco_mu_size] = iTrack->pt();
    Reco_mu_ptErr_inner[Reco_mu_size] = iTrack->ptError();
    
    if (muon->isGlobalMuon()) {
      reco::TrackRef gTrack = muon->globalTrack();
      Reco_mu_nMuValHits[Reco_mu_size] = gTrack->hitPattern().numberOfValidMuonHits();
      Reco_mu_normChi2_global[Reco_mu_size] = gTrack->normalizedChi2();
      Reco_mu_pt_global[Reco_mu_size] = gTrack->pt();
      Reco_mu_ptErr_global[Reco_mu_size] = gTrack->ptError();
    }
    else {
      Reco_mu_nMuValHits[Reco_mu_size] = -1;
      Reco_mu_normChi2_global[Reco_mu_size] = 999;
      Reco_mu_pt_global[Reco_mu_size] = -1;
      Reco_mu_ptErr_global[Reco_mu_size] = -1;
    }
  }

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

  reco::TrackRef iTrack_mupl;
  reco::TrackRef gTrack_mupl;

  reco::TrackRef iTrack_mumi;
  reco::TrackRef gTrack_mumi;

  if (muon1->charge() > muon2->charge()) {
    new((*Reco_QQ_mupl_4mom)[Reco_QQ_size])TLorentzVector(vMuon1);
    new((*Reco_QQ_mumi_4mom)[Reco_QQ_size])TLorentzVector(vMuon2);

    Reco_QQ_mupl_StationsMatched[Reco_QQ_size] = muon1->numberOfMatchedStations();
    Reco_QQ_mumi_StationsMatched[Reco_QQ_size] = muon2->numberOfMatchedStations();

    Reco_QQ_mupl_TrkMuArb[Reco_QQ_size] = muon1->muonID("TrackerMuonArbitrated");
    Reco_QQ_mupl_TMOneStaTight[Reco_QQ_size] = muon1->muonID("TMOneStationTight");

    Reco_QQ_mumi_TrkMuArb[Reco_QQ_size] = muon2->muonID("TrackerMuonArbitrated");
    Reco_QQ_mumi_TMOneStaTight[Reco_QQ_size] = muon2->muonID("TMOneStationTight");

    iTrack_mupl = muon1->innerTrack();
    iTrack_mumi = muon2->innerTrack();

    if (muon1->isGlobalMuon())
      gTrack_mupl = muon1->globalTrack();

    if (muon2->isGlobalMuon())
      gTrack_mumi = muon2->globalTrack();
  }
  else {
    new((*Reco_QQ_mupl_4mom)[Reco_QQ_size])TLorentzVector(vMuon2);
    new((*Reco_QQ_mumi_4mom)[Reco_QQ_size])TLorentzVector(vMuon1);

    Reco_QQ_mupl_StationsMatched[Reco_QQ_size] = muon2->numberOfMatchedStations();
    Reco_QQ_mumi_StationsMatched[Reco_QQ_size] = muon1->numberOfMatchedStations();

    Reco_QQ_mupl_TrkMuArb[Reco_QQ_size] = muon2->muonID("TrackerMuonArbitrated");
    Reco_QQ_mupl_TMOneStaTight[Reco_QQ_size] = muon2->muonID("TMOneStationTight");

    Reco_QQ_mumi_TrkMuArb[Reco_QQ_size] = muon1->muonID("TrackerMuonArbitrated");
    Reco_QQ_mumi_TMOneStaTight[Reco_QQ_size] = muon1->muonID("TMOneStationTight");


    iTrack_mupl = muon2->innerTrack();
    iTrack_mumi = muon1->innerTrack();

    if (muon2->isGlobalMuon())
      gTrack_mupl = muon2->globalTrack();

    if (muon1->isGlobalMuon())
      gTrack_mumi = muon1->globalTrack();
  }

  if (!_theMinimumFlag) {
    Reco_QQ_mupl_nTrkHits[Reco_QQ_size] = iTrack_mupl->found();
    Reco_QQ_mumi_nTrkHits[Reco_QQ_size] = iTrack_mumi->found();
    Reco_QQ_mupl_normChi2_inner[Reco_QQ_size] = iTrack_mupl->normalizedChi2();
    Reco_QQ_mumi_normChi2_inner[Reco_QQ_size] = iTrack_mumi->normalizedChi2();
    Reco_QQ_mupl_nPixWMea[Reco_QQ_size] = iTrack_mupl->hitPattern().pixelLayersWithMeasurement();
    Reco_QQ_mumi_nPixWMea[Reco_QQ_size] = iTrack_mumi->hitPattern().pixelLayersWithMeasurement();
    Reco_QQ_mupl_nTrkWMea[Reco_QQ_size] = iTrack_mupl->hitPattern().trackerLayersWithMeasurement();
    Reco_QQ_mumi_nTrkWMea[Reco_QQ_size] = iTrack_mumi->hitPattern().trackerLayersWithMeasurement();
    Reco_QQ_mupl_dxy[Reco_QQ_size] = iTrack_mupl->dxy(RefVtx);
    Reco_QQ_mumi_dxy[Reco_QQ_size] = iTrack_mumi->dxy(RefVtx);
    Reco_QQ_mupl_dxyErr[Reco_QQ_size] = iTrack_mupl->dxyError();
    Reco_QQ_mumi_dxyErr[Reco_QQ_size] = iTrack_mumi->dxyError();
    Reco_QQ_mupl_dz[Reco_QQ_size] = iTrack_mupl->dz(RefVtx);
    Reco_QQ_mumi_dz[Reco_QQ_size] = iTrack_mumi->dz(RefVtx);
    Reco_QQ_mupl_dzErr[Reco_QQ_size] = iTrack_mupl->dzError();
    Reco_QQ_mumi_dzErr[Reco_QQ_size] = iTrack_mumi->dzError();
    Reco_QQ_mupl_pt_inner[Reco_QQ_size] = iTrack_mupl->pt();
    Reco_QQ_mumi_pt_inner[Reco_QQ_size] = iTrack_mumi->pt();
    Reco_QQ_mupl_ptErr_inner[Reco_QQ_size] = iTrack_mupl->ptError();
    Reco_QQ_mumi_ptErr_inner[Reco_QQ_size] = iTrack_mumi->ptError();
    
    if (gTrack_mupl.isNonnull()) {
      Reco_QQ_mupl_nMuValHits[Reco_QQ_size] = gTrack_mupl->hitPattern().numberOfValidMuonHits();
      Reco_QQ_mupl_normChi2_global[Reco_QQ_size] = gTrack_mupl->normalizedChi2();
      Reco_QQ_mupl_pt_global[Reco_QQ_size] = gTrack_mupl->pt();
      Reco_QQ_mupl_ptErr_global[Reco_QQ_size] = gTrack_mupl->ptError();
    }
    else {
      Reco_QQ_mupl_nMuValHits[Reco_QQ_size] = -1;
      Reco_QQ_mupl_normChi2_global[Reco_QQ_size] = 999;
      Reco_QQ_mupl_pt_global[Reco_QQ_size] = -1;
      Reco_QQ_mupl_ptErr_global[Reco_QQ_size] = -1;
    }

    if (gTrack_mumi.isNonnull()) {
      Reco_QQ_mumi_nMuValHits[Reco_QQ_size] = gTrack_mumi->hitPattern().numberOfValidMuonHits();
      Reco_QQ_mumi_normChi2_global[Reco_QQ_size] = gTrack_mumi->normalizedChi2();
      Reco_QQ_mumi_pt_global[Reco_QQ_size] = gTrack_mumi->pt();
      Reco_QQ_mumi_ptErr_global[Reco_QQ_size] = gTrack_mumi->ptError();
    }
    else {
      Reco_QQ_mumi_nMuValHits[Reco_QQ_size] = -1;
      Reco_QQ_mumi_normChi2_global[Reco_QQ_size] = 999;
      Reco_QQ_mumi_pt_global[Reco_QQ_size] = -1;
      Reco_QQ_mumi_ptErr_global[Reco_QQ_size] = -1;
    }
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
  Reco_QQ_dca[Reco_QQ_size] = aJpsiCand->userFloat("DCA");
  Reco_QQ_MassErr[Reco_QQ_size] = aJpsiCand->userFloat("MassErr");

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

  float theCtau;
  //  float theCtauErr; 

  if (_useBS) {
    theCtau = 10.0*aJpsiCand->userFloat("ppdlBS");
    //    theCtauErr = 10.*aJpsiCand->userFloat("ppdlErrBS");
  }
  else {
    theCtau = 10.0*aJpsiCand->userFloat("ppdlPV");
    //    theCtauErr = 10.*aJpsiCand->userFloat("ppdlErrPV");
  }

  if (iSign==0 &&
      aJpsiCand->mass() >= JpsiMassMin && aJpsiCand->mass() < JpsiMassMax && 
      theCtau >= JpsiCtMin && theCtau < JpsiCtMax && 
      aJpsiCand->pt() >= JpsiPtMin && aJpsiCand->pt() < JpsiPtMax && 
      fabs(aJpsiCand->rapidity()) >= JpsiRapMin && fabs(aJpsiCand->rapidity()) < JpsiRapMax) {
    passedCandidates++;
  }

  if (_fillHistos) {
    if (_combineCategories && _thePassedCats[iSign].at(count)<=TrkTrk) { // for the moment consider Glb+Glb, Glb+Trk, Trk+Trk
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
  }

  this->fillHistosAndDS(_thePassedCats[iSign].at(count), aJpsiCand); 

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
    const pat::TriggerObjectStandAloneCollection mu1HLTMatchesFilter = muon1->triggerObjectMatchesByFilter( HLTLastFilters[iTr] );
    const pat::TriggerObjectStandAloneCollection mu2HLTMatchesFilter = muon2->triggerObjectMatchesByFilter( HLTLastFilters[iTr] );
    
    const pat::TriggerObjectStandAloneCollection mu1HLTMatchesPath = muon1->triggerObjectMatchesByPath( theTriggerNames.at(iTr) );
    const pat::TriggerObjectStandAloneCollection mu2HLTMatchesPath = muon2->triggerObjectMatchesByPath( theTriggerNames.at(iTr) );
    
    bool pass1 = false;
    bool pass2 = false;
    if (iTr<7) { // not sure the Multiplicity100_DoubleMu3 has the right filter name, so match by path for that one.
      pass1 = mu1HLTMatchesFilter.size() > 0;
      pass2 = mu2HLTMatchesFilter.size() > 0;
    }
    else {
      pass1 = mu1HLTMatchesPath.size() > 0;
      pass2 = mu2HLTMatchesPath.size() > 0;
    }

    if (iTr > 3 && iTr<7) {  // single triggers here
      isTriggerMatched[iTr] = pass1 || pass2;
    } else {        // double triggers here
      isTriggerMatched[iTr] = pass1 && pass2;
    }
  }

  for (unsigned int iTr=1;iTr<NTRIGGERS;++iTr) {
    if (isTriggerMatched[iTr]) {
      // fill event counting histogram only once per event, also if several muons fired trigger
      //      if (alreadyFilled[iTr]) continue;
      // since we have bins for event info, let's try to fill here the trigger info for each pair
      // also if there are several pairs matched to the same kind of trigger
      hStats->Fill(iTr+NTRIGGERS); // pair info
      //      alreadyFilled[iTr]=true;
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
	/*	
	// global + global?
	if (checkCuts(cand,muon1,muon2,&HiOniaAnalyzer::selGlobalMuon,&HiOniaAnalyzer::selGlobalMuon)){
	_thePassedCats[sign].push_back(GlbGlb);  _thePassedCands[sign].push_back(cand);
	//	  continue;
	}

	// global + tracker? (x2)    
	if (checkCuts(cand,muon1,muon2,&HiOniaAnalyzer::selGlobalMuon,&HiOniaAnalyzer::selTrackerMuon)){
	_thePassedCats[sign].push_back(GlbTrk);  _thePassedCands[sign].push_back(cand);
	//	  continue;
	}

	if (checkCuts(cand,muon2,muon1,&HiOniaAnalyzer::selGlobalMuon,&HiOniaAnalyzer::selTrackerMuon)){
	_thePassedCats[sign].push_back(GlbTrk);  _thePassedCands[sign].push_back(cand);
	//	  continue;
	}
	*/

	// tracker + tracker?  
	if (checkCuts(cand,muon1,muon2,&HiOniaAnalyzer::selTrackerMuon,&HiOniaAnalyzer::selTrackerMuon)){
	  _thePassedCats[sign].push_back(TrkTrk);  _thePassedCands[sign].push_back(cand);
	  continue;
	}
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
  // return (fabs(aMuon->eta()) < 2.4 &&
  // 	  ((fabs(aMuon->eta()) < 1.0 && aMuon->pt() >= 3.4) ||
  // 	   (1.0 <= fabs(aMuon->eta()) && fabs(aMuon->eta()) < 1.5 && aMuon->pt() >= 5.8-2.4*fabs(aMuon->eta())) ||
  // 	   (1.5 <= fabs(aMuon->eta()) && aMuon->pt() >= 3.3667-7.0/9.0*fabs(aMuon->eta()))));
  return (fabs(aMuon->eta()) < 2.4 &&
	  ((fabs(aMuon->eta()) < 1.3 && aMuon->pt() >= 3.3) ||
	   (1.3 <= fabs(aMuon->eta()) && fabs(aMuon->eta()) < 2.2 && aMuon->p() >= 2.9) ||
	   (2.2 <= fabs(aMuon->eta()) && aMuon->pt() >= 0.8)));
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
  //  const reco::HitPattern& q = gTrack->hitPattern();
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
	  gTrack->chi2()/gTrack->ndof() < 20.0 &&
	  //	  q.numberOfValidMuonHits() > 6 &&
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
	  iTrack->normalizedChi2() < 1.8 &&
	  aMuon->muonID("TrackerMuonArbitrated") &&
	  aMuon->muonID("TMOneStationTight") &&
	  p.trackerLayersWithMeasurement() > 5 &&
	  p.pixelLayersWithMeasurement() > 1 &&
	  fabs(iTrack->dxy(RefVtx)) < 3.0 &&
	  fabs(iTrack->dz(RefVtx)) < 30.0 );
}


void
HiOniaAnalyzer::InitEvent()
{
  for (unsigned int iTr=1;iTr<NTRIGGERS;++iTr) {
    alreadyFilled[iTr]=false;
  }
  HLTriggers = 0;

  nEP = 0;
  nNfEP = 0;

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

  for(std::map< std::string, int >::iterator clearIt= mapTriggerNameToIntFired_.begin(); clearIt != mapTriggerNameToIntFired_.end(); clearIt++){
    clearIt->second=0;
  }
  for(std::map< std::string, int >::iterator clearIt= mapTriggerNameToPrescaleFac_.begin(); clearIt != mapTriggerNameToPrescaleFac_.end(); clearIt++){
    clearIt->second=-1;
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

      if (abs(gen->pdgId()) == _oniaPDG  && gen->status() == 2 &&
	  gen->numberOfDaughters() == 2) {

	const reco::Candidate* genMuon1 = gen->daughter(0);
	const reco::Candidate* genMuon2 = gen->daughter(1);
	if ( abs(genMuon1->pdgId()) == 13 &&
	     abs(genMuon2->pdgId()) == 13 &&
	     genMuon1->status() == 1 &&
	     genMuon2->status() == 1 ) {
	  
	  Gen_QQ_type[Gen_QQ_size] = _isPromptMC ? 0 : 1; // prompt: 0, non-prompt: 1
	  
	  TLorentzVector vJpsi = lorentzMomentum(gen->p4());
	  new((*Gen_QQ_4mom)[Gen_QQ_size])TLorentzVector(vJpsi);


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
  int nL3Mu12Muons=0;
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
      std::string theLabel = theTriggerNames.at(0) + "_" + theCentralities.at(iCent);

      if (_fillHistos) {
	if (_combineCategories) {
	  if ( (muon->isGlobalMuon() &&
		selGlobalMuon(muon)) ||
	       (muon->isTrackerMuon() &&
		selTrackerMuon(muon)) ) {
	    myRecoMuonHistos->Fill(muon, "All_"+theLabel);
	    if (isBarrel)
	      myRecoMuonHistos->Fill(muon, "Barrel_"+theLabel);
	    else
	      myRecoMuonHistos->Fill(muon, "EndCap_"+theLabel);
	  }
	}
	else {
	  if (muon->isGlobalMuon() &&
	      selGlobalMuon(muon)) {
	  
	    myRecoGlbMuonHistos->Fill(muon, "All_"+theLabel);
	    if (isBarrel)
	      myRecoGlbMuonHistos->Fill(muon, "Barrel_"+theLabel);
	    else
	      myRecoGlbMuonHistos->Fill(muon, "EndCap_"+theLabel);
	  }
	  else if (muon->isTrackerMuon() &&
		   selTrackerMuon(muon)) {
	    myRecoTrkMuonHistos->Fill(muon, "All_"+theLabel);
	    if (isBarrel)
	      myRecoTrkMuonHistos->Fill(muon, "Barrel_"+theLabel);
	    else
	      myRecoTrkMuonHistos->Fill(muon, "EndCap_"+theLabel);
	  }
	}
      }

      if (muon->isGlobalMuon() && selGlobalMuon(muon)) muType = 0;
      else if (muon->isTrackerMuon() && selTrackerMuon(muon)) muType = 1;
      else muType = -1;

      if ( //muType==0 ||
	   muType==1 ) {
	nGoodMuons++;

	int trigBits=0;
	for (unsigned int iTr=1; iTr<NTRIGGERS; ++iTr) {
	  const pat::TriggerObjectStandAloneCollection muHLTMatchesFilter = muon->triggerObjectMatchesByFilter(  HLTLastFilters[iTr] );
	  const pat::TriggerObjectStandAloneCollection muHLTMatchesPath = muon->triggerObjectMatchesByPath( theTriggerNames.at(iTr) );

	  // apparently matching by path gives false positives so we use matching by filter for all triggers for which we know the filter name
	  if ( muHLTMatchesFilter.size() > 0 ) {
	    std::string theLabel = theTriggerNames.at(iTr) + "_" + theCentralities.at(iCent);

	    if (_fillHistos) {
	      if (_combineCategories) {
		myRecoMuonHistos->Fill(muon, "All_"+theLabel);
		if (isBarrel)
		  myRecoMuonHistos->Fill(muon, "Barrel_"+theLabel);
		else
		  myRecoMuonHistos->Fill(muon, "EndCap_"+theLabel);
	      }
	      else if (muType==0) {
		myRecoGlbMuonHistos->Fill(muon, "All_"+theLabel);
		if (isBarrel)
		  myRecoGlbMuonHistos->Fill(muon, "Barrel_"+theLabel);
		else
		  myRecoGlbMuonHistos->Fill(muon, "EndCap_"+theLabel);
	      }
	      else if (muType==1) {
		myRecoTrkMuonHistos->Fill(muon, "All_"+theLabel);
		if (isBarrel)
		  myRecoTrkMuonHistos->Fill(muon, "Barrel_"+theLabel);
		else
		  myRecoTrkMuonHistos->Fill(muon, "EndCap_"+theLabel);
	      }
	    }

	    trigBits += pow(2,iTr-1);

	    if (iTr==1) nL1DoubleMuOpenMuons++;
	    if (iTr==3) nL2DoubleMu3Muons++;
	    if (iTr==6) nL3Mu12Muons++;
	  }
	}
	if (_fillTree)
	  this->fillTreeMuon(muon, muType, trigBits);
      }
    }
  }
  
  hGoodMuonsNoTrig->Fill(nGoodMuonsNoTrig);
  hGoodMuons->Fill(nGoodMuons);
  hL1DoubleMuOpen->Fill(nL1DoubleMuOpenMuons);
  hL2DoubleMu3->Fill(nL2DoubleMu3Muons);
  hL3Mu12->Fill(nL3Mu12Muons);

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

  if (_isHI) {
    myTree->Branch("nEP", &nEP, "nEP/I");
    myTree->Branch("nNfEP", &nNfEP, "nNfEP/I");
    myTree->Branch("rpAng", &rpAng, "rpAng[nEP]/F");
    myTree->Branch("rpSin", &rpSin, "rpSin[nEP]/F");
    myTree->Branch("rpCos", &rpCos, "rpCos[nEP]/F");
    myTree->Branch("NfRpAng", &NfRpAng, "NfRpAng[nNfEP]/F");
    myTree->Branch("NfRpSin", &NfRpSin, "NfRpSin[nNfEP]/F");
    myTree->Branch("NfRpCos", &NfRpCos, "NfRpCos[nNfEP]/F");
  }

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
  myTree->Branch("Reco_QQ_dca", Reco_QQ_dca,   "Reco_QQ_dca[Reco_QQ_size]/F");
  myTree->Branch("Reco_QQ_MassErr", Reco_QQ_MassErr,   "Reco_QQ_MassErr[Reco_QQ_size]/F");

  if (!_theMinimumFlag) {
    myTree->Branch("Reco_QQ_mupl_TrkMuArb", Reco_QQ_mupl_TrkMuArb,   "Reco_QQ_mupl_TrkMuArb[Reco_QQ_size]/O");
    myTree->Branch("Reco_QQ_mumi_TrkMuArb", Reco_QQ_mumi_TrkMuArb,   "Reco_QQ_mumi_TrkMuArb[Reco_QQ_size]/O");
    myTree->Branch("Reco_QQ_mupl_TMOneStaTight", Reco_QQ_mupl_TMOneStaTight, "Reco_QQ_mupl_TMOneStaTight[Reco_QQ_size]/O");
    myTree->Branch("Reco_QQ_mumi_TMOneStaTight", Reco_QQ_mumi_TMOneStaTight, "Reco_QQ_mumi_TMOneStaTight[Reco_QQ_size]/O");
    myTree->Branch("Reco_QQ_mupl_nMuValHits", Reco_QQ_mupl_nMuValHits,   "Reco_QQ_mupl_nMuValHits[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mumi_nMuValHits", Reco_QQ_mumi_nMuValHits,   "Reco_QQ_mumi_nMuValHits[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mupl_nTrkHits",Reco_QQ_mupl_nTrkHits, "Reco_QQ_mupl_nTrkHits[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mumi_nTrkHits",Reco_QQ_mumi_nTrkHits, "Reco_QQ_mumi_nTrkHits[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mupl_normChi2_inner",Reco_QQ_mupl_normChi2_inner, "Reco_QQ_mupl_normChi2_inner[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_normChi2_inner",Reco_QQ_mumi_normChi2_inner, "Reco_QQ_mumi_normChi2_inner[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_normChi2_global",Reco_QQ_mupl_normChi2_global, "Reco_QQ_mupl_normChi2_global[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_normChi2_global",Reco_QQ_mumi_normChi2_global, "Reco_QQ_mumi_normChi2_global[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_nPixWMea",Reco_QQ_mupl_nPixWMea, "Reco_QQ_mupl_nPixWMea[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mumi_nPixWMea",Reco_QQ_mumi_nPixWMea, "Reco_QQ_mumi_nPixWMea[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mupl_nTrkWMea",Reco_QQ_mupl_nTrkWMea, "Reco_QQ_mupl_nTrkWMea[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mumi_nTrkWMea",Reco_QQ_mumi_nTrkWMea, "Reco_QQ_mumi_nTrkWMea[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mupl_StationsMatched",Reco_QQ_mupl_StationsMatched, "Reco_QQ_mupl_StationsMatched[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mumi_StationsMatched",Reco_QQ_mumi_StationsMatched, "Reco_QQ_mumi_StationsMatched[Reco_QQ_size]/I");
    myTree->Branch("Reco_QQ_mupl_dxy",Reco_QQ_mupl_dxy, "Reco_QQ_mupl_dxy[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_dxy",Reco_QQ_mumi_dxy, "Reco_QQ_mumi_dxy[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_dxyErr",Reco_QQ_mupl_dxyErr, "Reco_QQ_mupl_dxyErr[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_dxyErr",Reco_QQ_mumi_dxyErr, "Reco_QQ_mumi_dxyErr[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_dz",Reco_QQ_mupl_dz, "Reco_QQ_mupl_dz[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_dz",Reco_QQ_mumi_dz, "Reco_QQ_mumi_dz[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_dzErr",Reco_QQ_mupl_dzErr, "Reco_QQ_mupl_dzErr[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_dzErr",Reco_QQ_mumi_dzErr, "Reco_QQ_mumi_dzErr[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_pt_inner",Reco_QQ_mupl_pt_inner, "Reco_QQ_mupl_pt_inner[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_pt_inner",Reco_QQ_mumi_pt_inner, "Reco_QQ_mumi_pt_inner[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_pt_global",Reco_QQ_mupl_pt_global, "Reco_QQ_mupl_pt_global[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_pt_global",Reco_QQ_mumi_pt_global, "Reco_QQ_mumi_pt_global[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_ptErr_inner",Reco_QQ_mupl_ptErr_inner, "Reco_QQ_mupl_ptErr_inner[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_ptErr_inner",Reco_QQ_mumi_ptErr_inner, "Reco_QQ_mumi_ptErr_inner[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mupl_ptErr_global",Reco_QQ_mupl_ptErr_global, "Reco_QQ_mupl_ptErr_global[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_mumi_ptErr_global",Reco_QQ_mumi_ptErr_global, "Reco_QQ_mumi_ptErr_global[Reco_QQ_size]/F");
  }

  myTree->Branch("Reco_mu_size", &Reco_mu_size,  "Reco_mu_size/I");
  myTree->Branch("Reco_mu_type", Reco_mu_type,   "Reco_mu_type[Reco_mu_size]/I");
  myTree->Branch("Reco_mu_charge", Reco_mu_charge,   "Reco_mu_charge[Reco_mu_size]/I");
  myTree->Branch("Reco_mu_4mom", "TClonesArray", &Reco_mu_4mom, 32000, 0);
  //  myTree->Branch("Reco_mu_3vec", "TClonesArray", &Reco_mu_3vec, 32000, 0);
  myTree->Branch("Reco_mu_trig", Reco_mu_trig,   "Reco_mu_trig[Reco_mu_size]/I");

  if (!_theMinimumFlag) {
    myTree->Branch("Reco_mu_TrkMuArb", Reco_mu_TrkMuArb,   "Reco_mu_TrkMuArb[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_TMOneStaTight", Reco_mu_TMOneStaTight, "Reco_mu_TMOneStaTight[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_nMuValHits", Reco_mu_nMuValHits,   "Reco_mu_nMuValHits[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nTrkHits",Reco_mu_nTrkHits, "Reco_mu_nTrkHits[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_normChi2_inner",Reco_mu_normChi2_inner, "Reco_mu_normChi2_inner[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_normChi2_global",Reco_mu_normChi2_global, "Reco_mu_normChi2_global[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_nPixWMea",Reco_mu_nPixWMea, "Reco_mu_nPixWMea[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nTrkWMea",Reco_mu_nTrkWMea, "Reco_mu_nTrkWMea[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_StationsMatched",Reco_mu_StationsMatched, "Reco_mu_StationsMatched[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_dxy",Reco_mu_dxy, "Reco_mu_dxy[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dxyErr",Reco_mu_dxyErr, "Reco_mu_dxyErr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dz",Reco_mu_dz, "Reco_mu_dz[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dzErr",Reco_mu_dzErr, "Reco_mu_dzErr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_pt_inner",Reco_mu_pt_inner, "Reco_mu_pt_inner[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_pt_global",Reco_mu_pt_global, "Reco_mu_pt_global[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_ptErr_inner",Reco_mu_ptErr_inner, "Reco_mu_ptErr_inner[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_ptErr_global",Reco_mu_ptErr_global, "Reco_mu_ptErr_global[Reco_mu_size]/F");
  }

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

  return;
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
  hL3Mu12         = new TH1F("hL3Mu12","hL3Mu12",10,0,10);
  
  hGoodMuonsNoTrig->Sumw2();
  hGoodMuons->Sumw2();
  hL1DoubleMuOpen->Sumw2();
  hL2DoubleMu3->Sumw2();
  hL3Mu12->Sumw2();

  // muons
  if (_combineCategories) 
    myRecoMuonHistos = new MyCommonHistoManager("RecoMuon");
  else {
    myRecoGlbMuonHistos = new MyCommonHistoManager("GlobalMuon");
    myRecoTrkMuonHistos = new MyCommonHistoManager("TrackerMuon");
  }

  // J/psi
  if (_combineCategories)
    myRecoJpsiHistos = new MyCommonHistoManager("RecoJpsi");
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
	if (_combineCategories) {
	  myRecoMuonHistos->Add(theAppendix);
	  myRecoMuonHistos->GetHistograms(theAppendix)->SetMassBinning(1,0.10,0.11);
	  myRecoMuonHistos->GetHistograms(theAppendix)->SetPtBinning(200,0.0,100.0);
	}
	else {
	  myRecoGlbMuonHistos->Add(theAppendix);
	  myRecoTrkMuonHistos->Add(theAppendix);
	  
	  myRecoGlbMuonHistos->GetHistograms(theAppendix)->SetMassBinning(1,0.10,0.11);
	  myRecoGlbMuonHistos->GetHistograms(theAppendix)->SetPtBinning(200,0.0,100.0);
	  
	  myRecoTrkMuonHistos->GetHistograms(theAppendix)->SetMassBinning(1,0.10,0.11);
	  myRecoTrkMuonHistos->GetHistograms(theAppendix)->SetPtBinning(200,0.0,100.0);
	}

	for (unsigned int l=0; l<theSign.size(); ++l) {
	  // J/psi
	  if (_combineCategories)
	    myRecoJpsiHistos->Add(theAppendix + "_" + theSign.at(l));
	  else {
	    myRecoJpsiGlbGlbHistos->Add(theAppendix + "_" + theSign.at(l));
	    myRecoJpsiGlbTrkHistos->Add(theAppendix + "_" + theSign.at(l));
	    myRecoJpsiTrkTrkHistos->Add(theAppendix + "_" + theSign.at(l));
	  }
	}
      }
    }
  }

  if (_combineCategories)
    myRecoMuonHistos->Print();
  else
    myRecoGlbMuonHistos->Print();

  hStats = new TH1F("hStats","hStats;;Number of Events",20,0,20);
  hStats->GetXaxis()->SetBinLabel(1,"All");
  for (int i=2; i< (int) theTriggerNames.size()+1; ++i) {
    hStats->GetXaxis()->SetBinLabel(i,theTriggerNames.at(i-1).c_str()); // event info
    hStats->GetXaxis()->SetBinLabel(i+NTRIGGERS,theTriggerNames.at(i-1).c_str()); // muon pair info
  }
  hStats->Sumw2();

  hCent = new TH1F("hCent","hCent;centrality bin;Number of Events",100,0,100);
  hCent->Sumw2();

  hPileUp = new TH1F("hPileUp","Number of Primary Vertices;n_{PV};counts", 50, 0, 50);
  hPileUp->Sumw2();

  hZVtx = new TH1F("hZVtx","Primary z-vertex distribution;z_{vtx} [cm];counts", 120, -30, 30);
  hZVtx->Sumw2();

  return;
}

void 
HiOniaAnalyzer::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
  //init HLTConfigProvider
  const std::string pro = _tagTriggerResults.process();
  bool changed = true;

  //bool init(const edm::Run& iRun, const edm::EventSetup& iSetup, const std::string& processName, bool& changed);
  hltConfigInit = false;
  if( hltConfig.init(iRun, iSetup, pro, changed) ) hltConfigInit = true;

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
  hL3Mu12->Write();

  if (_fillHistos) {
    // muons
    if (_combineCategories)
      myRecoGlbMuonHistos->Write(fOut);
    else {
      myRecoGlbMuonHistos->Write(fOut);
      myRecoTrkMuonHistos->Write(fOut);
    }

    // J/psi
    if (_combineCategories)
      myRecoJpsiHistos->Write(fOut);
    else {
      myRecoJpsiGlbGlbHistos->Write(fOut);
      myRecoJpsiGlbTrkHistos->Write(fOut);
      myRecoJpsiTrkTrkHistos->Write(fOut);
    }
  }

  return;
}

TLorentzVector
HiOniaAnalyzer::lorentzMomentum(const reco::Candidate::LorentzVector& p) {
  TLorentzVector res;
  res.SetPtEtaPhiM(p.pt(), p.eta(), p.phi(), p.mass());

  return res;
}

void
HiOniaAnalyzer::hltReport(const edm::Event &iEvent ,const edm::EventSetup& iSetup)
{
  std::map<std::string, bool> mapTriggernameToTriggerFired;
  std::map<std::string, unsigned int> mapTriggernameToHLTbit;

  for(std::vector<std::string>::const_iterator it=theTriggerNames.begin(); it !=theTriggerNames.end(); ++it){
    mapTriggernameToTriggerFired[*it]=false;
    mapTriggernameToHLTbit[*it]=1000;
  }

  // HLTConfigProvider
  if ( hltConfigInit ) {
    //! Use HLTConfigProvider
    const unsigned int n= hltConfig.size();
    for (std::map<std::string, unsigned int>::iterator it = mapTriggernameToHLTbit.begin(); it != mapTriggernameToHLTbit.end(); it++) {
      unsigned int triggerIndex= hltConfig.triggerIndex( it->first );
      if (triggerIndex >= n) {
	//	std::cout << "[HiOniaAnalyzer::hltReport] --- TriggerName " << it->first << " not available in config!" << std::endl;
      }
      else {
	it->second= triggerIndex;
	//	std::cout << "[HiOniaAnalyzer::hltReport] --- TriggerName " << it->first << " available in config!" << std::endl;
      }
    }
  }
    
  // Get Trigger Results
  try {
    iEvent.getByLabel( _tagTriggerResults, collTriggerResults );
    //    cout << "[HiOniaAnalyzer::hltReport] --- J/psi TriggerResult is present in current event" << endl;
  }
  catch(...) {
    //    cout << "[HiOniaAnalyzer::hltReport] --- J/psi TriggerResults NOT present in current event" << endl;
  }
  if ( collTriggerResults.isValid() ){
    //    cout << "[HiOniaAnalyzer::hltReport] --- J/psi TriggerResults IS valid in current event" << endl;
      
    // loop over Trigger Results to check if paths was fired
    for(std::vector< std::string >::iterator itHLTNames= theTriggerNames.begin(); itHLTNames != theTriggerNames.end(); itHLTNames++){
      const std::string triggerPathName =  *itHLTNames;
      if ( mapTriggernameToHLTbit[triggerPathName] < 1000 ) {
	if (collTriggerResults->accept( mapTriggernameToHLTbit[triggerPathName] ) ){
	  mapTriggerNameToIntFired_[triggerPathName] = 3;
	}

	//-------prescale factor------------
	/*
	if (!_isMC) {
	  const std::pair<int,int> prescales(hltConfig.prescaleValues(iEvent,iSetup,triggerPathName));
	  mapTriggerNameToPrescaleFac_[triggerPathName] = prescales.first * prescales.second;
	}
	*/
      }
    }
  } else cout << "[HiOniaAnalyzer::hltReport] --- TriggerResults NOT valid in current event" << endl;

  return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiOniaAnalyzer);
