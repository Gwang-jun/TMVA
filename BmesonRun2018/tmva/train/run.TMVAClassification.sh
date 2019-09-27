#!/bin/bash

##
ptmin=30 ; ptmax=40 ;
inputs=/raid5/data/gwangjun/crab_Bfinder_20190624_Hydjet_Pythia8_Official_BuToJpsiK_1033p1_pt3tkpt0p7dls2_allpthat_pthatweight.root ;
output=rootfiles/TMVA_Bplus ;
inputb=/raid5/data/gwangjun/crab_Bfinder_20190513_HIDoubleMuon__PsiPeri__HIRun2018A_04Apr2019_v1_1033p1_GoldenJSON_skimhltBsize_ntKp.root ;

#inputm=$inputs ;
inputm=$inputb ;
# outputmva=/mnt/hadoop/cms/store/user/gwangjun/tmva ;
# outputmva=/afs/lns.mit.edu/user/gwangjun/CMSSW_10_3_2/src/BmesonRun2018/tmva/train ;
outputmva=/home/gwangjun/BmesonRun2018/tmva/train ;

# prefilter
cut="pprimaryVertexFilter && phfCoincFilter2Th4 && pclusterCompatibilityFilter && Btrk1Pt>0.9 && Bpt>5.0 && (BsvpvDistance/BsvpvDisErr)>2.0 && Bchi2cl>0.05 && TMath::Abs(Btrk1Eta)<2.4 && TMath::Abs(By)<2.4 && TMath::Abs(PVz)<15 && Bmass>5 && Bmass<6 && TMath::Abs(Bmumumass-3.096900)<0.15 && Bmu1SoftMuID && Bmu2SoftMuID && ((TMath::Abs(Bmu1eta)<1.2 && Bmu1pt>3.5) || (TMath::Abs(Bmu1eta)>1.2 && TMath::Abs(Bmu1eta)<2.1 && Bmu1pt>5.47-1.89*TMath::Abs(Bmu1eta)) || (TMath::Abs(Bmu1eta)>2.1 && TMath::Abs(Bmu1eta)<2.4 && Bmu1pt>1.5)) && ((TMath::Abs(Bmu2eta)<1.2 && Bmu2pt>3.5) || (TMath::Abs(Bmu2eta)>1.2 && TMath::Abs(Bmu2eta)<2.1 && Bmu2pt>5.47-1.89*TMath::Abs(Bmu2eta)) || (TMath::Abs(Bmu2eta)>2.1 && TMath::Abs(Bmu2eta)<2.4 && Bmu2pt>1.5)) && Bmu1isTriggered && Bmu2isTriggered && (Btrk1PixelHit+Btrk1StripHit)>=11 && (Btrk1Chi2ndf/(Btrk1nStripLayer+Btrk1nPixelLayer))<0.18 && TMath::Abs(Btrk1PtErr/Btrk1Pt)<0.1"

#algo="CutsGA,BDT,BDTD,BDTG"
algo="CutsGA,BDT"
stages="6,4,8,0,13,15" # see definition below

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
    '#  15 :  ("Btrk1Dxysig" , "TMath::Abs(Btrk1Dxy1/Btrk1DxyError1)"                                                        , "FMax")  #'
    '#  16 :  ("Btrk1Dzsig" , "TMath::Abs(Btrk1Dz1/Btrk1DzError1)"                                                           , "FMax")  #'
)


cuts="${cut} && Bgen==23333"
cutb="${cut} && ((TMath::Abs(Bmass-5.27932)>0.15) || (TMath::Abs(Bmass-5.27932)<0.25))" # Bmass_pdg=5.27932GeV, sideband 0.15 ~ 0.25 for each side.
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
