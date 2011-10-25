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
#options.inputFiles = 'rfio:/castor/cern.ch/user/m/mironov/cmssw440patch10/MC425/jpsi/v3/jpsimc_rawrecohltdebug_50_1_06m.root'
options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START44_V7::All'


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
process.hltMinBiasHFOrBSCEarly.throw = False
process.hltMinBiasHFOrBSCEarly.andOr = True

process.hltMinBiasHFOrBSC = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHfOrBSC"]
process.hltMinBiasHFOrBSC.throw = False


process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltOniaHI.HLTPaths = ["HLT_HIL1DoubleMuOpen","HLT_HIL2DoubleMu0","HLT_HIL2DoubleMu3",
                              "HLT_HIL1SingleMu3","HLT_HIL1SingleMu5","HLT_HIL1SingleMu7",
                              "HLT_HIL2Mu20","HLT_HIL2Mu3","HLT_HIL2Mu5Tight"]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True


from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *

onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=True, HLT="HLT", Filter=False)

process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = False
process.onia2MuMuPatGlbGlb.resolvePileUpAmbiguity = False

# modify stuff!
process.Onia2MuMuPAT.remove(process.bscOrHfCoinc)
process.Onia2MuMuPAT.remove(process.hltOniaHI)
process.Onia2MuMuPAT.remove(process.collisionEventSelection)
process.patTrigger.processName = "HLT1"

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.source.fileNames = cms.untracked.vstring(
    options.inputFiles
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.outOnia2MuMu.fileName = cms.untracked.string( options.outputFile )
process.outSta.fileName = cms.untracked.string('tnpSta_MC.root')
process.outMuID.fileName = cms.untracked.string('tnpMuID_MC.root')
process.outTrig.fileName = cms.untracked.string('tnpTrig_MC.root')

process.schedule = cms.Schedule(process.L1Reco_step,process.Onia2MuMuPAT,
                                #process.TagAndProbeSta, process.TagAndProbeMuID, process.TagAndProbeTrig,
                                process.e)
