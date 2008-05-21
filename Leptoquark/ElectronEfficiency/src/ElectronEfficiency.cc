// -*- C++ -*-
//
// Package:    ElectronEfficiency
// Class:      ElectronEfficiency
// 
/**\class ElectronEfficiency ElectronEfficiency.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  pts/56
//         Created:  Mon May 12 14:37:26 CEST 2008
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
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
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

//Jets
#include "DataFormats/JetReco/interface/CaloJet.h"

//genparticles
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//HLT
#include "FWCore/Framework/interface/TriggerNames.h"

//Isolation
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaCandidates/interface/PMGsfElectronIsoCollection.h"
#include "DataFormats/EgammaCandidates/interface/PMGsfElectronIsoNumCollection.h"


//
// class decleration
//

class ElectronEfficiency : public edm::EDAnalyzer {
   public:
      explicit ElectronEfficiency(const edm::ParameterSet&);
      ~ElectronEfficiency();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      TH1D * histo; 

      TH1F * h_pt_genEle; 
      TH1F * h_pt_matched_Ele; 
      TH1F * h_pt_Efficiency; 
      TH1F * h_pt_matched_Ele_IDcut; 
      TH1F * h_pt_Efficiency_IDcut; 

      TH1F * h_pt_genEle_FidVol; 
      TH1F * h_pt_matched_Ele_FidVol; 
      TH1F * h_pt_matched_Ele_FidVol_IDcut; 

      TH1F * h_eta_genEle; 
      TH1F * h_eta_matched_Ele; 
      TH1F * h_eta_Efficiency;
      TH1F * h_eta_matched_Ele_IDcut; 
      TH1F * h_eta_Efficiency_IDcut;

      TH1F * h_eta_genEle_FidVol; 
      TH1F * h_eta_matched_Ele_FidVol; 
      TH1F * h_eta_matched_Ele_FidVol_IDcut; 

      TH1F * h_DeltaR_genEle_recoEle;
      TH1F * h_DeltaR_genEle_recoEle_min;

  
      int event;

      //cuts
      double hOverEBarrelCut;
      double sigmaeeBarrelCut;
      double deltaPhiInBarrelCut;
      double deltaEtaInBarrelCut;

      double hOverEEndcapCut;
      double sigmaeeEndcapCut;
      double deltaPhiInEndcapCut;
      double deltaEtaInEndcapCut;

      double pTcut_genEle;
      double pTcut_recoEle;

      double DeltaR_genEle_recoEle_min_cut;


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

// namespaces
using namespace std;
using namespace reco;

//
// static data member definitions
//

//
// constructors and destructor
//
ElectronEfficiency::ElectronEfficiency(const edm::ParameterSet& iConfig)

{
  
  //parameter from cfg file
  hOverEBarrelCut = iConfig.getUntrackedParameter<double>("hOverEBarrelCut"); 
  sigmaeeBarrelCut = iConfig.getUntrackedParameter<double>("sigmaeeBarrelCut"); 
  deltaPhiInBarrelCut = iConfig.getUntrackedParameter<double>("deltaPhiInBarrelCut"); 
  deltaEtaInBarrelCut = iConfig.getUntrackedParameter<double>("deltaEtaInBarrelCut"); 
  
  hOverEEndcapCut = iConfig.getUntrackedParameter<double>("hOverEEndcapCut"); 
  sigmaeeEndcapCut = iConfig.getUntrackedParameter<double>("sigmaeeEndcapCut"); 
  deltaPhiInEndcapCut = iConfig.getUntrackedParameter<double>("deltaPhiInEndcapCut"); 
  deltaEtaInEndcapCut = iConfig.getUntrackedParameter<double>("deltaEtaInEndcapCut"); 
  
  pTcut_genEle = iConfig.getUntrackedParameter<double>("pTcutGenEle"); 
  pTcut_recoEle = iConfig.getUntrackedParameter<double>("pTcutRecoEle"); 
  
  DeltaR_genEle_recoEle_min_cut = iConfig.getUntrackedParameter<double>("DeltaRgenElerecoEleMinCut"); 

  //now do what ever initialization is needed
  edm::Service<TFileService> fs;

  //   h_pt_genEle = fs->make<TH1F>("h_pt_genEle","h_pt_genEle",100,0.,1000.);
  //   h_pt_matched_Ele = fs->make<TH1F>("h_pt_matched_Ele","h_pt_matched_Ele",100,0.,1000.);
  //   h_pt_Efficiency = fs->make<TH1F>("h_pt_Efficiency","h_pt_Efficiency",100,0.,1000.);
  Float_t VarPtBin[10];
  VarPtBin[0]=0.;
  VarPtBin[1]=20.;
  VarPtBin[2]=40.;
  VarPtBin[3]=60.;
  VarPtBin[4]=100.;
  VarPtBin[5]=150.;
  VarPtBin[6]=200.;
  VarPtBin[7]=300.;
  VarPtBin[8]=400.;
  VarPtBin[9]=1000.;

  h_pt_genEle = fs->make<TH1F>("h_pt_genEle","h_pt_genEle",9,VarPtBin);
  h_pt_matched_Ele = fs->make<TH1F>("h_pt_matched_Ele","h_pt_matched_Ele",9,VarPtBin);
  h_pt_Efficiency = fs->make<TH1F>("h_pt_Efficiency","h_pt_Efficiency",9,VarPtBin);
  h_pt_matched_Ele_IDcut = fs->make<TH1F>("h_pt_matched_Ele_IDcut","h_pt_matched_Ele_IDcut",9,VarPtBin);
  h_pt_Efficiency_IDcut = fs->make<TH1F>("h_pt_Efficiency_IDcut","h_pt_Efficiency_IDcut",9,VarPtBin);

  h_pt_genEle_FidVol = fs->make<TH1F>("h_pt_genEle_FidVol","h_pt_genEle_FidVol",9,VarPtBin);
  h_pt_matched_Ele_FidVol = fs->make<TH1F>("h_pt_matched_Ele_FidVol","h_pt_matched_Ele_FidVol",9,VarPtBin);
  h_pt_matched_Ele_FidVol_IDcut = fs->make<TH1F>("h_pt_matched_Ele_FidVol_IDcut","h_pt_matched_Ele_FidVol_IDcut",9,VarPtBin);

  h_eta_genEle = fs->make<TH1F>("h_eta_genEle","h_eta_genEle",100,-5.,5.);
  h_eta_matched_Ele = fs->make<TH1F>("h_eta_matched_Ele","h_eta_matched_Ele",100,-5.,5.);
  h_eta_Efficiency = fs->make<TH1F>("h_eta_Efficiency","h_eta_Efficiency",100,-5.,5.);
  h_eta_matched_Ele_IDcut = fs->make<TH1F>("h_eta_matched_Ele_IDcut","h_eta_matched_Ele_IDcut",100,-5.,5.);
  h_eta_Efficiency_IDcut = fs->make<TH1F>("h_eta_Efficiency_IDcut","h_eta_Efficiency_IDcut",100,-5.,5.);

  h_eta_genEle_FidVol = fs->make<TH1F>("h_eta_genEle_FidVol","h_eta_genEle_FidVol",100,-5.,5.);
  h_eta_matched_Ele_FidVol = fs->make<TH1F>("h_eta_matched_Ele_FidVol","h_eta_matched_Ele_FidVol",100,-5.,5.);
  h_eta_matched_Ele_FidVol_IDcut = fs->make<TH1F>("h_eta_matched_Ele_FidVol_IDcut","h_eta_matched_Ele_FidVol_IDcut",100,-5.,5.);

  h_DeltaR_genEle_recoEle = fs->make<TH1F>("h_DeltaR_genEle_recoEle","h_DeltaR_genEle_recoEle",200,0.,0.5);
  h_DeltaR_genEle_recoEle_min = fs->make<TH1F>("h_DeltaR_genEle_recoEle_min","h_DeltaR_genEle_recoEle_min",200,0.,0.5);

}


ElectronEfficiency::~ElectronEfficiency()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
ElectronEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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


  //*******************************************************

  // Read in GenParticles
  edm::Handle<reco::CandidateCollection> genParticles;
  iEvent.getByLabel ( "genParticleCandidates", genParticles);

  // Get the objects that were fed into the isolation producer (not necessary for method 2)
  edm::Handle< edm::View<reco::Candidate> > emObjectHandle_;
  iEvent.getByLabel("pixelMatchGsfElectrons",emObjectHandle_);
  const edm::View<reco::Candidate> *emObjectHandle = emObjectHandle_.product();
  
  // Get the association vector for isolations 
  // --> REMEMBER to correctly set the cut values of the isolations in
  // EgammaAnalysis/EgammaIsolationProducers/data 

  //hcaliso ( EgammaTowerIsolationProducer )
  //   edm::Handle< reco::CandViewDoubleAssociations > hcalIsolationHandle;
  //   iEvent.getByLabel("egammaTowerIsolation",hcalIsolationHandle);

  //ecaliso ( EgammaEcalIsolationProducer )
  edm::Handle< reco::CandViewDoubleAssociations > ecalIsolationHandle;
  iEvent.getByLabel("egammaEcalRelIsolation",ecalIsolationHandle);

  //trkiso ( EgammaElectronTkIsolationProducer )
  edm::Handle< reco::PMGsfElectronIsoCollection > trkIsolationHandle;
  iEvent.getByLabel("egammaElectronTkRelIsolation",trkIsolationHandle);

  //numtrksio ( EgammaElectronTkNumIsolationProducer )
  edm::Handle< reco::PMGsfElectronIsoNumCollection > trkNumIsolationHandle;
  iEvent.getByLabel("egammaElectronTkNumIsolation",trkNumIsolationHandle);

  //*******************************************************

  // Loop over gen particles
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
	      h_eta_genEle->Fill(eta_genEle);
	      h_pt_genEle->Fill(pt_genEle);

	      //ECAL fiducial volume
	      if( fabs(eta_genEle)<1.4 
		  || 
		  (fabs(eta_genEle)>1.6 && fabs(eta_genEle)<2)  )
		{
		  h_eta_genEle_FidVol->Fill(eta_genEle);
		  h_pt_genEle_FidVol->Fill(pt_genEle);
		}
	      
	      //## Loop over reco electrons
 	      float DeltaR_genEle_recoEle_min=9999;
 	      float DeltaR_genEle_recoEle_min_IDcut=9999;
 	      int idx_nearest_recoEle=-1;
 	      int idx_nearest_recoEleIDcut=-1;

	      for(int elecand_idx = 0; elecand_idx < (int)emObjectHandle->size(); elecand_idx++) 
		{
		  
		  const PixelMatchGsfElectronRef electron = emObjectHandle->refAt(elecand_idx).castTo<PixelMatchGsfElectronRef>();
		  const reco::SuperClusterRef& SCref = electron->superCluster();  
		  
		  //## Remove electrons associated to the same SC ##
		  bool IsCopy=false;      
		  for(int elecand_idx_1 = elecand_idx+1; elecand_idx_1 < (int)emObjectHandle->size(); elecand_idx_1++) 
		    {
		      const PixelMatchGsfElectronRef electron_1 = emObjectHandle->refAt(elecand_idx_1).castTo<PixelMatchGsfElectronRef>();
		      const reco::SuperClusterRef& SCref_1 = electron_1->superCluster();  
		      
		      if(SCref==SCref_1)
			{
			  IsCopy=true;
			  break;
			}
		    }
		  
		  //skip this electron 
		  if(IsCopy==true)
		    continue;
		  
		  //#################################################

		  //## Cluster shape collection
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
  
		  //## Robust electron ID variables       
		  float hOverE = electron->hadronicOverEm();
		  float sigmaee = sqrt(sClShape->covEtaEta());
		  float deltaPhiIn = electron->deltaPhiSuperClusterTrackAtVtx();
		  float deltaEtaIn = electron->deltaEtaSuperClusterTrackAtVtx();

		  //correct sigmaetaeta dependence on eta in endcap
		  if (fabs(electron->eta()) > 1.479) 
		    sigmaee = sigmaee - 0.02*(fabs(electron->eta()) - 2.3);   
      
		  //     # cut value arrays of form {hoe, sigmaEtaEta, dPhiIn, dEtaIn}
		  //     PSet robustEleIDCuts =
		  //     {
		  //         vdouble barrel     = {0.115, 0.0140, 0.053, 0.0090}
		  //         vdouble endcap     = {0.150, 0.0275, 0.092, 0.0105}
		  //     }

		  bool passIDbarrel = false;
		  bool passIDendcap = false;
		  if(fabs(electron->eta())<1.479
		     && hOverE<hOverEBarrelCut 
		     && sigmaee<sigmaeeBarrelCut
		     && deltaPhiIn<deltaPhiInBarrelCut
		     && deltaEtaIn<deltaEtaInBarrelCut)
		    passIDbarrel=true;

		  if(fabs(electron->eta())>1.479
		     && hOverE<hOverEEndcapCut 
		     && sigmaee<sigmaeeEndcapCut
		     && deltaPhiIn<deltaPhiInEndcapCut
		     && deltaEtaIn<deltaEtaInEndcapCut)
		    passIDendcap=true;


		  //********************************************************
		  //## Electron isolation variables
		  // this retrieves the index in the original collection associated to the reference to electron
		  int index = electron.key();

		  //-- hcal tower isolation
		  //double hcalIso = (*hcalIsolationHandle)[index].second;
		  double ecalIso = (*ecalIsolationHandle)[index].second;
		  double trkIso = (*trkIsolationHandle)[index].second; 
		  double trkNumIso = (*trkNumIsolationHandle)[index].second; 

		  if(printOut)
		    {
		      cout << "ecalIso: " << ecalIso  
			   << "trkIso: " << trkIso  
			   << "trkNumIso: " << trkNumIso 
			   << endl;
		    }  
      
		  //********************************************************      

		  //## Some print out
		  if(printOut)
		    {
		      cout << endl;
		      cout << "**** Electron " << elecand_idx << endl <<endl;
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

		  //############ Efficiency plots ##############
		  
 		  float E_recoEle=electron->energy();
 		  float pT_recoEle=electron->pt();
 		  float eta_recoEle=electron->eta();
 		  float phi_recoEle=electron->phi();
 		  
 		  TVector3 recoEle_vec;
 		  recoEle_vec.SetPtEtaPhi(pT_recoEle,eta_recoEle,phi_recoEle);
 		  
 		  float DeltaR_genEle_recoEle=genEle_vec.DeltaR(recoEle_vec);
 		  
 		  h_DeltaR_genEle_recoEle->Fill(DeltaR_genEle_recoEle);
 		  
 		  if(DeltaR_genEle_recoEle<DeltaR_genEle_recoEle_min)
 		    {
 		      DeltaR_genEle_recoEle_min=DeltaR_genEle_recoEle;
 		      idx_nearest_recoEle=elecand_idx;

		      //## pass ID 
		      if(passIDbarrel==true || passIDendcap==true)
			{
			  DeltaR_genEle_recoEle_min_IDcut=DeltaR_genEle_recoEle;
			  idx_nearest_recoEleIDcut=elecand_idx;
			}

 		    }
 		  
		}//end loop over reco electrons
 

	      //## no ID cut
 	      if(idx_nearest_recoEle==-1)
		continue; 	      

 	      h_DeltaR_genEle_recoEle_min->Fill(DeltaR_genEle_recoEle_min);
 	      
 	      //match genEle and recoEle using DeltaRmin
 	      if(DeltaR_genEle_recoEle_min<DeltaR_genEle_recoEle_min_cut)
		{
 		  h_eta_matched_Ele->Fill(eta_genEle);
 		  h_pt_matched_Ele->Fill(pt_genEle);

		  //ECAL fiducial volume
		  if( fabs(eta_genEle)<1.4 
		      || 
		      (fabs(eta_genEle)>1.6 && fabs(eta_genEle)<2)  )
		    {
		      h_eta_matched_Ele_FidVol->Fill(eta_genEle);
		      h_pt_matched_Ele_FidVol->Fill(pt_genEle);
		    }
  		} 


	      //## apply ID cut
	      if(idx_nearest_recoEleIDcut==-1)
		continue;

 	      //match genEle and recoEle using DeltaRmin
 	      if(DeltaR_genEle_recoEle_min_IDcut<DeltaR_genEle_recoEle_min_cut)
		{
 		  h_eta_matched_Ele_IDcut->Fill(eta_genEle);
 		  h_pt_matched_Ele_IDcut->Fill(pt_genEle);

		  //ECAL fiducial volume
		  if( fabs(eta_genEle)<1.4 
		      || 
		      (fabs(eta_genEle)>1.6 && fabs(eta_genEle)<2)  )
		    {
		      h_eta_matched_Ele_FidVol_IDcut->Fill(eta_genEle);
		      h_pt_matched_Ele_FidVol_IDcut->Fill(pt_genEle);
		    }
  		} 


	    } //if mom->leptoquark ID

	} // if electrons 
    
    } //loop over gen particles




} //end of analyze


// ------------ method called once each job just before starting event loop  ------------
void 
ElectronEfficiency::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ElectronEfficiency::endJob() {

  h_eta_Efficiency->Divide(h_eta_matched_Ele,h_eta_genEle,1,1);
  h_pt_Efficiency->Divide (h_pt_matched_Ele,h_pt_genEle,1,1);
  h_eta_Efficiency_IDcut->Divide(h_eta_matched_Ele_IDcut,h_eta_genEle,1,1);
  h_pt_Efficiency_IDcut->Divide (h_pt_matched_Ele_IDcut,h_pt_genEle,1,1);

}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronEfficiency);
