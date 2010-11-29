#include <iostream>
#include <string>
#include <vector>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

#include <DataFormats/RecoCandidate/interface/RecoCandidate.h>
//#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>

using namespace std;

class MyCommonHistograms;

class binning {
 public:
  binning();
  binning(int n, float min, float max){nBins=n; minVal = min; maxVal = max;};
  ~binning();

  inline void SetBinning(int n, float min, float max) {nBins=n; minVal = min; maxVal = max; return;};

  inline void SetNbins(int n){nBins = n; return;};
  inline void SetMinVal(float min){minVal = min; return;};
  inline void SetMaxVal(float max){maxVal = max; return;};

  inline int GetNbins(){return nBins;};
  inline float GetMinVal(){return minVal;};
  inline float GetMaxVal(){return maxVal;};


 private:
  int nBins;
  float minVal;
  float maxVal;
};


class MyCommonHistoManager {
 public:
  MyCommonHistoManager(string theName) {hName = theName;};
  ~MyCommonHistoManager();

  inline void SetName(string theName) {hName = theName; return;};
  void Add(string theAppendix, string theName2="");

  void Fill(const reco::Candidate *p, string theFullName);
  void Fill(const reco::Candidate *p1, const reco::Candidate *p2, string theFullName, string theName2);
  void Print();
  void Write(TFile *outf);

  MyCommonHistograms *GetHistograms(string theName) {return myHistos.at(theCategories.find(theName)->second);};
  MyCommonHistograms *GetHistograms(string theName, string theName2) {return myHistos.at(theCategories.find(theName + "_" + theName2)->second);};

 private:
  map<string,int> theCategories;
  map<string,int>::iterator idx;
  pair<map<string,int>::iterator,bool> ret;

  vector<MyCommonHistograms*> myHistos;
  string hName;

};

class MyCommonHistograms {
 public:
  //  MyCommonHistograms();
  MyCommonHistograms(string theFullName);
  ~MyCommonHistograms();

  inline void SetMassBinning(int n, float min, float max) {theMassBinning->SetBinning(n, min, max); return;};
  inline void SetEBinning(int n, float min, float max)    {theEBinning->SetBinning(n, min, max);    return;};
  inline void SetPtBinning(int n, float min, float max)   {thePtBinning->SetBinning(n, min, max);   return;};
  inline void SetEtaBinning(int n, float min, float max)  {theEtaBinning->SetBinning(n, min, max);  return;};
  inline void SetPhiBinning(int n, float min, float max)  {thePhiBinning->SetBinning(n, min, max);  return;};
  inline void SetCentBinning(int n, float min, float max)  {theCentBinning->SetBinning(n, min, max);  return;};

  inline void Set3dEBinning(int n, float min, float max)    {the3dEBinning->SetBinning(n, min, max);    return;};
  inline void Set3dPtBinning(int n, float min, float max)   {the3dPtBinning->SetBinning(n, min, max);   return;};
  inline void Set3dEtaBinning(int n, float min, float max)  {the3dEtaBinning->SetBinning(n, min, max);  return;};

  void Fill(const reco::Candidate *p);
  void Fill(const reco::Candidate *p1, const reco::Candidate *p2, string hName2);

  void Write(TFile *outf);

 private:
  void BookParticleHistos();
  void BookParticleHistos(string hName2);
  string MakeLabel(string hName);

  binning *theMassBinning;
  binning *theEBinning;
  binning *thePtBinning;
  binning *theEtaBinning;
  binning *thePhiBinning;
  binning *theCentBinning;

  binning *the3dEBinning;
  binning *the3dPtBinning;
  binning *the3dEtaBinning;
  
  string hName;
  string hLabel;
  string hLabel2;
  string hAppendix;

  bool useRapidity;
  
  TH1F* hMass;
  TH1F* hE;
  TH1F* hPt;
  TH1F* hEta;
  TH1F* hPhi;
  
  TH2F* hE_Mass;
  TH2F* hPt_Mass;
  TH2F* hEta_Mass;
  TH2F* hPhi_Mass;
  TH2F* hCent_Mass;

  TH2F* hE_Eta;
  TH2F* hPt_Eta;
  TH2F* hPhi_Eta;

  TH2F* hE_Phi;
  TH2F* hPt_Phi;
  
  
  bool booked1ParticleHistos;
  bool booked2ParticleHistos;

};
