#ifndef DATA_2011
#define DATA_2011


 // incl. and prompt J/psi
int npartShift=2;
int nBins_np = 7; // 6= 30-50; 7= 30-40,40-50
// 0-100, 0-10, 10-20, 20-30,30-40, 40-50, 50-100
double promptNpart[7]            = {113.0518, 355.3528, 261.4178, 187.1470, 129.9835, 86.2622, 22.0709};
double promptNpartError[7]       = {0,0,0,0,0,0,0};
double promptNpart_shiftMinus[7] = {promptNpart[0]-npartShift,promptNpart[1]-npartShift,promptNpart[2]-npartShift,
				     promptNpart[3]-npartShift,promptNpart[4]-npartShift,promptNpart[5]-npartShift,promptNpart[6]-npartShift};
double promptNpart_shiftPlus[7]  = {promptNpart[0]+npartShift,promptNpart[1]+npartShift,promptNpart[2]+npartShift,
				     promptNpart[3]+npartShift,promptNpart[4]+npartShift,promptNpart[5]+npartShift,promptNpart[6]+npartShift};

// 0-5,5-10,10-15,15-20,20-25,25-30,30-35,35-40,40-45,45-50,50-60,60-100
const int nPromptBinsNpart12 = 12;
double promptNpart12[nPromptBinsNpart_2012]      = {381.41, 329.48, 282.67, 240.31, 203.62, 171.08, 142.58, 117.54, 95.78, 76.68, 53.53, 14.19};
double promptNpartError12[nPromptBinsNpart_2012] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0};
double promptSystX12[nPromptBinsNpart_2012]          = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,10.0, 10.0, 10.0,10.0, 10.0};

// 0-100, 0-10, 10-20,20-30,30-50,50-100
double nonPromptNpart_2012_cent3050[6]      = {113.09, 355.45, 261.49, 187.35,108.15,22.06};
double nonPromptNpartError_2012_cent3050[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double nonPromptSystX_2012_cent3050[6]      = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0};

// 0-100, 0-10, 10-20, 20-30,30-40, 40-50, 50-100
double nonPromptNpart_2012[7]      = {113.0518, 355.3528, 261.4178, 187.1470, 129.9835, 86.2622, 22.0709};
double nonPromptNpartError_2012[7] = {0.0,   0.0,  0.0,  0.0,  0.0,  0.0, 0.0};
double nonPromptSystX_2012[7]      = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};


double nonPromptNpart_2012_shiftMinus[7]  = { nonPromptNpart_2012[0]-npartShift, nonPromptNpart_2012[1]-npartShift, nonPromptNpart_2012[2]-npartShift,
				      nonPromptNpart_2012[3]-npartShift, nonPromptNpart_2012[4]-npartShift, nonPromptNpart_2012[5]-npartShift,promptNpart[6]-npartShift};
double nonPromptNpart_2012_shiftPlus[7]   = { nonPromptNpart_2012[0]+npartShift, nonPromptNpart_2012[1]+npartShift, nonPromptNpart_2012[2]+npartShift,
				      nonPromptNpart_2012[3]+npartShift, nonPromptNpart_2012[4]+npartShift, nonPromptNpart_2012[5]+npartShift, nonPromptNpart_2012[6]+npartShift};


  // prompt RAA
  double promptJpsiRAA_2012_2010bins[]                   = {0.,0.24,0.31,0.39,0.47,0.55,0.69};
  double promptJpsiRAAError_2012_2010bins_nopperr[]      = {0.,0.01,0.01,0.01,0.02,0.03,0.03};
  double promptJpsiRAAErrorSyst_2012_2010bins_nopperr[]  = {0.,0.03,0.03,0.04,0.05,0.06,0.07};

 
  double promptJpsiRAA_2012[nPromptBinsNpart_2012]           = {0.23,0.25,0.29,0.33,0.38,0.41,0.49,0.44,0.54,0.56,0.68,0.70};
  double promptJpsiRAAError_2012[nPromptBinsNpart_2012]      = {0.01,0.01,0.01,0.01,0.02,0.02,0.03,0.03,0.03,0.04,0.04,0.05};
  double promptJpsiRAAErrorSyst_2012[nPromptBinsNpart_2012]  = {0.02,0.03,0.03,0.03,0.04,0.04,0.05,0.05,0.06,0.06,0.07,0.07};

  // rapidity dependence of high-pT
  double promptJpsiRAA_pt6530y012_2012[]          = {0,0.21,0.29,0.37,0.46,0.52,0.67};
  double promptJpsiRAAError_pt6530y012_2012[]     = {0, 0.01,0.01,0.02,0.03,0.04,0.04};
  double promptJpsiRAAErrorSyst_pt6530y012_2012[] = {0,0.02,0.03,0.04,0.05,0.05,0.07};

  double promptJpsiRAA_pt6530y1216_2012[]           = {0, 0.23,0.30,0.38,0.43,0.59,0.67};
  double promptJpsiRAAError_pt6530y1216_2012[]      = {0, 0.02,0.02,0.03,0.04,0.06,0.06};
  double promptJpsiRAAErrorSyst_pt6530y1216_2012[]  = {0,0.03,0.03,0.04,0.05,0.06,0.07};

  double promptJpsiRAA_pt6530y1624_2012[]       = {0, 0.28,0.33,0.39,0.45,0.54,0.69};
  double promptJpsiRAAError_pt6530y1624_2012[]  = {0, 0.02,0.02,0.03,0.04,0.05,0.06};
  double promptJpsiRAAErrorSyst_pt6530y1624_2012[]  = {0,0.03,0.04,0.04,0.05,0.06,0.08};

  // low-pt fwd region
