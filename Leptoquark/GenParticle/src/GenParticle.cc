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
// $Id$
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

#include "TH1.h"

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
      TH1D * h_NullHisto; 
};

//
// constants, enums and typedefs
//

using namespace std;

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
  h_NullHisto = fs->make<TH1D>("h_NullHisto" , "h_NullHisto" , 200 , -2 , 2 );
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
void
GenParticle::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
