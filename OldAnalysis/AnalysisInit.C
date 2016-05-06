Analysis::Analysis(ExRootTreeReader *t_Reader){
	entry=0;
	weight=1.;
	Debug=false;
    IsHist=false;
	IsWeighted=false;
    IsWriteOutput=false;
	treeReader = t_Reader;
    // number of entries
    numberOfEntries = treeReader->GetEntries();
	// Get pointers to branches used in this analysis
	branchEvent 	= treeReader->UseBranch("Event");
	branchJet 		= treeReader->UseBranch("Jet");
	//branchTrack     = treeReader->UseBranch("Track");
	//branchTower     = treeReader->UseBranch("Tower");
	branchEFlowTrack= treeReader->UseBranch("EFlowTrack");
	branchEFlowTower= treeReader->UseBranch("EFlowTower");
	branchCAJet     = treeReader->UseBranch("CAJet");
	//branchEFlowNHad = treeReader->UseBranch("EFlowNeutralHadron");
	branchMuon 		= treeReader->UseBranch("EFlowMuon");
	branchIsoMuon 	= treeReader->UseBranch("Muon");
	branchIsoEl 	= treeReader->UseBranch("Electron");
	branchParticle 	= treeReader->UseBranch("Particle");
	branchMissingET = treeReader->UseBranch("MissingET");
	branchScalarHT  = treeReader->UseBranch("ScalarHT");
	// Initialize cutflow
	CutFlow.clear(); CutFlow.push_back(0.);
	CutName.clear(); CutName.push_back("Total");
    IsJetCuts=false;
    IsHTcuts=false;
    IsMETcuts=false;
    IsZCuts=false;
    IsLepCuts=false;
    IsDphiCuts=false;
    IsTopTagOne=false;
    IsTopTagTwo=false;
}

void Analysis::WeightsOn(){
	IsWeighted=true;
}

void Analysis::WeightsOff(){
	IsWeighted=false;
}

void Analysis::DebugMode(){
	Debug = true;
}

void Analysis::WriteOutput(const char* outputFile){
	IsWriteOutput=true;
	file = new TFile(outputFile, "RECREATE");
}