//--- 
double promptJpsiRAA_pt330y1624_2012[]        = {0, 0., 0, 0, 0, 0,0};
  double promptJpsiRAAError_pt330y1624_2012[]   = {0, 0.02,0.02,0.02,0.04,0.05,0.06};
  double promptJpsiRAAErrorSyst_pt330y1624_2012[]   = {0,0.04,0.05,0.06,0.07,0.1,0.11};
//----


  double promptJpsiRAA_pt365y1624_2012[]        = {0, 0.38,0.43,0.46,0.48,0.68,0.64};
  double promptJpsiRAAError_pt365y1624_2012[]   = {0, 0.02,0.03,0.03,0.03,0.05,0.05};
  double promptJpsiRAAErrorSyst_pt365y1624_2012[]   = {0,0.05,0.06,0.06,0.06,0.08,0.08};

  // ----------- non-prompt RAA
double nonPromptJpsiRAA_2012_2010bins[]                      = {0,0.40, 0.54};
double nonPromptJpsiRAAError_2012_2010bins_nopperr[]         = {0,0.02, 0.02};
double nonPromptJpsiRAAErrorSyst_2012_2010bins_nopperr[]     = {0,0.04, 0.06};

double nonPromptJpsiRAA_2012[]           = {0.,0.38,0.43,0.48,0.52,0.65,0.69};
double nonPromptJpsiRAAError_2012[]      = {0.,0.02,0.03,0.03,0.04,0.06,0.07};
double nonPromptJpsiRAAErrorSyst_2012[]  = {0.,0.04,0.05,0.05,0.06,0.07,0.07};

//  high-pt rapidity dependence
double nonPromptJpsiRAA_pt6530y012_2012[]            = {0, 0.40,0.49,0.54,0.59,0.75,0.79};
double nonPromptJpsiRAAError_pt6530y012_2012[]       = {0, 0.03,0.04,0.05,0.06,0.09,0.11};
double nonPromptJpsiRAAErrorSyst_pt6530y012_2012[]   = {0,0.04,0.05,0.06,0.06,0.08,0.08};

double nonPromptJpsiRAA_pt6530y1216_2012[]       = {0,0.34,0.35,0.41,0.29,0.51,0.63};
double nonPromptJpsiRAAError_pt6530y1216_2012[]  = {0, 0.04,0.05,0.06,0.07,0.12,0.13};
double nonPromptJpsiRAAErrorSyst_pt6530y1216_2012[]  = {0,0.04,0.04,0.05,0.03,0.06,0.07};

double nonPromptJpsiRAA_pt6530y1624_2012[]       = {0, 0.32,0.34,0.33,0.51,0.52,0.53};
double nonPromptJpsiRAAError_pt6530y1624_2012[]  = {0, 0.04,0.05,0.05,0.09,0.12,0.13};
double nonPromptJpsiRAAErrorSyst_pt6530y1624_2012[]  = {0,0.04,0.04,0.04,0.06,0.06,0.06};

  // low-pT forward region
