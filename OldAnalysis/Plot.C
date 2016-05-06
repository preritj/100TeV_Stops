void Analysis::DrawHist(){
	IsHist=true;
	BookHist();
}

void Analysis::BookHist(){
  plots = new Plots;
  plots->h_JetPT[0]     = new TH1F("jet_pt_1", "j_{1} p_{T}", 40, 300.,5300.);
  plots->h_JetPT[1]     = new TH1F("jet_pt_2", "j_{2} p_{T}", 32, 200.,4200.);
  plots->h_JetPT[2]     = new TH1F("jet_pt_3", "j_{3} p_{T}", 16, 100.,2100.);
  plots->h_JetPT[3]     = new TH1F("jet_pt_4", "j_{4} p_{T}", 16, 100.,2100.);
  plots->h_JetMass[0]   = new TH1F("jet_mass_1", "m j_{1} ", 40, 0.,1000.);
  plots->h_JetMass[1]   = new TH1F("jet_mass_2", "m j_{2} ", 40, 0.,1000.);
  plots->h_JetMass[2]   = new TH1F("jet_mass_3", "m j_{3} ", 40, 0.,1000.);
  plots->h_JetMass[3]   = new TH1F("jet_mass_4", "m j_{4} ", 40, 0.,1000.);
  plots->h_TrimJetMass[0]   = new TH1F("trimjet_mass_1", "m j_{1} ", 40, 0.,1000.);
  plots->h_TrimJetMass[1]   = new TH1F("trimjet_mass_2", "m j_{2} ", 40, 0.,1000.);
  plots->h_TrimJetMass[2]   = new TH1F("trimjet_mass_3", "m j_{3} ", 40, 0.,1000.);
  plots->h_TrimJetMass[3]   = new TH1F("trimjet_mass_4", "m j_{4} ", 40, 0.,1000.);
  plots->h_MET[0]       = new TH1F("MET", "MET", 16, 0., 4000.);
  plots->h_MET[1]       = new TH1F("MET_1tag", "MET", 16, 0., 4000.);
  plots->h_MET[2]       = new TH1F("MET_2tag", "MET", 16, 0., 4000.);
  plots->h_HT[0]        = new TH1F("HT", "HT", 40, 2000., 22000.);
  plots->h_HT[1]        = new TH1F("HT_1tag", "HT", 40, 2000., 22000.);
  plots->h_HT[2]        = new TH1F("HT_2tag", "HT", 40, 2000., 22000.);
  plots->h_MET_HT[0]    = new TH2F("MET_HT", "MET vs HT", 20, 0., 5000., 40, 3000., 23000.);
  plots->h_MET_HT[1]    = new TH2F("MET_HT_1tag", "MET vs HT", 20, 0., 5000., 40, 3000., 23000.);
  plots->h_MET_HT[2]    = new TH2F("MET_HT_2tag", "MET vs HT", 20, 0., 5000., 40, 3000., 23000.);
  plots->h_muPT         = new TH1F("mu_pt", "Leading muon p_{T}", 40, 0., 1000.);
  plots->h_z            = new TH1F("z", "z", 20, 0., 2.0);
  plots->h_nTops[0]     = new TH1F("nTops_1", "Top multiplicity in j_{1}", 4, 0, 4);
  plots->h_nTops[1]     = new TH1F("nTops_2", "Top multiplicity in j_{2}", 4, 0, 4);
  plots->h_nTops[2]     = new TH1F("nTops_3", "Top multiplicity in j_{3}", 4, 0, 4);
  plots->h_nTops[3]     = new TH1F("nTops_4", "Top multiplicity in j_{4}", 4, 0, 4);
}

//
void Analysis::FillHist(){
  //Fill jet pT histograms
  if (branchJet){
    int nJets = branchJet->GetEntriesFast();	
	if (nJets > 4) nJets=4;
    for(int i=0; i< nJets; i++){
      Jet *jet = (Jet*) branchJet->At(i); 
  	  plots->h_JetPT[i]->Fill(jet->PT, weight);
  	  //plots->h_JetMass[i]->Fill(jet->Mass, weight);
    }
  }
  //Fill MET histograms
  FillHist_MET(0);
  //Fill HT histograms
  FillHist_HT(0);
  // Fill MET & HT 2D histogram
  FillHist_MET_HT(0);
  //Fill leading muon pT
  plots->h_muPT->Fill(MuonPT[0], weight);
  //Fill z variable
  plots->h_z->Fill(zVar,weight); 
  //Fill # of tops
  if (branchCAJet){
    int nJets = branchCAJet->GetEntriesFast();	
	vector<int> nTops = TopsInJets();
	//vector<double> trimJmass = TrimJetMass();
	if (nJets > 4) nJets=4;
    for(int i=0; i< nJets; i++){
      Jet *jet = (Jet*) branchCAJet->At(i); 
  	  plots->h_JetMass[i]->Fill(jet->Mass, weight);
  	  //plots->h_TrimJetMass[i]->Fill(trimJmass[i], weight);
	  plots->h_nTops[i]->Fill(nTops[i] ,weight);
	}
  }
}

void Analysis::FillHist_MET_HT(int nTopTags){
  // Fill MET & HT 2D histogram
  if (branchMissingET && branchScalarHT){
    MissingET *met = (MissingET*) branchMissingET->At(0);
    ScalarHT *hT = (ScalarHT*) branchScalarHT->At(0);
    plots->h_MET_HT[nTopTags]->Fill(met->MET, hT->HT, weight);
  }	
}

void Analysis::FillHist_MET(int nTopTags){
  // Fill MET  histogram
  if (branchMissingET){
    MissingET *met = (MissingET*) branchMissingET->At(0);
    plots->h_MET[nTopTags]->Fill(met->MET, weight);
  }	
}

void Analysis::FillHist_HT(int nTopTags){
  // Fill HT histogram
  if (branchScalarHT){
    ScalarHT *hT = (ScalarHT*) branchScalarHT->At(0);
    plots->h_HT[nTopTags]->Fill(hT->HT, weight);
  }	
}
