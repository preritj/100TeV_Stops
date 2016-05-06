#include "PFlowJet.h"
namespace analysis{


PFlowJet::PFlowJet(const std::vector<fastjet::PseudoJet> & JetConsts){
  PFjetConsts = JetConsts;
  tracks.clear();
  calcJetP4();
}

//-------------------------------------------------------------------
// Check if a hard muon (pT>200 GeV) is present inside the input jet 
bool PFlowJet::MuonInJet(){
	return ((jetP4.muon).Pt() > 200.);
}



//-----------------------------------------------------------------------
// Calculates net 4-momentum of tracks (including muons), towers and 
// the hardest muon (if any) inside input jet
void  PFlowJet::calcJetP4(){
	PFlowJetP4 p;
	TLorentzVector pConst;
	for (unsigned long i=0; i < PFjetConsts.size(); i++){
		fastjet::PseudoJet pJet = PFjetConsts[i];
		pConst.SetPxPyPzE(pJet.px(), pJet.py(), pJet.pz(), pJet.e());
		if (pJet.user_index() == 0) {
		  p.track += pConst;
		  tracks.push_back(PFjetConsts[i]);
		}
		else if (pJet.user_index() == 1) p.tower += pConst;
		else if (pJet.user_index() == 2) {
			p.track += pConst;
		    tracks.push_back(PFjetConsts[i]);
			if (pJet.pt() > (p.muon).Pt()) p.muon = pConst;
		}
		else p.other += pConst;
	}
	jetP4 = p;
}


//-------------------------------------------------------------------
// Calculates jet mass 
// for jets with pT < 1 TeV, all particle flow information 
// (i.e. tracks + muons and towers) is used
// for jets with pT > 1 TeV, only tracks (incl. muons) are used to 
// reconstruct track-jet mass, and then scaling correction is applied 
// m_jet = m_track * p_jet/p_track
double PFlowJet::JetMass(){
	TLorentzVector pTrack = jetP4.track;
	TLorentzVector pTotal = pTrack + jetP4.tower;
	double JetPT = pTotal.Pt() ;
	if (JetPT < 1000.) 
		return pTotal.M();
	return pTrack.M() * JetPT/pTrack.Pt();
}

//return pT of the jet
double PFlowJet::pT(){
  TLorentzVector pTotal = jetP4.track + jetP4.tower;
  return pTotal.Pt();	
}

// Calculate N-subjettiness variable tau_3/tau_2
/// for jets with pT < 1 TeV, all particle flow info is used 
/// for jets with pT > 1 TeV, only track info is used
double PFlowJet::tau32(){
  double jetPT = pT(); 
  /// N-subjettiness only takes jet as input, so we have to form 
  /// a jet from the constituents ensuring all the constituents 
  /// are in the jet. Trick is to choose kT algorithm and large R 
  double R = 100.; 
  fastjet::JetDefinition jetdef = 
				fastjet::JetDefinition(fastjet::antikt_algorithm, R);
  std::vector <fastjet::PseudoJet> input; input.clear();
  if (jetPT < 1000.) input = PFjetConsts;
  else input = tracks;
  if (input.empty()) return -1.; 
  fastjet::ClusterSequence clust_seq(input,jetdef);
  std::vector<fastjet::PseudoJet> incl_jets  
						= sorted_by_pt(clust_seq.inclusive_jets());
  if (incl_jets.empty()) return -1.;
  double beta = 1.0;
  fastjet::contrib::NsubjettinessRatio nSub32(3,2, 
							fastjet::contrib::WTA_KT_Axes(), 
							fastjet::contrib::UnnormalizedMeasure(beta));
  return nSub32(incl_jets[0]);
}


// calculate pT fraction of the jet carried by muon 
double PFlowJet::zMu(){
  if (!MuonInJet()) return -1.;
  //return (jetP4.muon).Pt()/(jetP4.track).Pt(); 
  //return (jetP4.track - jetP4.muon).Pt(); 
  return (jetP4.muon).Pt()/(jetP4.track + jetP4.tower).Pt(); 
}


// calculate mini-isolation for tagging top with muon
double PFlowJet::miniIso(){
  if (!MuonInJet()) return -1.;
  double pTmu = (jetP4.muon).Pt();
  double Riso = 20./pTmu;
  double pTcone = 0.;
  TLorentzVector pConst;
  for (unsigned long i=0; i < PFjetConsts.size(); i++){
	fastjet::PseudoJet pJet = PFjetConsts[i];
	pConst.SetPxPyPzE(pJet.px(), pJet.py(), pJet.pz(), pJet.e());
	double pTj = pJet.pt();
	//if (pTj>1. && (pJet.user_index() == 0 || pJet.user_index() == 2))
	if (pTj>5. && (pJet.user_index() == 0 || pJet.user_index() == 2))
	  if ( pConst.DeltaR(jetP4.muon) < Riso) pTcone += pTj;
  }
  return pTmu/pTcone;
}

// calculate DeltaR between b-jet candidate and muon
double PFlowJet::DeltaR_bMu(){
  if (!MuonInJet()) return -1.;
  double jetPT = pT(); 
  TLorentzVector bJetP4 = jetP4.track - jetP4.muon;
  //if (jetPT < 1000.) 
  	bJetP4 = bJetP4 + jetP4.tower;
  return (jetP4.muon).DeltaR(bJetP4);
}


// calculate the fraction of energy carried by charged tracks
double PFlowJet::RatioTrk(){
	return (jetP4.track).E()/((jetP4.track).E()+(jetP4.tower).E());
}

} // end namespace analysis
