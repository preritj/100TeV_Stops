void Analysis::JetSubStruct(){
	EventInfo();
	if (branchCAJet){
  		Jet *jet = (Jet*) branchCAJet->At(0);
		vector<fastjet::PseudoJet> JetConsts;
		vector<fastjet::PseudoJet> ReclusJets;
		// Get jet constituents of 0th C/A jet (R=0.8) 
		GetJetConsts(0, JetConsts, true);  
		double Rtop = 700./jet->PT;
		// Re-cluster jet constituents with anti-kt (R = 600/pT)
		ReCluster(JetConsts, ReclusJets, Rtop);
		//cout << JetConsts.size() << endl;
	}
}

vector<int> Analysis::TruthTops(){
	vector<int> tops;
	if (branchParticle){
		for (int i=0; i < branchParticle->GetEntriesFast(); i++){
  			GenParticle *particle = (GenParticle*) branchParticle->At(i);
			//cout << particle->PID << " " << particle->Status << endl;
			if (abs(particle->PID) == 6 && (particle->Status == 3 || particle->Status == 22)) tops.push_back(i); 
		} 
	}
	return tops;
}

vector<int> Analysis::TopsInJets(){
	vector<int> nTops;
	if (!branchCAJet) return nTops;
	vector<int> tops = TruthTops();
	//cout << "# of tops : " << tops.size() << endl;
	int nJets = branchCAJet->GetEntriesFast();
	if (nJets > 6) nJets = 6;
	for (int i=0; i < nJets; i++){
  	  Jet *jet = (Jet*) branchCAJet->At(i);
	  int n=0;
	  for (int itop=0; itop < tops.size(); itop++){
  		GenParticle *top = (GenParticle*) branchParticle->At(tops[itop]);
		if (top->P4().DeltaR(jet->P4())<0.5) n++ ;
	  }	
	//	cout << n << " " ;
	  //cout << "# of tops inside leading jet : " << nTops << endl;
      nTops.push_back(n);
    }
    return nTops;
}

/*
vector<double> Analysis::TrimJetMass(){
	vector<double> JetMass;
	if (!branchCAJet) return JetMass;
	int nJets = branchCAJet->GetEntriesFast();
	if (nJets > 6) nJets = 6;
	for (int i=0; i < nJets; i++){
  		Jet *jet = (Jet*) branchCAJet->At(i);
		vector<fastjet::PseudoJet> JetConsts;
		GetJetConsts(i, JetConsts);  // Get jet constituents of ith C/A jet (R=0.8) 
		double pTmin = 500.;
		double Rtop = 700./jet->PT;
		fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, Rtop,
									fastjet::WTA_pt_scheme);
		fastjet::ClusterSequence clust_seq(JetConsts, jet_def);
		vector<fastjet::PseudoJet> incl_jets = sorted_by_pt(clust_seq.inclusive_jets(pTmin));
		int ntops = incl_jets.size();
		for (int j=1; j< incl_jets.size(); j++){
			if (incl_jets[j].pt() < 0.3*incl_jets[0].pt()) ntops=ntops-1;
		}
        cout << ntops << " ";
		if (incl_jets.size() > 0){
			PFlowJet PF = JetP4(incl_jets[0]);
			double M_track = (PF.track + PF.muon).M();
			double pT_track = (PF.track + PF.muon).Pt();
			double pT = incl_jets[0].pt(); 
			JetMass.push_back(M_track*pT/pT_track) ;
		}
		else JetMass.push_back(-1.);
	}
	return JetMass;
}
*/

//-----------------------------------------------------------------------
// Gives event information (generator level)
void Analysis::EventInfo(){
	if (!branchParticle) return;
	cout << "=================== Event info ====================="<<endl;
	printf("%7s %7s %10s %10s %10s %10s\n", "ID", "Status", 
			"px", "py", "pz", "E"); 
    for (int i=0; i< branchParticle->GetEntriesFast(); i++){
  		GenParticle *particle = (GenParticle*) branchParticle->At(i);
		printf("%7d %7d %10.2f %10.2f %10.2f %10.2f\n", 
				particle->PID, particle->Status, particle->Px,
				particle->Py, particle->Pz, particle->E);
	}
	cout << "===================================================="<<endl;
}


