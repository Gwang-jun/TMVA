// https://root.cern.ch/doc/v610/TMVAGui_8cxx_source.html
// https://root.cern.ch/doc/v608/efficiencies_8cxx_source.html
#include <iostream>
#include <string>

#include <TKey.h>
#include <TList.h>
#include "TMVA/efficiencies.h"
#include "TMVA/mvas.h"
#include "TMVA/correlations.h"

#include "mvaeffs.h"
#include "xjjcuti.h"

namespace mytmva
{
  void guiefficiencies(std::string outputname, float ptmin, float ptmax, std::string mymethod, std::string stage = "0,1,2,3");
  void efficiencies(std::string outfname);
}

void mytmva::guiefficiencies(std::string outputname, float ptmin, float ptmax, std::string mymethod, std::string stage/* = "0,1,2,3"*/)
{
  mymethod = xjjc::str_replaceall(mymethod, " ", "");
  std::string outfname(Form("%s_%s_%s_%s_%s.root", outputname.c_str(), xjjc::str_replaceallspecial(mymethod).c_str(), 
                            xjjc::number_to_string(ptmin).c_str(), (ptmax<0?"inf":xjjc::number_to_string(ptmax).c_str()), 
                            xjjc::str_replaceall(stage, ",", "-").c_str()));
  mytmva::efficiencies(outfname);
}

void mytmva::efficiencies(std::string outfname)
{
  TString dataset("");
  std::string outputstr = xjjc::str_replaceallspecial(outfname);

  // set up dataset
  TFile* file = TFile::Open( outfname.c_str() );
  if(!file)
    { std::cout << "==> Abort " << __FUNCTION__ << ", please verify filename." << std::endl; return; }
  if(file->GetListOfKeys()->GetEntries()<=0)
    { std::cout << "==> Abort " << __FUNCTION__ << ", please verify if dataset exist." << std::endl; return; }
  // --->>
  if( (dataset==""||dataset.IsWhitespace()) && (file->GetListOfKeys()->GetEntries()==1))
    { dataset = ((TKey*)file->GetListOfKeys()->At(0))->GetName(); }
  // <<---
  else if((dataset==""||dataset.IsWhitespace()) && (file->GetListOfKeys()->GetEntries()>=1))
    {
      std::cout << "==> Warning " << __FUNCTION__ << ", more than 1 dataset." << std::endl;
      file->ls(); return;
    }
  else { return; }

  // TMVA::efficiencies(dataset.Data(), outfname.c_str(), 1);
  TMVA::efficiencies(dataset.Data(), outfname.c_str(), 2);
  // TMVA::efficiencies(dataset.Data(), outfname.c_str(), 3);
  TMVA::mvas(dataset.Data(), outfname.c_str(), TMVA::kCompareType);
  TMVA::correlations(dataset.Data(), outfname.c_str());
  //mytmva::mvaeffs(dataset.Data(), outfname.c_str());
  //mytmva::mvaeffs(dataset.Data(), outfname.c_str(), 1.e+3, 1.e+5);
  mytmva::mvaeffs(dataset.Data(), outfname.c_str(), 76.27, 237.12); //S^prime and B^prime values, respectively
  /*
    New Bpt weight and interbinnings
    S^prime 11.20  90.92    266.22   187.49  234.09  76.27  35.13 11.87
    B^prime 127.92 94436.68 49741.74 7575.04 1964.87 237.12 28.11 10.43
    New Bpt weight and Muon selections
    S^prime 20.80     82.88    244.98   172.21  235.14  112.34 13.66  
    B^prime 122868.75 94285.03 49713.63 7575.41 1963.65 265.15 18.82
  */

  gSystem->Exec(Form("rm %s/plots/*.png", dataset.Data()));
  gSystem->Exec(Form("mkdir -p %s/plots/%s", dataset.Data(), outputstr.c_str()));
  gSystem->Exec(Form("mv %s/plots/*.eps %s/plots/%s/", dataset.Data(), dataset.Data(), outputstr.c_str()));
}

int main(int argc, char* argv[])
{
  if(argc==6)
    { mytmva::guiefficiencies(argv[1], atof(argv[2]), atof(argv[3]), argv[4], argv[5]); return 0; }
  return 1;
}
