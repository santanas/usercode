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
// Original Author:  Francesco Santanastasio
//         Created:  Thu May 22 21:54:39 CEST 2008
// $Id: RootNtupleMaker.cc,v 1.6 2008/07/19 17:30:44 santanas Exp $
//
//

// system include files
#include <memory>
#include <typeinfo>

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
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

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

//Muons
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "RecoMuon/MuonIdentification/interface/IdGlobalFunctions.h"
#include "DataFormats/TrackReco/interface/Track.h"

// About tree
#define MAXGENPARTICLES  2000
#define MAXHLTBITS    200
#define MAXELECTRONS  100
#define MAXGENJETS    100
#define MAXCALOJETS   100
#define MAXMUONS      100


// namespaces
using namespace std;
using namespace reco;

typedef std::pair<const PixelMatchGsfElectron*, int> my_pair;

bool ComparePtPair(const my_pair& left , const my_pair& right)
{
  return left.first->pt() > right.first->pt();
}

bool sortByET(const reco::Muon &x, const reco::Muon &y)
    {
      return x.et()>y.et();
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

  int singleEleRelHLTCounter;

  // ----------member data ---------------------------

  // read from cfg file
  std::string          rootfile_;
  int                  maxgenparticles_;
  int                  maxgenjets_;
  int                  maxelectrons_;
  int                  maxcalojets_;
  int                  maxmuons_;
  bool                 aodsim_;
  bool                 soup_;
  bool                 fastSim_;
  bool                 debug_;
  double               luminosity_;
  int                  numEvents_;              
  bool                 saveTrigger_;
  std::string          skim_;
  int                  prescaleSingleEleRel_;

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
  Float_t              caloJetIC5EMF[MAXCALOJETS];
  Float_t              caloJetIC5HADF[MAXCALOJETS];
  Float_t              caloJetIC5Pt_raw[MAXCALOJETS];
  Float_t              caloJetIC5Energy_raw[MAXCALOJETS];
  Float_t              caloJetIC5Pt[MAXCALOJETS];
  Float_t              caloJetIC5Energy[MAXCALOJETS];
  Float_t              caloJetIC5_L5corr[MAXCALOJETS];


  // Muons
  Int_t                muonCount;
  Float_t              muonEta[MAXMUONS];
  Float_t              muonPhi[MAXMUONS];
  Float_t              muonPt[MAXMUONS];
  Float_t              muonEnergy[MAXMUONS];
  Float_t              muonTrkHits[MAXMUONS];
  Float_t              muonTrkD0[MAXMUONS];
  Float_t              muonTrkDz[MAXMUONS];
  Float_t              muonEcalIso[MAXMUONS];
  Float_t              muonTrkIso[MAXMUONS];
  Float_t              muonHcalIso[MAXMUONS];
  Float_t              muonHOIso[MAXMUONS];

  // MET 
  Float_t              genMET;
  Float_t              MET;

  // Parameters from cfg
  std::string mUDSCorrectorName;


};

//
// constants, enums and typedefs
//


