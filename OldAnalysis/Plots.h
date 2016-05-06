#ifndef Plots_h
#define Plots_h

//====================================
// Add plots here
//====================================
struct Plots{
  TH1 *h_JetPT[4];
  TH1 *h_JetMass[4];
  TH1 *h_TrimJetMass[4];
  TH1 *h_MET[3], *h_HT[3];
  TH1 *h_z, *h_muPT;
  TH2 *h_MET_HT[3];
  TH1 *h_nTops[4]; 
};

//------------------------------------
#endif
