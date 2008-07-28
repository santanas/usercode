***********************************************
Full Simulation Production Sequence (a la CSA07)  
***********************************************

------------------------------------------------------------------------------
------------------------------------------------------------------------------

In this example: 

- The sample name is [MySampleName]

- The sample directory is [SAMPLE DIRECTORY]. It is a copy of the Leptoquark FullSimulation Template directory, as copied via the twiki. It contains the cfg template files, run____.pl files, and directories (GEN-SIM, DIGI2RAW, HLT, RECO) for output files (Log, Cfg & Csh files). If following the twiki directions, this will be your current directory, and of the form /afs/cern.ch/user/initial/username/.../[MySampleName]. 

- The work directory is called [WORKDIRECTORY_Version]. This is the directory where the .pl file will perform the " eval 'scramv1 runtime -csh(sh)'". It will be the source directory of a CMSSW version (e.g. .../CMSSW_1_x_x/src). 
  For GEN-SIM, CMSSW_1_4_12 is used. The work directory is of the form /afs/cern.ch/user/initial/username/.../CMSSW_1_4_12/src. For this example, it is called [WORKDIRECTORY_1_4_12]. 
  --> Note that in order to checkout 14x release you need to do
      setenv SCRAM_ARCH slc3_ia32_gcc323 (tcsh)
      or 
      export SCRAM_ARCH=slc3_ia32_gcc323 (bash)

  For DIGI2RAW, HLT and RECO, CMSSW_1_6_12 is used. The work directory is of the form /afs/cern.ch/user/initial/username/.../CMSSW_1_6_12/src. For this example, it is called [WORKDIRECTORY_1_6_12]. 

- P.S. more cfg files can be found at /CMSSW/Configuration/CSA07Production/operation
  A good contact person is Guillelmo Gomez-Ceballos ( guillelmo.gomez-ceballos@cern.ch )

------------------------------------------------------------------------------
------------------------------------------------------------------------------

0)
# Create output directories on castor (here you will find output .root files)
> rfmkdir /castor/cern.ch/user/initial/username/FullSim/[MySampleName]
> rfmkdir /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/{GEN-SIM,DIGI2RAW,HLT,RECO}

1) GEN-SIM

# edit CSA07_gen-sim_14x.cfg
>> modify pythia datacard
>> modify untracked PSet maxEvents = {untracked int32 input = 50}
>> modify untracked double crossSection = 0.072

# launch gen-sim 
> ./runGenSim.pl -c CSA07_gen-sim_14x.cfg -w [WORKDIRECTORY_1_4_12] -j 20 -i 1000 -b [MySampleName] -s [SAMPLE DIRECTORY]/GEN-SIM -C /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/GEN-SIM -r -q 1nd  

# Wait for GEN-SIM to complete before running DIGI2RAW. Check status with 'bjobs' command, or check for output root files with ' nsls -l castor/cern.ch/user/initial/username/FullSim/[MySampleName]/GEN-SIM '.

2) DIGI2RAW

# launch digi2raw
> ./runDigi2Raw.pl -c CSA07_Digi2Raw.cfg -w [WORKDIRECTORY_1_6_12] -j 20 -i 1000 -b [MySampleName] -S /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/GEN-SIM -s [SAMPLE DIRECTORY]/DIGI2RAW -C /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/DIGI2RAW -r -q 1nh  

# Wait for DIGI2RAW step to complete before running HLT.

3) HLT

# launch hlt
> ./runHLTFromRaw.pl -c CSA07_Raw2HLT.cfg -w [WORKDIRECTORY_1_6_12] -j 20 -i 1000 -b [MySampleName] -S /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/DIGI2RAW -s [SAMPLE DIRECTORY]/HLT -C /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/HLT -r -q 1nh  

# Wait for HLT step to complete before running RECO

4) RECO

# launch reco
> ./runReco.pl -c CSA07_Reco_woRS-frontier-100pb.cfg -w [WORKDIRECTORY_1_6_12] -j 20 -i 1000 -b [MySampleName] -S /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/HLT -s [SAMPLE DIRECTORY]/RECO -C /castor/cern.ch/user/initial/username/FullSim/[MySampleName]/RECO -r -q 1nh  


# Helpful Information:
-c:
Designates the template cfg to work from
-w:
Designates the work directory (directory where eval `scramv1 runtime -csh` is done)
-j #
Designates the number (#) of jobs to be conducted (or the number of root files created. Each root file contains number of events from PSet maxEvents). e.g. 'j 20' runs 20 jobs. 
-i #
Designates the monte carlo initial seed (default value is 1000)
-b
Designates the base name for the cfg files and output (root) files. It is convenient to make this the same as the '[MySampleName]' which you designated for the directory in which you copied the template files.
-s
Designates the directory where the output cfg, csh and log files are placed.
-S
Designates the castor directory from which the root file for the prior step is accessed (DIGI2RAW step accesses GEN-SIM root file, HLT  step accesses DIGI2RAW root file, RECO step accesses HLT root files)
-C
Designates the castor directory where the output root files are placed.
-r
Command to randomize the names of the jobs.
-q 1nd, -q 1nh
Designates the job to a queue of length. (n means 'normalised') 1nh = 1 hour; 1nd = 1 day; 1nw = 1 week.
Designate a length greater than (# events)*(est. CPU Time per event).
