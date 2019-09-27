#include "uti.h"

#define MAX_XB       20000

int main()
{
  /*
  TFile* input1 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusMC_New3_5_7.root");
  TFile* input2 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusMC_7_10.root");
  TFile* input3 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusMC_10_15.root");
  TFile* input4 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusMC_15_20.root");
  TFile* input5 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusMC_20_30.root");
  TFile* input6 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusMC_30_50.root");
  TFile* input7 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusMC_50_100.root");
  */
  
  TFile* input1 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_New3_5_7.root");
  TFile* input2 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_7_10.root");
  TFile* input3 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_10_15.root");
  TFile* input4 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_15_20.root");
  TFile* input5 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_20_30.root");
  TFile* input6 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_30_50.root");
  TFile* input7 = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_50_100.root");
  

  TTree* BDT1 = (TTree*) input1->Get("BDT");
  TTree* BDT2 = (TTree*) input2->Get("BDT");
  TTree* BDT3 = (TTree*) input3->Get("BDT");
  TTree* BDT4 = (TTree*) input4->Get("BDT");
  TTree* BDT5 = (TTree*) input5->Get("BDT");
  TTree* BDT6 = (TTree*) input6->Get("BDT");
  TTree* BDT7 = (TTree*) input7->Get("BDT");

  int n = BDT1->GetEntries();

  int Bsize;
  double BDT_5_7[MAX_XB];
  double BDT_7_10[MAX_XB];
  double BDT_10_15[MAX_XB];
  double BDT_15_20[MAX_XB];
  double BDT_20_30[MAX_XB];
  double BDT_30_50[MAX_XB];
  double BDT_50_100[MAX_XB];
  
  BDT1->SetBranchAddress("Bsize", &Bsize);
  BDT1->SetBranchAddress("BDT_5_7", BDT_5_7);
  BDT2->SetBranchAddress("BDT_7_10", BDT_7_10);
  BDT3->SetBranchAddress("BDT_10_15", BDT_10_15);
  BDT4->SetBranchAddress("BDT_15_20", BDT_15_20);
  BDT5->SetBranchAddress("BDT_20_30", BDT_20_30);
  BDT6->SetBranchAddress("BDT_30_50", BDT_30_50);
  BDT7->SetBranchAddress("BDT_50_100", BDT_50_100);

  TFile* output = new TFile("/raid5/data/gwangjun/BDTC/TMVA_BplusData_New.root","recreate");
  TTree* BDT = new TTree("BDT","BDT");
  double BDT_5_7_New[MAX_XB];
  double BDT_7_10_New[MAX_XB];
  double BDT_10_15_New[MAX_XB];
  double BDT_15_20_New[MAX_XB];
  double BDT_20_30_New[MAX_XB];
  double BDT_30_50_New[MAX_XB];
  double BDT_50_100_New[MAX_XB];

  BDT->Branch("Bsize", &Bsize);
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
      for(int j=0;j<Bsize;j++)
	{	  	  
	  BDT_5_7_New[j]=BDT_5_7[j];
	  BDT_7_10_New[j]=BDT_7_10[j];
	  BDT_10_15_New[j]=BDT_10_15[j];
	  BDT_15_20_New[j]=BDT_15_20[j];
	  BDT_20_30_New[j]=BDT_20_30[j];
	  BDT_30_50_New[j]=BDT_30_50[j];
	  BDT_50_100_New[j]=BDT_50_100[j];
	}
      BDT->Fill();
    }

  output->Write();
  output->Close();

  return 0;
}
