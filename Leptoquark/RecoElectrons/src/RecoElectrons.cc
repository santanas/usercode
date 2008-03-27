// -*- C++ -*-
//
// Package:    RecoElectrons
// Class:      RecoElectrons
// 
/**\class RecoElectrons RecoElectrons.cc workspace/RecoElectrons/src/RecoElectrons.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  pts/10
//         Created:  Tue Feb 19 10:07:45 CET 2008
// $Id: RecoElectrons.cc,v 1.2 2008/03/24 10:27:12 santanas Exp $
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
#include "TH2.h"
#include "TVector3.h"

//reco electrons
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchElectron.h"

#include "EgammaAnalysis/ElectronIDAlgos/interface/CutBasedElectronID.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"

//genparticles
#include "DataFormats/Candidate/interface/Candidate.h"

//HLT
#include "FWCore/Framework/interface/TriggerNames.h"


//
// class decleration
//

class RecoElectrons : public edm::EDAnalyzer {
   public:
      explicit RecoElectrons(const edm::ParameterSet&);
      ~RecoElectrons();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      TH1F * h_N_recoEle; 
      TH1F * h_N_recoEle_pTcut; 
      TH1F * h_energy_recoEle;
      TH1F * h_pT_recoEle;
      TH1F * h_eta_recoEle;
      TH2F * h_p_minus_Esc_recoEle_vs_Esc; 
      TH2F * h_p_minus_Esc_recoEle_vs_ETsc; 
      TH2F * h_p_minus_Esc_recoEle_vs_etasc; 
      TH2F * h_p_minus_Esc_recoEle_vs_fBrem;
      TH1F * h_DeltaR_genEle_recoEle;
      TH1F * h_DeltaR_genEle_recoEle_min;
      TH1F * h_E_recoEle_over_E_genEle_DeltaRmatch;
      int event;
};

//Options

bool printOut=false;

//
// constants, enums and typedefs
//

int id_LQ=42;
int id_electron=11;
int id_down=1;
int id_up=2;

float pTcut_genEle=50; //GeV
float pTcut_recoEle=50; //GeV

// namespaces
using namespace std;
using namespace reco;


//
// static data member definitions
//

float DeltaR_genEle_recoEle_min_cut=0.1;

//
// constructors and destructor
//
RecoElectrons::RecoElectrons(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  //histo = fs->make<TH1D>("charge" , "Charges" , 200 , -2 , 2 );

  //## number of reco electrons
  h_N_recoEle = fs->make<TH1F>("h_N_recoEle","h_N_recoEle",30,-0.5,30.5);
  h_N_recoEle_pTcut = fs->make<TH1F>("h_N_recoEle_pTcut","h_N_recoEle_pTcut",30,-0.5,30.5);

  //## pT/eta reco electrons
  h_energy_recoEle = fs->make<TH1F>("h_energy_recoEle","h_energy_recoEle",100,0,1000);
  h_pT_recoEle = fs->make<TH1F>("h_pT_recoEle","h_pT_recoEle",100,0,1000);
  h_eta_recoEle = fs->make<TH1F>("h_eta_recoEle","h_eta_recoEle",100,-4,4);

  //## p_at_Vtx - E_sc  
  h_p_minus_Esc_recoEle_vs_Esc =  fs->make<TH2F>("h_p_minus_Esc_recoEle_vs_Esc",
 						 "h_p_minus_Esc_recoEle_vs_Esc",
 						 20,0,500,
 						 100,-200,200);

  h_p_minus_Esc_recoEle_vs_ETsc =  fs->make<TH2F>("h_p_minus_Esc_recoEle_vs_ETsc",
						  "h_p_minus_Esc_recoEle_vs_ETsc",
						  20,0,500,
						  100,-200,200);
  
  h_p_minus_Esc_recoEle_vs_etasc =  fs->make<TH2F>("h_p_minus_Esc_recoEle_vs_etasc",
 						   "h_p_minus_Esc_recoEle_vs_etasc",
						   50,-3,3, 						   
						   100,-200,200);

  h_p_minus_Esc_recoEle_vs_fBrem =  fs->make<TH2F>("h_p_minus_Esc_recoEle_vs_fBrem",
 						   "h_p_minus_Esc_recoEle_vs_fBrem",
						   100,-1,1, 						   
						   100,-200,200);


  //## Energy Resolution with MC matching
  h_DeltaR_genEle_recoEle = fs->make<TH1F>("h_DeltaR_genEle_recoEle","h_DeltaR_genEle_recoEle",200,0.,10.);
  h_DeltaR_genEle_recoEle_min = fs->make<TH1F>("h_DeltaR_genEle_recoEle_min","h_DeltaR_genEle_recoEle_min",200,0.,10.);
  h_E_recoEle_over_E_genEle_DeltaRmatch = fs->make<TH1F>("h_E_recoEle_over_E_genEle_DeltaRmatch",
							 "h_E_recoEle_over_E_genEle_DeltaRmatch",1000,0.,2.);

}


RecoElectrons::~RecoElectrons()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
RecoElectrons::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  event = iEvent.id().event();
  if(printOut)
    {
      cout << endl;
      cout << "**************** Event: " << event << endl; 
      cout << endl;
    }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  // Read in Gsf electrons
  const reco::PixelMatchGsfElectronCollection* electrons;
  edm::Handle<reco::PixelMatchGsfElectronCollection> pElectrons;
  iEvent.getByLabel(edm::InputTag("pixelMatchGsfElectrons",""), pElectrons);
  electrons = pElectrons.product();

  // Read in GenParticles
  edm::Handle<reco::CandidateCollection> genParticles;
  iEvent.getByLabel ( "genParticleCandidates", genParticles);
  
  if(printOut)
    cout << "******************* electron->size() : " << electrons->size() << endl;
  h_N_recoEle->Fill(electrons->size());

  // Loop over gsf electrons
  int ele_idx=0;
  int N_recoEle_pTcut=0;
  reco::PixelMatchGsfElectronCollection::const_iterator electron;
  for (electron = (*electrons).begin();
       electron != (*electrons).end(); ++electron) 
    {

      //## Main variables       
      //       float pT_ele=electron->pt();
      //       float E_ele=electron->energy();
      //       float E_sc_ele=electron->caloEnergy();
      //       float eta_ele=electron->eta();
      //       float phi_ele=electron->phi();

      if(electron->pt()>pTcut_recoEle)
	N_recoEle_pTcut++;

      h_energy_recoEle->Fill(electron->energy());
      h_pT_recoEle->Fill(electron->pt());
      h_eta_recoEle->Fill(electron->eta());
      
      //## Cluster shape variables
      bool hasBarrel=true;
      bool hasEndcap=true;

      edm::Handle<reco::BasicClusterShapeAssociationCollection> barrelClShpHandle;
      iEvent.getByLabel("hybridSuperClusters","hybridShapeAssoc", barrelClShpHandle);
      const reco::BasicClusterShapeAssociationCollection& barrelClShpMap = *barrelClShpHandle;
	  
      edm::Handle<reco::BasicClusterShapeAssociationCollection> endcapClShpHandle;
      iEvent.getByLabel("islandBasicClusters","islandEndcapShapeAssoc", endcapClShpHandle); 
      const reco::BasicClusterShapeAssociationCollection& endcapClShpMap = *endcapClShpHandle;
	  
      reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;
      seedShpItr = barrelClShpMap.find(electron->superCluster()->seed());
      if(seedShpItr==barrelClShpMap.end()) 
	{
	  hasBarrel=false;
	  seedShpItr=endcapClShpMap.find(electron->superCluster()->seed());
	  if(seedShpItr==endcapClShpMap.end()) hasEndcap=false;
	}
      
      const reco::ClusterShapeRef& sClShape = seedShpItr->val;  
      // 	  privateData_->e3x3->push_back(sClShape->e3x3());
      // 	  privateData_->e5x5->push_back(sClShape->e5x5());
      // 	  privateData_->eMax->push_back(sClShape->eMax());
      // 	  privateData_->lat->push_back(sClShape->lat());
      // 	  privateData_->phiLat->push_back(sClShape->phiLat());
      // 	  privateData_->etaLat->push_back(sClShape->etaLat());
      // 	  privateData_->e2x2->push_back(sClShape->e2x2());
      // 	  privateData_->e2nd->push_back(sClShape->e2nd());
      // 	  privateData_->s1s9->push_back(sClShape->eMax()/sClShape->e3x3());
      // 	  privateData_->s9s25->push_back(sClShape->e3x3()/sClShape->e5x5());
      // 	  privateData_->covEtaEta->push_back(sClShape->covEtaEta());
      // 	  privateData_->covEtaPhi->push_back(sClShape->covEtaPhi());
      // 	  privateData_->covPhiPhi->push_back(sClShape->covPhiPhi());
      // 	  privateData_->a20->push_back(sClShape->zernike20());
      // 	  privateData_->a42->push_back(sClShape->zernike42());
  
      //## Robust electron ID variables       
      float hOverE = electron->hadronicOverEm();
      float sigmaee = sqrt(sClShape->covEtaEta());
      float deltaPhiIn = electron->deltaPhiSuperClusterTrackAtVtx();
      float deltaEtaIn = electron->deltaEtaSuperClusterTrackAtVtx();

      //correct sigmaetaeta dependence on eta in endcap
      sigmaee = sigmaee - 0.02*(fabs(electron->eta()) - 2.3); 
      
      //Matteo Sani's comment ( in italian, sorry:) )
      //Per quanto riguarda la correzione a sigmaee questa e` una semplice valutazione della variazione di sigmaee dovuta alla
      //particolare geometria dei cristalli nell'endcap. In pratica si corregge la sigmaee in funzione di eta per la diversa
      //area dei cristalli vista dal punto di interazione.

      //## Other useful variables       
      //       float eOverP = electron->eSuperClusterOverP();
      //       float eSeed = electron->superCluster()->seed()->energy();
      float pin  = electron->trackMomentumAtVtx().R();   
      float pout = electron->trackMomentumOut().R(); 
      float fBrem = (pin-pout)/pin;

      //## Some print out
      if(printOut)
	{
	  cout << endl;
	  cout << "**** Electron " << ele_idx << endl <<endl;
	  cout << "## momentum" << endl;
	  cout << "electron->p(): " << electron->p() << endl;
	  cout << "electron->trackMomentumAtVtx().R(): " << electron->trackMomentumAtVtx().R() << endl;   
	  cout << "electron->trackMomentumAtCalo().R(): " << electron->trackMomentumAtCalo().R() << endl;  
	  cout << "## energy" << endl;
	  cout << "electron->energy(): " <<  electron->energy() << endl;
	  cout << "electron->caloEnergy(): " << electron->caloEnergy() << endl;
	  cout << "## eta" << endl;
	  cout << "electron->eta(): " << electron->eta() << endl;
	  cout << "electron->p4().Eta(): " << electron->p4().Eta() << endl;  
	  cout << "electron->trackMomentumAtVtx().Eta(): " << electron->trackMomentumAtVtx().Eta() << endl;
	  cout << "electron->trackMomentumAtCalo().Eta(): " << electron->trackMomentumAtCalo().Eta() << endl;
	  cout << "## phi" << endl;
	  cout << "electron->phi(): " << electron->phi() << endl;
	  cout << "electron->p4().Phi(): " << electron->p4().Phi() << endl;  
	  cout << "electron->trackMomentumAtVtx().Phi(): " << electron->trackMomentumAtVtx().Phi() << endl; 
	  cout << "electron->trackMomentumAtCalo().Phi(): " << electron->trackMomentumAtCalo().Phi() << endl;  
	  cout << "## Robust Electron ID variables" << endl;
	  cout << "hOverE: " << hOverE << endl;
	  cout << "sigmaee: " << sigmaee << endl;
	  cout << "deltaPhiIn: " << deltaPhiIn << endl;
	  cout << "deltaEtaIn: " << deltaEtaIn << endl;
	  cout << endl; 
	}

      //*******************************************************************************************************

      //## p_at_Vtx - E_sc
      float p_minus_Esc_recoEle = electron->trackMomentumAtVtx().R() - electron->caloEnergy();
      float ET_sc=electron->caloEnergy()/cosh(electron->superCluster()->eta());

      h_p_minus_Esc_recoEle_vs_Esc->Fill(electron->caloEnergy(),p_minus_Esc_recoEle);
      h_p_minus_Esc_recoEle_vs_ETsc->Fill(ET_sc,p_minus_Esc_recoEle);
      h_p_minus_Esc_recoEle_vs_etasc->Fill(electron->eta(),p_minus_Esc_recoEle);
      h_p_minus_Esc_recoEle_vs_fBrem->Fill(fBrem,p_minus_Esc_recoEle);

      //
      ele_idx++;
    }//end loop over gsf electrons


  if(printOut)
    cout << "******************* electron->size() : " << electrons->size() << endl;

  h_N_recoEle->Fill(electrons->size());
  h_N_recoEle_pTcut->Fill(N_recoEle_pTcut);

  //## Energy resolution with MC matching
  //loop over gen particle
  
  for(size_t i = 0; i < genParticles->size(); ++ i) 
    {
      const Candidate & p = (*genParticles)[i];
      const Candidate * mom = p.mother();
      
      int id = p.pdgId();
      
      //select gen electron
      if(abs(id)==id_electron)
	{
	  float E_genEle = p.energy();
	  float pt_genEle = p.pt();
	  float eta_genEle = p.eta();
	  float phi_genEle = p.phi();
	  
	  TVector3 genEle_vec;
	  genEle_vec.SetPtEtaPhi(pt_genEle,eta_genEle,phi_genEle);
	  
	  //select gen electron from LQ decay
	  if(abs(mom->pdgId())==id_LQ)
	    {
	      
	      //select high pT gen electrons
	      if(pt_genEle<pTcut_genEle)
		continue;

	      float DeltaR_genEle_recoEle_min=1000;
	      int idx_nearest_recoEle=-1;
	      int idx_ele_it=0;
	      reco::PixelMatchGsfElectronCollection::const_iterator electron_it;
	      for (electron_it = (*electrons).begin();
		   electron_it != (*electrons).end(); ++electron_it) 
		{
		  float E_recoEle=electron_it->energy();
		  float pT_recoEle=electron_it->pt();
		  float eta_recoEle=electron_it->eta();
		  float phi_recoEle=electron_it->phi();
		  
		  TVector3 recoEle_vec;
		  recoEle_vec.SetPtEtaPhi(pT_recoEle,eta_recoEle,phi_recoEle);
		  
		  float DeltaR_genEle_recoEle=genEle_vec.DeltaR(recoEle_vec);
		  
		  h_DeltaR_genEle_recoEle->Fill(DeltaR_genEle_recoEle);
		  
		  if(DeltaR_genEle_recoEle<DeltaR_genEle_recoEle_min)
		    {
		      DeltaR_genEle_recoEle_min=DeltaR_genEle_recoEle;
		      idx_nearest_recoEle=idx_ele_it;
		    }
		  
		  idx_ele_it++;
		}//end loop over reco electrons
	      
	      if(idx_nearest_recoEle==-1)
		continue;
	      
	      h_DeltaR_genEle_recoEle_min->Fill(DeltaR_genEle_recoEle_min);
	      
	      const Candidate & nearest_recoEle = (*electrons)[idx_nearest_recoEle];
	      float E_recoEle_over_E_genEle = nearest_recoEle.energy() / E_genEle;
	      
	      //match genEle and recoEle using DeltaRmin
	      if(DeltaR_genEle_recoEle_min<DeltaR_genEle_recoEle_min_cut)
		{
		  h_E_recoEle_over_E_genEle_DeltaRmatch->Fill(E_recoEle_over_E_genEle);
		}
	      
	    }//end select electrons from LQ decay
	  
	}//end select electrons
      
    }//end loop over gen particles
  
}//end loop over events


// ------------ method called once each job just before starting event loop  ------------
void 
RecoElectrons::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RecoElectrons::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecoElectrons);
