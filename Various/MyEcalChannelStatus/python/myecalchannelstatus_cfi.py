import FWCore.ParameterSet.Config as cms

demo = cms.EDAnalyzer('MyEcalChannelStatus',
                      EcalEBInputTag = cms.InputTag("reducedEcalRecHitsEB"),
                      EcalEEInputTag = cms.InputTag("reducedEcalRecHitsEE")
)
