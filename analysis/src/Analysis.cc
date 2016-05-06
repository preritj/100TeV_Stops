#include "Analysis.h"
namespace analysis{
//==========================================================================

Analysis::Analysis(ExRootTreeReader *t_Reader, 
					std::string OutputFile, int flagEventWt){
  /// Initialize output file
  file = TFile::Open(OutputFile.c_str(),"RECREATE"); 
  if (!file) {
	std::cout <<"Could not open file for writing. aborting..." << std::endl;
	abort();
  }

  /// Create tree with event info
  eventTree = new TTree("event", "Tree with event info");
  /// Following info is stored in output root file
  eventTree->Branch("weight", &weight); 
  eventTree->Branch("MET", &eventOut.MET); 
  eventTree->Branch("HT", &eventOut.HT); 
  eventTree->Branch("Jet_Pt", &eventOut.Jet_Pt); 
  eventTree->Branch("Jet_Mass", &eventOut.Jet_Mass); 
  eventTree->Branch("Jet_MET_DeltaR", &eventOut.Jet_MET_DeltaR); 
  eventTree->Branch("IsoEl_Pt", &eventOut.IsoEl_Pt); 
  eventTree->Branch("IsoMu_Pt", &eventOut.IsoMu_Pt); 
  eventTree->Branch("CAJet_Pt", &eventJSOut.CAJet_Pt); 
  eventTree->Branch("CAJet_Mass", &eventJSOut.CAJet_Mass); 
  eventTree->Branch("Top_Pt", &eventJSOut.Top_Pt); 
  eventTree->Branch("Top_Mass", &eventJSOut.Top_Mass); 
  eventTree->Branch("Top_zMu", &eventJSOut.Top_zMu); 
  eventTree->Branch("Top_Tau32", &eventJSOut.Top_Tau32); 
  eventTree->Branch("Top_xMu", &eventJSOut.Top_xMu); 
  eventTree->Branch("Top_DeltaR_bMu", &eventJSOut.Top_DeltaR_bMu); 
  eventTree->Branch("Top_RatioTrk", &eventJSOut.Top_RatioTrk); 

  /// initialize analysis
  treeReader = t_Reader;
  nEvents = treeReader->GetEntries();
  UseEventWeight = flagEventWt;

  /// store pre-selection info
  PScuts = new TVectorD(2);
}

void Analysis::RunEvents(){
  Event ev(treeReader);
  JetSub js(treeReader);
  for (Long64_t i=0; i < nEvents; i++){
	ev.next();
    weight = 1.; // default weight
    if (UseEventWeight) weight = ev.GetEventWeight();
    (*PScuts)(0)=(*PScuts)(0)+weight; // sum of weights
    if (ev.IsVeto()) continue; // MET > 200 GeV, HT > 2 TeV
    (*PScuts)(1)=(*PScuts)(1)+weight; // sum of weights passing pre-selection
    js.next();
    eventOut = ev.Output();
    eventJSOut = js.Output();
    eventTree->Fill(); 
  }
}

void Analysis::Write(){
  PScuts->Write("PScuts");
  file->Write();
  //delete eventTree;
  delete file;
}


//==========================================================================
} // namespace analysis
