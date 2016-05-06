#ifndef EVENTANALYSIS_H
#define EVENTANALYSIS_H
#include "StlHeaders.h"
#include "RootHeaders.h"
#include "DelphesHeaders.h"
namespace analysis{

struct EventOutput{
  double HT;
  double MET;
  std::vector<double> Jet_Pt;
  std::vector<double> Jet_Mass;
  std::vector<double> Jet_MET_DeltaPhi;
  std::vector<double> IsoEl_Pt;
  std::vector<double> IsoMu_Pt;
};

class Event
{
private:
  Long64_t entry=0;
  ExRootTreeReader *treeReader;
  TClonesArray *branchMET, *branchJet, *branchEvent;
  TClonesArray *branchIsoMuon, *branchIsoEl;
  EventOutput event;
  bool veto;
public:
  Event(ExRootTreeReader *);
  void Reset(), next(); 
  double GetEventWeight();
  void GetMETjetInfo(), GetIsoLepInfo();
  inline EventOutput Output();
  inline bool IsVeto();
};

inline EventOutput Event::Output(){return event;}
inline bool Event::IsVeto(){return veto;}

}
#endif // EVENTANALYSIS_H
