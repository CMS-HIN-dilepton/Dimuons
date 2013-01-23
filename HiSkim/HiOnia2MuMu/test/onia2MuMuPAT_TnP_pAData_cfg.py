# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms

# set up process
process = cms.Process("Onia2MuMuPAT")

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_E_V33::All'
#process.GlobalTag.globaltag = 'GR_P_V43D::All'

# Common offline event selection
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

# Centrality for pPb
process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')

# HLT dimuon trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltOniaHI.HLTPaths = ["HLT_PAL1DoubleMuOpen_v*",
                              "HLT_PAL1DoubleMu0_HighQ_v*",
                              "HLT_PAL2DoubleMu3_v*",
                              "HLT_PAMu3_v*",
                              "HLT_PAMu7_v*",
                              "HLT_PAMu12_v*",
                              "HLT_PAPixelTrackMultiplicity100_L2DoubleMu3_v*"
                              ]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True
process.hltOniaHI.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")


from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=False, HLT="HLT", Filter=True)

from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import changeRecoMuonInput
changeRecoMuonInput(process, "mergedMuons")

process.patMuonSequence = cms.Sequence(
    #process.scrapingFilter *
    process.mergedMuons *
    process.patMuonsWithTriggerSequence
    )

#process.onia2MuMuPatTrkTrk.addMuonlessPrimaryVertex = False
#process.onia2MuMuPatTrkTrk.resolvePileUpAmbiguity = False

process.source.fileNames = cms.untracked.vstring(
    '/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/DE41AB14-1964-E211-AD16-001D09F2A465.root'    )

# filter on lumisections
#from HiSkim.HiOnia2MuMu.goodLumiSectionListHI_cfi import *
#process.source.lumisToProcess = goodLumisToProcess

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.outTnP.fileName = cms.untracked.string( 'tnp.root' )
process.pACentrality_step = cms.Path(process.hltOniaHI*process.PAcollisionEventSelection*process.pACentrality)

process.e = cms.EndPath(process.outTnP)

process.schedule = cms.Schedule(process.pACentrality_step,
                                process.TagAndProbeTrig,
                                process.TagAndProbeSta,
                                process.TagAndProbeMuID,
                                process.e)


