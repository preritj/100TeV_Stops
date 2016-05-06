
void Analysis::TopTag(){
  //determine number of jets to loop over (at most 4)
  if (branchJet){
    int nJets = branchJet->GetEntriesFast();
    if (nJets >3) nJets=4;
	CheckMuJets(nJets);
  }
}

void Analysis::CheckMuJets(int nJets){
  MuonPT.clear();
  for(int iJet=0; iJet<nJets; iJet++){
    MuonPT.push_back(-1.);
    GenMuonTag(iJet);  
    JetMuonTag(iJet);  
    IsoMuonTag(iJet);  
  }
  // sort in pT
  sort(MuonPT.begin(), MuonPT.end());
  reverse(MuonPT.begin(), MuonPT.end());
}

// Top-tag using generator level muons
void Analysis::GenMuonTag(int iJet){
  Jet *jet = (Jet*) branchJet->At(iJet);
  if (branchParticle){
  for (int j=0; j< branchParticle->GetEntriesFast(); j++){
    GenParticle *particle = (GenParticle*) branchParticle->At(j);
    // find muons in hard interaction or final state
	if (abs(particle->PID) == 13 && (particle->Status == 1 || particle->Status==3)) 
    // Require muon to be within dR < 0.5 of the jet
	if (particle->P4().DeltaR(jet->P4())<0.5)
    // Store the pT of the muon 
    if (particle->PT > MuonPT.back()) MuonPT.back()=particle->PT;
  }}
}

//Top-tag using "muon jets"
void Analysis::JetMuonTag(int iJet){
  Jet *jet = (Jet*) branchJet->At(iJet);
  if (branchMuon){
  for (int j=0; j< branchMuon->GetEntriesFast(); j++){
    Muon *muon = (Muon*) branchMuon->At(j);
    // Require muon to be within dR < 0.5 of the jet
	if (muon->P4().DeltaR(jet->P4())<0.5)
    // Store the pT of the muon 
    if (muon->PT > MuonPT.back()) MuonPT.back()=muon->PT;
  }}
}

//Top-tag using isolated muons (unlikely but just to be safe)
void Analysis::IsoMuonTag(int iJet){
  Jet *jet = (Jet*) branchJet->At(iJet);
  if (branchIsoMuon){
  for (int j=0; j< branchIsoMuon->GetEntriesFast(); j++){
    Muon *muon = (Muon*) branchIsoMuon->At(j);
    // Require muon to be within dR < 0.5 of the jet
	if (muon->P4().DeltaR(jet->P4())<0.5)
    // Store the pT of the muon 
    if (muon->PT > MuonPT.back()) MuonPT.back()=muon->PT;
  }}
}
