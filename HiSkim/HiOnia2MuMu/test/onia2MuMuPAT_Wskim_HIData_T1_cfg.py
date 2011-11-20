# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms

# set up process
process = cms.Process("Onia2MuMuPAT")

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_44_V10::All'

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("hiCentrality")
    )

# BSC or HF coincidence (masked unprescaled L1 bits)
process.load('L1Trigger.Skimmer.l1Filter_cfi')
process.bscOrHfCoinc = process.l1Filter.clone(
    algorithms = cms.vstring('L1_HcalHfCoincPmORBscMinBiasThresh1_BptxAND_instance1', 'L1_NotBsc2_BscMinBiasOR', 'L1_HcalHfCoincidencePm')
    )
    

# Common offline event selection
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

# HLT dimuon trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltOniaHI.HLTPaths = ["HLT_HIL1DoubleMu0_HighQ_v1",
                              "HLT_HIL2Mu3_NHitQ_v1",
                              "HLT_HIL2Mu7_v1","HLT_HIL2Mu15_v1",
                              "HLT_HIL2DoubleMu0_NHitQ_v1",
                              "HLT_HIL2DoubleMu3_v1",
                              "HLT_HIL3Mu3_v1",
                              "HLT_HIL3DoubleMuOpen_v1","HLT_HIL3DoubleMuOpen_Mgt2_OS_NoCowboy_v1"
                              ]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *

onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=False, HLT="HLT", Filter=True)

process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = False
process.onia2MuMuPatGlbGlb.resolvePileUpAmbiguity = False

process.source.fileNames = cms.untracked.vstring(
    '/store/hidata/HIRun2011/HIDiMuon/RECO/PromptReco-v1/000/181/604/6224AF39-D50F-E111-89BE-003048D2BBF0.root'
    )

# filter on lumisections
#from HiSkim.HiOnia2MuMu.goodLumiSectionListHI_cfi import *
#process.source.lumisToProcess = goodLumisToProcess

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.outOnia2MuMu.fileName = cms.untracked.string( 'onia2MuMuPAT.root' )

# add event plane information
process.load("RecoHI.HiEvtPlaneAlgos.HiEvtPlane_cfi")
process.ProdEvtPlane = cms.Path(process.hiEvtPlane)


# W part
process.goodMuons = cms.EDFilter("MuonSelector",
                                 src = cms.InputTag("muons"),
                                 cut = cms.string('pt > 19.0 && (isGlobalMuon && isTrackerMuon) && abs(innerTrack.dxy)<1.0'),
                                 filter = cms.bool(True)
                                 )

process.goodPatMuons = cms.EDFilter("PATMuonSelector",
                                    src = cms.InputTag("patMuonsWithTrigger"),
                                    cut = cms.string("pt > 19.0 && (isGlobalMuon && isTrackerMuon) && abs(dB)<1.0 && !triggerObjectMatchesByPath('HLT_HIL2Mu15_v1').empty()"),
                                    filter = cms.bool(True)
                                    )


process.WMuNuSkimPath = cms.Path(process.goodMuons)
process.WMuNuPATSkimPath = cms.Path(process.patMuonsWithTriggerSequence * process.goodPatMuons)


# Choose collections for output
process.load("Configuration.EventContent.EventContentHeavyIons_cff")
process.EWK_WMuNuEventContent = cms.PSet(outputCommands = process.RECOEventContent.outputCommands)
process.EWK_WMuNuEventContent.outputCommands.extend(cms.untracked.vstring('keep *_goodMuons_*_*',
                                                                          'keep patMuons_goodPatMuons_*_*'))

process.outWMuNuSkim = cms.OutputModule("PoolOutputModule",
                                        process.EWK_WMuNuEventContent,
                                        dropMetaData = cms.untracked.string("DROPPED"), 
                                        SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('WMuNuSkimPath','WMuNuPATSkimPath')),
                                        fileName = cms.untracked.string('WMuNu_RECO.root')
                                        )

process.e = cms.EndPath(process.outOnia2MuMu + process.outTnP + process.outWMuNuSkim)


process.schedule = cms.Schedule(process.ProdEvtPlane,process.Onia2MuMuPAT,
                                process.TagAndProbeSta, process.TagAndProbeMuID, process.TagAndProbeTrig,
                                process.WMuNuSkimPath, process.WMuNuPATSkimPath,
                                process.e)


