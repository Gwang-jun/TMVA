#include "../../includes/uti.h"
#include "./dataset/weights/rootfiles_TMVA_X_BDT_BDTG_CutsGA_CutsSA_LD_5p0_7p0_6-5-8-0-13_root/TMVAClassification_BDT.class.C"

#define MAX_XB       20000

void BDT(TString inputname, TString outputname,
	 Float_t ptmin, Float_t ptmax, TString mvatype)
{
  void makeoutput(TString infname, TString ofname, TString treename);
  makeoutput(inputname,Form("%s.root",outputname.Data()),Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax));
}

void makeoutput(TString infname, TString ofname, TString treename)
{
  TFile* inf = new TFile(infname);
  TTree* t = (TTree*)inf->Get("Bfinder/ntKp");
  //t->AddFriend("mvaTree");

  Int_t           Bsize;
  Float_t         Bchi2cl[MAX_XB];   //[Bsize]
  Float_t         BsvpvDistance[MAX_XB];
  Float_t         BsvpvDisErr[MAX_XB];
  Float_t         Btrk1Pt[MAX_XB];   //[Bsize]   
  Float_t         Btrk1Eta[MAX_XB];   //[Bsize]   
  Float_t         Bdtheta[MAX_XB]; 

  t->SetBranchAddress("Bsize", &Bsize);
  t->SetBranchAddress("Bchi2cl", Bchi2cl);
  t->SetBranchAddress("BsvpvDistance", BsvpvDistance);
  t->SetBranchAddress("BsvpvDisErr", BsvpvDisErr);
  //  t->SetBranchAddress("MVA", MVA);
  t->SetBranchAddress("Btrk1Pt", Btrk1Pt);
  t->SetBranchAddress("Bdtheta", Bdtheta);
  t->SetBranchAddress("Btrk1Eta", Btrk1Eta);

  std::vector<std::string> theInputVars;
  std::string a1 = "TMath::Abs(BsvpvDistance/BsvpvDisErr)";
  std::string a2 = "TMath::Cos(Bdtheta)";
  std::string a3 = "Btrk1Pt";
  std::string a4 = "Bchi2cl";
  std::string a5 = "TMath::Abs(Btrk1Eta)";  

  theInputVars.push_back(a1);
  theInputVars.push_back(a2);
  theInputVars.push_back(a3);
  theInputVars.push_back(a4);
  theInputVars.push_back(a5);

  std::vector<double> inputValues;
  ReadBDT mva(theInputVars);

  TFile* outf = new TFile(ofname,"recreate");
  TTree* mvaTree = new TTree(Form("%s",treename.Data()),"BDT");

  double BDT[MAX_XB];
  mvaTree->Branch("Bsize",&Bsize,"Bsize/I");
  mvaTree->Branch(Form("%s",treename.Data()),BDT,Form("%s[Bsize]/D",treename.Data()));
  std::cout<<std::endl;
  std::cout<<"  Input file: "<<infname<<std::endl;
  std::cout<<"  Calculating MVA values ..."<<std::endl;
  for(int i=0;i< t->GetEntries();i++)
    {
      if(i%100==0) std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<t->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/t->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;
      t->GetEntry(i);
      for(int j=0;j<Bsize;j++)
	{
	  inputValues.clear();

	  inputValues.push_back(TMath::Abs(BsvpvDistance[j]/BsvpvDisErr[j]));
	  inputValues.push_back(TMath::Cos(Bdtheta[j]));
	  inputValues.push_back(Bchi2cl[j]);
	  inputValues.push_back(Btrk1Pt[j]);
	  inputValues.push_back(TMath::Abs(Btrk1Eta[j]));
	  
	  BDT[j] = mva.GetMvaValue(inputValues);      
	}

      mvaTree->Fill();
    }
  std::cout<<std::endl<<"  Processed "<<"\033[1;31m"<<t->GetEntries()<<"\033[0m event(s)."<<std::endl;
  outf->cd();
  outf->Write();
  outf->Close();
}

int main(int argc, char* argv[])
{
  if(argc==6)
    {
      BDT(argv[1],argv[2],atof(argv[3]),atof(argv[4]),argv[5]);
      return 0;
    }
  else
    {
      std::cout<<"  Error: invalid argument number - BDT()"<<std::endl;
      return 1;
    }
}
