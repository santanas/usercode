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
// $Id: RecoElectrons.cc,v 1.9 2008/05/12 10:34:52 santanas Exp $
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

//Reco jets
#include "DataFormats/JetReco/interface/CaloJet.h"

//Genparticles
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
      TH1F * h_N_recoEle_pTcut_IDcut; 

      TH1F * h_energy_recoEle;
      TH1F * h_pT_recoEle;
      TH1F * h_eta_recoEle;

      TH1F * h_energy_recoEle_IDcut;
      TH1F * h_pT_recoEle_IDcut;
      TH1F * h_eta_recoEle_IDcut;

      TH2F * h_p_minus_Esc_recoEle_vs_Esc; 
      TH2F * h_p_minus_Esc_recoEle_vs_ETsc; 
      TH2F * h_p_minus_Esc_recoEle_vs_etasc; 
      TH2F * h_p_minus_Esc_recoEle_vs_fBrem;

      TH1F * h_DeltaR_genEle_recoEle;
      TH1F * h_DeltaR_genEle_recoEle_min;

      TH1F * h_E_recoEle_over_E_genEle_MCmatch_barrel;
      TH1F * h_Eraw_recoEle_over_E_genEle_MCmatch_barrel;
      TH1F * h_E_recoEle_over_E_genEle_MCmatch_endcap;
      TH1F * h_Eraw_recoEle_over_E_genEle_MCmatch_endcap;

      TH1F * h_E_recoEle_over_E_genEle_MCmatch_IDcut_barrel;
      TH1F * h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_barrel;
      TH1F * h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap;
      TH1F * h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_endcap;

      TH1F * h_hOverE_recoEle_pTcut;
      TH1F * h_sigmaee_recoEle_pTcut;
      TH1F * h_deltaPhiIn_recoEle_pTcut;
      TH1F * h_deltaEtaIn_recoEle_pTcut;

      TH1F * h_hOverE_recoEle_MCmatch;
      TH1F * h_sigmaee_recoEle_MCmatch;
      TH1F * h_deltaPhiIn_recoEle_MCmatch;
      TH1F * h_deltaEtaIn_recoEle_MCmatch;
      TH1F * h_N_recoEle_MCmatch; 
      TH1F * h_N_recoEle_MCmatch_IDcut; 
      TH1F * h_energy_recoEle_MCmatch;
      TH1F * h_pT_recoEle_MCmatch;
      TH1F * h_eta_recoEle_MCmatch;
      TH1F * h_EcalIsoRel_recoEle_MCmatch;
      TH1F * h_TrkIsoRel_recoEle_MCmatch;
      TH1F * h_TrkNumIso_recoEle_MCmatch;


      TH1F * h_hOverE_recoEle_noMCmatch;
      TH1F * h_sigmaee_recoEle_noMCmatch;
      TH1F * h_deltaPhiIn_recoEle_noMCmatch;
      TH1F * h_deltaEtaIn_recoEle_noMCmatch;
      TH1F * h_N_recoEle_noMCmatch; 
      TH1F * h_N_recoEle_noMCmatch_IDcut; 
      TH1F * h_energy_recoEle_noMCmatch;
      TH1F * h_pT_recoEle_noMCmatch;
      TH1F * h_eta_recoEle_noMCmatch;
      TH1F * h_EcalIsoRel_recoEle_noMCmatch;
      TH1F * h_TrkIsoRel_recoEle_noMCmatch;
      TH1F * h_TrkNumIso_recoEle_noMCmatch;


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
RecoElectrons::RecoElectrons(const edm::ParameterSet& iConfig)

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
  //histo = fs->make<TH1D>("charge" , "Charges" , 200 , -2 , 2 );

  //### Plots for all reco candidates

  //## Electron ID variables
  h_hOverE_recoEle_pTcut = fs->make<TH1F>("h_hOverE_recoEle_pTcut","h_hOverE_recoEle_pTcut",200,0.,0.5);
  h_sigmaee_recoEle_pTcut = fs->make<TH1F>("h_sigmaee_recoEle_pTcut","h_sigmaee_recoEle_pTcut",200,0.,0.5);
  h_deltaPhiIn_recoEle_pTcut = fs->make<TH1F>("h_deltaPhiIn_recoEle_pTcut","h_deltaPhiIn_recoEle_pTcut",200,0.,0.2);
  h_deltaEtaIn_recoEle_pTcut = fs->make<TH1F>("h_deltaEtaIn_recoEle_pTcut","h_deltaEtaIn_recoEle_pTcut",200,0.,0.2);

  //## number of reco electrons
  h_N_recoEle = fs->make<TH1F>("h_N_recoEle","h_N_recoEle",30,-0.5,30.5);
  h_N_recoEle_pTcut = fs->make<TH1F>("h_N_recoEle_pTcut","h_N_recoEle_pTcut",30,-0.5,30.5);
  h_N_recoEle_pTcut_IDcut = fs->make<TH1F>("h_N_recoEle_pTcut_IDcut","h_N_recoEle_pTcut_IDcut",30,-0.5,30.5);

  //## pT/eta reco electrons
  h_energy_recoEle = fs->make<TH1F>("h_energy_recoEle","h_energy_recoEle",100,0,1000);
  h_pT_recoEle = fs->make<TH1F>("h_pT_recoEle","h_pT_recoEle",100,0,1000);
  h_eta_recoEle = fs->make<TH1F>("h_eta_recoEle","h_eta_recoEle",100,-4,4);

  h_energy_recoEle_IDcut = fs->make<TH1F>("h_energy_recoEle_IDcut","h_energy_recoEle_IDcut",100,0,1000);
  h_pT_recoEle_IDcut = fs->make<TH1F>("h_pT_recoEle_IDcut","h_pT_recoEle_IDcut",100,0,1000);
  h_eta_recoEle_IDcut = fs->make<TH1F>("h_eta_recoEle_IDcut","h_eta_recoEle_IDcut",100,-4,4);


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


  //### Plots with/without MC matching

  //## DeltaR plots
  h_DeltaR_genEle_recoEle = fs->make<TH1F>("h_DeltaR_genEle_recoEle","h_DeltaR_genEle_recoEle",200,0.,0.5);
  h_DeltaR_genEle_recoEle_min = fs->make<TH1F>("h_DeltaR_genEle_recoEle_min","h_DeltaR_genEle_recoEle_min",200,0.,0.5);

  //## matched ele
  h_E_recoEle_over_E_genEle_MCmatch_barrel = fs->make<TH1F>("h_E_recoEle_over_E_genEle_MCmatch_barrel",
								"h_E_recoEle_over_E_genEle_MCmatch_barrel",1000,0.,2.);
  h_Eraw_recoEle_over_E_genEle_MCmatch_barrel = fs->make<TH1F>("h_Eraw_recoEle_over_E_genEle_MCmatch_barrel",
								  "h_Eraw_recoEle_over_E_genEle_MCmatch_barrel",1000,0.,2.);
  h_E_recoEle_over_E_genEle_MCmatch_endcap = fs->make<TH1F>("h_E_recoEle_over_E_genEle_MCmatch_endcap",
								"h_E_recoEle_over_E_genEle_MCmatch_endcap",1000,0.,2.);
  h_Eraw_recoEle_over_E_genEle_MCmatch_endcap = fs->make<TH1F>("h_Eraw_recoEle_over_E_genEle_MCmatch_endcap",
								  "h_Eraw_recoEle_over_E_genEle_MCmatch_endcap",1000,0.,2.);


  h_E_recoEle_over_E_genEle_MCmatch_IDcut_barrel = fs->make<TH1F>("h_E_recoEle_over_E_genEle_MCmatch_IDcut_barrel",
								"h_E_recoEle_over_E_genEle_MCmatch_IDcut_barrel",1000,0.,2.);
  h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_barrel = fs->make<TH1F>("h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_barrel",
								  "h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_barrel",1000,0.,2.);
  h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap = fs->make<TH1F>("h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap",
								"h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap",1000,0.,2.);
  h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_endcap = fs->make<TH1F>("h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_endcap",
								  "h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_endcap",1000,0.,2.);


  h_hOverE_recoEle_MCmatch = fs->make<TH1F>("h_hOverE_recoEle_MCmatch","h_hOverE_recoEle_MCmatch",200,0.,0.5);
  h_sigmaee_recoEle_MCmatch = fs->make<TH1F>("h_sigmaee_recoEle_MCmatch","h_sigmaee_recoEle_MCmatch",200,0.,0.5);
  h_deltaPhiIn_recoEle_MCmatch = fs->make<TH1F>("h_deltaPhiIn_recoEle_MCmatch","h_deltaPhiIn_recoEle_MCmatch",200,0.,0.2);
  h_deltaEtaIn_recoEle_MCmatch = fs->make<TH1F>("h_deltaEtaIn_recoEle_MCmatch","h_deltaEtaIn_recoEle_MCmatch",200,0.,0.2);
  h_N_recoEle_MCmatch = fs->make<TH1F>("h_N_recoEle_MCmatch","h_N_recoEle_MCmatch",30,-0.5,30.5);
  h_N_recoEle_MCmatch_IDcut = fs->make<TH1F>("h_N_recoEle_MCmatch_IDcut","h_N_recoEle_MCmatch_IDcut",30,-0.5,30.5);
  h_energy_recoEle_MCmatch = fs->make<TH1F>("h_energy_recoEle_MCmatch","h_energy_recoEle_MCmatch",100,0,1000);
  h_pT_recoEle_MCmatch = fs->make<TH1F>("h_pT_recoEle_MCmatch","h_pT_recoEle_MCmatch",100,0,1000);
  h_eta_recoEle_MCmatch = fs->make<TH1F>("h_eta_recoEle_MCmatch","h_eta_recoEle_MCmatch",100,-4,4);

  h_EcalIsoRel_recoEle_MCmatch = fs->make<TH1F>("h_EcalIsoRel_recoEle_MCmatch","h_EcalIsoRel_recoEle_MCmatch",200,0.,2.);
  h_TrkIsoRel_recoEle_MCmatch = fs->make<TH1F>("h_TrkIsoRel_recoEle_MCmatch","h_TrkIsoRel_recoEle_MCmatch",200,0.,2.);
  h_TrkNumIso_recoEle_MCmatch = fs->make<TH1F>("h_TrkNumIso_recoEle_MCmatch","h_TrkNumIso_recoEle_MCmatch",16,-0.5,15.5);


  //## NOT matched ele

  h_hOverE_recoEle_noMCmatch = fs->make<TH1F>("h_hOverE_recoEle_noMCmatch","h_hOverE_recoEle_noMCmatch",200,0.,0.5);
  h_sigmaee_recoEle_noMCmatch = fs->make<TH1F>("h_sigmaee_recoEle_noMCmatch","h_sigmaee_recoEle_noMCmatch",200,0.,0.5);
  h_deltaPhiIn_recoEle_noMCmatch = fs->make<TH1F>("h_deltaPhiIn_recoEle_noMCmatch","h_deltaPhiIn_recoEle_noMCmatch",200,0.,0.2);
  h_deltaEtaIn_recoEle_noMCmatch = fs->make<TH1F>("h_deltaEtaIn_recoEle_noMCmatch","h_deltaEtaIn_recoEle_noMCmatch",200,0.,0.2);
  h_N_recoEle_noMCmatch = fs->make<TH1F>("h_N_recoEle_noMCmatch","h_N_recoEle_noMCmatch",30,-0.5,30.5);
  h_N_recoEle_noMCmatch_IDcut = fs->make<TH1F>("h_N_recoEle_noMCmatch_IDcut","h_N_recoEle_noMCmatch_IDcut",30,-0.5,30.5);
  h_energy_recoEle_noMCmatch = fs->make<TH1F>("h_energy_recoEle_noMCmatch","h_energy_recoEle_noMCmatch",100,0,1000);
  h_pT_recoEle_noMCmatch = fs->make<TH1F>("h_pT_recoEle_noMCmatch","h_pT_recoEle_noMCmatch",100,0,1000);
  h_eta_recoEle_noMCmatch = fs->make<TH1F>("h_eta_recoEle_noMCmatch","h_eta_recoEle_noMCmatch",100,-4,4);

  h_EcalIsoRel_recoEle_noMCmatch = fs->make<TH1F>("h_EcalIsoRel_recoEle_noMCmatch","h_EcalIsoRel_recoEle_noMCmatch",200,0.,2.);
  h_TrkIsoRel_recoEle_noMCmatch = fs->make<TH1F>("h_TrkIsoRel_recoEle_noMCmatch","h_TrkIsoRel_recoEle_noMCmatch",200,0.,2.);
  h_TrkNumIso_recoEle_noMCmatch = fs->make<TH1F>("h_TrkNumIso_recoEle_noMCmatch","h_TrkNumIso_recoEle_noMCmatch",16,-0.5,15.5);

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
  edm::Handle<reco::PixelMatchGsfElectronCollection> pElectrons;
  iEvent.getByLabel(edm::InputTag("pixelMatchGsfElectrons",""), pElectrons);
  const reco::PixelMatchGsfElectronCollection* electrons;
  electrons = pElectrons.product();

  // Read in GenParticles
  edm::Handle<reco::CandidateCollection> genParticles;
  iEvent.getByLabel ( "genParticleCandidates", genParticles);

  //*******************************************************
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

  
  if(printOut)
    cout << "******************* electron->size() : " << electrons->size() << endl;

  // Loop over gsf electrons
  int ele_idx=0;
  int N_recoEle_pTcut=0;
  int N_recoEle_pTcut_IDcut=0;
  int N_recoEle_MCmatch=0;
  int N_recoEle_MCmatch_IDcut=0;
  int N_recoEle_noMCmatch=0;
  int N_recoEle_noMCmatch_IDcut=0;
  
  
  //   reco::PixelMatchGsfElectronCollection::const_iterator electron;
  //   for (electron = (*electrons).begin();
  //        electron != (*electrons).end(); ++electron) 
  //     {
  
  //**************************************************
  //   for(edm::View<reco::Candidate>::const_iterator iter = emObjectHandle->begin()  ; iter != emObjectHandle->end(); ++iter) 
  //     {
  
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


      //## Main variables       
      //       float pT_ele=electron->pt();
      //       float E_ele=electron->energy();
      //       float E_sc_ele=electron->caloEnergy();
      //       float eta_ele=electron->eta();
      //       float phi_ele=electron->phi();

      
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
      if (electron->eta() > 1.479) 
	sigmaee = sigmaee - 0.02*(fabs(electron->eta()) - 2.3);   
      
      //Matteo Sani's comment ( in italian, sorry:) )
      //Per quanto riguarda la correzione a sigmaee questa e` una semplice valutazione della variazione di sigmaee  
      //dovuta alla
      //particolare geometria dei cristalli nell'endcap. In pratica si corregge la sigmaee in funzione di eta per la diversa
      //area dei cristalli vista dal punto di interazione.

      //     # cut value arrays of form {hoe, sigmaEtaEta, dPhiIn, dEtaIn}
      //     PSet robustEleIDCuts =
      //     {
      //         vdouble barrel     = {0.115, 0.0140, 0.053, 0.0090}
      //         vdouble endcap     = {0.150, 0.0275, 0.092, 0.0105}
      //     }

      bool passIDbarrel = false;
      bool passIDendcap = false;
      if(electron->eta()<1.4
	 && hOverE<hOverEBarrelCut 
	 && sigmaee<sigmaeeBarrelCut
	 && deltaPhiIn<deltaPhiInBarrelCut
	 && deltaEtaIn<deltaEtaInBarrelCut)
	passIDbarrel=true;

      if(electron->eta()>1.6
	 && hOverE<hOverEEndcapCut 
	 && sigmaee<sigmaeeEndcapCut
	 && deltaPhiIn<deltaPhiInEndcapCut
	 && deltaEtaIn<deltaEtaInEndcapCut)
	passIDendcap=true;


      //********************************************************
      //## Electron isolation variables
      // first follow the instructions at
      // https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEgammaIsolation 
      // 
      // example at EgammaAnalysis/Examples/src/EgammaIsolationAnalyser.cc
      // and at http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/HiggsToWW2e/src/CmsEleIDTreeFiller.cc?revision=1.15&view=markup

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


      //## p_at_Vtx - E_sc
      float p_minus_Esc_recoEle = electron->trackMomentumAtVtx().R() - electron->caloEnergy();
      float ET_sc=electron->caloEnergy()/cosh(electron->superCluster()->eta());

      h_p_minus_Esc_recoEle_vs_Esc->Fill(electron->caloEnergy(),p_minus_Esc_recoEle);
      h_p_minus_Esc_recoEle_vs_ETsc->Fill(ET_sc,p_minus_Esc_recoEle);
      h_p_minus_Esc_recoEle_vs_etasc->Fill(electron->eta(),p_minus_Esc_recoEle);
      h_p_minus_Esc_recoEle_vs_fBrem->Fill(fBrem,p_minus_Esc_recoEle);

      //## Electron plots

      // ID variables
      if(electron->pt()>pTcut_recoEle)
	{
	  h_hOverE_recoEle_pTcut->Fill(hOverE);
	  h_sigmaee_recoEle_pTcut->Fill(sigmaee);
	  h_deltaPhiIn_recoEle_pTcut->Fill(deltaPhiIn);
	  h_deltaEtaIn_recoEle_pTcut->Fill(deltaEtaIn);
   	}

      // no electron ID
      if(electron->pt()>pTcut_recoEle)
	N_recoEle_pTcut++;

      h_energy_recoEle->Fill(electron->energy());
      h_pT_recoEle->Fill(electron->pt());
      h_eta_recoEle->Fill(electron->eta());

      // electron ID applied
      if(passIDbarrel || passIDendcap)
	{
	  if(electron->pt()>pTcut_recoEle)
	    N_recoEle_pTcut_IDcut++;
	  
	  h_energy_recoEle_IDcut->Fill(electron->energy());
	  h_pT_recoEle_IDcut->Fill(electron->pt());
	  h_eta_recoEle_IDcut->Fill(electron->eta());
	}

      //## Plots for MC matched/NOT matched electrons
      float E_recoEle=electron->energy();
      float Eraw_recoEle=electron->superCluster()->rawEnergy();
      float pT_recoEle=electron->pt();
      float eta_recoEle=electron->eta();
      float phi_recoEle=electron->phi();
      
      TVector3 recoEle_vec;
      recoEle_vec.SetPtEtaPhi(pT_recoEle,eta_recoEle,phi_recoEle);
      
      float DeltaR_genEle_recoEle_min=9999;
      int idx_nearest_genEle=-1;
      int idx_ele_it=0;

      //loop over gen particles      
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
	      //if(true)
		{
		  
		  //select high pT gen electrons
		  if(pt_genEle<pTcut_genEle)
		    continue;

		  float DeltaR_genEle_recoEle=genEle_vec.DeltaR(recoEle_vec);

		  h_DeltaR_genEle_recoEle->Fill(DeltaR_genEle_recoEle);
		  
		  if(DeltaR_genEle_recoEle<DeltaR_genEle_recoEle_min)
		    {
		      DeltaR_genEle_recoEle_min=DeltaR_genEle_recoEle;
		      idx_nearest_genEle=i;
		    }

		}//end select electron from LQ decay
	    }//end select gen electron

	}//end loop over gen particles

      //match genEle and recoEle using DeltaRmin
      bool IsMatched=false;

      if(idx_nearest_genEle==-1)
	continue;
      
      h_DeltaR_genEle_recoEle_min->Fill(DeltaR_genEle_recoEle_min);

      if(DeltaR_genEle_recoEle_min<DeltaR_genEle_recoEle_min_cut)
	IsMatched=true;

      if(IsMatched==true)
	{
	  float E_recoEle_over_E_genEle = E_recoEle / (*genParticles)[idx_nearest_genEle].energy();
	  float Eraw_recoEle_over_E_genEle = Eraw_recoEle / (*genParticles)[idx_nearest_genEle].energy();

	  if(eta_recoEle<1.4)
	    {
	      h_E_recoEle_over_E_genEle_MCmatch_barrel->Fill(E_recoEle_over_E_genEle);
	      h_Eraw_recoEle_over_E_genEle_MCmatch_barrel->Fill(Eraw_recoEle_over_E_genEle);

	      if(passIDbarrel)
		{
		  h_E_recoEle_over_E_genEle_MCmatch_IDcut_barrel->Fill(E_recoEle_over_E_genEle);
		  h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_barrel->Fill(Eraw_recoEle_over_E_genEle);
		}
	    }
	  
	  if(eta_recoEle>1.6)
	    {
	      h_E_recoEle_over_E_genEle_MCmatch_endcap->Fill(E_recoEle_over_E_genEle);
	      h_Eraw_recoEle_over_E_genEle_MCmatch_endcap->Fill(Eraw_recoEle_over_E_genEle);

	      if(passIDendcap)
		{
		  h_E_recoEle_over_E_genEle_MCmatch_IDcut_endcap->Fill(E_recoEle_over_E_genEle);
		  h_Eraw_recoEle_over_E_genEle_MCmatch_IDcut_endcap->Fill(Eraw_recoEle_over_E_genEle);
		}
	    }

	  h_hOverE_recoEle_MCmatch->Fill(hOverE);
	  h_sigmaee_recoEle_MCmatch->Fill(sigmaee);
	  h_deltaPhiIn_recoEle_MCmatch->Fill(deltaPhiIn);
	  h_deltaEtaIn_recoEle_MCmatch->Fill(deltaEtaIn);

	  h_EcalIsoRel_recoEle_MCmatch->Fill(ecalIso);
	  h_TrkIsoRel_recoEle_MCmatch->Fill(trkIso);
	  h_TrkNumIso_recoEle_MCmatch->Fill(trkNumIso);

	  h_energy_recoEle_MCmatch->Fill(electron->energy());
	  h_pT_recoEle_MCmatch->Fill(electron->pt());
	  h_eta_recoEle_MCmatch->Fill(electron->eta());

	  N_recoEle_MCmatch++;
	  
	  if(passIDbarrel || passIDendcap)
	    N_recoEle_MCmatch_IDcut++;
	  
	}//matched electrons

      else if(IsMatched==false)

	{
	  h_hOverE_recoEle_noMCmatch->Fill(hOverE);
	  h_sigmaee_recoEle_noMCmatch->Fill(sigmaee);
	  h_deltaPhiIn_recoEle_noMCmatch->Fill(deltaPhiIn);
	  h_deltaEtaIn_recoEle_noMCmatch->Fill(deltaEtaIn);

	  h_EcalIsoRel_recoEle_noMCmatch->Fill(ecalIso);
	  h_TrkIsoRel_recoEle_noMCmatch->Fill(trkIso);
	  h_TrkNumIso_recoEle_noMCmatch->Fill(trkNumIso);
	  
	  h_energy_recoEle_noMCmatch->Fill(electron->energy());
	  h_pT_recoEle_noMCmatch->Fill(electron->pt());
	  h_eta_recoEle_noMCmatch->Fill(electron->eta());
	  
	  N_recoEle_noMCmatch++;

	  if(passIDbarrel || passIDendcap)
	    N_recoEle_noMCmatch_IDcut++;
	    
	}//not matched electrons

      //
      ele_idx++;
    }//end loop over gsf electrons

  //## Electron multiplicity
  if(printOut)
    cout << "******************* electron->size() : " << electrons->size() << endl;

  h_N_recoEle->Fill(electrons->size());
  h_N_recoEle_pTcut->Fill(N_recoEle_pTcut);
  h_N_recoEle_pTcut_IDcut->Fill(N_recoEle_pTcut_IDcut);
  h_N_recoEle_MCmatch->Fill(N_recoEle_MCmatch);
  h_N_recoEle_noMCmatch->Fill(N_recoEle_noMCmatch);
  h_N_recoEle_MCmatch_IDcut->Fill(N_recoEle_MCmatch_IDcut);
  h_N_recoEle_noMCmatch_IDcut->Fill(N_recoEle_noMCmatch_IDcut);

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
