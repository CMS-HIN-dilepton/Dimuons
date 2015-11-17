# fitting
Utilities related to fitting onia

# HOWTO
- main macro: fit2015.C
- run it with 'root -l fit2015.C+'
- arguments: fit2015(const char* filename, bool ispbpb, int oniamode, bool isdata)
     - filename: ROOT file with onia tree
     - ispbpb: false for pp, true for PbPb
     - oniamode: 1 for J/psi, 2 for upsilon
     - isdata: false for MC, true for data

