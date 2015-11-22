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
process.GlobalTag.globaltag = 'GR_R_44_V15::All'

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowers"),
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

process.hltDblMuOpen = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_HIL1DoubleMu0_HighQ_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hionia = cms.EDAnalyzer('HiOniaAnalyzer',
                                srcMuon = cms.InputTag("patMuonsWithTrigger"),
                                srcMuonNoTrig = cms.InputTag("patMuonsWithoutTrigger"),
                                src = cms.InputTag("onia2MuMuPatGlbGlb"),
                                srcTracks = cms.InputTag("hiSelectedTracks"),
                                genParticles = cms.InputTag("hiGenParticles"),
                                primaryVertexTag = cms.InputTag("hiSelectedVertex"),
                                triggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                srcCentrality = cms.InputTag("hiCentrality"),
                                
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
                                muonLessPV = cms.bool(True),
                                
                                #-- Gen Details
                                oniaPDG = cms.int32(443),
                                isHI = cms.untracked.bool(True),
                                isPA = cms.untracked.bool(False),
                                isMC = cms.untracked.bool(False),
                                isPromptMC = cms.untracked.bool(False),
                                useEvtPlane = cms.untracked.bool(False),
                                runVersionChange = cms.untracked.uint32(182133),

                                #-- Histogram configuration
                                combineCategories = cms.bool(False),
                                fillRooDataSet = cms.bool(False),
                                fillTree = cms.bool(True),
                                fillHistos = cms.bool(False),
                                minimumFlag = cms.bool(True),
                                fillSingleMuons = cms.bool(True),
                                fillRecoTracks = cms.bool(False),
                                histFileName = cms.string(options.outputFile),		
                                dataSetName = cms.string(options.secondaryOutputFile),
                                
                                #--
                                # NumberOfTriggers = cms.uint32(8),
                                dblTriggerPathNames = cms.vstring("HLT_HIL1DoubleMu0_HighQ_v*",
                                                                  "HLT_HIL2DoubleMu3_v*",
                                                                  "HLT_HIL3DoubleMuOpen_v*",
                                                                  "HLT_HIL3DoubleMuOpen_Mgt2_OS_NoCowboy_v*"),
                                dblTriggerFilterNames = cms.vstring("hltHIDoubleMuLevel1PathL1HighQFiltered",
                                                                    "hltHIL2DoubleMu3L2Filtered",
                                                                    "hltHIDimuonL3FilteredOpen",
                                                                    "hltHIDimuonL3FilteredMg2OSnoCowboy"),
                                sglTriggerPathNames = cms.vstring("HLT_HIL2Mu3_NHitQ_v*",
                                                                  "HLT_HIL2Mu7_v*",
                                                                  "HLT_HIL2Mu15_v*",
                                                                  "HLT_HIL3Mu3_v*"),
                                sglTriggerFilterNames = cms.vstring("hltHIL2Mu3NHitL2Filtered",
                                                                    "hltHIL2Mu7L2Filtered",
                                                                    "hltHIL2Mu15L2Filtered",
                                                                    "hltHISingleMu3L3Filtered")


                                )


#process.p = cms.Path(process.hltDblMuOpen*process.hionia)
process.p = cms.Path(process.hionia)
