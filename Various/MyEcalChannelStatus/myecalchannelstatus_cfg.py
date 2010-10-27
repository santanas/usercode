import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.load('Configuration/StandardSequences/GeometryDB_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

# Output ROOT file
process.TFileService = cms.Service("TFileService",
fileName = cms.string("EcalDeadChannels.root")
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR10_P_V10::All'
process.GlobalTag.globaltag = 'START3X_V26::All'

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    #        '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/450/A2DED983-7BD3-DF11-9C4B-001617C3B6FE.root'
    '/store/relval/CMSSW_3_5_7/RelValTTbar/GEN-SIM-RECO/START3X_V26-v1/0012/F8624D39-5349-DF11-A757-001A92971B36.root'
    #        'file:myfile.root'
    )
)


process.load('Various.MyEcalChannelStatus.myecalchannelstatus_cfi')

from Various.MyEcalChannelStatus.myecalchannelstatus_cfi import *

#process.demo = cms.EDAnalyzer('MyEcalChannelStatus'
#                              EcalEBInputTag = cms.InputTag("reducedEcalRecHitsEB"),
#                              EcalEEInputTag = cms.InputTag("reducedEcalRecHitsEE")
#)


process.p = cms.Path(process.demo)