double nonPromptJpsiRAA_pt365y1624_2012[]        = {0, 0.53,0.40,0.57,0.60,0.71,1.06};
double nonPromptJpsiRAAError_pt365y1624_2012[]   = {0, 0.08,0.08,0.10,0.12,0.18,0.22};
double nonPromptJpsiRAAErrorSyst_pt365y1624_2012[]   = {0,0.07,0.05,0.07,0.07,0.09,0.13};

//---
double nonPromptJpsiRAA_pt330y1624_2012[]        = {0, 0.,0,0,0,0,0};
double nonPromptJpsiRAAError_pt330y1624_2012[]   = {0, 0.08,0.08,0.1,0.11,0.17,0.2};
double nonPromptJpsiRAAErrorSyst_pt330y1624_2012[]   = {0,0.07,0.05,0.07,0.06,0.08,0.12};
//----
// pt
 // 2011 points
double ptSystX4_2012[] = {0.75, 0.75, 0.75, 0.75};

//<pT> from 2011
double ptBins4_genJpsi_2012[] = {7.0,  8.7, 11.1, 16.3};
double ptBins4_genB_2012[]    = {8.6, 10.7, 13.3, 19.1};

double ptBins4_2012[]  = {7.31,8.97,11.32,16.52}; // 0--2.4
double ptError4_2012[] = {0.0,0.0,0.0,0.0};
double ptBins4Low_2012[] = {0.81,0.97,1.32,3.52}; // 0--2.4
double ptBins4Up_2012[]  = {0.69,1.03,1.68,13.48}; // 0--2.4

double ptBins3_2012[]  = {7.31,8.97,13.34}; // 0--2.4
double ptError3_2012[] = {0.0,0.0,0.0};
double ptBins3Low_2012[] = {0.81,0.97,3.34}; // 0--2.4
double ptBins3Up_2012[]  = {0.69,1.03,16.66}; // 0--2.4

double ptBins2_2012[]  = {8.15,13.34}; // 0--2.4
double ptError2_2012[] = {0.0,0.0};
double ptBins2Low_2012[] = {1.65,3.34}; // 0--2.4
double ptBins2Up_2012[]  = {1.85,16.66}; // 0--2.4

  // prompt RAA
double promptJpsiPt4_2012[]      = {0.33,0.34,0.35,0.29}; 
double promptJpsiPtError4_2012[] = {0.03,0.03,0.03,0.04};
double promptJpsiPtError4Syst_2012[]  = {0.04,0.04,0.04,0.03};

double promptJpsiPt2_2012[]                   = {0.34, 0.33}; 
double promptJpsiPtError2_2012_nopperr[]      = {0.01,0.01};
double promptJpsiPtError2Syst_2012_nopperr[]  = {0.04,0.03};

  // non-prompt RAA
double nonPromptJpsiPt4_2012[]           = {0.52,0.43,0.43,0.34}; 
double nonPromptJpsiPtError4_2012[]      = {0.12,0.08,0.09,0.07};
double nonPromptJpsiPtError4Syst_2012[]  = {0.06,0.05,0.05,0.04};


// rapidity
// same <rapdity> as in 2011
// these are included in data_2010.h
/* double etaSystX[]   = {0.075, 0.075, 0.075, 0.075}; */
/* double etaBins[]    = {0.771127,1.47388,1.86824}; // 6.5--30 */
/* double etaBinsM[]   = {0.0,-1.4,-2.0}; // 6.5--30 */
/* double etaError[]   = {0.0,0.0,0.0,0.0}; */
/* double etaBinsLow[] = {0.7652,0.27388,0.26824}; */
/* double etaBinsUp[]  = {0.428873,0.12612,0.52696}; */

  // 2012 RAA
double promptJpsiEta_2012[]      = {0.31, 0.33, 0.37}; 
double promptJpsiEtaError_2012[] = {0.02, 0.03, 0.03};
double promptJpsiEtaSyst_2012[]  = {0.03, 0.04, 0.04};

double promptJpsiEtaError_2012_nopperr[] = {0.01, 0.01, 0.02};
double promptJpsiEtaSyst_2012_nopperr[]  = {0.03, 0.04, 0.04};

double nonPromptJpsiEta_2012[]      = {0.49, 0.37, 0.37};
double nonPromptJpsiEtaError_2012[] = {0.07, 0.07, 0.07};
double nonPromptJpsiEtaSyst_2012[]  = {0.05, 0.05, 0.04};

#endif

