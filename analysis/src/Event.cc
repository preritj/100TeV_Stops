#include "Event.h"
namespace analysis{
//=======================================================


Event::Event(ExRootTreeReader *t_Reader) {
  entry =0; /// event index 
  treeReader = t_Reader;
  branchJet        = treeReader->UseBranch("Jet");
  branchIsoEl        = treeReader->UseBranch("Electron");
  branchIsoMuon      = treeReader->UseBranch("Muon");
  branchMET          = treeReader->UseBranch("MissingET");
  branchEvent        = treeReader->UseBranch("Event");
}

void Event::next(){
  // Read the event entry
  treeReader->ReadEntry(entry++);
  Reset();
  GetMETjetInfo();
  if (veto) return;
  GetIsoLepInfo();
}

void Event::Reset(){
  veto = false;
  event.MET=0.; event.HT=0.;
  event.Jet_Pt.clear();
  event.Jet_Mass.clear();
  event.Jet_MET_DeltaPhi.clear();
  event.IsoEl_Pt.clear();
  event.IsoMu_Pt.clear();
}

double Event::GetEventWeight(){
  // Store weights for the event
  double weight=1.;
  if (branchEvent){
    LHEFEvent* LHEFev = (LHEFEvent*) branchEvent->At(0);
    weight = LHEFev->Weight;
  }
  return weight;
}

void Event::GetMETjetInfo(){
  /// MET and jet info
  MissingET *met;
  bool IsMET = false;
  if (branchMET){
    IsMET=true;
    met = (MissingET*) branchMET->At(0);
	event.MET = met->MET;
  }
  if(branchJet){
	Int_t nJets = branchJet->GetEntriesFast();
	double ScalarHT = 0.;	
	for (Int_t i=0; i< nJets; i++){
      Jet *jet = (Jet*) branchJet->At(i); 
	  double JetPT = jet->PT;
	  if (JetPT < 200. || std::abs(jet->Eta) > 2.5) continue;
	  event.Jet_Pt.push_back(JetPT);
	  event.Jet_Mass.push_back(jet->Mass);
	  double deltaPhi = -1.;
	  if (IsMET) {
        met = (MissingET*) branchMET->At(0);
		TLorentzVector metP4;
		metP4.SetPxPyPzE(met->MET*cos(met->Phi),
						 met->MET*sin(met->Phi), 0., 0.);
		deltaPhi=jet->P4().DeltaPhi(metP4);
      }
	  event.Jet_MET_DeltaPhi.push_back(deltaPhi);
	  ScalarHT += JetPT;
	}
	event.HT = ScalarHT;
  } 
  if (event.MET < 200. || event.HT < 2000.) veto=true;
}

void Event::GetIsoLepInfo(){
  /// check for isolated muons
  if (branchIsoMuon){
    for (int i=0; i < branchIsoMuon->GetEntriesFast(); i++){
      Muon* muon = (Muon*) branchIsoMuon->At(i);
      if (muon->PT < 50.) continue;
	  event.IsoMu_Pt.push_back(muon->PT);
	}
  }

  /// check for isolated electrons
  if (branchIsoEl){
    for (int i=0; i < branchIsoEl->GetEntriesFast(); i++){
      Electron* electron = (Electron*) branchIsoEl->At(i);
      if (electron->PT < 50.) continue;
	  event.IsoEl_Pt.push_back(electron->PT);
	}
  }
}

//=======================================================
} /// namespace analysis
