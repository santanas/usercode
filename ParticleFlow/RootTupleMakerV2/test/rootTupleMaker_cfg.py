import FWCore.ParameterSet.Config as cms
process = cms.Process("PFNTUPLE")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

############## IMPORTANT ########################################
# If you run over many samples and you save the log, remember to reduce
# the size of the output by prescaling the report of the event number
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 10
#################################################################

# Load RootTupleMakerV2 modules
process.load('ParticleFlow.RootTupleMakerV2.Ntuple_cff')

# Output ROOT file
process.TFileService = cms.Service("TFileService",
        #fileName = cms.string('RootTupleMakerV2_output.root')
        fileName = cms.string('RootTupleMakerV2_output_RelValSingleElectronPt35_GEN-SIM-RECO_MC_42_V9-v1_420.root')
)

# Global tag (make sure it always matches with the global tag used to reconstruct input files)
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['mc']
#process.GlobalTag.globaltag = 'GR_R_39X_V6::All' # /EG/Run2010A-Dec22ReReco_v1/RECO and /Electron/Run2010B-Dec22ReReco_v1/RECO

# Events to process
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Options and Output Report
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input files
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
    'file:/home/santanas/ParticleFlow/Data/RelVal_420/RelValSingleElectronPt35_GEN-SIM-RECO_MC_42_V9-v1.root'
    #'rfio:/castor/cern.ch/cms/store/relval/CMSSW_4_2_0/RelValSingleElectronPt35/GEN-SIM-RECO/MC_42_V9-v1/0055/2EEEBED4-E65E-E011-B0BF-002618943868.root'
    ),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

# RootTupleMakerV2 tree
process.rootTupleTree = cms.EDAnalyzer("RootTupleMakerV2_Tree",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_rootTupleEvent_*_*',
        'keep *_rootTuplePFElectrons_*_*'
    )
)

# Local re-reco: Produce tracker rechits, pf rechits and pf clusters
process.localReReco = cms.Sequence(process.particleFlowCluster)

# Particle Flow re-processing
process.pfReReco = cms.Sequence(process.particleFlowReco)

# The complete reprocessing
process.rereco = cms.Path(process.localReReco+
                          process.pfReReco)

# Path definition (ntuple maker)
process.p = cms.Path(
    (
    process.rootTupleEvent +
    process.rootTuplePFElectrons
    #process.XXXXXX +
    )
    *process.rootTupleTree
)

# Schedule definition
#process.schedule = cms.Schedule(process.p)
process.schedule = cms.Schedule(process.rereco,process.p) #reco core pflow + ntuplemaker
