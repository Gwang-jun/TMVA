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
  mytmva::mvaeffs(dataset.Data(), outfname.c_str(), 16.921443, 59095.846448); //S^prime and B^prime values, respectively
  //new sample and MC for 0-90%, after appropriate pthat, Ncoll, PVz and Bpt weight (pythia ref)
  //S^prime 16.921443    75.407109    213.175034   177.773357  152.039621  74.444409  12.971996
  //B^prime 59095.846448 55657.447266 32742.880769 5203.884706 1307.928288 191.675674 12.955197
  //new sample and MC for 0-90%, after appropriate pthat, Ncoll, PVz and Bpt weight (Gpt weight)
  //S^prime 17.346138    77.242990    218.711687   179.338176  152.039621  74.444409  12.971996
  //B^prime 59095.846448 55657.447266 32742.880769 5203.884706 1307.928288 191.675674 12.955197
  //new sample and MC for 0-90%, after pthat, Ncoll, PVz and Bpt weight
  //S^prime 18.115898    79.545126    223.637476   180.586069  152.054297  74.409970  12.974196
  //B^prime 59095.941497 55657.591854 32743.588366 5203.885727 1307.928213 191.695282 12.954612
  //new sample and MC for 0-90%, after pthat and Ncoll weight
  //S^prime 16.801998    75.824796    215.318613   179.262906  152.054297  74.409970  12.974196 
  //B^prime 59095.941497 55657.591854 32743.588366 5203.885727 1307.928213 191.695282 12.954612
  //new calculation for 0-90%
  //S^prime 22.814087    139.716880   296.019361   227.784184  209.593211  114.596774 17.199071
  //B^prime 42358.245609 47651.133124 31177.497188 5396.739317 1610.529490 224.436488 13.905774
  //previous calculation for 0-100%
  //S^prime 101.953617   194.055714   498.886649  208.675714  221.456861 102.726613 9.017866
  //B^prime 13730.981893 12019.550583 6573.084962 1073.808659 325.775993 41.322664  10.236871

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
