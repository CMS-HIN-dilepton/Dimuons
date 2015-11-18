//---mass cut to run over oniaTree---//

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TString.h"
#include <iostream>
double massmin = 7;
double massmax = 14;

TTree* massCut(TTree* told,double massMin,double massMax) {
   TTree *tnew = told->CloneTree(0);
   int nentries = told->GetEntries();
   int Reco_QQ_size;
   float mass;

   TClonesArray *Reco_QQ_4mom = 0;
   TBranch *b_Reco_QQ_4mom;
   TBranch *b_Reco_QQ_size;
   told -> SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
   told -> SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);

   for (int i=0; i<nentries; i++) {
      told->GetEntry(i);
      for (int j=0; j< Reco_QQ_size; j++){	
      TLorentzVector *DiMuon = (TLorentzVector *) Reco_QQ_4mom->At(j);
      mass = DiMuon->M();
      if ( mass >= massMin && mass <= massMax){      
      	tnew->Fill();
	}
      }
   }
   return tnew;
}


void oniaTreeMassCut(const char *filein,int oniamode=2) {

   char *signame = "";
   if (oniamode==1) {
      signame = "Jpsi";
      massmin = 2.5;
      massmax = 4;
   }
   else if (oniamode==2) {
      signame = "Upsilon";
      massmin = 9;
      massmax = 10;
   }

   TFile *fin = new  TFile(filein);
   //cout << filein <<endl;
   TFile *fout = new TFile(Form("%s_massCut_%s",signame,filein),"RECREATE");
   
   TTree *myTree = massCut((TTree*) fin->Get("myTree"),massmin,massmax);

   fout->Write();
   fout->Close();
}


