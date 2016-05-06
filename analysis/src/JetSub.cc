#include "JetSub.h"
#include "PFlowJet.h"
namespace analysis{
//=======================================================


JetSub::JetSub(ExRootTreeReader *t_Reader){
  treeReader = t_Reader;
  branchCAJet        = treeReader->UseBranch("CAJet");
  branchTrack        = treeReader->UseBranch("EFlowTrack");
  branchTower        = treeReader->UseBranch("EFlowTower");
  branchEFlowMuon    = treeReader->UseBranch("EFlowMuon");
  branchParticle     = treeReader->UseBranch("Particle");
}
  
void JetSub::next(){
  Reset();
  GetCAjetInfo();
/// test
//  if (branchParticle){
//    for (Int_t i=0; i < branchParticle->GetEntriesFast(); i++){
//  	  GenParticle *particle = (GenParticle*) branchParticle->At(i);
//	  if (particle->Status == 3 && (particle->PID == 13 || particle->PID == 15)) return; 
//	}
//  }
/// end test
  if (branchCAJet){
    Int_t nJets = branchCAJet->GetEntriesFast();
    for (Int_t i=0; i < nJets; i++){
  	  Jet *jet = (Jet*) branchCAJet->At(i);
	  std::vector <fastjet::PseudoJet> JetConsts; JetConsts.clear();
	  DelphesToFastjet(jet, JetConsts);
	  std::vector <PFlowJet> PF; PF.clear();
	  double Rtop = 1000./jet->PT;
	  ReCluster(JetConsts, PF, Rtop);
	  for (unsigned long j=0; j < PF.size(); j++){
		eventJS.Top_zMu.push_back(PF[j].zMu());
		eventJS.Top_xMu.push_back(PF[j].xMu());
		eventJS.Top_DeltaR_bMu.push_back(PF[j].DeltaR_bMu());
		eventJS.Top_Pt.push_back(PF[j].pT()); 
		eventJS.Top_Mass.push_back(PF[j].JetMass()); 
		eventJS.Top_Tau32.push_back(PF[j].tau32()); 
		eventJS.Top_RatioTrk.push_back(PF[j].RatioTrk()); 
	  }
	}
  }
}

void JetSub::Reset(){
  eventJS.CAJet_Pt.clear();
  eventJS.CAJet_Mass.clear();
  eventJS.Top_Pt.clear();
  eventJS.Top_Mass.clear();
  eventJS.Top_zMu.clear();
  eventJS.Top_Tau32.clear();
  eventJS.Top_xMu.clear();
  eventJS.Top_DeltaR_bMu.clear();
  eventJS.Top_RatioTrk.clear();
}

//void JetSub::GetTopInfo(){
  
//}

void JetSub::GetCAjetInfo(){
  if(branchCAJet){
	Int_t nJets = branchCAJet->GetEntriesFast();
	for (Int_t i=0; i< nJets; i++){
      Jet *jet = (Jet*) branchCAJet->At(i); 
	  double JetPT = jet->PT;
	  if (JetPT < 500. || std::abs(jet->Eta) > 2.5) continue;
	  eventJS.CAJet_Pt.push_back(JetPT);
	  eventJS.CAJet_Mass.push_back(jet->Mass);
    }
  }
}

//Converts Delphes C/A jet constituents to fastjet pseudojets
void JetSub::DelphesToFastjet(Jet *jet, 
		std::vector<fastjet::PseudoJet> & JetConsts,  bool verbose){
  //cout << "mass " << jet->Mass << endl;
  int nConst = jet->Constituents.GetEntriesFast(); 
  if (verbose) printf ("Constituents of C/A jet : ");
  for (int ci=0; ci < nConst; ci++){
     TObject* object = jet->Constituents.At(ci);
     TLorentzVector momentum;
     const char* ConstType;
	 int jet_index = -1; // 0:track, 1:tower, 2:muon, -1:default 
     momentum.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
     if (object == 0) {
     	if (verbose) printf ("NULL constituent \n") ; 
         continue;
     }
     else if (object->IsA() == GenParticle::Class()) {
     	ConstType = "generator particle";
     	momentum += ((GenParticle*) object)->P4();
     }
     else if (object->IsA() == Track::Class()){
     	ConstType = "Track";
		jet_index = 0;
     	momentum += ((Track*) object)->P4();
     } 
     else if (object->IsA() == Tower::Class()){
     	ConstType = "Tower";
		jet_index = 1;
     	momentum += ((Tower*) object)->P4();
     }
     else if (object->IsA() == Muon::Class()){
     	ConstType = "Muon";
		jet_index = 2;
     	momentum += ((Muon*) object)->P4();
     }
     else {
     	if (verbose) printf("Unknown constituent type\n");
         continue; 
     }
     if (verbose) printf("%s : px=%10.2f, py=%10.2f, pz=%10.2f \n", 
     		ConstType, momentum.Px(), momentum.Py(),momentum.Pz());
	 fastjet::PseudoJet pjet(momentum.Px(), momentum.Py(),
							momentum.Pz(), momentum.E());
	 pjet.set_user_index(jet_index); // store particle flow info
	 JetConsts.push_back(pjet);
  }
}

//-----------------------------------------------------------------------
// Re-clusters input jet constituents using anti-KT algorithm 
// with jet-radius paramter R = Rsub and WTA recombination scheme, 
// resulting re-clustered output jets have pT > 500 GeV
void JetSub::ReCluster(const std::vector<fastjet::PseudoJet> & inputJet, 
						 std::vector<PFlowJet> & reclus_jets,
						 double Rsub){
	double pTmin = 500.;      // minimum pT for reclustered jets
	fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, Rsub,
									fastjet::WTA_pt_scheme);
	fastjet::ClusterSequence clust_seq(inputJet, jet_def);
	std::vector <fastjet::PseudoJet> incl_jets;
	incl_jets = sorted_by_pt(clust_seq.inclusive_jets(pTmin));
	for (unsigned long i=0; i < incl_jets.size(); i++){
	  std::vector<fastjet::PseudoJet> JetConsts; JetConsts.clear();
	  std::vector<fastjet::PseudoJet> TopConsts; TopConsts.clear();
	  JetConsts = incl_jets[i].constituents();
	  double Rtop = 600./incl_jets[i].pt();
	  fastjet::JetDefinition jet_def_top(fastjet::antikt_algorithm, Rtop,
									fastjet::WTA_pt_scheme);
	  fastjet::ClusterSequence clust_seq_top(JetConsts, jet_def_top);
	  std::vector <fastjet::PseudoJet> top_incl_jets;
	  top_incl_jets = sorted_by_pt(clust_seq_top.inclusive_jets(pTmin));
	  if (top_incl_jets.empty()) continue;
	  TopConsts = top_incl_jets[0].constituents();
	  PFlowJet PFJet(TopConsts);
	  reclus_jets.push_back(PFJet);
	}
}


//=======================================================
} /// namespace analysis