//
// static data member definitions
//


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
  maxcalojets_       = iConfig.getUntrackedParameter<int>("maxcalojets",10); 
  maxmuons_          = iConfig.getUntrackedParameter<int>("maxmuons",5); 

  aodsim_            = iConfig.getUntrackedParameter<bool>("aodsim",0);
  soup_              = iConfig.getUntrackedParameter<bool>("soup",0);
  fastSim_           = iConfig.getUntrackedParameter<bool>("fastSim",0);
  debug_             = iConfig.getUntrackedParameter<bool>("mydebug",0);
  luminosity_        = iConfig.getUntrackedParameter<double>("luminosity",100); // pb -1
  numEvents_         = iConfig.getUntrackedParameter<int>("numEvents",100); 

  saveTrigger_           = iConfig.getUntrackedParameter<bool>("saveTrigger",1);
  hltTriggerResultTag_   =  iConfig.getParameter<edm::InputTag> ("HLTTriggerResultsTag");
  prescaleSingleEleRel_  = iConfig.getUntrackedParameter<int>("prescaleSingleEleRel",30); 
  skim_                  = iConfig.getUntrackedParameter<std::string>("skim","electron");

  mUDSCorrectorName = iConfig.getParameter <std::string> ("UDSQuarksCorrector");

  //Initialize some variables
  singleEleRelHLTCounter=0;

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
  if(soup_ == 1 && fastSim_ == 0 && aodsim_==1)  //if running on soup
    {

      if(debug_==true)
	cout << "Running on soup" << endl;

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
  
  else if(soup_ == 0 && fastSim_ == 0 && aodsim_==0) // if running on reco
    
    {

      if(debug_==true)
	cout << "Running on reco" << endl;

      
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

  else if(soup_ == 0 && fastSim_ == 1 && aodsim_==1) // if *not* running on soup and running on fastSim (aodsim)

    {

      if(debug_==true)
	cout << "Running on fastSim" << endl;


      Handle< GenInfoProduct > gi;
      iEvent.getRun().getByLabel( "source", gi);
      
      m_cross_section = gi->external_cross_section(); // is the precalculated one written in the cfg file -- units is pb
      m_auto_cross_section = gi->cross_section()*float(pow(10.0,9)); // is automatically calculated at the end of each RUN --  units in mb (converted to pb)

      m_filter_eff = gi->filter_efficiency();

      // (needed PhysicsTools/HepMCCandAlgos/data/genEventWeight.cfi
      // and  PhysicsTools/HepMCCandAlgos/data/genEventScale.cfi,.. )

      //PROCESS ID
      Handle< int > genProcessID;
      iEvent.getByLabel( "genEventProcID", genProcessID );
      m_processID = *genProcessID;

      //PT HAT
      Handle< double > genEventScale;
      iEvent.getByLabel( "genEventScale", genEventScale );
      m_pthat = *genEventScale;


      //m_processID = -999;
      //m_pthat = -999; 
      m_ALPGENprocessID = -999;
      m_weight = m_cross_section * luminosity_ / (float) numEvents_;

    }

  else if(soup_ == 0 && fastSim_ == 0 && aodsim_ == 1)  //if running on aodsim (non soup)

    {

      if(debug_==true)
	cout << "Running on aodsim" << endl;


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

      m_weight = m_cross_section * luminosity_ / (float) numEvents_;

      if(debug_==true)
	cout << "weight: " << m_weight << endl;

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
  // Get the objects that were fed into the isolation producer (not necessary for method 2)
  edm::Handle< edm::View<reco::Candidate> > emObjectHandle_;
  iEvent.getByLabel("pixelMatchGsfElectrons",emObjectHandle_);
  const edm::View<reco::Candidate> *emObjectHandle = emObjectHandle_.product();

  //cout << "********** BEFORE SORTING **********" << endl;
  std::list<my_pair> electronRefListPair;
  for(int elecand_idx = 0; elecand_idx < (int)emObjectHandle->size(); elecand_idx++) 
    {
      const PixelMatchGsfElectronRef electron = emObjectHandle->refAt(elecand_idx).castTo<PixelMatchGsfElectronRef>();
      electronRefListPair.push_front( my_pair(&*electron,electron.key()) );

      //cout << "pT: " << electron->pt() << " " << "key: " << electron.key() << endl;

    }
  electronRefListPair.sort(ComparePtPair);  

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

  //******** ADD ISOLATIONMAPS RELATIVE TO DIFFERENT ISOLATION PARAMETERS ********


  //Loop over electrons
  eleCount = 0;
  int eleidx=-1;
  std::list<my_pair>::const_iterator electron;
  for(electron=electronRefListPair.begin(); electron!=electronRefListPair.end(); electron++) 
    {
      eleidx++;

      const reco::SuperClusterRef& SCref = (*electron).first->superCluster();  

      //## Remove electrons associated to the same SC ##
      bool IsCopy=false;      

      int eleidx_1=-1;
      std::list<my_pair>::const_iterator electron_1;
      for(electron_1=electronRefListPair.begin(); electron_1!=electronRefListPair.end(); electron_1++) 
	{
	  eleidx_1++;	  
	  if(eleidx_1<=eleidx)
	    continue;
	  
	  const reco::SuperClusterRef& SCref_1 = (*electron_1).first->superCluster();  	  
	  
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
      //       float pT_ele=(*electron).first->pt();
      //       float E_ele=(*electron).first->energy();
      //       float E_sc_ele=(*electron).first->caloEnergy();
      //       float eta_ele=(*electron).first->eta();
      //       float phi_ele=(*electron).first->phi();
      
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
      seedShpItr = barrelClShpMap.find((*electron).first->superCluster()->seed());
      if(seedShpItr==barrelClShpMap.end()) 
	{
	  hasBarrel=false;
	  seedShpItr=endcapClShpMap.find((*electron).first->superCluster()->seed());
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
      float hOverE = (*electron).first->hadronicOverEm();
      float sigmaee = sqrt(sClShape->covEtaEta());
      float deltaPhiIn = (*electron).first->deltaPhiSuperClusterTrackAtVtx();
      float deltaEtaIn = (*electron).first->deltaEtaSuperClusterTrackAtVtx();

      //correct sigmaetaeta dependence on eta in endcap
      if (fabs((*electron).first->eta()) > 1.479) 
	sigmaee = sigmaee - 0.02*(fabs((*electron).first->eta()) - 2.3);   

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
      int index = (*electron).second;

      //-- hcal tower isolation
      double trkIso = (*trkIsolationHandle)[index].second; 
      double trkNumIso = (*trkNumIsolationHandle)[index].second; 
      double ecalIso = (*ecalIsolationHandle)[index].second;
      //double hcalIso = (*hcalIsolationHandle)[index].second;


      // Set variables in RootNtuple
      eleEta[eleCount]=(*electron).first->eta();
      elePhi[eleCount]=(*electron).first->phi();
      elePt[eleCount]=(*electron).first->pt();
      eleEnergy[eleCount]=(*electron).first->energy();
      eleCaloEnergy[eleCount]=(*electron).first->caloEnergy();

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

  //raw
  edm::Handle<reco::CaloJetCollection> caloJetsIC5_raw;
  iEvent.getByLabel ("iterativeCone5CaloJets", caloJetsIC5_raw); 

  //L2+L3 correction
  edm::Handle<reco::CaloJetCollection> caloJetsIC5;
  iEvent.getByLabel ("L3JetCorJetIcone5", caloJetsIC5); 

  // get Jet Flavor corrector (L5)
  const JetCorrector* udsJetCorrector = JetCorrector::getJetCorrector (mUDSCorrectorName, iSetup);

  caloJetIC5Count=0;
  for( CaloJetCollection::const_iterator calojet = caloJetsIC5->begin(); calojet != caloJetsIC5->end(); calojet++ ) 
    {
      //exit from loop when you reach the required number of electrons
      if(caloJetIC5Count > maxcalojets_)
	break;

      float JEScorrection = float ( udsJetCorrector->correction( (*caloJetsIC5)[caloJetIC5Count], iEvent, iSetup) );

      float EMF = calojet->emEnergyFraction();
      float HADF = calojet->energyFractionHadronic();

      caloJetIC5_L5corr[caloJetIC5Count]=JEScorrection;
      caloJetIC5Pt[caloJetIC5Count]=calojet->pt()*JEScorrection;
      caloJetIC5Energy[caloJetIC5Count]=calojet->energy()*JEScorrection;
      caloJetIC5Pt_raw[caloJetIC5Count]=(*caloJetsIC5_raw)[caloJetIC5Count].pt();
      caloJetIC5Energy_raw[caloJetIC5Count]=(*caloJetsIC5_raw)[caloJetIC5Count].energy();
      caloJetIC5Phi[caloJetIC5Count]=calojet->phi();
      caloJetIC5Eta[caloJetIC5Count]=calojet->eta();
      caloJetIC5EMF[caloJetIC5Count]=EMF;
      caloJetIC5HADF[caloJetIC5Count]=HADF;
      
      caloJetIC5Count++;
    }
 

  // Muons
  //***********
  //testing
  //edm::Handle<reco::CandidateCollection> MuonObjectHandle_;
  //const reco::CandidateCollection *MuonObjectHandle_tmp = MuonObjectHandle_.product();

  // Read in Muons
  edm::Handle<reco::MuonCollection> MuonObjectHandle_;

  if(fastSim_==1)
    {
      //FastSim
      std::string m_muonModule = "paramMuons";
      std::string m_muonLabel  = "ParamGlobalMuons";
      iEvent.getByLabel(m_muonModule,m_muonLabel,MuonObjectHandle_);
    }
  else if(fastSim_==0)
    {
      //FullSim
      std::string m_muonLabel  = "muons";
      iEvent.getByLabel(m_muonLabel,MuonObjectHandle_);
    }

  const reco::MuonCollection* MuonObjectHandle_tmp;
  MuonObjectHandle_tmp = MuonObjectHandle_.product();

  /// sort muons in decending ET
  reco::MuonCollection muonColl = *MuonObjectHandle_tmp;
  std::sort(muonColl.begin(), muonColl.end(), sortByET);

  muonCount = 0;  
  for(reco::MuonCollection::const_iterator muon = muonColl.begin(); muon != muonColl.end(); muon++)
    //for(reco::MuonCollection::const_iterator muon = MuonObjectHandle_tmp->begin(); muon != MuonObjectHandle_tmp->end(); muon++)
    {

      //exit from loop when you reach the required number of muons
      if(muonCount > maxmuons_)
	break;
      
      //std::cout << "My candidate is a " << typeid(&*candmu).name() << std::endl;
      //https://hypernews.cern.ch/HyperNews/CMS/get/physTools/327/1.html
      //PixelMatchGsfElectronRef electron = (*cand)->masterClone().castTo<PixelMatchGsfElectronRef>();
      // CandidateBaseRef master = (*candmu)->masterClone();
      // cout << master->pt() << endl;
      //MuonRef muon = (*candmu)->masterClone().castTo<MuonRef>();

      unsigned int trkhits = 0;
      float trkd0 = 0.;
      float trkdz = 0.;

      if(fastSim_==1)
	{
	  //FastSim
	  trkhits  = muon->track()->numberOfValidHits();
	  trkd0    = muon->track()->d0();
	  trkdz    = muon->track()->dz();
	}	  
      else if(fastSim_==0)
	{
	  //FullSim
	  trkhits  = muon->combinedMuon()->numberOfValidHits();
	  trkd0    = muon->combinedMuon()->d0();
	  trkdz    = muon->combinedMuon()->dz();
	}	  

      float ptInCone       = 0.;
      float coneEMenergy   = 0.;
      float coneHADenergy  = 0.;
      float coneHOenergy   = 0.;

      bool m_useTrackConeSize03 = true;
      bool m_useTrackConeSize05 = false;

      if (m_useTrackConeSize03)
	{
	  ptInCone      = muon->getIsolationR03().sumPt;
	  coneEMenergy  = muon->getIsolationR03().emEt;
	  coneHADenergy = muon->getIsolationR03().hadEt;
	  coneHOenergy  = muon->getIsolationR03().hoEt;
	}
      else if (m_useTrackConeSize05)
	{
 	  ptInCone      = muon->getIsolationR05().sumPt;
 	  coneEMenergy  = muon->getIsolationR03().emEt;
 	  coneHADenergy = muon->getIsolationR03().hadEt;
 	  coneHOenergy  = muon->getIsolationR03().hoEt;
 	}

      muonEta[muonCount] = muon->eta();
      muonPhi[muonCount] = muon->phi();
      muonPt[muonCount]= muon->pt();
      muonEnergy[muonCount] = muon->energy();
      muonTrkHits[muonCount] = trkhits;
      muonTrkD0[muonCount] = trkd0;
      muonTrkDz[muonCount] = trkdz;
      muonEcalIso[muonCount] = coneEMenergy;
      muonTrkIso[muonCount] = ptInCone;
      muonHcalIso[muonCount] = coneHADenergy;
      muonHOIso[muonCount] = coneHOenergy;

      //       cout << "muon candidate: " << muonCount << endl;
      //       cout << "pT: " << muon->pt() << endl;
      //       cout << "p: " << muon->p() << endl;
      //       cout << "energy: " << muon->energy() << endl;
      //       cout << "eta: " << muon->eta() << endl;
      //       cout << "phi: " << muon->phi() << endl;
      //       cout << "theta: " << muon->theta() << endl;
      //       cout << "pTInCone: " << ptInCone << endl;
      //       cout << "coneEMenergy: " << coneEMenergy << endl;
      //       cout << "coneHADenergy: " << coneHADenergy << endl;
      //       cout << "coneHOenergy: " << coneHOenergy << endl;
      //       cout << "trkhits: " << trkhits << endl;
      //       cout << "trkd0: " << trkd0 << endl;
      //       cout << "trkdz: " << trkdz << endl;
      //       cout << "*************" << endl;
      
      muonCount++;

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


  // Skims
  // -------------------

  //######### Skim "none" ###########
  if(skim_=="none")
    {
      //  Fill tree for each event
      //  ********************************************************
      //  ********************************************************
      
      if(debug_==true)
	cout << "About to fill tree" << endl;
      m_tree->Fill();
      
      //  ********************************************************
      //  ********************************************************
    }//end skim "none"      


  //######### Skim "electron" ###########
  if(skim_=="electron" && saveTrigger_==true)
    {
      int N_GSFele=1;
      float pT_GSFele=85.;

      bool passHLT=false;
      bool passHLTprescale=false;
      bool passNEle=false;
      bool passPtEle=false;
      bool passSkim=false;
      
      //main triggers
      if(aHLTResults[40] || aHLTResults[41])
	passHLT=true;

      //prescaled single electron trigger
      if(aHLTResults[33]==true)
	{
	  singleEleRelHLTCounter++;
	  if(singleEleRelHLTCounter==prescaleSingleEleRel_)
	    {
	      passHLTprescale=true;
	      singleEleRelHLTCounter=0;
	    }
	}

      //N GSF electrons
      if(eleCount>=N_GSFele)
	passNEle=true;

      //pT cut on electrons
      if(passNEle)
	{
	  int n_ele = N_GSFele - 1;

	  if(elePt[n_ele]>pT_GSFele)
	    passPtEle=true;
	}

      //skim selection
      if( (passHLT==true && passNEle==true && passPtEle==true) 
	  || passHLTprescale==true)
	{
	  passSkim=true;
	}

      //skip event if not pass skim
      if(passSkim==true)
	{
	  //  Fill tree for each event
	  //  ********************************************************
	  //  ********************************************************
	  
	  if(debug_==true)
	    cout << "About to fill tree" << endl;
	  m_tree->Fill();
	  
	  //  ********************************************************
	  //  ********************************************************
	}//end if pass skim

    }//end skim "electron"


  //######### Skim "muon" ###########
  if(skim_=="muon" && saveTrigger_==true)
    {
      //########## Add muon skim here ############
    }//end skim "muon"


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
  m_tree->Branch("caloJetIC5Pt_raw",&caloJetIC5Pt_raw,"caloJetIC5Pt_raw[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5Energy_raw",&caloJetIC5Energy_raw,"caloJetIC5Energy_raw[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5EMF",&caloJetIC5EMF,"caloJetIC5EMF[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5HADF",&caloJetIC5HADF,"caloJetIC5HADF[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5_L5corr",&caloJetIC5_L5corr,"caloJetIC5_L5corr[caloJetIC5Count]/F");


  m_tree->Branch("muonCount",&muonCount,"muonCount/I");
  m_tree->Branch("muonEta",&muonEta,"muonEta[muonCount]/F");
  m_tree->Branch("muonPhi",&muonPhi,"muonPhi[muonCount]/F");
  m_tree->Branch("muonPt",&muonPt,"muonPt[muonCount]/F");
  m_tree->Branch("muonEnergy",&muonEnergy,"muonEnergy[muonCount]/F");
  m_tree->Branch("muonTrkHits",&muonTrkHits,"muonTrkHits[muonCount]/F");
  m_tree->Branch("muonTrkD0",&muonTrkD0,"muonTrkD0[muonCount]/F");
  m_tree->Branch("muonTrkDz",&muonTrkDz,"muonTrkDz[muonCount]/F");
  m_tree->Branch("muonEcalIso",&muonEcalIso,"muonEcalIso[muonCount]/F");
  m_tree->Branch("muonTrkIso",&muonTrkIso,"muonTrkIso[muonCount]/F");
  m_tree->Branch("muonHcalIso",&muonHcalIso,"muonHcalIso[muonCount]/F");
  m_tree->Branch("muonHOIso",&muonHOIso,"muonHOIso[muonCount]/F");

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
