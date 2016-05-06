namespace analysis{

//------------------------------------------------------------
class DelphesJet{
protected:
	/// Delphes jet
	Jet *jet;

	/// jet-constituents
	vector<fastjet::PseudoJet> JetConsts ;

	/// jet 4-momentum separated into tracks, towers and hardest muon
	PFlowJet PFjetP4;

public:
	/// Converts Delphes jet constituents to fastjet pseudojets
	void ConvertToFastjet();

	/// Displayes Delphes jet constituents 
	void DisplayConstituents();

};


//------------------------------------------------------------
class DelphesCAJet : public DelphesJet, public Event{
private:
	// Vector of sub-jets after reclusering 
	vector<PFlowJet> ReClust;
public:
	DelphesCAJet(int iJet);	
	TopTag(Top)
};


//------------------------------------------------------------
/// Particle-flow jet class derived from fastjet::PseudoJet 
class PFlowJet : fastjet::PseudoJet{
public:
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



//------------------------------------------------------------
class TopCandidate : public PFlowJet{


public :
	void AnalyseTop




};


class TopCandidates{
private :
	/// vector of semi-leptonic top candidates
	vector<SLTopCandidates> SLTops;
 
	/// vector of fully-hadronic top candidates
	vector<FHTopCandidates> FHTops; 

public:
	/// Add a Partcile Flow jet to the list of top candidates
	/// classify as either semi-leptonic or fully hadronic 
	Add(const PFlowJet &); 

}


//------------------------------------------------------------
/// Semi-leptonic top candidates
struct SLTopCandidates{
	double z_mu, R_b_mu, Riso;
};


//------------------------------------------------------------
/// Fully-hadronic top candidates
struct FHTopCandidate{
	double JetMass, tau32;
};

}
