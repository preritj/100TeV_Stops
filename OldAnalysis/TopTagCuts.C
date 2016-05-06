
void Analysis::ApplyTopTagOne(){
  IsTopTagOne=true;
  CutFlow.push_back(0.);
  CutName.push_back("One Top Tag");
}

void Analysis::ApplyTopTagTwo(){
  IsTopTagTwo=true;
  CutFlow.push_back(0.);
  CutName.push_back("Two Top Tags");
}

bool Analysis::TopTagOne(){
	bool IsCutPass = false; 
	if (MuonPT[0]>200.) IsCutPass=true;
	IsEventPass = IsEventPass && IsCutPass;
	return IsCutPass;
}

bool Analysis::TopTagTwo(){
	bool IsCutPass = false; 
	if (MuonPT[1]>200.) IsCutPass=true;
	IsEventPass = IsEventPass && IsCutPass;
	return IsCutPass;
}
