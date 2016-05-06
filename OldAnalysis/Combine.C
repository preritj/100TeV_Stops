#include "TH1.h"
#include "TH2.h"

void Combine(const char* inputFile){
  TFile* f = TFile::Open(inputFile,"READ");
  TH1 *myhist = (TH1F*)f->Get("MET_1tag");
  TVectorF CutFlow = (TVectorF)f->Get("CutFlow");
  CutFlow.Print();
  myhist->Draw();
}
