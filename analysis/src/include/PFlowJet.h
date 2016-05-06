#ifndef PFLOWJET_H
#define PFLOWJET_H
#include "StlHeaders.h"
#include "RootHeaders.h"
#include "DelphesHeaders.h"
namespace analysis{



struct PFlowJetP4{
	TLorentzVector track, tower, muon, other;
	PFlowJetP4(){
    	track.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    	tower.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    	muon.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    	other.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
	}
};

class PFlowJet
{
private:
  std::vector <fastjet::PseudoJet> PFjetConsts;
  std::vector <fastjet::PseudoJet> tracks;
  PFlowJetP4  jetP4;
public:
  PFlowJet(const std::vector<fastjet::PseudoJet> &);
  void calcJetP4();
  bool MuonInJet();
  double pT(), JetMass(), tau32(), RatioTrk();
  double zMu(),  miniIso(), DeltaR_bMu();
};


}
#endif //PFLOWJET_H


