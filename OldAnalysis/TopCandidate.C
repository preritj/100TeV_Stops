//------------------------------------------------------------
double DelphesJet::JetMass(){
	// get particle flow info about the jet
	PFlowJet PF = JetP4(jet); 
	TLorentzVector pTrack = PF.track;
	TLorentzVector pTotal = pTrack + PF.tower;
	double JetPT = pTotal.Pt() ;
	/// if pT(jet)<1 TeV, construct jet mass using tracks+tower
	if (JetPT < 1000.) 
		return pTotal.M();
	/// if pT(jet)>1 TeV, construct jet mass using tracks only 
	/// and then rescale : m_jet = m_track * p_jet/p_track
	return pTrack.M() * JetPT/pTrack.Pt();
}


//-----------------------------------------------------------------------
PFlowJet DelphesJet::JetP4(const fastjet::PseudoJet & jet){
	vector<fastjet::PseudoJet> constituents = jet.constituents();
	PFlowJet p;
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


//-----------------------------------------------------------------------
DelphesCAJet::DelphesCAJet(int iJet){
    if (!branchCAJet) 
  		{cout << "No CA jets " << endl; return;}
    if (branchCAJet->GetEntriesFast() <= iJet) 
  		{cout << "invalid CA jet index" << endl; return;} 
    jet = (Jet*) branchCAJet->At(iJet);
	ConvertToFastjet();
	PFjetP4 = JetP4();	
}



//-----------------------------------------------------------------------
void DelphesJet::ConvertToFastjet(){
  int nConst = jet->Constituents.GetEntriesFast(); 
  if (verbose) printf ("Constituents of C/A jet # %d : \n", iJet+1);
  for (int ci=0; ci < nConst; ci++){
     TObject* object = jet->Constituents.At(ci);
     TLorentzVector momentum;
	 int jet_index = -1; // 0:track, 1:tower, 2:muon, -1:default 
     momentum.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
     if (object == 0) {
         continue;
     }
     else if (object->IsA() == GenParticle::Class()) {
     	momentum += ((GenParticle*) object)->P4();
     }
     else if (object->IsA() == Track::Class()){
		jet_index = 0;
     	momentum += ((Track*) object)->P4();
     } 
     else if (object->IsA() == Tower::Class()){
		jet_index = 1;
     	momentum += ((Tower*) object)->P4();
     }
     else if (object->IsA() == Muon::Class()){
		jet_index = 2;
     	momentum += ((Muon*) object)->P4();
     }
     else {
         continue; 
     }
	 fastjet::PseudoJet pjet(momentum.Px(), momentum.Py(),
							momentum.Pz(), momentum.E());
	 pjet.set_user_index(jet_index); // store particle flow info
	 JetConsts.push_back(pjet);
  }
}


//-----------------------------------------------------------------------
void DelphesJet::DisplayConstituents(){
  int nConst = jet->Constituents.GetEntriesFast(); 
  printf ("Constituents of  jet  : \n");
  for (int ci=0; ci < nConst; ci++){
     TObject* object = jet->Constituents.At(ci);
     const char* ConstType;
     if (object == 0) {
     	printf ("NULL constituent \n") ; 
        continue;
     }
     else if (object->IsA() == GenParticle::Class()) {
     	ConstType = "generator particle";
     }
     else if (object->IsA() == Track::Class()){
     	ConstType = "Track";
     } 
     else if (object->IsA() == Tower::Class()){
     	ConstType = "Tower";
     }
     else if (object->IsA() == Muon::Class()){
     	ConstType = "Muon";
     }
     else {
     	printf("Unknown constituent type\n");
        continue; 
     }
     printf("%s : px=%10.2f, py=%10.2f, pz=%10.2f \n", 
     		ConstType, momentum.Px(), momentum.Py(),momentum.Pz());
  }
}

