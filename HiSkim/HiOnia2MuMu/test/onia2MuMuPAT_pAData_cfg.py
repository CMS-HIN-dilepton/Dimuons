# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms

# set up process
process = cms.Process("Onia2MuMuPAT")

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_P_V43D::All'

# Common offline event selection
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

# Centrality for pPb
process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')

# HLT dimuon trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltOniaHI.HLTPaths = ["HLT_PAL1DoubleMuOpen_v*",
                              "HLT_PAL1DoubleMu0_HighQ_v*",
                              "HLT_PAL2DoubleMu3_v*",
                              "HLT_PAMu3_v*",
                              "HLT_PAMu7_v*",
                              "HLT_PAMu12_v*"
                              ]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True
process.hltOniaHI.TriggerResultsTag = cms.InputTag("TriggerResults","","RECO")


from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=False, HLT="RECO", Filter=True)

process.onia2MuMuPatTrkTrk.addMuonlessPrimaryVertex = False
#process.onia2MuMuPatTrkTrk.resolvePileUpAmbiguity = False

process.source.fileNames = cms.untracked.vstring(
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_10_1_V8P.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_11_1_XjQ.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_12_1_wIt.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_13_1_3j5.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_14_1_24I.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_15_1_R8E.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_16_1_3Gg.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_17_1_JpZ.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_18_1_S1W.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_19_1_HwH.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_1_1_EYR.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_20_1_nz5.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_21_1_ToT.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_22_1_Tei.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_23_1_9xI.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_24_1_EUi.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_25_1_ecT.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_26_1_Yig.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_27_1_7PO.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_28_1_980.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_29_1_aM3.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_2_1_UiL.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_30_1_2EM.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_31_1_GZk.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_32_1_8eq.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_33_1_n67.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_34_1_bba.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_35_1_TKP.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_36_1_eVK.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_37_1_kP9.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_38_1_5Wk.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_39_1_ysT.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_3_1_vBH.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_40_1_mQL.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_41_1_FBm.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_42_1_aNs.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_43_1_ZOi.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_44_1_05k.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_45_1_iSj.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_46_1_GzD.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_47_1_4jD.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_48_1_0De.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_49_1_VFr.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_4_1_nXr.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_5_1_pkk.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_6_1_reg.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_7_1_ONu.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_8_1_dKZ.root',
    '/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_RAWRECO_L1Em_PrescaleActiveBitsSkimNoZB_CMSSW538HI_V94_FinalWorkflow_2MHz_v2_v1_v2/8737906f1c367f1d51881c1e107110a2/PAPhysics_RAWRECO_inRECO_9_1_yVy.root'
    )

# filter on lumisections
#from HiSkim.HiOnia2MuMu.goodLumiSectionListHI_cfi import *
#process.source.lumisToProcess = goodLumisToProcess

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.outOnia2MuMu.fileName = cms.untracked.string( 'onia2MuMuPAT.root' )
process.pACentrality_step = cms.Path(process.hltOniaHI*process.PAcollisionEventSelection*process.pACentrality)

process.e = cms.EndPath(process.outOnia2MuMu)

process.schedule = cms.Schedule(process.pACentrality_step,
                                process.Onia2MuMuPAT,process.e)


