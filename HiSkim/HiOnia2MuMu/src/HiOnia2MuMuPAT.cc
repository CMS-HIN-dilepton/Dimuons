#include "HiSkim/HiOnia2MuMu/interface/HiOnia2MuMuPAT.h"

//Headers for the data items
#include <DataFormats/TrackReco/interface/TrackFwd.h>
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/Common/interface/View.h>
#include <DataFormats/HepMCCandidate/interface/GenParticle.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/VertexReco/interface/VertexFwd.h>

//Headers for services and tools
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "TMath.h"
#include "Math/VectorUtil.h"
#include "TVector3.h"
#include "HiSkim/HiOnia2MuMu/interface/VertexReProducer.h"


HiOnia2MuMuPAT::HiOnia2MuMuPAT(const edm::ParameterSet& iConfig):
  muons_(iConfig.getParameter<edm::InputTag>("muons")),
  thebeamspot_(iConfig.getParameter<edm::InputTag>("beamSpotTag")),
  thePVs_(iConfig.getParameter<edm::InputTag>("primaryVertexTag")),
  higherPuritySelection_(iConfig.getParameter<std::string>("higherPuritySelection")),
  lowerPuritySelection_(iConfig.getParameter<std::string>("lowerPuritySelection")),
  dimuonSelection_(iConfig.existsAs<std::string>("dimuonSelection") ? iConfig.getParameter<std::string>("dimuonSelection") : ""),
  addCommonVertex_(iConfig.getParameter<bool>("addCommonVertex")),
  addMuonlessPrimaryVertex_(iConfig.getParameter<bool>("addMuonlessPrimaryVertex")),
  resolveAmbiguity_(iConfig.getParameter<bool>("resolvePileUpAmbiguity")),
  addMCTruth_(iConfig.getParameter<bool>("addMCTruth"))
{  
    produces<pat::CompositeCandidateCollection>();  
}


