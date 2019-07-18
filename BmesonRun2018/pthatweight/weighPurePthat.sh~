#!/bin/bash

###
inputfile="/mnt/T2_US_MIT/submit-hi2/scratch/gwangjun/Official/Bplus/crab_Bfinder_20190624_Hydjet_Pythia8_Official_BuToJpsiK_1033p1_pt3tkpt0p7dls2_allpthat.root"
outputfile="/mnt/T2_US_MIT/submit-hi2/scratch/gwangjun/Official/Bplus/crab_Bfinder_20190624_Hydjet_Pythia8_Official_BuToJpsiK_1033p1_pt3tkpt0p7dls2_allpthat_pthatweight.root"
chan=3 # ([0: Bs]; [1: prompt psi']; [2: prompt X->jpsi + rho]; [3: B+])

##

crosssec=(
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={7.845e+06, 3.002e+06, 1.048e+06, 1.257e+05, 1.962e+04, 0.}; int genSignal[1]={6};' # 0: Bs
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={1.248e+05, 1.981e+04, 4.927e+03, 3.523e+02, 4.701e+01, 0.}; int genSignal[1]={7};' # 1: prompt psi'
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={3.775e+04, 2.059e+04, 6.592e+03, 3.118e+02, 2.730e+01, 0.}; int genSignal[1]={7};' # 2: prompt X->jpsi + rho
    'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={4.898e+07, 1.534e+07, 5.325e+06, 5.640e+05, 8.233e+04, 0.}; int genSignal[1]={1};' # 3: B+
    #'const int nBins=5; float pthatBin[nBins]={5, 10, 15, 30, 50}; float crosssec[nBins+1]={1.296e+07, 3.175e+06, 9.996e+05, 8.699e+04, 1.100e+04, 0.}; int genSignal[1]={1};' # 3: B+ (non-prompt)
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

