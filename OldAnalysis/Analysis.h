#ifndef Analysis_h
#define Analysis_h

namespace analysis{

struct PFlowJet{
	TLorentzVector track, tower, muon;
	PFlowJet(){
    	track.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    	tower.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
    	muon.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
	}
};



}

class Analysis{
  private :
  	Int_t entry;
  	float weight;
  	bool IsHist, IsWeighted, IsEventPass, Debug;
  	ExRootTreeReader *treeReader;
  	Long64_t numberOfEntries;
  	Plots* plots;
    TFile *file;
	TClonesArray *branchCAJet;
	TClonesArray *branchEFlowTrack, *branchEFlowTower;
  	TClonesArray *branchEvent, *branchJet, *branchMuon; 
  	TClonesArray *branchParticle, *branchIsoMuon, *branchIsoEl;
  	TClonesArray *branchMissingET, *branchScalarHT;
  	vector <float> CutFlow, MuonPT; 
  	vector <string> CutName; 
    int CutFlowIt;
    float zVar;
	bool IsHTcuts, IsMETcuts;
    bool IsJetCuts, IsZCuts, IsDphiCuts, IsWriteOutput; 
    bool IsTopTagOne, IsTopTagTwo, IsLepCuts;
  public :
  	Analysis(ExRootTreeReader*); 
  	void DebugMode(), WeightsOn(), WeightsOff();
  	void DrawHist(), BookHist(), FillHist(), PrintHist();
  	void LoadEvent(), CutFlowUpdate(), PrintCutFlow() ;
    void TopTag(), CalcZ(), ApplyJetCuts(), ApplyZCuts();
    void Run(), ApplyTopTagOne(), ApplyTopTagTwo();
    void ApplyLepCuts(), ApplyDphiCuts();
	void ApplyHTcuts(), ApplyMETcuts(), JetSubStruct();
    void FillHist_MET(int), FillHist_HT(int), FillHist_MET_HT(int);
    void DrawHist(TH2*), DrawHist(TH1*, bool same=false), WriteOutput(const char*);
  	bool JetCuts(), TopTagOne(), TopTagTwo();
	bool HTcuts(), METcuts();
    bool ZCuts(), LepCuts(), DphiCuts();
  	void GenMuonTag(int), CheckMuJets(int);
	void IsoMuonTag(int),  JetMuonTag(int);
	void GetJetConsts(int, vector<fastjet::PseudoJet> & , bool verbose = false);
	void ReCluster(const vector<fastjet::PseudoJet> & , 
				   vector<fastjet::PseudoJet> &, double);
	double JetMass(const fastjet::PseudoJet &);
	PFlowJet JetP4(const fastjet::PseudoJet &);
	vector<int> TopsInJets();
	vector<int> TruthTops();
	//vector<double> TrimJetMass();
	void MuonInJet(const fastjet::PseudoJet &);
	void EventInfo();
};

#endif
