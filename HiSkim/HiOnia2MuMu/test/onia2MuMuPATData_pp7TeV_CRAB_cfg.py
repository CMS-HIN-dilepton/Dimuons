# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms

# set up process
process = cms.Process("Onia2MuMuPAT")

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_44_V15::All'

# produce missing l1extraParticles
process.load('Configuration.StandardSequences.L1Reco_cff')
process.L1Reco_step = cms.Path(process.l1extraParticles)


# BSC or HF coincidence (masked unprescaled L1 bits)
process.load('L1Trigger.Skimmer.l1Filter_cfi')
process.bscOrHfCoinc = process.l1Filter.clone(
    algorithms = cms.vstring('L1Tech_BSC_minBias_OR.v*', 'L1Tech_HCAL_HF_coincidence_PM.v*', 'L1Tech_BPTX_plus_AND_minus.v*')
    )
    

# Common offline event selection
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

# HLT dimuon trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltOniaHI.HLTPaths = ["HLT_DoubleMu3_Upsilon_v*","HLT_L1DoubleMu0_v"]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *

onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=False, HLT="HLT", Filter=True)

process.onia2MuMuPatTrkTrk.addMuonlessPrimaryVertex = False
process.onia2MuMuPatTrkTrk.resolvePileUpAmbiguity = False

process.primaryVertexFilter.src = "offlinePrimaryVertices"

process.collisionEventSelection = cms.Sequence(process.hfCoincFilter *
                                               process.primaryVertexFilter
                                               #process.siPixelRecHits *
                                               #process.hltPixelClusterShapeFilter
                                               )

process.source.fileNames = cms.untracked.vstring(
    '/dpm/in2p3.fr/home/cms/trivcat/store/user/nfilipov/MuOnia/UpsilonSkim_pp7TeV_v1_ReRecoNov08/aba00261b8d7670688292e001ea3f1e1/
    onia2MuMuPAT_79_1_fMk.root'
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.outOnia2MuMu.fileName = cms.untracked.string( 'onia2MuMuPAT.root' )

process.schedule = cms.Schedule(process.L1Reco_step, process.Onia2MuMuPAT, process.e)

