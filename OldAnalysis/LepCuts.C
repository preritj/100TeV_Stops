bool Analysis::LepCuts(){
    //Check for isolated muons
    bool IsNoMuon = true; 
	if (branchIsoMuon){
      for (int i=0; i < branchIsoMuon->GetEntriesFast(); i++){
        Muon* muon = (Muon*) branchIsoMuon->At(i);
        if (muon->PT > 35.) {IsNoMuon=false; break;}
	  }
    }
	IsEventPass = IsEventPass && IsNoMuon;
    if (!IsNoMuon) return IsNoMuon;
    //check for isolated electrons
	bool IsNoElectron = true; 
	if (branchIsoEl){
      IsNoElectron=true;
      for (int i=0; i < branchIsoEl->GetEntriesFast(); i++){
        Electron* electron = (Electron*) branchIsoEl->At(i);
        if (electron->PT > 35.) {IsNoElectron=false; break;}
	  }
    }
	IsEventPass = IsEventPass && IsNoElectron;
	return IsNoElectron;
}

void Analysis::ApplyLepCuts(){
  IsLepCuts=true;
  CutFlow.push_back(0.);
  CutName.push_back("lepton Cuts");
}
