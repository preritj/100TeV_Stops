namespace analysis{ /// begin namespace analysis

//-----------------------------------------------------------------------
PFlowJetP4 PFLowJet::JetP4(){
	vector<fastjet::PseudoJet> constituents = fastjet::constituents();
	PFlowJetP4 p;
	TLorentzVector pConst;
	for (int i=0; i < constituents.size(); i++){
		fastjet::PseudoJet pJet = constituents[i];
		pConst.SetPxPyPzE(pJet.px(), pJet.py(), pJet.pz(), pJet.e());
		// store momentum of tracks and towers separately 
		if (pJet.user_index() == 0) p.track += pConst;
		else if (pJet.user_index() == 1) p.tower += pConst;
		else if (pJet.user_index() == 2) {
			p.track += pConst; // treat muons as part of track
			// store the momentum of hardest muon 
			if (pJet.pt() > (p.muon).Pt()) p.muon = pConst;
		}
	}
	return p;
}


//------------------------------------------------------------
PFlowJet::PFlowJet(){
	track.clear();
	tower.clear();
	muon.clear();
	HardMuon.SetPxPyPzE(0., 0., 0., 0.);
	AllTracks.SetPxPyPzE(0., 0., 0., 0.);
	AllTowers.SetPxPyPzE(0., 0., 0., 0.);
}

//------------------------------------------------------------
double PFlowJet::JetMass(){
	// get particle flow info about the jet
	PFlowJetP4 PF = JetP4(); 
	double JetPT = fastjet::pt() ;
	/// if pT(jet)<1 TeV, construct jet mass using tracks+tower
	if (JetPT < 1000.) 
		return pTotal.M();
	/// if pT(jet)>1 TeV, construct jet mass using tracks only 
	/// and then rescale : m_jet = m_track * p_jet/p_track
	return (PF.track).M() * JetPT / (PF.track).Pt();
}

//-------------------------------------------------------------------
// Check if a hard muon (pT>200 GeV) is present inside the input jet 
bool PFlowJet::MuonInJet(){
	PFlowJetP4 PF = JetP4();
	return ((PF.muon).Pt() > 200.);
}

/// calculate z_muon = fraction of jet energy carried by hardest muon
double PFLowJet::z_muon(){
	PFlowJetP4 PF = JetP4();
	if (fastjet::E() <= 0.) return  -1.;
	else return (PF.muon).E() / fastjet::E() ;
}

/// calculate deltaR between 'b-jet' candidate and muon
double PFlowJet::R_b_mu(){
	PFlowJetP4 PF = JetP4();
	TLorentzVector bjet = PF.track + PF.tower - PF.muon;
	if ( fastjet::pt() < 1000.) 
		return bjet.DeltaR(PF.muon);
	/// use track-only info for high pT jets
	bjet = PF.track - PF.muon; 
	return bjet.DeltaR(PF.muon); 
}

/// calculate muon mini-isolation 
double PFlowJet::MuonIso(){
	vector<fastjet::PseudoJet> constituents = fastjet::constituents();
	for (int i=0; i < constituents.size(); i++){
		fastjet::PseudoJet pJet = constituents[i];
			
	}
}

} /// end namespace analysis
