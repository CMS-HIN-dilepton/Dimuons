#ifndef V2_DATANUMBERS_2015
#define V2_DATANUMBERS_2015

  // Binning: bin0= minbias; then have promptBins, then non-prompt bins; (all together stored in 1 vector)
 // ------------------------------------- event plane resolution corrections
  double dEvPlResCorr[]    = { 0.8168,                         // MB: 10-60%
			       0.6311, 0.8166, 0.8418, 0.7812, // prompt: 0-10%, 10-20%, 20-30%, 30-60%;
			       0.6311, 0.8270, 0.7812};        // non-prompt: 0-10%, 10-30%, 30-60%
  double dEvPlResCorrErr[] = { 0.0013,                         // MB: 10-60%
			       0.0023, 0.0018, 0.0025, 0.0029, // prompt: 0-10%, 10-20%, 20-30%, 30-60%;
			       0.0023, 0.0020, 0.0029};        // non-prompt: 0-10%, 10-30%, 30-60%

//------------------------------------------ legends and names
  const char* legend[4]      = {"","Prompt J/#psi","Non-prompt J/#psi","Background"};
  const char* outFilePlot[4] = {"mb","pt","rap","cent"};

  const int nPtBins   = 8;
  const int nYBins    = 6;
  const int nCentBins = 8;

  const char* ptBinsName[nPtBins]     = {"65300","3065","6580","80100","100300","3065","65100","100300"};
  const char* yBinsName[nYBins]       = {"0024","0012","1216","1624","0012","1224"};
  const char* centBinsName[nCentBins] = {"1060","010", "1020","2030","3060", "010","1030","3060"};


  const char* ptBinsLegend[nPtBins]     = {"6.5<p_{T}<30 GeV/c",// MB
					   "3<p_{T}<6.5","6.5<p_{T}<8","8<p_{T}<10","10<p_{T}<30",//prompt
					   "3<p_{T}<6.5","6.5<p_{T}<10","10<p_{T}<30"}; // non-prompt
  const char* yBinsLegend[nYBins]       = {"|y|<2.4",// MB
					   "|y|<1.2","1.2<|y|<1.6","1.6<|y|<2.4",//prompt
					   "|y|<1.2","1.2<|y|<2.4"}; // non-prompt
  const char* centBinsLegend[nCentBins] = {"Cent. 10-60\%",// MB
					   "10-0\%","20-10\%","30-20\%","60-30\%",//prompt
					   "10-0\%","30-10\%","60-30\%"}; // non-prompt

//------------------------------------- BINNING and limits
  // prompt bins
  double ptBins_pr[]    = {3.0, 6.5, 8.0, 10, 30.0};
  double yBins_pr[]     = {0.0, 1.2, 1.6, 2.4};
  double centBins_pr[]  = {0.0, 10.0, 20.0, 30.0, 60.0};

  // non-prompt bins
  double ptBins_np[]   = {3.0, 6.5, 10.0, 30.0};
  double yBins_np[]    = {0.0, 1.2, 2.4};
  double centBins_np[] = {0.0, 10.0, 30.0, 60.0};
  // integrated bin
  double centBins_int[] = {10.0, 60.0};

  const int nPtBins_pr   = sizeof(ptBins_pr)/sizeof(double) -1;
  const int nYBins_pr    = sizeof(yBins_pr)/sizeof(double) -1;
  const int nCentBins_pr = sizeof(centBins_pr)/sizeof(double) -1;
  const int nPtBins_np   = sizeof(ptBins_np)/sizeof(double) -1;
  const int nYBins_np    = sizeof(yBins_np)/sizeof(double) -1;
  const int nCentBins_np = sizeof(centBins_np)/sizeof(double) -1;

 
//--------------------------------------- plotting location

// pt axis
  double adXaxisPt_np_l[] = {0.7, 2.0};
  double adXaxisPt_np_h[] = {2.8,18.0};
  double adXaxisPt_np[]   = {7.2,12.0};

  double adXaxisPt_pr[]    = {7.2, 8.9, 13.2};// location on x-axis  
  double adXaxisPt_pr_l[]  = {0.7, 0.9,  3.2}; // bin width to the left
  double adXaxisPt_pr_h[]  = {0.8, 1.1, 16.8};// bin width to the right
 

  double adXaxis_low[1]         = {4.3}; // bin width to the left
  double adXaxis_low_l[1]       = {1.3}; // bin width to the left
  double adXaxis_low_h[1]       = {2.2};// bin width to the right
  double adWidth_low_systBox[1] = {0.5}; // width of the systm. uncert.

// rapidity axis
  double adXaxisY_np_l[] = {0.6,0.6};
  double adXaxisY_np_h[] = {0.6,0.6};
  double adXaxisY_np[]   = {0.6,1.8};

  double adXaxisY_pr[]    = {0.6, 1.4, 2.0};// location on x-axis  
  double adXaxisY_pr_l[]  = {0.6, 0.2, 0.4}; // bin width to the left
  double adXaxisY_pr_h[]  = {0.6, 0.2, 0.4};// bin width to the right
 
// cent axis
double adXaxisCent_pr[]   = {90,187,261,355}; // Npart (60-30, 30-20, 20-10, 10-0)
double adXaxisCent_np[]   = {90,224,355}; // Npart (60-30, 30-10, 10-0)
// ------------------------------------------------------- systematic uncert (these are bogus for the moment)
  double adV2Pt_pr_syst[nPtBins_pr]     = {0.011,0.011,0.011};
  double adV2Y_pr_syst[nYBins_pr]       = {0.011,0.011,0.011};
  double adV2Cent_pr_syst[nCentBins_pr] = {0.011,0.011,0.011,0.011};


  double adV2Pt_np_syst[nPtBins_np]     = {0.011,0.011};
  double adV2Y_np_syst[nYBins_np]       = {0.011,0.011};
  double adV2Cent_np_syst[nCentBins_np] = {0.011,0.011,0.011};

// 
  double adV2_low_syst[1]      = {0};
  double adV2_low_pr_syst[1] = {0.011};
  double adV2_low_np_syst[1] = {0.011};

#endif
