#include "uti.h"

Float_t criteria = 50.;
TString inputfile="/mnt/T2_US_MIT/submit-hi2/scratch/gwangjun/crab_Bfinder_20190520_Hydjet_Pythia8_BuToJpsiK_Pthat50_1033p1_pt3tkpt0p7dls2_v2.root";
TString outputfile="/mnt/T2_US_MIT/submit-hi2/scratch/gwangjun/crab_Bfinder_20190520_Hydjet_Pythia8_BuToJpsiK_Pthat50_1033p1_pt3tkpt0p7dls2_v2_hardcut.root";

int main()
{
  TFile* input = new TFile(inputfile.Data());
  TTree* oldhlt = (TTree*) input->Get("hltanalysis/HltTree");
  TTree* oldBfi_ntKp = (TTree*) input->Get("Bfinder/ntKp");
  TTree* oldBfi_ntGen = (TTree*) input->Get("Bfinder/ntGen");
  TTree* oldski = (TTree*) input->Get("skimanalysis/HltTree");
  TTree* oldHiF = (TTree*) input->Get("HiForest/HiForestInfo");
  //TTree* oldrun = (TTree*) input->Get("runAnalyzer/run");
  TTree* oldhiE = (TTree*) input->Get("hiEvtAnalyzer/HiTree");

  TFile* output = new TFile(outputfile.Data(),"recreate");
  TDirectory* newhlt = output->mkdir("hltanalysis","");
  TDirectory* newBfi = output->mkdir("Bfinder","");
  TDirectory* newski = output->mkdir("skimanalysis","");
  TDirectory* newHiF = output->mkdir("HiForest","");
  //TDirectory* newrun = output->mkdir("runAnalyzer","");
  TDirectory* newhiE = output->mkdir("hiEvtAnalyzer","");

  newhlt->cd();
  TTree* new_hlt = oldhlt->CloneTree(0);
  newBfi->cd();
  TTree* new_Bfi_ntKp = oldBfi_ntKp->CloneTree(0);
  TTree* new_Bfi_ntGen = oldBfi_ntGen->CloneTree(0);
  newski->cd();
  TTree* new_ski = oldski->CloneTree(0);
  newHiF->cd();
  TTree* new_HiF = oldHiF->CloneTree(0);
  //newrun->cd();
  //TTree* new_run = oldrun->CloneTree(0);
  newhiE->cd();
  TTree* new_hiE = oldhiE->CloneTree(0);

  int n = oldhiE->GetEntries();
  float pthat;
  oldhiE->SetBranchAddress("pthat",&pthat);
  for(int i=0;i<n;i++)
    {
      oldhlt->GetEntry(i);
      oldBfi_ntKp->GetEntry(i);
      oldBfi_ntGen->GetEntry(i);
      oldski->GetEntry(i);
      oldhiE->GetEntry(i);

      if(pthat<criteria) continue;

      newhlt->cd();
      new_hlt->Fill();
      newBfi->cd();
      new_Bfi_ntKp->Fill();
      new_Bfi_ntGen->Fill();
      newski->cd();
      new_ski->Fill();
      newhiE->cd();
      new_hiE->Fill();
    }

  int m = oldHiF->GetEntries();
  for(int i=0;i<m;i++)
    {
      oldHiF->GetEntry(i);
      //oldrun->GetEntry(i);

      newHiF->cd();
      new_HiF->Fill();
      //newrun->cd();
      //new_run->Fill();
    }

  output->Write();
  output->Close();

  return 0;
}