HiOnia2MuMuPAT::~HiOnia2MuMuPAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HiOnia2MuMuPAT::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  using namespace edm;
  using namespace std;
  using namespace reco;
  typedef Candidate::LorentzVector LorentzVector;

  std::auto_ptr<pat::CompositeCandidateCollection> oniaOutput(new pat::CompositeCandidateCollection);
  
  Vertex thePrimaryV;
  Vertex theBeamSpotV; 

  Handle<BeamSpot> theBeamSpot;
  iEvent.getByLabel(thebeamspot_,theBeamSpot);
  BeamSpot bs = *theBeamSpot;
  theBeamSpotV = Vertex(bs.position(), bs.covariance3D());

  Handle<VertexCollection> priVtxs;
  iEvent.getByLabel(thePVs_, priVtxs);
  if ( priVtxs->begin() != priVtxs->end() ) {
    thePrimaryV = Vertex(*(priVtxs->begin()));
  }
  else {
    thePrimaryV = Vertex(bs.position(), bs.covariance3D());
  }

  Handle< View<pat::Muon> > muons;
  iEvent.getByLabel(muons_,muons);

  edm::ESHandle<TransientTrackBuilder> theTTBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);
  KalmanVertexFitter vtxFitter;

  // JPsi candidates only from muons
  for(View<pat::Muon>::const_iterator it = muons->begin(), itend = muons->end(); it != itend; ++it){
    // both must pass low quality
    if(!lowerPuritySelection_(*it)) continue; 
    for(View<pat::Muon>::const_iterator it2 = it+1; it2 != itend;++it2){
      // both must pass low quality
      if(!lowerPuritySelection_(*it2)) continue; 
      // one must pass tight quality
      if (!(higherPuritySelection_(*it) || higherPuritySelection_(*it2))) continue;

      pat::CompositeCandidate myCand;
      vector<TransientVertex> pvs;

      // ---- no explicit order defined ----
      myCand.addDaughter(*it, "muon1");
      myCand.addDaughter(*it2,"muon2");	

      // ---- define and set candidate's 4momentum  ----  
      LorentzVector jpsi = it->p4() + it2->p4();
      myCand.setP4(jpsi);
      myCand.setCharge(it->charge()+it2->charge());

      // ---- apply the dimuon cut ----
      if(!dimuonSelection_(myCand)) continue;

      // ---- fit vertex using Tracker tracks (if they have tracks) ----
      if (it->track().isNonnull() && it2->track().isNonnull()) {

	// Make a PV with everything else
	if (addMuonlessPrimaryVertex_) {
	  VertexReProducer revertex(priVtxs, iEvent);
	  Handle<TrackCollection> pvtracks;   
	  iEvent.getByLabel(revertex.inputTracks(),   pvtracks);
	  Handle<BeamSpot>        pvbeamspot; 
	  iEvent.getByLabel(revertex.inputBeamSpot(), pvbeamspot);
	  if (pvbeamspot.id() != theBeamSpot.id()) edm::LogWarning("Inconsistency") << "The BeamSpot used for PV reco is not the same used in this analyzer.";
	  // I need to go back to the reco::Muon object, as the TrackRef in the pat::Muon can be an embedded ref.
	  const reco::Muon *rmu1 = dynamic_cast<const reco::Muon *>(it->originalObject());
	  const reco::Muon *rmu2 = dynamic_cast<const reco::Muon *>(it2->originalObject());
	  // check that muons are truly from reco::Muons (and not, e.g., from PF Muons)
	  // also check that the tracks really come from the track collection used for the BS
	  if (rmu1 != 0 && rmu2 != 0 && rmu1->track().id() == pvtracks.id() && rmu2->track().id() == pvtracks.id()) { 
	    // Prepare the collection of tracks without the two muon tracks
	    TrackCollection muonLess;
	    muonLess.reserve(pvtracks->size());
	    for (size_t i = 0, n = pvtracks->size(); i < n; ++i) {
	      if (i == rmu1->track().key()) continue;
	      if (i == rmu2->track().key()) continue;
	      muonLess.push_back((*pvtracks)[i]);
	    }
	    pvs = revertex.makeVertices(muonLess, *pvbeamspot, iSetup) ;
	    if (!pvs.empty()) {
	      Vertex muonLessPV = Vertex(pvs.front());
	      // myCand.addUserData("muonlessPV",muonLessPV);
              thePrimaryV = muonLessPV;
	    }
	  }
	}
	
	vector<TransientTrack> t_tks;
	t_tks.push_back(theTTBuilder->build(*it->track()));  // pass the reco::Track, not  the reco::TrackRef (which can be transient)
	t_tks.push_back(theTTBuilder->build(*it2->track())); // otherwise the vertex will have transient refs inside.
	TransientVertex myVertex = vtxFitter.vertex(t_tks);
	if (myVertex.isValid()) {
	  float vChi2 = myVertex.totalChiSquared();
	  float vNDF  = myVertex.degreesOfFreedom();
	  float vProb(TMath::Prob(vChi2,(int)vNDF));
	  
	  myCand.addUserFloat("vNChi2",vChi2/vNDF);
	  myCand.addUserFloat("vProb",vProb);
	  
	  TVector3 vtx;
          TVector3 pvtx;
          VertexDistanceXY vdistXY;

	  vtx.SetXYZ(myVertex.position().x(),myVertex.position().y(),0);
	  TVector3 pperp(jpsi.px(), jpsi.py(), 0);
	  AlgebraicVector3 vpperp(pperp.x(), pperp.y(), 0.);

	  if (resolveAmbiguity_) {
            float minDz = 999999.;
	    if (!addMuonlessPrimaryVertex_) {
	      for(VertexCollection::const_iterator itv = priVtxs->begin(), itvend = priVtxs->end(); itv != itvend; ++itv){
		float deltaZ = fabs(myVertex.position().z() - itv->position().z()) ;
		if ( deltaZ < minDz ) {
		  minDz = deltaZ;    
		  thePrimaryV = Vertex(*itv);
		}
	      }
	    } else {
	      for(vector<TransientVertex>::iterator itv2 = pvs.begin(), itvend2 = pvs.end(); itv2 != itvend2; ++itv2){
		float deltaZ = fabs(myVertex.position().z() - itv2->position().z()) ;
		if ( deltaZ < minDz ) {
		  minDz = deltaZ;    
		  Vertex muonLessPV = Vertex(*itv2); 
		  thePrimaryV = muonLessPV;
		}
	      }
	    }
	  } 
         
          if (addMuonlessPrimaryVertex_) {
            myCand.addUserData("muonlessPV",Vertex(thePrimaryV));
	  } else {
	    myCand.addUserData("PVwithmuons",thePrimaryV);
	  }

	  // lifetime using PV
          pvtx.SetXYZ(thePrimaryV.position().x(),thePrimaryV.position().y(),0);
	  TVector3 vdiff = vtx - pvtx;
	  double cosAlpha = vdiff.Dot(pperp)/(vdiff.Perp()*pperp.Perp());
	  Measurement1D distXY = vdistXY.distance(Vertex(myVertex), thePrimaryV);
	  double ctauPV = distXY.value()*cosAlpha*3.09688/pperp.Perp();
	  GlobalError v1e = (Vertex(myVertex)).error();
	  GlobalError v2e = thePrimaryV.error();
          AlgebraicSymMatrix33 vXYe = v1e.matrix()+ v2e.matrix();
	  double ctauErrPV = sqrt(ROOT::Math::Similarity(vpperp,vXYe))*3.09688/(pperp.Perp2());
	  
	  myCand.addUserFloat("ppdlPV",ctauPV);
          myCand.addUserFloat("ppdlErrPV",ctauErrPV);
	  myCand.addUserFloat("cosAlpha",cosAlpha);

	  // lifetime using BS
          pvtx.SetXYZ(theBeamSpotV.position().x(),theBeamSpotV.position().y(),0);
	  vdiff = vtx - pvtx;
	  cosAlpha = vdiff.Dot(pperp)/(vdiff.Perp()*pperp.Perp());
	  distXY = vdistXY.distance(Vertex(myVertex), theBeamSpotV);
	  double ctauBS = distXY.value()*cosAlpha*3.09688/pperp.Perp();
	  GlobalError v1eB = (Vertex(myVertex)).error();
	  GlobalError v2eB = theBeamSpotV.error();
          AlgebraicSymMatrix33 vXYeB = v1eB.matrix()+ v2eB.matrix();
	  double ctauErrBS = sqrt(ROOT::Math::Similarity(vpperp,vXYeB))*3.09688/(pperp.Perp2());
	  
	  myCand.addUserFloat("ppdlBS",ctauBS);
          myCand.addUserFloat("ppdlErrBS",ctauErrBS);
	  
	  if (addCommonVertex_) {
	    myCand.addUserData("commonVertex",Vertex(myVertex));
	  }
	} else {
	  myCand.addUserFloat("vNChi2",-1);
	  myCand.addUserFloat("vProb", -1);
	  myCand.addUserFloat("ppdlPV",-100);
          myCand.addUserFloat("ppdlErrPV",-100);
	  myCand.addUserFloat("cosAlpha",-100);
	  myCand.addUserFloat("ppdlBS",-100);
          myCand.addUserFloat("ppdlErrBS",-100);
	  if (addCommonVertex_) {
	    myCand.addUserData("commonVertex",Vertex());
	  }
	  if (addMuonlessPrimaryVertex_) {
            myCand.addUserData("muonlessPV",Vertex());
	  } else {
	    myCand.addUserData("PVwithmuons",Vertex());
	  }
	}	
      }
      
      // ---- MC Truth, if enabled ----
      if (addMCTruth_) {
	reco::GenParticleRef genMu1 = it->genParticleRef();
	reco::GenParticleRef genMu2 = it2->genParticleRef();
	if (genMu1.isNonnull() && genMu2.isNonnull()) {
	  if (genMu1->numberOfMothers()>0 && genMu2->numberOfMothers()>0){
	    reco::GenParticleRef mom1 = genMu1->motherRef();
	    reco::GenParticleRef mom2 = genMu2->motherRef();
	    if (mom1.isNonnull() && (mom1 == mom2)) {
	      myCand.setGenParticleRef(mom1); // set
	      myCand.embedGenParticle();      // and embed
	      std::pair<int, float> MCinfo = findJpsiMCInfo(mom1);
	      myCand.addUserInt("momPDGId",MCinfo.first);
	      myCand.addUserFloat("ppdlTrue",MCinfo.second);
	    } else {
	      myCand.addUserInt("momPDGId",0);
	      myCand.addUserFloat("ppdlTrue",-99.);
	    }
	  } else {
	    Handle<GenParticleCollection>theGenParticles;
	    iEvent.getByLabel("genParticles", theGenParticles);
	    if (theGenParticles.isValid()){
	      for(size_t iGenParticle=0; iGenParticle<theGenParticles->size();++iGenParticle) {
		const Candidate & genCand = (*theGenParticles)[iGenParticle];
		if (genCand.pdgId()==443 || genCand.pdgId()==100443 || 
		    genCand.pdgId()==553 || genCand.pdgId()==100553 || genCand.pdgId()==200553) {
		  reco::GenParticleRef mom1(theGenParticles,iGenParticle);
		  myCand.setGenParticleRef(mom1);
		  myCand.embedGenParticle();
		  std::pair<int, float> MCinfo = findJpsiMCInfo(mom1);
		  myCand.addUserInt("momPDGId",MCinfo.first);
		  myCand.addUserFloat("ppdlTrue",MCinfo.second);
		}
	      }
	    } else {
	      myCand.addUserInt("momPDGId",0);
	      myCand.addUserFloat("ppdlTrue",-99.);
	    }
	  }
	} else {
	  myCand.addUserInt("momPDGId",0);
	  myCand.addUserFloat("ppdlTrue",-99.);
	}
      }

      // ---- Push back output ----  
      oniaOutput->push_back(myCand);
    }
  }

  std::sort(oniaOutput->begin(),oniaOutput->end(),pTComparator_);

  iEvent.put(oniaOutput);

}


