#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "RootHeaders.h"
#include "DelphesHeaders.h"
#include "Event.h"
#include "JetSub.h"
namespace analysis{
//========================================================

class Analysis{
private: 
  ExRootTreeReader *treeReader;
  Long64_t nEvents;
  TTree *eventTree;
  TVectorD *PScuts;
  TFile *file;
  EventOutput eventOut;
  JetSubOutput eventJSOut;
  int UseEventWeight;
  double weight;
public:
  Analysis(ExRootTreeReader *, std::string, int);
  void RunEvents(), Write();
};


//========================================================
}
#endif // ANALYSIS_H
