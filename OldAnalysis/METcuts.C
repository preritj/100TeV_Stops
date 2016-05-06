//----------------------------------------------------------
// MET cut : MET > 500 GeV
//---------------------------------------------------------
bool Analysis::METcuts(){
	bool IsCutPass = false; 
	// Number of jets in the event 
	if (branchMissingET){
    	MissingET *met = (MissingET*) branchMissingET->At(0);
		IsCutPass = (met->MET > 500.);
		}
	IsEventPass = IsEventPass && IsCutPass;
	return IsCutPass;
}

void Analysis::ApplyMETcuts(){
  IsMETcuts=true;
  CutFlow.push_back(0.);
  CutName.push_back("MET Cuts");
}
