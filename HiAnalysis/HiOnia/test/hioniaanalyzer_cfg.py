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
#process.GlobalTag.globaltag = 'GR_R_39X_V6B::All' #re-reco
process.GlobalTag.globaltag = 'GR_R_44_V10::All' # prompt reco

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowers"), #HFhits for prompt reco
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("hiCentrality")
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        options.inputFiles
    )
)

process.hltDoubleMuOpen = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_HIL1DoubleMuOpen"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hionia = cms.EDAnalyzer('HiOniaAnalyzer',
                                srcMuon = cms.InputTag("patMuonsWithTrigger"),
                                srcMuonNoTrig = cms.InputTag("patMuonsWithoutTrigger"),
                                src = cms.InputTag("onia2MuMuPatGlbGlb"),
                                genParticles = cms.InputTag("genMuons"),
                                primaryVertexTag = cms.InputTag("hiSelectedVertex"),

                                #-- Reco Details
                                useBeamSpot = cms.bool(False),
                                useRapidity = cms.bool(True),
                                
                                #--
                                maxAbsZ = cms.double(24.0),
                                
                                pTBinRanges = cms.vdouble(0.0, 6.0, 8.0, 9.0, 10.0, 12.0, 15.0, 40.0),
                                etaBinRanges = cms.vdouble(0.0, 2.5),
                                centralityRanges = cms.vdouble(10,20,40,60,100),

                                onlyTheBest = cms.bool(False),		
                                applyCuts = cms.bool(True),			
                                storeEfficiency = cms.bool(False),
                      
                                removeSignalEvents = cms.untracked.bool(False),
                                removeTrueMuons = cms.untracked.bool(False),
                                storeSameSign = cms.untracked.bool(True),
                                
                                #-- Gen Details
                                oniaPDG = cms.int32(443),
                                isHI = cms.untracked.bool(True),
                                isMC = cms.untracked.bool(False),
                                isPromptMC = cms.untracked.bool(True),

                                #-- Histogram configuration
                                combineCategories = cms.bool(False),
                                fillRooDataSet = cms.bool(False),
                                fillTree = cms.bool(True),
                                minimumFlag = cms.bool(True),
                                fillSingleMuons = cms.bool(True),
                                histFileName = cms.string(options.outputFile),		
                                dataSetName = cms.string(options.secondaryOutputFile),
                                
                                #--
                                NumberOfTriggers = cms.uint32(9),
                                )


#process.p = cms.Path(process.hltDoubleMuOpen + process.hionia)
process.p = cms.Path(process.hionia)
