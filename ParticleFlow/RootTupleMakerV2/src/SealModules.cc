#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "ParticleFlow/RootTupleMakerV2/interface/RootTupleMakerV2_Tree.h"
#include "ParticleFlow/RootTupleMakerV2/interface/RootTupleMakerV2_Event.h"
#include "ParticleFlow/RootTupleMakerV2/interface/RootTupleMakerV2_PFElectrons.h"

DEFINE_FWK_MODULE(RootTupleMakerV2_Tree);
DEFINE_FWK_MODULE(RootTupleMakerV2_Event);
DEFINE_FWK_MODULE(RootTupleMakerV2_PFElectrons);
