***********************************
Fast Simulation Production Sequence
***********************************

------------------------------------------------------------------------------
------------------------------------------------------------------------------

In example 1): 

- the sample name is --> LQ1stgenM250_fast169_100pbScenario (Leptoquark 1st generation)

- the output directory where are cfg, csh and log files is --> 
  /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_9_fastSim/src/LQ1stgenM250_fast169_100pbScenario/
  (for example you can use the current directory where you find the AODIM directory 
  already created.)

- the work-directories where you make eval `scramv1 runtime -csh(sh)` are:
  /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_9_fastSim/src          for AODIM (1_6_9)

- P.S. more info can be found at 
  https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFastSimulation
  Contact people are
  Patrizia Azzi, Florian Beaudette
  
------------------------------------------------------------------------------
------------------------------------------------------------------------------

# Create output directory on castor (here you will find output .root files)
> rfmkdir /castor/cern.ch/user/s/santanas/FastSim/LQ1stgenM250_fast169_100pbScenario
> rfmkdir /castor/cern.ch/user/s/santanas/FastSim/LQ1stgenM250_fast169_100pbScenario/AODSIM

1) AODSIM, CMSSW_1_6_9, 100 pb-1 miscalibration/misalignment scenario

# edit makeAODSIM_fast169_100pbScenario.cfg
>> modify cfi file "Leptoquark/FastSimulation/data/LQ1stgen.cfi" (or create a new one)
   - modify: pythia datacard
   - modify: untracked double crossSection = xxx
>> modify untracked PSet maxEvents = {untracked int32 input = 1000}


# launch aodsim 
> ./runAODSIM.pl -c makeAODSIM_fast169_100pbScenario.cfg -w /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_9_fastSim/src -j 20 -i 1000 -b LQ1stgenM250_fast169_100pbScenario -s /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_9_fastSim/src/LQ1stgenM250_fast169_100pbScenario/AODSIM -C /castor/cern.ch/user/s/santanas/FastSim/LQ1stgenM250_fast169_100pbScenario/AODSIM -r -q 8nh  

2)  AODSIM, CMSSW_1_6_9, ideal miscalibration/misalignment scenario

# edit makeAODSIM_fast169_idealScenario.cfg
>> modify cfi file "Leptoquark/FastSimulation/data/LQ1stgen.cfi"
   - modify: pythia datacard
   - modify: untracked double crossSection = xxx
>> modify untracked PSet maxEvents = {untracked int32 input = 1000}


# launch aodsim 
> ./runAODSIM.pl -c makeAODSIM_fast169_idealScenario.cfg -w /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_9_fastSim/src -j 20 -i 1000 -b LQ1stgenM250_fast169_idealScenario -s /afs/cern.ch/user/s/santanas/scratch0/Releases/CMSSW_1_6_9_fastSim/src/LQ1stgenM250_fast169_idealScenario/AODSIM -C /castor/cern.ch/user/s/santanas/FastSim/LQ1stgenM250_fast169_idealScenario/AODSIM -r -q 8nh
