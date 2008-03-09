// -*- C++ -*-
//
// Package:    GenParticle
// Class:      GenParticle
// 
/**\class GenParticle GenParticle.cc GenParticle/GenParticle/src/GenParticle.cc

 Description: plot generator level quantities

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Francesco Santanastasio
//         Created:  Tue Feb 12 15:04:47 CET 2008
// $Id: GenParticle.cc,v 1.1 2008/02/12 15:47:54 santanas Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

// genparticles
#include "DataFormats/Candidate/interface/Candidate.h"

#include "TH1.h"
#include "TLorentzVector.h"

#include<iostream>

//
// class decleration
//

class GenParticle : public edm::EDAnalyzer {
   public:
      explicit GenParticle(const edm::ParameterSet&);
      ~GenParticle();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      int event;

      // ----------member data - histo -------------------
      TH1F * h_NullHisto; 

      TH1F * h_pT_LQ; 
      TH1F * h_pT_lepton; 
      TH1F * h_pT_quark; 

      TH1F * h_eta_LQ; 
      TH1F * h_eta_lepton; 
      TH1F * h_eta_quark; 

      TH1F * h_phi_LQ; 
      TH1F * h_phi_lepton; 
      TH1F * h_phi_quark; 

      TH1F * h_invMass_LQ; 
      TH1F * h_delta_invMass_LQ; 

      TH1F * h_decayRadius_LQ;
};

//
// namepsaces 
//

using namespace std;
using namespace reco;

//
// constants, enums and typedefs
//

int id_LQ=42;
int id_electron=11;
int id_down=1;
int id_up=2;

//
// static data member definitions
//

//
// constructors and destructor
//
GenParticle::GenParticle(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  h_NullHisto = fs->make<TH1F>("h_NullHisto" , "h_NullHisto" , 200 , -2 , 2 );

  h_pT_LQ = fs->make<TH1F>("h_pT_LQ" , "h_pT_LQ" , 100 , 0. , 1000. );
  h_pT_lepton = fs->make<TH1F>("h_pT_lepton" , "h_pT_lepton" , 100 , 0. , 1000. );
  h_pT_quark = fs->make<TH1F>("h_pT_quark" , "h_pT_quark" , 100 , 0. , 1000. );

  h_eta_LQ = fs->make<TH1F>("h_eta_LQ" , "h_eta_LQ" , 50 , -5. , 5. );
  h_eta_lepton = fs->make<TH1F>("h_eta_lepton" , "h_eta_lepton" , 50 , -5. , 5. );
  h_eta_quark = fs->make<TH1F>("h_eta_quark" , "h_eta_quark" , 50 , -5. , 5. );

  h_phi_LQ = fs->make<TH1F>("h_phi_LQ" , "h_phi_LQ" , 50 , -5. , 5. );
  h_phi_lepton = fs->make<TH1F>("h_phi_lepton" , "h_phi_lepton" , 50 , -5. , 5. );
  h_phi_quark = fs->make<TH1F>("h_phi_quark" , "h_phi_quark" , 50 , -5. , 5. );

  h_invMass_LQ = fs->make<TH1F>("h_invMass_LQ" , "h_invMass_LQ" , 4000 , 0. , 2000. );
  h_delta_invMass_LQ = fs->make<TH1F>("h_delta_invMass_LQ" , "h_delta_invMass_LQ" , 1000 , -1. , 1. );

  h_decayRadius_LQ = fs->make<TH1F>("h_decayRadius_LQ" , "h_decayRadius_LQ" , 1500 , 0. , 150. );

}


GenParticle::~GenParticle()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void GenParticle::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   event = iEvent.id().event();
   //cout << "event: " << event << endl; 


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif

   // -------- write here your analysis code ---------

   int nullvalue = 0;
   h_NullHisto->Fill( nullvalue );

   // Gen Particles
   edm::Handle<reco::CandidateCollection> genParticles;
   iEvent.getByLabel ( "genParticleCandidates", genParticles);
   // the replacement for this from release 1.6.8. on is:
   // Handle<GenParticleCollection> genParticles
   // event.getByLabel("genParticles", genParticles);


   for(size_t i = 0; i < genParticles->size(); ++ i) 
     {
        const Candidate & p = (*genParticles)[i];
	//const Candidate * mom = p.mother();

	int id = p.pdgId();
	float pt = p.pt();
	double eta = p.eta();
	double phi = p.phi();
	double mass = p.mass();

	// find mother index
	// 	int i_mom=-1;
	// 	int i1=0;
	// 	reco::CandidateCollection::const_iterator candIter;
	// 	for(candIter=genParticles->begin(); candIter!=genParticles->end(); candIter++) 
	// 	  {
	//  	    if(&(*candIter)==mom)
	//  	      {
	//  		i_mom=i1;
	// 	      }
	// 	    i1++;
	// 	  }
	//double vx = p.vx(), vy = p.vy(), vz = p.vz();
	//         int st = p.status();  
	//         const Candidate * mom = p.mother();
	//         int charge = p.charge();
	//         int n = p.numberOfDaughters();
	//         for(size_t j = 0; j < n; ++ j) {
	// 	  const Candidate * d = p.daughter( j );
	//  	 int dauId = d->pdgId();
	//}

	//LQ
	if(abs(id)==id_LQ)
	  {

	    //Distributions for LQ
	    h_pT_LQ->Fill(pt);
	    h_eta_LQ->Fill(eta);
	    h_phi_LQ->Fill(phi);

	    //LQ daughters
	    int n = p.numberOfDaughters();
	    TLorentzVector LQdau_lepton;
	    TVector3 LQdau_lepton_vertex;
	    TLorentzVector LQdau_quark;
	    TVector3 LQdau_quark_vertex;
	
	    //loop over LQ daughters
	    for(int j = 0; j < n; ++ j) 
	      {
		const Candidate * d = p.daughter( j );
		int dauId = d->pdgId();
		
		//lepton
		if(abs(dauId)==id_electron)
		  {
		    h_pT_lepton->Fill(d->pt());
		    h_eta_lepton->Fill(d->eta());
		    h_phi_lepton->Fill(d->phi());
		    LQdau_lepton.SetPtEtaPhiM(d->pt(),
					      d->eta(),
					      d->phi(),
					      d->mass());

		    LQdau_lepton_vertex.SetXYZ(d->vx(),d->vy(),d->vz());

		  }

		//quark
		if(abs(dauId)==id_up)
		  {
		    h_pT_quark->Fill(d->pt());
		    h_eta_quark->Fill(d->eta());
		    h_phi_quark->Fill(d->phi());
		    LQdau_quark.SetPtEtaPhiM(d->pt(),
					     d->eta(),
					     d->phi(),
					     d->mass());

		    LQdau_quark_vertex.SetXYZ(d->vx(),d->vy(),d->vz());
		  }
		
	      }//end loop over LQ daughters
	    
	    //LQ invariant mass
	    TLorentzVector LQ;
	    LQ = LQdau_lepton + LQdau_quark;
	    h_invMass_LQ->Fill(LQ.M());
	    float delta_invMass = LQ.M()-mass;
	    h_delta_invMass_LQ->Fill(delta_invMass);
	    
	    //LQ decay vertex
	    if(LQdau_quark_vertex.x() == LQdau_lepton_vertex.x() 
	       && LQdau_quark_vertex.y() == LQdau_lepton_vertex.y() 
	       && LQdau_quark_vertex.z() == LQdau_lepton_vertex.z() 
	       )
	      {
		h_decayRadius_LQ->Fill(LQdau_quark_vertex.Mag());
	      }
	    
	  }//end selecting LQ
	
     }//end loop over gen particles
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
GenParticle::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenParticle::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenParticle);
