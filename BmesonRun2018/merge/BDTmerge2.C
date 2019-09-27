#include "uti.h"

#define MAX_XB       20000

int main()
{
  
  TFile* input1 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusMC_CutsGA_BDT_5p0_7p0_6-4-8-0-13-15.root");
  TFile* input2 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusMC_CutsGA_BDT_7p0_10p0_6-4-8-0-13-15.root");
  TFile* input3 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusMC_CutsGA_BDT_10p0_15p0_6-4-8-0-13-15.root");
  TFile* input4 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusMC_CutsGA_BDT_15p0_20p0_6-4-8-0-13-15.root");
  TFile* input5 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusMC_CutsGA_BDT_20p0_30p0_6-4-8-0-13-15.root");
  TFile* input6 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusMC_CutsGA_BDT_30p0_50p0_6-4-8-0-13-15.root");
  TFile* input7 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusMC_CutsGA_BDT_50p0_100p0_6-4-8-0-13-15.root");
  
  /*
  TFile* input1 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusData_CutsGA_BDT_5p0_7p0_6-4-8-0-13-15.root");
  TFile* input2 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusData_CutsGA_BDT_7p0_10p0_6-4-8-0-13-15.root");
  TFile* input3 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusData_CutsGA_BDT_10p0_15p0_6-4-8-0-13-15.root");
  TFile* input4 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusData_CutsGA_BDT_15p0_20p0_6-4-8-0-13-15.root");
  TFile* input5 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusData_CutsGA_BDT_20p0_30p0_6-4-8-0-13-15.root");
  TFile* input6 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusData_CutsGA_BDT_30p0_50p0_6-4-8-0-13-15.root");
  TFile* input7 = new TFile("/raid5/data/gwangjun/mvaprod/TMVA_BplusData_CutsGA_BDT_50p0_100p0_6-4-8-0-13-15.root");
  */

  TTree* BDT1 = (TTree*) input1->Get("dataset/mva");
  TTree* BDT2 = (TTree*) input2->Get("dataset/mva");
  TTree* BDT3 = (TTree*) input3->Get("dataset/mva");
  TTree* BDT4 = (TTree*) input4->Get("dataset/mva");
  TTree* BDT5 = (TTree*) input5->Get("dataset/mva");
  TTree* BDT6 = (TTree*) input6->Get("dataset/mva");
  TTree* BDT7 = (TTree*) input7->Get("dataset/mva");

  int n = BDT1->GetEntries();

  int mvaBsize;
  float BDT_5p0_7p0[MAX_XB];
  float BDT_7p0_10p0[MAX_XB];
  float BDT_10p0_15p0[MAX_XB];
  float BDT_15p0_20p0[MAX_XB];
  float BDT_20p0_30p0[MAX_XB];
  float BDT_30p0_50p0[MAX_XB];
  float BDT_50p0_100p0[MAX_XB];
  
  BDT1->SetBranchAddress("mvaBsize", &mvaBsize);
  BDT1->SetBranchAddress("BDT_5p0_7p0", BDT_5p0_7p0);
  BDT2->SetBranchAddress("BDT_7p0_10p0", BDT_7p0_10p0);
  BDT3->SetBranchAddress("BDT_10p0_15p0", BDT_10p0_15p0);
  BDT4->SetBranchAddress("BDT_15p0_20p0", BDT_15p0_20p0);
  BDT5->SetBranchAddress("BDT_20p0_30p0", BDT_20p0_30p0);
  BDT6->SetBranchAddress("BDT_30p0_50p0", BDT_30p0_50p0);
  BDT7->SetBranchAddress("BDT_50p0_100p0", BDT_50p0_100p0);

  TFile* output = new TFile("/raid5/data/gwangjun/TMVA_BplusMC2.root","recreate");
  TTree* BDT = new TTree("BDT","BDT");
  float BDT_5_7_New[MAX_XB];
  float BDT_7_10_New[MAX_XB];
  float BDT_10_15_New[MAX_XB];
  float BDT_15_20_New[MAX_XB];
  float BDT_20_30_New[MAX_XB];
  float BDT_30_50_New[MAX_XB];
  float BDT_50_100_New[MAX_XB];

  BDT->Branch("Bsize", &mvaBsize);
  BDT->Branch("BDT_5_7", BDT_5_7_New, "BDT_5_7[Bsize]/D");
  BDT->Branch("BDT_7_10", BDT_7_10_New, "BDT_7_10[Bsize]/D");
  BDT->Branch("BDT_10_15", BDT_10_15_New, "BDT_10_15[Bsize]/D");
  BDT->Branch("BDT_15_20", BDT_15_20_New, "BDT_15_20[Bsize]/D");
  BDT->Branch("BDT_20_30", BDT_20_30_New, "BDT_20_30[Bsize]/D");
  BDT->Branch("BDT_30_50", BDT_30_50_New, "BDT_30_50[Bsize]/D");
  BDT->Branch("BDT_50_100", BDT_50_100_New, "BDT_50_100[Bsize]/D");
  
  for(int i=0;i<n;i++)
    {
      BDT1->GetEntry(i);
      BDT2->GetEntry(i);      
      BDT3->GetEntry(i);      
      BDT4->GetEntry(i);      
      BDT5->GetEntry(i);      
      BDT6->GetEntry(i);      
      BDT7->GetEntry(i);      
      for(int j=0;j<mvaBsize;j++)
	{	  	  
	  BDT_5_7_New[j]=BDT_5p0_7p0[j];
	  BDT_7_10_New[j]=BDT_7p0_10p0[j];
	  BDT_10_15_New[j]=BDT_10p0_15p0[j];
	  BDT_15_20_New[j]=BDT_15p0_20p0[j];
	  BDT_20_30_New[j]=BDT_20p0_30p0[j];
	  BDT_30_50_New[j]=BDT_30p0_50p0[j];
	  BDT_50_100_New[j]=BDT_50p0_100p0[j];
	}
      BDT->Fill();
    }

  output->Write();
  output->Close();

  return 0;
}
