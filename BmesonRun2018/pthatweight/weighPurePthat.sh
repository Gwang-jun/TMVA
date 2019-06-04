#!/bin/bash

###
inputfile="/mnt/T2_US_MIT/submit-hi2/scratch/gwangjun/crab_Bfinder_20190520_Hydjet_Pythia8_BuToJpsiK_1033p1_pt3tkpt0p7dls2_v2_hardcut.root"
outputfile="/mnt/T2_US_MIT/submit-hi2/scratch/gwangjun/crab_Bfinder_20190520_Hydjet_Pythia8_BuToJpsiK_1033p1_pt3tkpt0p7dls2_v2_pthatweight_hardcut.root"
chan=3 # ([0: Bs]; [1: prompt psi']; [2: prompt X->jpsi + rho]; [3: B+])

##

crosssec=(
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={6.123e+06, 2.864e+06, 9.652e+05, 1.299e+05, 1.895e+04, 0.}; int genSignal[1]={6};' # 0: Bs
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={1.248e+05, 1.981e+04, 4.927e+03, 3.523e+02, 4.701e+01, 0.}; int genSignal[1]={7};' # 1: prompt psi'
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={3.775e+04, 2.059e+04, 6.592e+03, 3.118e+02, 2.730e+01, 0.}; int genSignal[1]={7};' # 2: prompt X->jpsi + rho
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={5.790e+07, 1.734e+07, 5.364e+06, 5.793e+05, 7.805e+04, 0.}; int genSignal[1]={1};' # 3: B+
)

tmp=$(date +%y%m%d%H%M%S)
sed '1i'"${crosssec[$chan]}" weighPurePthat.C > weighPurePthat_${tmp}.C

##

g++ weighPurePthat_${tmp}.C $(root-config --cflags --libs) -g -o weighPurePthat_${tmp}.exe 
[[ ${1:-0} -eq 1 ]] && { 
    cp "$inputfile" "$outputfile"
    set -x
    ./weighPurePthat_${tmp}.exe "$inputfile" "$outputfile" 
    set +x
}
rm weighPurePthat_${tmp}.exe
rm weighPurePthat_${tmp}.C

