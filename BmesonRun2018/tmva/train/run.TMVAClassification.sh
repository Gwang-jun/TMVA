#!/bin/bash

##
ptmin=5; ptmax=7 ;
inputs="/mnt/T2_US_MIT/submit-hi2/scratch/gwangjun/crab_Bfinder_20190221_Pythia8_BuToJpsiK_Bpt0p0_1032_NoJSON_pthatweight_hardcut_v2.root" 
#inputs=/mnt/hadoop/cms/store/user/gwangjun/Bp2018/MC/crab_Bfinder_20190221_Pythia8_BuToJpsiK_Bpt0p0_1032_NoJSON_pthatweight_hardcut_v2.root ;
## inputs=/mnt/hadoop/cms/store/user/gwangjun/Bp2018/MC/Bntuple20160816_Bpt7svpv5p5Bpt10svpv3p5_BfinderMC_PbPb_Pythia8_BuToJpsiK_TuneCUEP8M1_20160816_bPt5jpsiPt0tkPt0p8_Bp_pthatweight_JingBDT.root ;
output=rootfiles/TMVA_X ;
inputb=/mnt/hadoop/cms/store/user/jwang/BntupleRun2018/crab_Bfinder_20181220_HIDoubleMuon_HIRun2018A_PromptReco_v1v2_1031_NoJSON_skimhltBsize_ntKp.root ;

inputm=$inputs ;
# outputmva=/mnt/hadoop/cms/store/user/gwangjun/tmva ;
outputmva=/afs/lns.mit.edu/user/gwangjun/CMSSW_10_3_2/src/BmesonRun2018/tmva/train ;

# prefilter
cut="Btrk1Pt>1.0 && Bpt>5.0 && (BsvpvDistance/BsvpvDisErr)>2.0 && Bchi2cl>0.05 && TMath::Abs(Btrk1Eta)<2.4 && TMath::Abs(By)<2.4 && TMath::Abs(PVz)<15 && Bmass>5 && Bmass<6 && Bmu1isGlobalMuon && Bmu2isGlobalMuon && Bmu1dxyPV<0.3 && Bmu2dxyPV<0.3 && Bmu1dzPV<20 && Bmu2dzPV<20 && Bmu1InPixelLayer>0 && (Bmu1InPixelLayer+Bmu1InStripLayer)>5 && Bmu2InPixelLayer>0 && (Bmu2InPixelLayer+Bmu2InStripLayer)>5 && ((TMath::Abs(Bmu1eta)<1.2 && Bmu1pt>3.5) || (TMath::Abs(Bmu1eta)>1.2 && TMath::Abs(Bmu1eta)<2.1 && Bmu1pt>(5.77-1.8*TMath::Abs(Bmu1eta))) || (TMath::Abs(Bmu1eta)>2.1 && TMath::Abs(Bmu1eta)<2.4 && Bmu1pt>1.8)) && TMath::Abs(Bmumumass-3.096900)<0.15 && Bmu1TMOneStationTight && Bmu2TMOneStationTight && Btrk1highPurity && (Btrk1PixelHit+Btrk1StripHit)>=11 && (Btrk1Chi2ndf/(Btrk1nStripLayer+Btrk1nPixelLayer))<0.18 && TMath::Abs(Btrk1PtErr/Btrk1Pt)<0.1 && hiBin>0 && hiBin<180"

#algo="CutsGA,CutsSA"
algo="BDT,BDTG,CutsGA,CutsSA"
#algo="BDT"
stages="6,5,8,0" # see definition below

sequence=0 # if it's on, remove stages 1 by 1. 

## ===== do not change the lines below =====

varlist=(
    '#  0  :  ("Bchi2cl"  , "Bchi2cl"                                                                                        , "FMax")  #' 
    '#  1  :  ("dRtrk1"   , "dRtrk1 := TMath::Sqrt(pow(TMath::ACos(TMath::Cos(Bujphi-Btrk1Phi)),2) + pow(Bujeta-Btrk1Eta,2))", "FMin")  #' 
    '#  2  :  ("dRtrk2"   , "dRtrk2 := TMath::Sqrt(pow(TMath::ACos(TMath::Cos(Bujphi-Btrk2Phi)),2) + pow(Bujeta-Btrk2Eta,2))", "FMin")  #' 
    '#  3  :  ("Qvalue"   , "Qvalue := (Bmass-3.096900-Btktkmass)"                                                           , "FMin")  #' 
    '#  4  :  ("Balpha"   , "Balpha"                                                                                         , "FMin")  #' 
    '#  5  :  ("costheta" , "costheta := TMath::Cos(Bdtheta)"                                                                , "FMax")  #' 
    '#  6  :  ("dls3D"    , "dls3D := TMath::Abs(BsvpvDistance/BsvpvDisErr)"                                                 , "FMax")  #' 
    '#  7  :  ("dls2D"    , "dls2D := TMath::Abs(BsvpvDistance_2D/BsvpvDisErr_2D)"                                           , "FMax")  #' 
    '#  8  :  ("Btrk1Pt"  , "Btrk1Pt"                                                                                        , "FMax")  #' 
    '#  9  :  ("Btrk2Pt"  , "Btrk2Pt"                                                                                        , "FMax")  #' 
    '#  10 :  ("trkptimba", "trkptimba := TMath::Abs((Btrk1Pt-Btrk2Pt) / (Btrk1Pt+Btrk2Pt))"                                 , "FMax")  #' 
    '#  11 :  ("By"       , "By"                                                                                             , ""    )  #'
    '#  12 :  ("Bmass"    , "Bmass"                                                                                          , ""    )  #'
    '#  13 :  ("Btrk1Eta" , "TMath::Abs(Btrk1Eta)"                                                                           , "FMin")  #'
    '#  14 :  ("Btrk2Eta" , "Btrk2Eta"                                                                                       , ""    )  #'
)


