// -*- C++ -*-
//
// Package:    RootNtupleMaker
// Class:      RootNtupleMaker
// 
/**\class RootNtupleMaker RootNtupleMaker.cc Leptoquark/RootNtupleMaker/src/RootNtupleMaker.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Francesco Santanastasio
//         Created:  Thu May 22 21:54:39 CEST 2008
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

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/HepMCProduct/interface/GenInfoProduct.h"
#include "FWCore/Framework/interface/Run.h"

#include "FWCore/Framework/interface/CachedProducts.h"


// ROOT include files
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"

// reco electrons
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

// Reco jets
#include "DataFormats/JetReco/interface/CaloJet.h"

// Genparticles and GenJets
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"

// HLT
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include <FWCore/Framework/interface/TriggerNames.h> 
#include <DataFormats/Common/interface/TriggerResults.h> 


// Isolation
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaCandidates/interface/PMGsfElectronIsoCollection.h"
#include "DataFormats/EgammaCandidates/interface/PMGsfElectronIsoNumCollection.h"

//MET
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/GenMET.h"


// About tree
#define MAXGENPARTICLES  2000
#define MAXHLTBITS    200
#define MAXELECTRONS  100
#define MAXGENJETS    100
#define MAXCALOJETS   100


// namespaces
using namespace std;
using namespace reco;


bool ComparePt(const Candidate * first, const Candidate * second)
{
       if(first != NULL && second != NULL)
       {
          if( first->pt() > second->pt())
              return true;
          else
              return false;
        }
       cout << "Compare failed!!!!" <<endl;
       return false;
}

//
// class decleration
//

class RootNtupleMaker : public edm::EDAnalyzer {
   public:
      explicit RootNtupleMaker(const edm::ParameterSet&);
      ~RootNtupleMaker();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

  void CreateParticleTree(const edm::Handle<reco::CandidateCollection> collection);

  void SetTriggers(const edm::Event& iEvent);



  // ----------member data ---------------------------


  // read from cfg file
  std::string          rootfile_;
  int                  maxgenparticles_;
  int                  maxgenjets_;
  int                  maxelectrons_;
  int                  maxcalojets_;
  bool                 soup_;
  bool                 fastSim_;
  bool                 debug_;
  double               luminosity_;
  int                  numEvents_;              
  bool                 saveTrigger_;

  //Output RootNtuple
  TTree *              m_tree;
  TFile *              m_file;

  // Event info
  int                  event;
  int                  runnum;

  // Gen Event Quantities  
  float                m_cross_section;
  float                m_auto_cross_section;
  int                  m_processID;
  int                  m_ALPGENprocessID;
  float                m_filter_eff;
  float                m_pthat;
  float                m_weight;              

  // GenParticles
  Int_t                m_GenParticleCount;
  Float_t              m_GenParticleVX[MAXGENPARTICLES];              
  Float_t              m_GenParticleVY[MAXGENPARTICLES];              
  Float_t              m_GenParticleVZ[MAXGENPARTICLES];              
  Float_t              m_GenParticleP[MAXGENPARTICLES];              
  Float_t              m_GenParticlePt[MAXGENPARTICLES];              
  Float_t              m_GenParticlePx[MAXGENPARTICLES];              
  Float_t              m_GenParticlePy[MAXGENPARTICLES];              
  Float_t              m_GenParticlePz[MAXGENPARTICLES];              
  Float_t              m_GenParticleE[MAXGENPARTICLES];              
  Float_t              m_GenParticleEta[MAXGENPARTICLES];              
  Float_t              m_GenParticlePhi[MAXGENPARTICLES];              
  Int_t                m_GenParticlePdgId[MAXGENPARTICLES];              
  Int_t                m_GenParticleMotherIndex[MAXGENPARTICLES];
  Int_t                m_GenParticleNumDaught[MAXGENPARTICLES];  

  // Trigger
  TString              aNames[MAXHLTBITS];
  char                 aHLTNames[6000];
  Int_t                hltNamesLen;
  Int_t                hltCount;
  bool                 aHLTResults[MAXHLTBITS];

  edm::InputTag hltTriggerResultTag_;


  // Electrons
  Int_t                eleCount;
  Float_t              eleEta[MAXELECTRONS];
  Float_t              elePhi[MAXELECTRONS];
  Float_t              elePt[MAXELECTRONS];
  Float_t              eleEnergy[MAXELECTRONS];
  Float_t              eleCaloEnergy[MAXELECTRONS];

  Float_t              eleHoE[MAXELECTRONS];
  Float_t              eleSigmaEE[MAXELECTRONS];
  Float_t              eleDeltaPhiTrkSC[MAXELECTRONS];
  Float_t              eleDeltaEtaTrkSC[MAXELECTRONS];

  Float_t              eleTrkIso[MAXELECTRONS];
  Float_t              eleNumTrkIso[MAXELECTRONS];
  Float_t              eleEcalIso[MAXELECTRONS];

  // GenJets
  Int_t                genJetCount;
  Float_t              genJetEta[MAXGENJETS];
  Float_t              genJetPhi[MAXGENJETS];
  Float_t              genJetPt[MAXGENJETS];
  Float_t              genJetEnergy[MAXGENJETS];
  Float_t              genJetEMF[MAXGENJETS];
  Float_t              genJetHADF[MAXGENJETS];

  // CaloJets
  Int_t                caloJetIC5Count;
  Float_t              caloJetIC5Eta[MAXCALOJETS];
  Float_t              caloJetIC5Phi[MAXCALOJETS];
  Float_t              caloJetIC5Pt[MAXCALOJETS];
  Float_t              caloJetIC5Energy[MAXCALOJETS];
  Float_t              caloJetIC5EMF[MAXCALOJETS];
  Float_t              caloJetIC5HADF[MAXCALOJETS];

  // MET 
  Float_t              genMET;
  Float_t              MET;


};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//  bool sortByET(reco::PixelMatchGsfElectron& x, reco::PixelMatchGsfElectron& y)
//  {
//    return x.et()>y.et();
//  }


//
// constructors and destructor
//
RootNtupleMaker::RootNtupleMaker(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  rootfile_          = iConfig.getUntrackedParameter<std::string>("rootfile","LQRootNtuple.root");
  maxgenparticles_   = iConfig.getUntrackedParameter<int>("maxgenparticles",100); 
  maxgenjets_        = iConfig.getUntrackedParameter<int>("maxgenjets",10); 
  maxelectrons_      = iConfig.getUntrackedParameter<int>("maxelectrons",5); 
  maxcalojets_           = iConfig.getUntrackedParameter<int>("maxcalojets",10); 

  soup_              = iConfig.getUntrackedParameter<bool>("soup",0);
  fastSim_           = iConfig.getUntrackedParameter<bool>("fastSim",0);
  debug_             = iConfig.getUntrackedParameter<bool>("mydebug",0);
  luminosity_        = iConfig.getUntrackedParameter<double>("luminosity",100); // pb -1
  numEvents_         = iConfig.getUntrackedParameter<int>("numEvents",100); 

  saveTrigger_         = iConfig.getUntrackedParameter<bool>("saveTrigger",1);
  hltTriggerResultTag_ =  iConfig.getParameter<edm::InputTag> ("HLTTriggerResultsTag");

  //Initialize some variables
  event=-999;
  runnum=-999;

  m_cross_section=-999.;
  m_auto_cross_section=-999.;
  m_processID=-999;
  m_ALPGENprocessID=-999;
  m_filter_eff=-999.;
  m_pthat=-999.;
  m_weight=-999.;              

  genMET=-999.;
  MET=-999.;
}


RootNtupleMaker::~RootNtupleMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
RootNtupleMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;


#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif


  //********** Write here your code ***********

  if(debug_==true)
    cout << "Analyze " << endl;
   
  // Fill Event info

  event = iEvent.id().event();
  runnum = iEvent.id().run();

  // Fill gen event info
  if(soup_ == 1 && fastSim_ == 0)  //if running on soup
    {

      //PROCESS ID
      Handle< int > genProcessID;
      iEvent.getByLabel( "genEventProcID", genProcessID );
      m_processID = *genProcessID;


      //ALPGEN PROCESS ID
      if(m_processID==4)
	{
	  Handle< int > alpgenProcessID;
	  iEvent.getByLabel( "weight","AlpgenProcessID", alpgenProcessID );
	  m_ALPGENprocessID = *alpgenProcessID;
	}
      else
	m_ALPGENprocessID = -999;


      //PT HAT
      if(m_processID==4) //alpgen process
	{
	  //code from CSA07WeightProducer
	  Handle<CandidateCollection> genParticles;
	  iEvent.getByLabel( "genParticleCandidates", genParticles );    
	  double pT = 0.; //pThat
	  
	  // first loop: which process?
	  for( size_t i = 0; i < genParticles->size(); ++ i ) {
	    const Candidate & p = (*genParticles)[ i ];
	    int id = p.pdgId();
	    int st = p.status();  

	    // W+jets
	    if(st == 3 && (id == 24 || id == -24) ) {
	      pT = p.pt();
	      i = genParticles->size()-1; // get out of the loop
	    }
	    // Z+jets
	    if(st == 3 && (id == 23 || id == -23) ) {
	      pT = p.pt();
	      i = genParticles->size()-1; // get out of the loop       
	    }
	    // tt+jets
	    if(st == 3 && (id == 6 || id == -6) ) {
	      pT = p.pt();
	      i = genParticles->size()-1; // get out of the loop       
	    }
	  }
	  m_pthat = pT;
	} 
      else // *not* alpgen process
	{
	  Handle< double > genEventScale;
	  iEvent.getByLabel( "genEventScale", genEventScale );
	  m_pthat = *genEventScale;
	}


      if (m_processID != 4) 
	{
	  //FILTER EFFICIENCY
	  Handle< double > genFilterEff;
	  iEvent.getByLabel( "genEventRunInfo", "FilterEfficiency", genFilterEff);
	  m_filter_eff = *genFilterEff;

	  //CROSS SECTION
	  Handle< double > genCrossSect;
	  iEvent.getByLabel( "genEventRunInfo", "PreCalculatedCrossSection", genCrossSect); 
	  m_cross_section = *genCrossSect;
	  
	  Handle< double > genAutoCrossSect;
	  iEvent.getByLabel( "genEventRunInfo", "AutoCrossSection", genAutoCrossSect);
	  m_auto_cross_section = (*genAutoCrossSect)*float(pow(10.0,9));
	}
      else
	{
	  m_filter_eff=-999;
	  m_cross_section=-999;
	  m_auto_cross_section=-999.;
	}

      //SOUP WEIGHT
      Handle< double > weightHandle;
      iEvent.getByLabel ("weight","weight", weightHandle);
      m_weight = *weightHandle;

      if(debug_==true)
	cout << "weight: " << m_weight << endl;

    }
  
  else if(soup_ == 0 && fastSim_ == 0) // if *not* running on soup and not fastSim
    
    {
      
      Handle< GenInfoProduct > gi;
      iEvent.getRun().getByLabel( "source", gi);
      
      m_cross_section = gi->external_cross_section(); // is the precalculated one written in the cfg file -- units is pb
      m_auto_cross_section = gi->cross_section()*float(pow(10.0,9)); // is automatically calculated at the end of each RUN --  units in mb (converted to pb)

      m_filter_eff = gi->filter_efficiency();
      
      Handle< HepMCProduct > mc;
      iEvent.getByLabel( "source", mc );
      const HepMC::GenEvent * genEvt = mc->GetEvent();

      m_processID = genEvt->signal_process_id();
      m_pthat = genEvt->event_scale(); 

      m_ALPGENprocessID = -999;
      
      m_weight = m_cross_section * luminosity_ / (float) numEvents_;

    }

  else if(soup_ == 0 && fastSim_ == 1) // if *not* running on soup and running on fastSim

    {

      Handle< GenInfoProduct > gi;
      iEvent.getRun().getByLabel( "source", gi);
      
      m_cross_section = gi->external_cross_section(); // is the precalculated one written in the cfg file -- units is pb
      m_auto_cross_section = gi->cross_section()*float(pow(10.0,9)); // is automatically calculated at the end of each RUN --  units in mb (converted to pb)

      m_filter_eff = gi->filter_efficiency();
      
//       Handle< HepMCProduct > mc;
//       iEvent.getByLabel( "source", mc );
//       const HepMC::GenEvent * genEvt = mc->GetEvent();
//       m_processID = genEvt->signal_process_id();
//       m_pthat = genEvt->event_scale(); 
//       m_ALPGENprocessID = -999;
//       m_weight = m_cross_section * luminosity_ / (float) numEvents_;

      m_processID = -999;
      m_pthat = -999; 
      m_ALPGENprocessID = -999;
      m_weight = m_cross_section * luminosity_ / (float) numEvents_;

    }


  if(debug_==true)
    {
      cout << "gen event info filled" << endl;
      cout <<  "processID: " << m_processID << endl;
      cout <<  "ALPGENprocessID: " << m_ALPGENprocessID << endl;
      cout << "pthat: " << m_pthat << endl;
      cout << "filter_eff: " << m_filter_eff << endl;
      cout << "cross_section: " << m_cross_section << endl;
      cout << "auto_cross_section: " << m_auto_cross_section << endl;
      cout << "weight: " << m_weight << endl;
    }
  
  // Fill Gen Particles
  edm::Handle<reco::CandidateCollection> genParticles;
  iEvent.getByLabel ("genParticleCandidates", genParticles);
  CreateParticleTree( genParticles );     
  if(debug_==true)
    cout << "gen particles filled" << endl;

  // Fill HLT bits
  if(saveTrigger_==true)  
    {
      SetTriggers(iEvent);
      if(debug_==true)
	cout << "HLT bits filled " << endl;
    }
  else 
    {
      // reset variables in ntuple
      for(unsigned int iHLT=0; iHLT<MAXHLTBITS; ++iHLT) {
	aHLTResults[iHLT] = false;
      }

      hltCount=-999;
      hltNamesLen=-999;
      strcpy(aHLTNames,"");

      if(debug_==true)
	cout << "HLT bits not filled" << endl;
    }

  // Fill Electrons quantities
  // -------------------------
  //pixelMatchGsfElectronsClone created by PixelMatchGsfElectronShallowCloneProducer (see cfg file) 
  edm::Handle<reco::CandidateCollection> emObjectHandle_;
  iEvent.getByLabel("pixelMatchGsfElectronsClone",emObjectHandle_);
  const reco::CandidateCollection *emObjectHandle_tmp = emObjectHandle_.product();

  //create new electron list of pointers to candidates and sort it  
  std::list<const Candidate*> emObjectHandle;
  for( unsigned int i = 0 ; i < emObjectHandle_tmp->size(); i++)
    {
      const Candidate & p = (*emObjectHandle_tmp)[i];
      emObjectHandle.push_front( &p );
    }
  emObjectHandle.sort(ComparePt);
  
  //trkiso ( EgammaElectronTkIsolationProducer )
  edm::Handle< reco::PMGsfElectronIsoCollection > trkIsolationHandle;
  iEvent.getByLabel("egammaElectronTkRelIsolation",trkIsolationHandle);
  //numtrksio ( EgammaElectronTkNumIsolationProducer )
  edm::Handle< reco::PMGsfElectronIsoNumCollection > trkNumIsolationHandle;
  iEvent.getByLabel("egammaElectronTkNumIsolation",trkNumIsolationHandle);
  //ecaliso ( EgammaEcalIsolationProducer )
  edm::Handle< reco::CandViewDoubleAssociations > ecalIsolationHandle;
  iEvent.getByLabel("egammaEcalRelIsolation",ecalIsolationHandle);
  //   hcaliso ( EgammaTowerIsolationProducer )
  //   edm::Handle< reco::CandViewDoubleAssociations > hcalIsolationHandle;
  //   iEvent.getByLabel("egammaTowerIsolation",hcalIsolationHandle);

  // ADD ISOLATIONMAPS RELATIVE TO DIFFERENT ISOLATION PARAMETERS

  //loop over electrons
  eleCount  = 0;
  int eleidx=-1;
  std::list<const Candidate*>::const_iterator cand;
  for(cand=emObjectHandle.begin(); cand!=emObjectHandle.end(); cand++) 
    {
      eleidx++;

      CandidateBaseRef master = (*cand)->masterClone();
      PixelMatchGsfElectronRef electron = (*cand)->masterClone().castTo<PixelMatchGsfElectronRef>();
      
      const reco::SuperClusterRef& SCref = electron->superCluster();  

      //## Remove electrons associated to the same SC ##
      bool IsCopy=false;      

      int eleidx_1=-1;
      std::list<const Candidate*>::const_iterator cand1;
      for(cand1=emObjectHandle.begin(); cand1!=emObjectHandle.end(); cand1++) 
	{
	  eleidx_1++;	  
	  if(eleidx_1<=eleidx)
	    continue;
	  
	  CandidateBaseRef master = (*cand1)->masterClone();
	  PixelMatchGsfElectronRef electron_1 = (*cand1)->masterClone().castTo<PixelMatchGsfElectronRef>();
	  
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
      //## end remove electrons associated to the same SC

      
      //## Start counting electrons from here ##
      
      //exit from loop when you reach the required number of electrons
      if(eleCount > maxelectrons_)
	break;
      
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
      if (fabs(electron->eta()) > 1.479) 
	sigmaee = sigmaee - 0.02*(fabs(electron->eta()) - 2.3);   

      //     # cut value arrays of form {hoe, sigmaEtaEta, dPhiIn, dEtaIn}
      //     PSet robustEleIDCuts =
      //     {
      //         vdouble barrel     = {0.115, 0.0140, 0.053, 0.0090}
      //         vdouble endcap     = {0.150, 0.0275, 0.092, 0.0105}
      //     }

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
      double trkIso = (*trkIsolationHandle)[index].second; 
      double trkNumIso = (*trkNumIsolationHandle)[index].second; 
      double ecalIso = (*ecalIsolationHandle)[index].second;
      //double hcalIso = (*hcalIsolationHandle)[index].second;


      // Set variables in RootNtuple
      eleEta[eleCount]=electron->eta();
      elePhi[eleCount]=electron->phi();
      elePt[eleCount]=electron->pt();
      eleEnergy[eleCount]=electron->energy();
      eleCaloEnergy[eleCount]=electron->caloEnergy();

      eleHoE[eleCount]=hOverE;
      eleSigmaEE[eleCount]=sigmaee;
      eleDeltaPhiTrkSC[eleCount]=deltaPhiIn;
      eleDeltaEtaTrkSC[eleCount]=deltaEtaIn;
      
      eleTrkIso[eleCount]=trkIso;
      eleNumTrkIso[eleCount]=trkNumIso;
      eleEcalIso[eleCount]=ecalIso;

      //go to next electron
      eleCount++;
    }



  // Fill GenJets quantities
  // -------------------------
  edm::Handle<reco::GenJetCollection> genJets;
  iEvent.getByLabel ("iterativeCone5GenJetsPt10", genJets);
  
  genJetCount=0;
  for( GenJetCollection::const_iterator genjet = genJets->begin(); genjet != genJets->end(); genjet++ ) 
    {

      //exit from loop when you reach the required number of electrons
      if(genJetCount > maxgenjets_)
	break;

      float EMF = genjet->emEnergy() / genjet->energy();
      float HADF = genjet->hadEnergy() / genjet->energy();

      genJetPt[genJetCount]=genjet->pt();
      genJetPhi[genJetCount]=genjet->phi();
      genJetEta[genJetCount]=genjet->eta();
      genJetEnergy[genJetCount]=genjet->energy();
      genJetEMF[genJetCount]=EMF;
      genJetHADF[genJetCount]=HADF;
      
      genJetCount++;
    }
  
  // Fill CaloJets quantities
  // -------------------------
  edm::Handle<reco::CaloJetCollection> caloJetsIC5;
  iEvent.getByLabel ("iterativeCone5CaloJets", caloJetsIC5);

  caloJetIC5Count=0;
  for( CaloJetCollection::const_iterator calojet = caloJetsIC5->begin(); calojet != caloJetsIC5->end(); calojet++ ) 
    {
      //exit from loop when you reach the required number of electrons
      if(caloJetIC5Count > maxcalojets_)
	break;

      float EMF = calojet->emEnergyFraction();
      float HADF = calojet->energyFractionHadronic();

      caloJetIC5Pt[caloJetIC5Count]=calojet->pt();
      caloJetIC5Phi[caloJetIC5Count]=calojet->phi();
      caloJetIC5Eta[caloJetIC5Count]=calojet->eta();
      caloJetIC5Energy[caloJetIC5Count]=calojet->energy();
      caloJetIC5EMF[caloJetIC5Count]=EMF;
      caloJetIC5HADF[caloJetIC5Count]=HADF;
      
      caloJetIC5Count++;
    }


  // Fill MET and GenMET
  // -------------------

  // GENMET
  Handle<GenMETCollection> genMETColl;
  iEvent.getByLabel( "genMet" , genMETColl );

  const GenMET & genmet = (*genMETColl)[0];
  genMET = genmet.et();

  // MET
  Handle<CaloMETCollection> recoMETColl;
  iEvent.getByLabel ( "met" , recoMETColl);

  const reco::CaloMET & recomet = (*recoMETColl)[0];
  MET = recomet.et();


  //  Fill tree for each event
  //  ********************************************************
  //  ********************************************************
  //  ********************************************************
   
  if(debug_==true)
    cout << "About to fill tree" << endl;
  m_tree->Fill();
   
  //  ********************************************************
  //  ********************************************************
  //  ********************************************************

}


// ------------ method called once each job just before starting event loop  ------------
void 
RootNtupleMaker::beginJob(const edm::EventSetup&)
{

  m_file = new TFile(rootfile_.c_str(),"RECREATE");
  m_tree = NULL;
  m_tree = new TTree ("RootNtupleMaker","RootNtupleMaker") ;

  m_tree->Branch("event",&event,"event/I");
  m_tree->Branch("run",&runnum,"runnum/I");

  m_tree->Branch("cross_section",&m_cross_section,"cross_section/F");
  m_tree->Branch("auto_cross_section",&m_auto_cross_section,"auto_cross_section/F");
  m_tree->Branch("processID",&m_processID,"processID/I");
  m_tree->Branch("ALPGENprocessID",&m_ALPGENprocessID,"ALPGENprocessID/I");
  m_tree->Branch("filter_eff",&m_filter_eff,"filter_eff/F"); 
  m_tree->Branch("pthat",&m_pthat,"pthat/F");
  m_tree->Branch("weight",&m_weight,"weight/F");
  
  m_tree->Branch("GenParticleCount",&m_GenParticleCount,"GenParticleCount/I");
  m_tree->Branch("GenParticleE",&m_GenParticleE,"GenParticleE[GenParticleCount]/F");
  m_tree->Branch("GenParticleP",&m_GenParticleP,"GenParticleP[GenParticleCount]/F");
  m_tree->Branch("GenParticlePt",&m_GenParticlePt,"GenParticlePt[GenParticleCount]/F");
  m_tree->Branch("GenParticlePx",&m_GenParticlePx,"GenParticlePz[GenParticleCount]/F");
  m_tree->Branch("GenParticlePy",&m_GenParticlePy,"GenParticlePy[GenParticleCount]/F");
  m_tree->Branch("GenParticlePz",&m_GenParticlePz,"GenParticlePz[GenParticleCount]/F");
  m_tree->Branch("GenParticlePdgId",&m_GenParticlePdgId,"GenParticlePdgId[GenParticleCount]/I");
  m_tree->Branch("GenParticleEta",&m_GenParticleEta,"GenParticleEta[GenParticleCount]/F");
  m_tree->Branch("GenParticlePhi",&m_GenParticlePhi,"GenParticlePhi[GenParticleCount]/F");
  m_tree->Branch("GenParticleVX",&m_GenParticleVX,"GenParticleVX[GenParticleCount]/F");
  m_tree->Branch("GenParticleVY",&m_GenParticleVY,"GenParticleVY[GenParticleCount]/F");
  m_tree->Branch("GenParticleVZ",&m_GenParticleVZ,"GenParticleVZ[GenParticleCount]/F");
  m_tree->Branch("GenParticleMotherIndex",&m_GenParticleMotherIndex,"GenParticleMotherIndex[GenParticleCount]/I");
  m_tree->Branch("GenParticleNumDaught",&m_GenParticleNumDaught,"GenParticleNumDaught[GenParticleCount]/I");

  m_tree->Branch("hltCount",&hltCount,"hltCount/I");
  m_tree->Branch("hltNamesLen",&hltNamesLen,"hltNamesLen/I");
  m_tree->Branch("HLTNames",&aHLTNames,"HLTNames[hltNamesLen]/C",6000);
  m_tree->Branch("HLTResults",&aHLTResults,"HLTResults[hltCount]/O");

  m_tree->Branch("eleCount",&eleCount,"eleCount/I");
  m_tree->Branch("eleEta",&eleEta,"eleEta[eleCount]/F");
  m_tree->Branch("elePhi",&elePhi,"elePhi[eleCount]/F");
  m_tree->Branch("elePt",&elePt,"elePt[eleCount]/F");
  m_tree->Branch("eleEnergy",&eleEnergy,"eleEnergy[eleCount]/F");
  m_tree->Branch("eleCaloEnergy",&eleCaloEnergy,"eleCaloEnergy[eleCount]/F");

  m_tree->Branch("eleHoE",&eleHoE,"eleHoE[eleCount]/F");
  m_tree->Branch("eleSigmaEE",&eleSigmaEE,"eleSigmaEE[eleCount]/F");
  m_tree->Branch("eleDeltaPhiTrkSC",&eleDeltaPhiTrkSC,"eleDeltaPhiTrkSC[eleCount]/F");
  m_tree->Branch("eleDeltaEtaTrkSC",&eleDeltaEtaTrkSC,"eleDeltaEtaTrkSC[eleCount]/F");

  m_tree->Branch("eleTrkIso",&eleTrkIso,"eleTrkIso[eleCount]/F");
  m_tree->Branch("eleNumTrkIso",&eleNumTrkIso,"eleNumTrkIso[eleCount]/F");
  m_tree->Branch("eleEcalIso",&eleEcalIso,"eleEcalIso[eleCount]/F");

  m_tree->Branch("genJetCount",&genJetCount,"genJetCount/I");
  m_tree->Branch("genJetEta",&genJetEta,"genJetEta[genJetCount]/F");
  m_tree->Branch("genJetPhi",&genJetPhi,"genJetPhi[genJetCount]/F");
  m_tree->Branch("genJetPt",&genJetPt,"genJetPt[genJetCount]/F");
  m_tree->Branch("genJetEnergy",&genJetEnergy,"genJetEnergy[genJetCount]/F");
  m_tree->Branch("genJetEMF",&genJetEMF,"genJetEMF[genJetCount]/F");
  m_tree->Branch("genJetHADF",&genJetHADF,"genJetHADF[genJetCount]/F");

  m_tree->Branch("caloJetIC5Count",&caloJetIC5Count,"caloJetIC5Count/I");
  m_tree->Branch("caloJetIC5Eta",&caloJetIC5Eta,"caloJetIC5Eta[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5Phi",&caloJetIC5Phi,"caloJetIC5Phi[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5Pt",&caloJetIC5Pt,"caloJetIC5Pt[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5Energy",&caloJetIC5Energy,"caloJetIC5Energy[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5EMF",&caloJetIC5EMF,"caloJetIC5EMF[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5HADF",&caloJetIC5HADF,"caloJetIC5HADF[caloJetIC5Count]/F");

  m_tree->Branch("genMET",&genMET,"genMET/F");
  m_tree->Branch("MET",&MET,"MET/F");
}


////====================================================================
void RootNtupleMaker::CreateParticleTree(edm::Handle<reco::CandidateCollection> collection)
{
////====================================================================
  m_GenParticleCount = 0;
  reco::CandidateCollection::const_iterator cand;
  for(cand=collection->begin(); cand!=collection->end(); cand++) 
  {

    if(m_GenParticleCount  > maxgenparticles_)
        break;

    m_GenParticleP[m_GenParticleCount] = cand->p();
    m_GenParticlePx[m_GenParticleCount] = cand->px();
    m_GenParticlePy[m_GenParticleCount] = cand->py();
    m_GenParticlePz[m_GenParticleCount] = cand->pz();
    m_GenParticlePt[m_GenParticleCount] = cand->pt();
    m_GenParticleEta[m_GenParticleCount] = cand->eta();
    m_GenParticlePhi[m_GenParticleCount] = cand->phi();
    m_GenParticleE[m_GenParticleCount] = cand->energy();
    m_GenParticlePdgId[m_GenParticleCount] = cand->pdgId();
    m_GenParticleNumDaught[m_GenParticleCount] = cand->numberOfDaughters();
   
    m_GenParticleVX[m_GenParticleCount] = cand->vx();
    m_GenParticleVY[m_GenParticleCount] = cand->vy();
    m_GenParticleVZ[m_GenParticleCount] = cand->vz();
 
    m_GenParticleMotherIndex[m_GenParticleCount] = -1;
    int idx=0;
    reco::CandidateCollection::const_iterator candIter;
    for(candIter = collection->begin(); candIter!=collection->end(); candIter++) {
      if(&(*candIter)==cand->mother()) {
       m_GenParticleMotherIndex[m_GenParticleCount] = idx;
       break;
      }
      idx++;
    }
  m_GenParticleCount++;
  }    
}


////====================================================================
void RootNtupleMaker::SetTriggers(const edm::Event& iEvent)
{
  
  ////====================================================================
  // reset variables in ntuple
  for(unsigned int iHLT=0; iHLT<MAXHLTBITS; ++iHLT) {
    aHLTResults[iHLT] = false;
  }

  strcpy(aHLTNames,"");
  hltNamesLen = 0;

  edm::Handle<edm::TriggerResults> hltTriggerResultHandle;
  iEvent.getByLabel(hltTriggerResultTag_, hltTriggerResultHandle);

  if(!hltTriggerResultHandle.isValid()) {
    std::cout << "invalid handle for HLT TriggerResults" << std::endl;
  } else {
    
    edm::TriggerNames HLTNames;
    HLTNames.init(*hltTriggerResultHandle);
    
    std::string tempnames;
    
    hltCount = hltTriggerResultHandle->size();
    //std::cout << "hltTriggerResult->size(): " << hltCount << std::endl;

    for(int i = 0 ; i < hltCount ; i++) {
      //      cout << "HLTTrigger: " << HLTNames.triggerName(i).c_str() << "  " << hltTriggerResultHandle->accept(i) << endl;
      tempnames += HLTNames.triggerName(i) + ":";
      aHLTResults[i] = hltTriggerResultHandle->accept(i);
    }
    
    hltNamesLen = tempnames.length();
    strcpy(aHLTNames,tempnames.c_str());
  
  }

}


// ------------ method called once each job just after ending the event loop  ------------
void 
RootNtupleMaker::endJob() {

  if(debug_==true)
    cout << "Before write tree" << endl;
  
  m_file = m_tree->GetCurrentFile();
  Int_t ret = m_file->Write();
  
  if(debug_==true)
    cout << "Tree saved." << ret <<  endl;

  m_file->Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(RootNtupleMaker);