//-----------------------------------------------------------------------
//Converts Delphes C/A jet constituents to fastjet pseudojets
void Analysis::GetJetConsts(int iJet, 
		vector<fastjet::PseudoJet> & JetConsts,  bool verbose){
  if (!branchCAJet) 
		{cout << "No CA jets " << endl; return;}
  if (branchCAJet->GetEntriesFast() <= iJet) 
		{cout << "invalid CA jet index" << endl; return;} 
  Jet *jet = (Jet*) branchCAJet->At(iJet);
  //cout << "mass " << jet->Mass << endl;
  int nConst = jet->Constituents.GetEntriesFast(); 
  if (verbose) printf ("Constituents of C/A jet # %d : \n", iJet+1);
  for (int ci=0; ci < nConst; ci++){
     TObject* object = jet->Constituents.At(ci);
     TLorentzVector momentum;
     const char* ConstType;
	 int jet_index = -1; // 0:track, 1:tower, 2:muon, -1:default 
     momentum.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
     if (object == 0) {
     	if (verbose) printf ("NULL constituent \n") ; 
         continue;
     }
     else if (object->IsA() == GenParticle::Class()) {
     	ConstType = "generator particle";
     	momentum += ((GenParticle*) object)->P4();
     }
     else if (object->IsA() == Track::Class()){
     	ConstType = "Track";
		jet_index = 0;
     	momentum += ((Track*) object)->P4();
     } 
     else if (object->IsA() == Tower::Class()){
     	ConstType = "Tower";
		jet_index = 1;
     	momentum += ((Tower*) object)->P4();
     }
     else if (object->IsA() == Muon::Class()){
     	ConstType = "Muon";
		jet_index = 2;
     	momentum += ((Muon*) object)->P4();
     }
     else {
     	if (verbose) printf("Unknown constituent type\n");
         continue; 
     }
     if (verbose) printf("%s : px=%10.2f, py=%10.2f, pz=%10.2f \n", 
     		ConstType, momentum.Px(), momentum.Py(),momentum.Pz());
	 fastjet::PseudoJet pjet(momentum.Px(), momentum.Py(),
							momentum.Pz(), momentum.E());
	 pjet.set_user_index(jet_index); // store particle flow info
	 JetConsts.push_back(pjet);
  }
}

//-----------------------------------------------------------------------
// Re-clusters input jet constituents using anti-KT algorithm 
// with jet-radius paramter R = Rsub and WTA recombination scheme, 
// resulting re-clustered output jets have pT > 500 GeV
void Analysis::ReCluster(const vector<fastjet::PseudoJet> & inputJet, 
						 vector<fastjet::PseudoJet> & reclus_jets,
						 double Rsub){
	double pTmin = 500.;      // minimum pT for reclustered jets
	fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, Rsub,
									fastjet::WTA_pt_scheme);
	fastjet::ClusterSequence clust_seq(inputJet, jet_def);
	reclus_jets = sorted_by_pt(clust_seq.inclusive_jets(pTmin));
	//JetMass(reclus_jets[0]);
	MuonInJet(reclus_jets[0]);
}

//-----------------------------------------------------------------------
// Calculates net 4-momentum of tracks (including muons), towers and 
// the hardest muon (if any) inside input jet
PFlowJet Analysis::JetP4(const fastjet::PseudoJet & jet){
	vector<fastjet::PseudoJet> constituents = jet.constituents();
	PFlowJet p;
	TLorentzVector pConst;
	for (int i=0; i < constituents.size(); i++){
		fastjet::PseudoJet pJet = constituents[i];
		pConst.SetPxPyPzE(pJet.px(), pJet.py(), pJet.pz(), pJet.e());
		if (pJet.user_index() == 0) p.track += pConst;
		else if (pJet.user_index() == 1) p.tower += pConst;
		else if (pJet.user_index() == 2) {
			p.track += pConst;
			if (pJet.pt() > (p.muon).Pt()) p.muon = pConst;
		}
	}
	return p;
}


//-------------------------------------------------------------------
// Calculates jet mass of the input jet (ensure clusterseq in scope)
// for jets with pT < 1 TeV, all particle flow information 
// (i.e. tracks + muons and towers) is used
// for jets with pT > 1 TeV, only tracks (incl. muons) are used to 
// reconstruct track-jet mass, and then scaling correction is applied 
// m_jet = m_track * p_jet/p_track
double JetMass(const fastjet::PseudoJet & jet){
	analysis::PFlowJet PF = JetP4(jet);
	TLorentzVector pTrack = PF.track;
	TLorentzVector pTotal = pTrack + PF.tower;
	double JetPT = pTotal.Pt() ;
	if (JetPT < 1000.) 
		return pTotal.M();
	return pTrack.M() * JetPT/pTrack.Pt();
}

//-------------------------------------------------------------------
// Check if a hard muon (pT>200 GeV) is present inside the input jet 
bool analysis::MuonInJet(const fastjet::PseudoJet & jet){
	PFlowJet PF = JetP4(jet);
	return ((PF.muon) > 200.);
}


//-------------------------------------------------------------------
double analysis::z_muon(const fastjet::PseusoJet & jet){
	PFlowJet PF = JetP4(jet);
	TLorentzVector pTotal = pTrack + PF.tower;
	if (pTotal.E() <= 0.) return -1.;
	return (PF.muon).E() / pTotal.E();		
}

//-------------------------------------------------------------------
double analysis::






