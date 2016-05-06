//----------------------------------------------------------
// HT cut :  scalar sum of pT all jets  > 3 TeV
// jets defined with anti-kT algo with R=0.5 and pT>200 GeV
//---------------------------------------------------------
bool Analysis::HTcuts(){
	bool IsCutPass = false; 
	double HT=0.;
	// Number of jets in the event 
	if (branchJet){
	  int nJets = branchJet->GetEntriesFast();	
	  if (nJets > 1){
	    for (int i=0; i< nJets; i++){
      	  Jet *jet = (Jet*) branchJet->At(i); 
		  if (jet->PT > 200.) HT = HT + jet->PT;	

		}
	    IsCutPass = (HT>3000.) ;
	  }
	}
	IsEventPass = IsEventPass && IsCutPass;
	return IsCutPass;
}

void Analysis::ApplyHTcuts(){
  IsHTcuts=true;
  CutFlow.push_back(0.);
  CutName.push_back("HT Cuts");
}