cuts="${cut} && Bgen==23333"
cutb="${cut} && ((Bmass>4.98&&Bmass<5.08) || (Bmass>5.48&&Bmass<5.58))" # Bmass_pdg=5.28GeV, sideband 0.2 ~ 0.3 for each side.
# rootfiles=rootfiles

## ===== do not do not do not change the lines below =====
IFS=','; allstages=($stages); unset IFS;
echo -e '

####################################################################################################################################
#                                                Variables \e[1;32m(To be used)\e[0m                                                            #
####################################################################################################################################
#                                                                                                                                  #'
vv=0
while(( $vv < ${#varlist[@]})) ; do
    for ss in ${allstages[@]} ; do [[ $ss == $vv ]] && { echo -en "\e[1;32m" ; break ; } ; done ;
    echo -e "${varlist[vv]}\e[0m"
    vv=$((vv+1))
done
echo '#                                                                                                                                  #
####################################################################################################################################

'
##
mkdir -p $output ; rm -r $output ;
tmp=$(date +%y%m%d%H%M%S)

##
[[ $# -eq 0 ]] && echo "usage: ./run_TMVAClassification.sh [train] [draw curves] [create BDT tree]"

g++ TMVAClassification.C $(root-config --libs --cflags) -lTMVA -lTMVAGui -g -o TMVAClassification_${tmp}.exe || exit 1
g++ guivariables.C $(root-config --libs --cflags) -lTMVA -lTMVAGui -g -o guivariables_${tmp}.exe || { rm *_${tmp}.exe ; exit 1 ; }
g++ guiefficiencies.C $(root-config --libs --cflags) -lTMVA -lTMVAGui -g -o guiefficiencies_${tmp}.exe || { rm *_${tmp}.exe ; exit 1 ; }
g++ guieffvar.C $(root-config --libs --cflags) -lTMVA -lTMVAGui -g -o guieffvar_${tmp}.exe || { rm *_${tmp}.exe ; exit 1 ; }
g++ mvaprod.C $(root-config --libs --cflags) -lTMVA -lTMVAGui -g -o mvaprod_${tmp}.exe || { rm *_${tmp}.exe ; exit 1 ; }

[[ ${1:-0} -eq 1 ]] && {
    conf=
    echo -e "\e[2m==> Do you really want to run\e[0m \e[1mTMVAClassification.C\e[0m \e[2m(it might be very slow)?\e[0m [y/n]"
    read conf
    while [[ $conf != 'y' && $conf != 'n' ]] ; do { echo "warning: input [y/n]" ; read conf ; } ; done ;
    [[ $conf == 'n' ]] && { rm *_${tmp}.exe ; exit ; }
}

stage=$stages
while [[ $stage == *,* ]]
do
# train
    [[ ${1:-0} -eq 1 ]] && { ./TMVAClassification_${tmp}.exe $inputs $inputb "$cuts" "$cutb" $output $ptmin $ptmax "$algo" "$stage"; } &
    [[ $sequence -eq 0 ]] && break;
    while [[ $stage != *, ]] ; do stage=${stage%%[0-9]} ; done ;
    stage=${stage%%,}
done
wait

# draw curves
[[ ${2:-0} -eq 1 ]] && { 
    ./guivariables_${tmp}.exe $output $ptmin $ptmax "$algo" "$stages"
    ./guiefficiencies_${tmp}.exe $output $ptmin $ptmax "$algo" "$stages"
}

# draw curve vs. var
[[ ${2:-0} -eq 1 && $sequence -eq 1 ]] && ./guieffvar_${tmp}.exe $output $ptmin $ptmax "$algo" "$stages"

# produce mva values
[[ ${3:-0} -eq 1 ]] && ./mvaprod_${tmp}.exe $inputm "Bfinder/ntKp" $output $outputmva $ptmin $ptmax "$algo" "${stages}"

##
rm *_${tmp}.exe