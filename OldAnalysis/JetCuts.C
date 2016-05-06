//----------------------------------------------------------
// Jets cuts : (1) at least 4 jets with pT > 250 GeV each
//             (2) leading two jets have pT > 750 GeV each
//---------------------------------------------------------
bool Analysis::JetCuts(){
	bool IsCutPass = false; 
	// Number of jets in the event 
	if (branchJet){
	  int nJets = branchJet->GetEntriesFast();	
	  if (nJets > 3){
      	  Jet *jet_1 = (Jet*) branchJet->At(0); //1st jet
      	  Jet *jet_2 = (Jet*) branchJet->At(1); //2nd jet
      	  Jet *jet_3 = (Jet*) branchJet->At(1); //3rd jet
      	  Jet *jet_4 = (Jet*) branchJet->At(3); //4th jet
		  IsCutPass = (jet_2->PT>1000. && jet_4->PT>250. && abs(jet_1->Eta)<2.5 
					  && abs(jet_2->Eta)<2.5 && abs(jet_3->Eta)<2.5 && abs(jet_4->Eta)<2.5) ;
		  //IsCutPass = (jet_2->PT>1000. && abs(jet_1->Eta)<2.5 && abs(jet_2->Eta)<2.5) ;
	  }
	}
	IsEventPass = IsEventPass && IsCutPass;
	return IsCutPass;
}

void Analysis::ApplyJetCuts(){
  IsJetCuts=true;
  CutFlow.push_back(0.);
  CutName.push_back("Jet Cuts");
}
