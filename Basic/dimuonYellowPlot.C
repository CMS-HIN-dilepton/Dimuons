dimuonYellowPlot(bool isHI=false){
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TFile *inf;
  if (isHI)
    //inf = new TFile("Histos_OniaSkim_ReReco_v13_h3_sum.root","READ");
    //    inf = new TFile("~/Jpsi_Histos_181611-183013_150invmub.root","READ");
    //    inf = new TFile("~/Jpsi_Histos_181611-183013_lowmass_147invmub.root","READ");

    inf = new TFile("./Jpsi_Histos_Runs_210498-211631_merged.root","READ");
  //    inf = new TFile("~/Jpsi_Histos_Runs_210498-210837.root","READ");//Jpsi_Histos_Runs_210498-210534.root","READ");


    // inf = new TFile("~/LowMass_Histos_182396-182609.root","READ");
  //    inf = new TFile("~/Jpsi_Histos_181912-182395.root","READ");
  else
    // inf = new TFile("~/Jpsi_Histos_ppReReco_lowmass.root","READ");
    inf = new TFile("/Users/eusmartass/Software/data2013/Jpsi_Histos_Runs_211739-211831_HFvars.root","READ");
  //    inf = new TFile("Histos_OniaSkim_HiReReco_pp2760_v5h0_sum.root","READ");
  //inf = new TFile("~/Jpsi_Histos_ppReReco.root","READ");
    //    inf = new TFile("Histos_OniaSkim_pp2760_v1h1_sum.root","READ");

  double bins[100000];
  bins[0] = 0.2;
  int nBins = 0;
  for (int i=1; bins[i-1]<200; i++) {
    //    bins[i] = bins[i-1]*1.02;//1.04;//1.015;//
    bins[i] = bins[i-1]*1.015;//1.04;//1.015;//
    //    bins[i] = bins[i-1]*1.012;//1.04;//1.015;//
    nBins++;
  }
  cout << nBins << endl;
  TH1F* h = new TH1F("h","h",nBins,bins);
  TH1F* h1 = new TH1F("h1","h1",nBins,bins);
  TH1F* h2 = new TH1F("h2","h2",nBins,bins);
  TH2F* h3 = new TH2F("h3","h3;#eta_{#mu^{+}} - #eta_{#mu^{-}};#mu^{+}#mu^{-} mass",500,-5.0,5.0,nBins,bins);

  TCanvas* c1 = new TCanvas("c1","c1",700,500);
  // c1->SetTopMargin(0.03);
  // c1->SetRightMargin(0.05);
  c1->SetLogx();
  c1->SetLogy();

  if (isHI) {
    //    myTree->Draw("Reco_QQ_4mom.M()>>h","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==0","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h1","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign!=0","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h1","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==1","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h2","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==2","e");

    //    myTree->Draw("Reco_QQ_4mom.M()>>h","(HLTriggers&1)==1&&(Reco_QQ_trig&1)==1&&Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_4mom.Pt()>6.5","e");

    /*
    myTree->Draw("Reco_QQ_4mom.M()>>h","Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4.0&&Reco_QQ_mumi_4mom.Pt()>4.0","e");
    myTree->Draw("Reco_QQ_4mom.M()>>h1","Reco_QQ_type==0&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4.0&&Reco_QQ_mumi_4mom.Pt()>4.0","e");
    */
    myTree->Draw("Reco_QQ_4mom.M()>>h","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e");
    myTree->Draw("Reco_QQ_4mom.M()>>h1","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e");

    //    myTree->Draw("Reco_QQ_4mom.M():(Reco_QQ_mupl_4mom.Eta()-Reco_QQ_mumi_4mom.Eta())>>h3","(HLTriggers&1)==1&&(Reco_QQ_trig&1)==1&&Reco_QQ_type==0&&Reco_QQ_sign==0","colz");
  }
  else {
    //   myTree->Draw("Reco_QQ_4mom.M()>>h","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e");
    // myTree->Draw("Reco_QQ_4mom.M()>>h1","(Reco_QQ_trig&1)==1&&Reco_QQ_type==2&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e");

    myTree->Draw("Reco_QQ_4mom.M()>>h","Reco_QQ_type==2&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e");
    myTree->Draw("Reco_QQ_4mom.M()>>h1","Reco_QQ_type==2&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4&&Reco_QQ_mumi_4mom.Pt()>4","e");

    // myTree->Draw("Reco_QQ_4mom.M()>>h","Reco_QQ_type==0&&Reco_QQ_sign==0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e");
    // myTree->Draw("Reco_QQ_4mom.M()>>h1","Reco_QQ_type==0&&Reco_QQ_sign!=0&&Reco_QQ_mupl_4mom.Pt()>4.5&&Reco_QQ_mumi_4mom.Pt()>4.5","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h1","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==1","e");
    //    myTree->Draw("Reco_QQ_4mom.M()>>h2","HLTriggers&1==1&&Reco_QQ_trig&1==1&&Reco_QQ_type==0&&Reco_QQ_sign==2","e");
  }
  for (int i=1; i<=h1->GetNbinsX(); i++) {
    h->SetBinContent(i, h->GetBinContent(i)/h->GetBinWidth(i));
    h1->SetBinContent(i, h1->GetBinContent(i)/h1->GetBinWidth(i));
  }
  //  h->Add(h1,-1);


  h->SetTitle(";m_{#mu#mu} (GeV/c^{2});Events/(GeV/c^{2})");
  h->GetYaxis()->SetTitleOffset(1.0);
  h->GetXaxis()->SetRangeUser(2,2e2);
  h->GetYaxis()->SetRangeUser(0.2,5e5);
  h->GetXaxis()->CenterTitle(kTRUE);
  h->SetFillColor(kYellow);
  h1->SetLineColor(kRed);
  h1->SetFillColor(kRed-10);
  h2->SetLineColor(kBlue);
  h2->SetFillColor(kBlue-10);
  h2->SetFillStyle(3004);
  //  h->SetFillColor(kRed-10);
  h->Draw(""); 
  //  h1->Draw("same");
  //  h2->Draw("same");

  tex = new TLatex(0.62,0.88,"CMS Preliminary");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(23);
  tex->SetLineWidth(1);
  tex->Draw();
  tex4 = new TLatex(0.62,0.8,"pp  #sqrt{s_{NN}} = 2.76 TeV");
  tex4->SetNDC();
  tex4->SetTextAlign(13);
  tex4->SetTextFont(43);
  tex4->SetTextSize(18);
  tex4->SetLineWidth(2);
  tex4->Draw();
  if (isHI) {
    tex = new TLatex(0.62,0.89,"pPb  #sqrt{s_{NN}} = 5 TeV");
    //    tex2 = new TLatex(0.57,0.83,"L_{int} = 150 #mub^{-1}");
    tex2 = new TLatex(0.71,0.83,"L_{int} = 31 nb^{-1}");
    tex3 = new TLatex(0.59,0.78,"L_{int} (2010) = 7.28 #mub^{-1}");
    tex2->SetNDC();
    tex2->SetTextAlign(13);
    tex2->SetTextFont(43);
    tex2->SetTextSize(18);
    tex2->SetLineWidth(2);
    tex2->Draw();

    tex3->SetNDC();
    tex3->SetTextAlign(13);
    tex3->SetTextFont(43);
    tex3->SetTextSize(18);
    tex3->SetLineWidth(2);
    //    tex3->Draw();
  }
  else {
    //tex = new TLatex(0.30,0.89,"pp  #sqrt{s_{NN}} = 2.76 TeV,  L_{int} = 5.4 pb^{-1} nb^{-1}");
    tex = new TLatex(0.6,0.73,"2013: L_{int} = 5.4 pb^{-1}");

  }
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(23);
  tex->SetLineWidth(2);
  tex->Draw();
  /*
  tex = new TLatex(0.24,0.88,"#rho, #omega");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(25);
  tex->SetLineWidth(1);
  tex->Draw();
  tex = new TLatex(0.32,0.88,"#phi");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(25);
  tex->SetLineWidth(2);
  tex->Draw();
  */
  tex = new TLatex(0.15,0.88,"J/#psi");
  // tex = new TLatex(0.38,0.94,"J/#psi");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(25);
  tex->SetLineWidth(2);
  tex->Draw();
  tex = new TLatex(0.2,0.7,"#psi(2S)");
  //  tex = new TLatex(0.45,0.82,"#psi(2S)");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(25);
  tex->SetLineWidth(2);
  tex->Draw();
  tex = new TLatex(0.35,0.8,"#varUpsilon(1,2,3S)");
  //  tex = new TLatex(0.59,0.76,"#varUpsilon(1,2,3S)");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(25);
  tex->SetLineWidth(2);
  tex->Draw();  
  tex = new TLatex(0.75,0.5,"Z");
  // tex = new TLatex(0.84,0.57,"Z");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(25);
  tex->SetLineWidth(2);
  tex->Draw();

  tex = new TLatex(0.25,0.20,"p_{T}^{#mu} > 4 GeV/c");
  tex->SetNDC();
  tex->SetTextAlign(13);
  tex->SetTextFont(43);
  tex->SetTextSize(18);
  tex->SetLineWidth(2);
  //  if (isHI)
    tex->Draw();

  gPad->RedrawAxis();

  //
  //  TCanvas *c2 = new TCanvas();
  //  h3->Draw("colz");
}
