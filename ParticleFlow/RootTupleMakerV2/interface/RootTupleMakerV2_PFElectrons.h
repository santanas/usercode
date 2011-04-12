#ifndef RootTupleMakerV2PFElectrons
#define RootTupleMakerV2PFElectrons

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class RootTupleMakerV2_PFElectrons : public edm::EDProducer {
 public:
  explicit RootTupleMakerV2_PFElectrons(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix, suffix;
};

#endif
