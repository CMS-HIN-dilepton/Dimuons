import FWCore.ParameterSet.Config as cms

process = cms.Process("HIOnia")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR10_P_V12::All'

from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("hiCentrality")
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/cms/store/caf/user/tdahms/HeavyIons/Onia/Data2010/v1/Skims/Prompt/150844-152638/onia2MuMuPAT_9_3_Cdi.root'
    )
)

process.hionia = cms.EDAnalyzer('HiOniaAnalyzer',
                                srcMuon = cms.InputTag("patMuonsWithTrigger"),
                                srcMuonNoTrig = cms.InputTag("patMuonsWithoutTrigger"),
                                src = cms.InputTag("onia2MuMuPatTrkTrk"),
                                primaryVertexTag = cms.InputTag("hiSelectedVertex"),

                                #-- Reco Details
                                useBeamSpot = cms.bool(False),
                                useRapidity = cms.bool(True),
                                
                                #--
                                maxAbsZ = cms.double(24.0),
                                
                                pTBinRanges = cms.vdouble(0.5, 3.0, 6.0, 8.0, 10.0, 15.0, 35.0),
                                etaBinRanges = cms.vdouble(0.0, 2.5),

                                onlyTheBest = cms.bool(False),		
                                applyCuts = cms.bool(True),			
                                storeEfficiency = cms.bool(False),
                      
                                removeSignalEvents = cms.untracked.bool(False),
                                removeTrueMuons = cms.untracked.bool(False),
                                storeSameSign = cms.untracked.bool(True),

                                #-- Histogram configuration
                                combineCategories = cms.bool(True),
                                fillRooDataSet = cms.bool(False),
                                fillSingleMuons = cms.bool(True),
                                histFileName = cms.string("Jpsi_Histos.root"),		
                                dataSetName = cms.string("Jpsi_DataSet.root"),
                                
                                #--
                                NumberOfTriggers = cms.uint32(2),
                                )


process.p = cms.Path(process.hionia)
