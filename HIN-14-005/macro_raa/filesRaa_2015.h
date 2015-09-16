#ifndef FILESRAA_2015
#define FILESRAA_2015

// ---------------------------------------------- files with fit results!!!! they are in the readFitTable directory
// needed by makeRaa_x.C and compare_x.C macros
  // ##### Lxy fits and TnP corrections
  const char* yieldHistFile_yesWeight_3[2] = {"histsRaaYields_20150127_PbPb_raa_weightedEff_InEta.root",
               "histsRaaYields_20150127_pp_raa_weightedEff_InEta.root"};
  const char* yieldHistFile_noWeight_3[2]  = {"histsRaaYields_20150127_PbPb_raa_noWeight_InEta.root",
               "histsRaaYields_20150127_pp_raa_noWeight_InEta.root"};

  // ##### Lxyz with TnP corrections applied to the MC 4D efficiencies)  
  const char* yieldHistFile_yesWeight_2[2] = {
   "histsRaaYields_20150823_PbPb_Lxyz_weightedEff_Lxyz_pTtune_PRMC.root",
   "histsRaaYields_20150823_pp_Lxyz_weightedEff_Lxyz_finerpT_PRMC.root"
  };
  
  const char* yieldHistFile_noWeight_2[2] = {
   "histsRaaYields_20150823_PbPb_Lxyz_noWeight_Lxyz_pTtune_PRMC.root",
   "histsRaaYields_20150823_pp_Lxyz_noWeight_Lxyz_finerpT_PRMC.root"
  };

  // ##### Lxyz with TnP corrections applied to data (not to the MC 4D efficiencies)
/*  const char* yieldHistFile_yesWeight_1[2] = {
    "histsRaaYields_20150915_PbPb_weightedEff_TnPAtRD_4DEff_RapPtEffMap2.root",
    "histsRaaYields_20150915_pp_weightedEff_TnPAtRD_4DEff_RapPtEffMap2.root"
  };
  
  const char* yieldHistFile_noWeight_1[2] = {
    "histsRaaYields_20150915_PbPb_noWeight_TnPAtRD_4DEff_RapPtEffMap2.root",
    "histsRaaYields_20150915_pp_noWeight_TnPAtRD_4DEff_RapPtEffMap2.root"
  };
*/
  const char* yieldHistFile_yesWeight_1[2] = {
    "histsRaaYields_20150915_PbPb_weightedEff_TnPAtRD_3DEff_RapPtEffMap2.root",
    "histsRaaYields_20150915_pp_weightedEff_TnPAtRD_3DEff_RapPtEffMap2.root"
  };
  
  const char* yieldHistFile_noWeight_1[2] = {
    "histsRaaYields_20150915_PbPb_noWeight_TnPAtRD_4DEff_RapPtEffMap2.root",
    "histsRaaYields_20150915_pp_noWeight_TnPAtRD_4DEff_RapPtEffMap2.root"
  };

  // ##### Lxyz, no TnP corrections
// Lxyz, 4D eff is not applied
//  const char* yieldHistFile_yesWeight_0[2] = {
//   "histsRaaYields_20150911_PbPb_weightedEff_noTnPCorr_RapPtEffForw_noLxyzEff.root",
//   "histsRaaYields_20150911_pp_weightedEff_noTnPCorr_RapPtEffForw_noLxyzEff.root"
//  };
// Lxyz, 4D eff is applied
  const char* yieldHistFile_yesWeight_0[2] = {
   "histsRaaYields_20150911_PbPb_weightedEff_noTnPCorr_RapPtEffForw.root",
   "histsRaaYields_20150911_pp_weightedEff_noTnPCorr_RapPtEffForw.root"
  };
  
// Lxyz, 4D eff is not applied
//  const char* yieldHistFile_noWeight_0[2] = {
//   "histsRaaYields_20150903_PbPb_noWeight_Lxyz_pTtune_PRMC_noTnPCorr_3DEff.root",
//   "histsRaaYields_20150903_pp_noWeight_Lxyz_finerpT_PRMC_noTnPCorr_3DEff.root"
//  };
// Lxyz, 4D eff is applied
  const char* yieldHistFile_noWeight_0[2] = {
   "histsRaaYields_20150902_PbPb_noWeight_Lxyz_pTtune_PRMC_noTnPCorr.root",
   "histsRaaYields_20150902_pp_noWeight_Lxyz_finerpT_PRMC_noTnPCorr.root"
  };

  // ##### 3D efficiencies
//  const char* effHistFile[2]       = {"histEff_pbpb_tradEff_20150825.root",       "histEff_pp_tradEff_20150825.root"};
  const char* effHistFile[2]       = {"histEff_20150915_PbPb_Eff_wTnP_M2.95-3.25.root",       "histEff_20150915_pp_Eff_wTnP_M2.95-3.25.root"};
  const char* effHistFile_noTnP[2] = {"histEff_20150911_PbPb_Eff_noTnP_M2.95-3.25.root", "histEff_20150911_pp_Eff_noTnP_M2.95-3.25.root"};

#endif