bool 
HiOnia2MuMuPAT::isAbHadron(int pdgID) {

  if (abs(pdgID) == 511 || abs(pdgID) == 521 || abs(pdgID) == 531 || abs(pdgID) == 5122) return true;
  return false;

}

bool 
HiOnia2MuMuPAT::isAMixedbHadron(int pdgID, int momPdgID) {

  if ((abs(pdgID) == 511 && abs(momPdgID) == 511 && pdgID*momPdgID < 0) || 
      (abs(pdgID) == 531 && abs(momPdgID) == 531 && pdgID*momPdgID < 0)) 
      return true;
  return false;

}

std::pair<int, float>  
HiOnia2MuMuPAT::findJpsiMCInfo(reco::GenParticleRef genJpsi) {

  int momJpsiID = 0;
  float trueLife = -99.;

  TVector3 trueVtx(0.0,0.0,0.0);
  TVector3 trueP(0.0,0.0,0.0);
  TVector3 trueVtxMom(0.0,0.0,0.0);

  trueVtx.SetXYZ(genJpsi->vertex().x(),genJpsi->vertex().y(),genJpsi->vertex().z());
  trueP.SetXYZ(genJpsi->momentum().x(),genJpsi->momentum().y(),genJpsi->momentum().z());
	    
  bool aBhadron = false;
  reco::GenParticleRef Jpsimom = genJpsi->motherRef();       // find mothers
  if (Jpsimom.isNull()) {
    std::pair<int, float> result = std::make_pair(momJpsiID, trueLife);
    return result;
  } else {
    reco::GenParticleRef Jpsigrandmom = Jpsimom->motherRef();
    if (isAbHadron(Jpsimom->pdgId())) {
      if (Jpsigrandmom.isNonnull() && isAMixedbHadron(Jpsimom->pdgId(),Jpsigrandmom->pdgId())) {
	momJpsiID = Jpsigrandmom->pdgId();
	trueVtxMom.SetXYZ(Jpsigrandmom->vertex().x(),Jpsigrandmom->vertex().y(),Jpsigrandmom->vertex().z());
      } else {
	momJpsiID = Jpsimom->pdgId();
	trueVtxMom.SetXYZ(Jpsimom->vertex().x(),Jpsimom->vertex().y(),Jpsimom->vertex().z());
      }
      aBhadron = true;
    } else {
      if (Jpsigrandmom.isNonnull() && isAbHadron(Jpsigrandmom->pdgId())) {
	reco::GenParticleRef JpsiGrandgrandmom = Jpsigrandmom->motherRef();
	if (JpsiGrandgrandmom.isNonnull() && isAMixedbHadron(Jpsigrandmom->pdgId(),JpsiGrandgrandmom->pdgId())) {
	  momJpsiID = JpsiGrandgrandmom->pdgId();
	  trueVtxMom.SetXYZ(JpsiGrandgrandmom->vertex().x(),JpsiGrandgrandmom->vertex().y(),JpsiGrandgrandmom->vertex().z());
	} else {
	  momJpsiID = Jpsigrandmom->pdgId();
	  trueVtxMom.SetXYZ(Jpsigrandmom->vertex().x(),Jpsigrandmom->vertex().y(),Jpsigrandmom->vertex().z());
	}
	aBhadron = true;
      }
    }
    if (!aBhadron) {
      momJpsiID = Jpsimom->pdgId();
      trueVtxMom.SetXYZ(Jpsimom->vertex().x(),Jpsimom->vertex().y(),Jpsimom->vertex().z()); 
    }
  } 

  TVector3 vdiff = trueVtx - trueVtxMom;
  trueLife = vdiff.Perp()*3.09688/trueP.Perp();

  // std::cout << "1 " << momJpsiID << " 2 " << trueLife << std::endl; 

  std::pair<int, float> result = std::make_pair(momJpsiID, trueLife);
  return result;

}

// ------------ method called once each job just before starting event loop  ------------
void 
HiOnia2MuMuPAT::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HiOnia2MuMuPAT::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiOnia2MuMuPAT);
