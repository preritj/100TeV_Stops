bool Analysis::DphiCuts(){
	bool IsCutPass = false; 
	if (branchJet && branchMissingET){
	  MissingET *met = (MissingET*) branchMissingET->At(0);	
	  int nJets = branchJet->GetEntriesFast();	
	  for (int i=0; i < nJets; i++){
	    Jet *jet = (Jet*) branchJet->At(i);	
		if (jet->PT > 200.){
			if (jet->P4().DeltaR(met->P4()) > 1.) 
				IsCutPass=true; 
			else {IsCutPass=false; break;}
		}
	  }
	}
	IsEventPass = IsEventPass && IsCutPass;
	return IsCutPass;
}

void Analysis::ApplyDphiCuts(){
  IsDphiCuts=true;
  CutFlow.push_back(0.);
  CutName.push_back("dPhi Cuts");
}
