import FWCore.ParameterSet.Config as cms

hionia = cms.EDAnalyzer('HIOnia',
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
                      storeSameSign = cms.untracked.bool(False),
                      
                      #-- Histogram configuration
                      combineCategories = cms.bool(False),
                      fillRooDataSet = cms.bool(False),
                      histFileName = cms.string("Jpsi_Histos.root"),		
                      dataSetName = cms.string("Jpsi_DataSet.root"),
                      
                      #--
                      NumberOfTriggers = cms.uint32(2),
                      )
