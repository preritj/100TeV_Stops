namespace analysis{ /// begin namespace analysis

//------------------------------------------------------------
// Particle-flow jet 4-momentum separated into
// tracks, towers and hardest muon
struct PFlowJetP4{
	TLorentzVector track, tower, muon;
	PFlowJetP4(){
    	track.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    	tower.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    	muon.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
	}
};


//------------------------------------------------------------
// Particle-flow jet class derived from fastjet::PseudoJet 
class PFlowJet : fastjet::PseudoJet{
private:
	/// store 4-momenta of tracks, towers and muons
	vector<TLorentzVector> track, tower, muon;
	bool IsP4Set; /// flag to check if vectors are filled

	/// 4-momentum of hardest muon in jet
	TLorentzVector MuonP4;
public:
	/// constructor 
	PFlowJet();

	/// Fill 

	/// Calculates jet mass 
	/// For jets with pT > 1 TeV :
	/// m_jet = m_track * p_jet/p_track
	double JetMass();	

	/// Calculates net 4-momentum of tracks (including muons), towers and 
	/// the hardest muon (if any) in the jet constituents
	PFlowJetP4 JetP4();

	/// Check if a hard muon (pT>200 GeV) is present inside the jet 
	bool MuonInJet();

	/// calculate z_muon = fraction of jet energy carried by hardest muon
	double z_muon();

	/// calculate deltaR between 'b-jet' candidate and muon
	double R_b_mu();

	/// calculate muon mini-isolation 
	double MuonIso(); 
};

} /// end namespace analysis
