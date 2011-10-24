# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

# set up process
process = cms.Process("Onia2MuMuPAT")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
options.outputFile = 'onia2MuMuPAT_MC.root'
options.inputFiles = 'rfio:/castor/cern.ch/user/s/silvest/rootfiles/MCppWithHIalgo/Upsilon1S/root_last/PYTHIA6_Upsilon1SWithFSR_tuneD6T_2760GeV_job71.root'
options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'START39_V7HI::All'
#process.GlobalTag.globaltag = 'START311_V2::All'
process.GlobalTag.globaltag = 'START311_V2A::All' # for pp MC with HiReReco


# produce missing l1extraParticles
process.load('Configuration.StandardSequences.L1Reco_cff')
process.L1Reco_step = cms.Path(process.l1extraParticles)

from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string("Hydjet_Bass"), # different from data!
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
process.hltOniaHI.HLTPaths = ["HLT_DoubleMu3_v3","HLT_L1DoubleMu0_v1","HLT_L2DoubleMu0_v2",
                              "HLT_Mu0_v3","HLT_Mu3_v3","HLT_Mu5_v3","HLT_L1SingleMuOpen_v1"]
#process.hltOniaHI.HLTPaths = ["HLT_HIL1DoubleMuOpen","HLT_HIL2DoubleMu0","HLT_HIL2DoubleMu3",
#                              "HLT_HIL1SingleMu3","HLT_HIL1SingleMu5","HLT_HIL1SingleMu7",
#                              "HLT_HIL2Mu20","HLT_HIL2Mu3","HLT_HIL2Mu5Tight"]

#process.hltOniaHI.HLTPaths = ["HLT_HIL1DoubleMuOpen_Core","HLT_HIL2DoubleMu0_Core","HLT_HIL2DoubleMu3_Core",
#                              "HLT_HIL1SingleMu3_Core","HLT_HIL1SingleMu5_Core","HLT_HIL1SingleMu7_Core",
#                              "HLT_HIL2Mu20_Core","HLT_HIL2Mu3_Core","HLT_HIL2Mu5Tight_Core"]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True

process.MinBiasCounterEarly = cms.EDAnalyzer('MinBiasCounter',
                                             TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                             triggerName = cms.vstring("HLT_HIMinBiasHF","HLT_HIMinBiasBSC"),
                                             # triggerName = cms.vstring("HLT_HIMinBiasHF_Core","HLT_HIMinBiasBSC_Core"),
                                             histFileName = cms.string("MinBiasCentralityEarly_MC_Histo.root")
                                             )

process.MinBiasCounter = cms.EDAnalyzer('MinBiasCounter',
                                        TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                        triggerName = cms.vstring("HLT_HIMinBiasHfOrBSC"),
                                        # triggerName = cms.vstring("HLT_HIMinBiasHfOrBSC_Core"),
                                        histFileName = cms.string("MinBiasCentrality_MC_Histo.root")
                                        )

process.DoubleMuOpenCounter = cms.EDAnalyzer('MinBiasCounter',
                                             TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                             triggerName = cms.vstring("HLT_HIL1DoubleMuOpen"),
                                             # triggerName = cms.vstring("HLT_HIL1DoubleMuOpen_Core"),
                                             histFileName = cms.string("DoubleMuOpenCentrality_MC_Histo.root")
                                             )

#process.MinBiasEarlyPath = cms.Path(process.hltMinBiasHFOrBSCEarly *
#                                    process.collisionEventSelection *
#                                    process.MinBiasCounterEarly
#                                    )

#process.MinBiasPath = cms.Path(process.hltMinBiasHFOrBSC *
#                               process.collisionEventSelection *
#                               process.MinBiasCounter
#                               )

from HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cff import *

onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=True, HLT="HLT", Filter=False)

process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = False
process.onia2MuMuPatGlbGlb.resolvePileUpAmbiguity = False

# want to avoid this
#process.muonL1Info.matched = "hltL1extraParticles"

process.Onia2MuMuPAT.remove(process.DoubleMuOpenCounter)
process.Onia2MuMuPAT.remove(process.hltOniaHI)

process.genMuons.src = "hiGenParticles"

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.source.fileNames = cms.untracked.vstring(
    options.inputFiles
    # 'file:/tmp/camelia/output.root'
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.outOnia2MuMu.fileName = cms.untracked.string( options.outputFile )
process.outSta.fileName = cms.untracked.string('tnpSta_MC.root')
process.outMuID.fileName = cms.untracked.string('tnpMuID_MC.root')
process.outTrig.fileName = cms.untracked.string('tnpTrig_MC.root')

#disable TnP path temporarily
#process.TagAndProbeSta = cms.Path()
#process.TagAndProbeMuID = cms.Path()
#process.TagAndProbeTrig = cms.Path()

#process.e.remove(process.outSta)
#process.e.remove(process.outMuID)
#process.e.remove(process.outTrig)

process.schedule = cms.Schedule(process.L1Reco_step,process.Onia2MuMuPAT,
                                process.TagAndProbeSta, process.TagAndProbeMuID, process.TagAndProbeTrig,
                                process.e)

# for Jorge
process.outOnia2MuMu.outputCommands.extend(cms.untracked.vstring('keep *_simMuon*_*_*',
                                                                 'keep *_dt4DSegments*_*_*',
                                                                 'keep *_cscSegments*_*_*',
                                                                 'keep *_dt1DSegments*_*_*',
                                                                 'keep PSimHits_g4SimHits_*_*',
                                                                 'keep DTChamberIdDTRecSegment4DsOwnedRangeMap_dt4DSegments__*',
                                                                 'keep CSCDetIdCSCSegmentsOwnedRangeMap_cscSegments__*',
                                                                 'keep DTLayerIdDTRecHit1DPairsOwnedRangeMap_dt1DRecHits__*')
                                           )
