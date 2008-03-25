***********************************************
Full Simulation Production Sequence (a la CSA07)  
***********************************************

------------------------------------------------------------------------------
------------------------------------------------------------------------------

In this example: 

- the sample name is --> LQ6501stgen (Leptoquark 1st generation)

- the output directory where are cfg, csh and log files is --> 
  /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src/LQ6501stgen/
  (for example you can use the current directory where you find the 4 directories GEN-SIM, 
  DIGI2RAW, HLT and RECO already created.)

- the work-directory where you make eval `scramv1 runtime -csh(sh)` is:
  /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_4_12_CSA07-gen-sim/src   for GEN-SIM (1_4_12)
  /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src          for DIGI2RAW, HLT and RECO (1_6_7)

- P.S. more cfg files can be found at /CMSSW/Configuration/CSA07Production/operation
  A good contact person is Guillelmo Gomez-Ceballos ( guillelmo.gomez-ceballos@cern.ch )

------------------------------------------------------------------------------
------------------------------------------------------------------------------

# Create output directory on castor (here you will find output .root files)
> rfmkdir /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen
> rfmkdir /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/GEN-SIM
> rfmkdir /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/DIGI2RAW
> rfmkdir /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/HLT
> rfmkdir /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/RECO

1) GEN-SIM

# edit CSA07_gen-sim_14x.cfg
>> modify pythia datacard
>> modify untracked PSet maxEvents = {untracked int32 input = 50}
>> modify untracked double crossSection = 0.072

# launch gen-sim 
> ./runGenSim.pl -c CSA07_gen-sim_14x.cfg -w /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_4_12_CSA07-gen-sim/src -j 20 -i 1000 -b LQ6501stgen -s /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src/LQ6501stgen/GEN-SIM -C /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/GEN-SIM -r -q 1nd  

2) DIGI2RAW

# edit CSA07_Digi2Raw.cfg
>> modify untracked PSet maxEvents = {untracked int32 input = 50}

# launch digi2raw
> ./runDigi2Raw.pl -c CSA07_Digi2Raw.cfg -w /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src -j 20 -i 1000 -b LQ6501stgen -S /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/GEN-SIM -s /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src/LQ6501stgen/DIGI2RAW -C /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/DIGI2RAW -r -q 1nh  

3) HLT

# edit CSA07_Raw2HLT.cfg
>> modify untracked PSet maxEvents = {untracked int32 input = 50}

# launch hlt
> ./runHLTFromRaw.pl -c CSA07_Raw2HLT.cfg -w /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src -j 20 -i 1000 -b LQ6501stgen -S /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/DIGI2RAW -s /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src/LQ6501stgen/HLT -C /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/HLT -r -q 1nh  

4) RECO

# edit CSA07_Reco_woRS-frontier-100pb.cfg
>> modify untracked PSet maxEvents = {untracked int32 input = 50}

# launch reco
> ./runReco.pl -c CSA07_Reco_woRS-frontier-100pb.cfg -w /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src -j 20 -i 1000 -b LQ6501stgen -S /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/HLT -s /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_7_fullSim/src/LQ6501stgen/RECO -C /castor/cern.ch/user/s/santanas/FullSim/LQ6501stgen/RECO -r -q 1nh  

