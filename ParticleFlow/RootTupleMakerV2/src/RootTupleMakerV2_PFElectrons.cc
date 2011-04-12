#include "ParticleFlow/RootTupleMakerV2/interface/RootTupleMakerV2_PFElectrons.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "FWCore/Framework/interface/ESHandle.h"

RootTupleMakerV2_PFElectrons::RootTupleMakerV2_PFElectrons(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix"))
{
  produces <std::vector<double> > ( prefix + "Eta" + suffix );
  produces <std::vector<double> > ( prefix + "Phi" + suffix );
  produces <std::vector<double> > ( prefix + "Pt" + suffix );
  produces <std::vector<double> > ( prefix + "Energy" + suffix );
  produces <std::vector<double> > ( prefix + "RawEcalEnergy" + suffix );
  produces <std::vector<double> > ( prefix + "Mva" + suffix );
}

void RootTupleMakerV2_PFElectrons::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >  eta  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  phi  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pt  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  energy  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  rawecalenergy  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  mva  ( new std::vector<double>()  );

  //-----------------------------------------------------------------

  edm::Handle<reco::PFCandidateCollection> collection;
  iEvent.getByLabel(inputTag, collection);
  std::vector<reco::PFCandidate> candidates = (*collection.product());

  std::vector<reco::PFCandidate>::iterator it;
  for ( it = candidates.begin(); it != candidates.end(); ++it) {

    reco::PFCandidate::ParticleType type = it->particleId();
    // here you can ask for particle type, mu, e, gamma

    if ( type == reco::PFCandidate::e) {

      eta->push_back( it->eta() );
      phi->push_back( it->phi() );
      pt->push_back( it->pt() );
      energy->push_back( it->energy() );
      rawecalenergy->push_back( it->rawEcalEnergy() );
      mva->push_back( it->mva_e_pi() );
      
    } // end pf electron

  } // end loop on the candidates

  //-----------------------------------------------------------------
  // put vectors in the event
  iEvent.put( eta, prefix + "Eta" + suffix );
  iEvent.put( phi, prefix + "Phi" + suffix );
  iEvent.put( pt, prefix + "Pt" + suffix );
  iEvent.put( energy, prefix + "Energy" + suffix );
  iEvent.put( rawecalenergy, prefix + "RawEcalEnergy" + suffix );
  iEvent.put( mva, prefix + "Mva" + suffix );
}
