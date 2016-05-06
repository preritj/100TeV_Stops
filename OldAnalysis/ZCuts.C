//----------------------------------------------------------
// z cuts : z measures how uniformly jets are distributed
// Define jets with R=0.5 anti-kT and pT>200 GeV 
// Require at least 4 jets 
// z = x/y 
// x =(scalar sum of pT of all jets beyond sub-leading jet)
// y =(average pT of two leading jets) 
//---------------------------------------------------------
void Analysis::CalcZ(){
    zVar = -1.;
    float x,y; x=0.; y=0.; 
	if (branchJet){
	  int nJets = branchJet->GetEntriesFast();	
	  if (nJets > 3){
        Jet *jet_4 = (Jet*) branchJet->At(3); 
		if (jet_4->PT>200.){
          for (int i=0; i< nJets; i++){
            Jet *jet = (Jet*) branchJet->At(i); 
            if (jet->PT>200.){
              if (i<2) y += jet->PT;
			  else x += jet->PT;
            }
          }
		  zVar = 2.*x/y;
        }
	  }
	}
}

bool Analysis::ZCuts(){
	bool IsCutPass = false;
    if (zVar > 0.8) IsCutPass=true;
	IsEventPass = IsEventPass && IsCutPass;
	return IsCutPass;
}

void Analysis::ApplyZCuts(){
  IsZCuts=true;
  CutFlow.push_back(0.);
  CutName.push_back("z Cuts");
}
