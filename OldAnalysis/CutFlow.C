//==========================================================
// Main routine that loops over events and applies cuts
//-----------------------------------------------------------
void Analysis::Run(){
  Int_t nEvents;
  if (Debug) nEvents = 100;
  else nEvents= numberOfEntries;

  // Loop over all events
  for(Int_t entry = 0; entry < nEvents; ++entry)
  {
    // Load event, fill histograms and start cut-flow 
	LoadEvent(); // Initialize event
    TopTag(); // Applying top-tagging using muons 
    CalcZ();  // Calculate the variable z
    if (IsHist || IsWriteOutput) FillHist(); // Fill plots 
    CutFlowUpdate(); // cut-flow begins

	// Jet substructure 
	JetSubStruct();
	continue;	

	// HT cuts
	if (IsHTcuts) {
		if (!HTcuts()) continue; // failed cut
        else CutFlowUpdate();
    }

	// MET cuts
	if (IsMETcuts) {
		if (!METcuts()) continue; // failed cut
        else CutFlowUpdate();
    }

    // jet-cuts 
    if (IsJetCuts) {
		if (!JetCuts()) continue; // failed cut
        else CutFlowUpdate();
    }
    // z-cuts 
    if (IsZCuts) {
		if (!ZCuts()) continue; // failed cut
        else CutFlowUpdate();
    }
    // Isolated lepton cuts
    if (IsLepCuts){
		if (!LepCuts()) continue; //failed cut
        else CutFlowUpdate();	
	}
    //  dPhi(MET,jet) cuts
    if (IsDphiCuts){
		if (!DphiCuts()) continue; //failed cut
        else CutFlowUpdate();	
	}
    //Top-tagging using muons
    if (IsTopTagOne) {
       if (!TopTagOne()) continue;  
       else CutFlowUpdate();
       FillHist_MET(1); // MET histogram 
       FillHist_HT(1); // HT histogram 
       FillHist_MET_HT(1); // generate grid in MET-HT 
	}
    if (IsTopTagTwo) {
       if (!TopTagTwo()) continue;  
       else CutFlowUpdate();
       FillHist_MET(2); // MET histogram 
       FillHist_HT(2); // HT histogram 
       FillHist_MET_HT(2); // generate grid in MET-HT 
	}
  }
  if (IsHist || IsWriteOutput) PrintHist();
  PrintCutFlow();
  if (IsWriteOutput)
    file->Close();
}
//-----------------------------------------------------------

//==========================================================
// load new event info
//-----------------------------------------------------------
void Analysis::LoadEvent(){
	// Read the event entry
    treeReader->ReadEntry(entry++);
	// Store weights for the event
	if (IsWeighted){
	  LHEFEvent* event = (LHEFEvent*) branchEvent->At(0);
	  weight = event->Weight;
    }
	IsEventPass = true;
	CutFlowIt=0;
}
//-----------------------------------------------------------

//==========================================================
// Add weights if passed cuts
//-----------------------------------------------------------
void Analysis::CutFlowUpdate(){
  CutFlow[CutFlowIt] += weight;
  CutFlowIt++;
}
//-----------------------------------------------------------


//==========================================================
// Print results for cut flow
//-----------------------------------------------------------
void Analysis::PrintCutFlow(){
  TVectorF Cuts(CutFlow.size());
  for(int i=0; i< CutFlow.size(); i++){  
    Cuts[i] = CutFlow[i];
    cout << setw(20) << CutName[i]+" :  "  << CutFlow[i] << endl;
    if (IsWriteOutput) Cuts.Write("CutFlow");
  }
}
//-----------------------------------------------------------
