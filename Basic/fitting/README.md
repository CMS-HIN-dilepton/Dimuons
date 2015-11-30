# fitting
Utilities related to fitting onia

# HOWTO
- main macro: fit2015.C
- run it with 'root -l fit2015.C+'
- arguments: fit2015(const char* FileName, bool isData, bool isPbPb, int oniamode, bool doFit, bool inExcStat)
     - FileName: ROOT file with onia tree
     - isPbPb: false for pp, true for PbPb
     - doFit: Want to fit?
     - inExcStat: if true, the excited Y states are fitted
     - oniamode: 1 for J/psi, 2 for upsilon
     - isData: false for MC, true for data

