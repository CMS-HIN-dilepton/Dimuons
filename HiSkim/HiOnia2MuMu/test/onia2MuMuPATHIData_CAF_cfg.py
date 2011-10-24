# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms

# set up process
process = cms.Process("Onia2MuMuPAT")

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR10_P_V12::All'

from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("hiCentrality")
    )

# BSC or HF coincidence (masked unprescaled L1 bits)
process.load('L1Trigger.Skimmer.l1Filter_cfi')
process.bscOrHfCoinc = process.l1Filter.clone(
    algorithms = cms.vstring('L1_BscMinBiasThreshold1', 'L1_HcalHfCoincidencePm')
    )
    

# Common offline event selection
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

# HLT dimuon trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
# early runs (to be done)
process.hltMinBiasHFOrBSCEarly = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMinBiasHFOrBSCEarly.HLTPaths = ["HLT_HIMinBiasHF","HLT_HIMinBiasBSC"]
#process.hltMinBiasHFOrBSCEarly.HLTPaths = ["HLT_HIMinBiasHF_Core","HLT_HIMinBiasBSC_Core"]
process.hltMinBiasHFOrBSCEarly.throw = False
process.hltMinBiasHFOrBSCEarly.andOr = True

process.hltMinBiasHFOrBSC = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHfOrBSC"]
#process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHfOrBSC_Core"]
process.hltMinBiasHFOrBSC.throw = False


process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltOniaHI.HLTPaths = ["HLT_HIL1DoubleMuOpen","HLT_HIL2DoubleMu0","HLT_HIL2DoubleMu3",
                              "HLT_HIL1SingleMu3","HLT_HIL1SingleMu5","HLT_HIL1SingleMu7",
                              "HLT_HIL2Mu20","HLT_HIL2Mu3","HLT_HIL2Mu5Tight"]

#process.hltOniaHI.HLTPaths = ["HLT_HIL1DoubleMuOpen_Core","HLT_HIL2DoubleMu0_Core","HLT_HIL2DoubleMu3_Core",
#                              "HLT_HIL1SingleMu3_Core","HLT_HIL1SingleMu5_Core","HLT_HIL1SingleMu7_Core",
#                              "HLT_HIL2Mu20_Core","HLT_HIL2Mu3_Core","HLT_HIL2Mu5Tight_Core"]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True

process.MinBiasCounterEarly = cms.EDAnalyzer('MinBiasCounter',
                                             TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                             triggerName = cms.vstring("HLT_HIMinBiasHF","HLT_HIMinBiasBSC"),
                                             histFileName = cms.string("MinBiasCentralityEarly_Histo.root")
                                             )

process.MinBiasCounter = cms.EDAnalyzer('MinBiasCounter',
                                        TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                        triggerName = cms.vstring("HLT_HIMinBiasHfOrBSC"),
                                        histFileName = cms.string("MinBiasCentrality_Histo.root")
                                        )

process.DoubleMuOpenCounter = cms.EDAnalyzer('MinBiasCounter',
                                             TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                             triggerName = cms.vstring("HLT_HIL1DoubleMuOpen"),
                                             # triggerName = cms.vstring("HLT_HIL1DoubleMuOpen_Core"),
                                             histFileName = cms.string("DoubleMuOpenCentrality_Histo.root")
                                             )

process.MinBiasEarlyPath = cms.Path(process.hltMinBiasHFOrBSCEarly *
                                    process.collisionEventSelection *
                                    process.MinBiasCounterEarly
                                    )

process.MinBiasPath = cms.Path(process.hltMinBiasHFOrBSC *
                               process.collisionEventSelection *
                               process.MinBiasCounter
                               )

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *

onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=False, HLT="HLT", Filter=True)

process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = False
process.onia2MuMuPatGlbGlb.resolvePileUpAmbiguity = False

process.source.fileNames = cms.untracked.vstring(
    'rfio:/castor/cern.ch/cms/store/caf/user/silvest/HFCoinc_PrimaryVTX_DoubleMu_1GLB_CorePhysics_RECO-allv3/DiMuon_9_1_8He.root'
    )

# filter on lumisections
from HiSkim.HiOnia2MuMu.goodLumiSectionListHI_cfi import *
process.source.lumisToProcess = goodLumisToProcess

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.outOnia2MuMu.fileName = cms.untracked.string( 'onia2MuMuPAT.root' )


