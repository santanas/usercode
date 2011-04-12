import FWCore.ParameterSet.Config as cms

rootTuplePFElectrons = cms.EDProducer("RootTupleMakerV2_PFElectrons",
    #InputTag = cms.InputTag('particleFlow'),            #default
    InputTag = cms.InputTag('particleFlow','electrons'), #all electrons (also bad ones)
    Prefix = cms.string('PFElectron'),
    Suffix = cms.string(''),
)
