/*
Simple macro showing how to access branches from the delphes output root file,
loop over events, and plot simple quantities such as the jet pt and the di-electron invariant
mass.

root -l examples/Example1.C'("delphes_output.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
//#else 
//class ExRootTreeReader;
//class Jet;
#endif

#include "TH1.h"
#include "TH2.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
//#include <vector>
//#include "TChain.h"
//#include "TClonesArray.h"
//#include "TSystem.h"
//#include "TCanvas.h"
//#include "external/ExRootAnalysis/ExRootTreeReader.h"
//#include "classes/DelphesClasses.h"
//#include <iostream>
////#include "TClonesArray.h"
//#include <vector>
//#include "classes/DelphesClasses.h"
//#include "external/ExRootAnalysis/ExRootTreeReader.h"
//#include "external/ExRootAnalysis/ExRootResult.h"
using namespace std;


//class ExRootResult;
class ExRootTreeReader;
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

#include "Plots.h"
#include "Analysis.h"
#include "AnalysisInit.C"
#include "ZCuts.C"
#include "CutFlow.C"
#include "Plot.C"
#include "DrawHist.C"
#include "HTcuts.C"
#include "METcuts.C"
#include "JetCuts.C"
#include "LepCuts.C"
#include "DphiCuts.C"
#include "TopTag.C"
#include "TopTagCuts.C"
#include "JetSubStruct.C"







//Debugging generator level particles  
void Debug(TClonesArray *branchParticle){
	cout << "************ Event info ************" << endl;
	cout << "Generator level muons " << endl;
	//Loop over all generator level particles and display info
	for (int j=0; j< branchParticle->GetEntriesFast(); j++){
	   GenParticle *gen = (GenParticle*) branchParticle->At(j);
	   if (abs(gen->PID) == 13 && gen->Status == 1) 
       cout << gen->PT << " " << gen->Eta << " " << gen->Phi << endl;
    } 
}

//------------------------------------------------------------------------------
// Main Analysis

void RunAnalysis(const char *inputFile, const char *outputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);


  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Analysis* an = new Analysis(treeReader);

  // Settings for analysis 
  an->DebugMode(); // Debug mode
  an->DrawHist(); // Draw histograms
  //an->WriteOutput(outputFile); // write output (incl histograms if selected) to root file
  //an->WeightsOn(); // use event weights from LHEF (use this for snowmass background)
  an->WeightsOff(); // each event weight = 1 (default : use this for signal)

  // Cuts to apply 
  an->ApplyHTcuts();      // apply HT cuts
  an->ApplyMETcuts();     // aaply MET cuts
  an->ApplyJetCuts();     // apply jet pT cuts
  //an->ApplyZCuts();       // z-variable cuts
  an->ApplyLepCuts();     // No isolated leptons
  an->ApplyDphiCuts();    // dPhi(MET,jet) cuts 
  an->ApplyTopTagOne();   // 1 top-tag using muon
  an->ApplyTopTagTwo();   // 2 top-tags using muons

  // Run analysis code
  an->Run();
	
  delete treeReader;
  delete an;
}

