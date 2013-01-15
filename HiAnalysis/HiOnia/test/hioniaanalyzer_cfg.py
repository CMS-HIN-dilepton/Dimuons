import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("HIOnia")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
options.outputFile = "Jpsi_Histos.root"
options.secondaryOutputFile = "Jpsi_DataSet.root"
#options.inputFiles = 'rfio:/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/Data2010/v7/Skims/ReReco/onia2MuMuPAT_139.root'
options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_E_V33A::All' # express reco
process.GlobalTag.globaltag = 'GR_P_V43D::All' # prompt reco

from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowersPlusTrunc"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("pACentrality")
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        options.inputFiles
    )
)

process.hltDblMuOpen = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","RECO"),
                 HLTPaths = cms.vstring("HLT_PAL1DoubleMuOpen_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltDblMu0 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","RECO"),
                 HLTPaths = cms.vstring("HLT_PAL1DoubleMu0_HighQ_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltDblMu3 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","RECO"),
                 HLTPaths = cms.vstring("HLT_PAL2DoubleMu3_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltMu3 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","RECO"),
                 HLTPaths = cms.vstring("HLT_PAMu3_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltMu7 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","RECO"),
                 HLTPaths = cms.vstring("HLT_PAMu7_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltMu12 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","RECO"),
                 HLTPaths = cms.vstring("HLT_PAMu12_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hionia = cms.EDAnalyzer('HiOniaAnalyzer',
                                srcMuon = cms.InputTag("patMuonsWithTrigger"),
                                srcMuonNoTrig = cms.InputTag("patMuonsWithoutTrigger"),
                                src = cms.InputTag("onia2MuMuPatTrkTrk"),
                                genParticles = cms.InputTag("genParticles"),
                                primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
                                triggerResultsLabel = cms.InputTag("TriggerResults","","RECO"),

                                #-- Reco Details
                                useBeamSpot = cms.bool(False),
                                useRapidity = cms.bool(True),
                                
                                #--
                                maxAbsZ = cms.double(24.0),
                                
                                pTBinRanges = cms.vdouble(0.0, 6.0, 8.0, 9.0, 10.0, 12.0, 15.0, 40.0),
                                etaBinRanges = cms.vdouble(0.0, 2.5),
                                centralityRanges = cms.vdouble(20,40,100),

                                onlyTheBest = cms.bool(False),		
                                applyCuts = cms.bool(True),			
                                storeEfficiency = cms.bool(False),
                      
                                removeSignalEvents = cms.untracked.bool(False),
                                removeTrueMuons = cms.untracked.bool(False),
                                storeSameSign = cms.untracked.bool(True),
                                
                                #-- Gen Details
                                oniaPDG = cms.int32(443),
                                isHI = cms.untracked.bool(False),
                                isPA = cms.untracked.bool(True),
                                isMC = cms.untracked.bool(False),
                                isPromptMC = cms.untracked.bool(False),

                                #-- Histogram configuration
                                combineCategories = cms.bool(False),
                                fillRooDataSet = cms.bool(False),
                                fillTree = cms.bool(True),
                                fillHistos = cms.bool(False),
                                minimumFlag = cms.bool(True),
                                fillSingleMuons = cms.bool(True),
                                histFileName = cms.string(options.outputFile),		
                                dataSetName = cms.string(options.secondaryOutputFile),
                                
                                #--
                                NumberOfTriggers = cms.uint32(6),
                                )


#process.p = cms.Path(process.hltDblMuOpen*process.hionia)
process.p = cms.Path(process.hionia)
