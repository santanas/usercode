// -*- C++ -*-
//
// Package:    MyEcalChannelStatus
// Class:      MyEcalChannelStatus
// 
/**\class MyEcalChannelStatus MyEcalChannelStatus.cc Various/MyEcalChannelStatus/src/MyEcalChannelStatus.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Francesco Santanastasio,8 R-019,+41227675765,
//         Created:  Thu Oct 21 15:41:58 CEST 2010
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalElectronicsId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EcalScDetId.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "TH1.h"
#include "TH2.h"
#include "TMath.h"

//
// class declaration
//

class MyEcalChannelStatus : public edm::EDAnalyzer {
   public:
      explicit MyEcalChannelStatus(const edm::ParameterSet&);
      ~MyEcalChannelStatus();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      edm::InputTag   ecalEBInputTag,ecalEEInputTag;
      TH2D* EBmap_channelStatus;
      TH2D* EEPmap_channelStatus;
      TH2D* EEMmap_channelStatus;
      TH2D* ECALmap_EtaPhi_chstatus_12_13_14;
      TH1D* EB_dead_category;
      TH1D* EE_dead_category;
      edm::Service<TFileService> histServ;
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
MyEcalChannelStatus::MyEcalChannelStatus(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

  ecalEBInputTag = iConfig.getParameter<edm::InputTag>("EcalEBInputTag");
  ecalEEInputTag = iConfig.getParameter<edm::InputTag>("EcalEEInputTag");

  EBmap_channelStatus=histServ->make<TH2D>("EBmap_channelStatus","EB map channel status",370,0,370,200,-100,100);
  EEPmap_channelStatus=histServ->make<TH2D>("EEPmap_channelStatus","EEP map channel status",100,0,100,100,0,100);
  EEMmap_channelStatus=histServ->make<TH2D>("EEMmap_channelStatus","EEM map channel status",100,0,100,100,0,100);

  ECALmap_EtaPhi_chstatus_12_13_14=histServ->make<TH2D>("ECALmap_EtaPhi_chstatus_12_13_14",
							"ECAL eta/phi map, channel status: 12,13,14",
							120,-3,3,120,-TMath::Pi(),+TMath::Pi() );

  EBmap_channelStatus->GetXaxis()->SetTitle("i\\phi");
  EBmap_channelStatus->GetYaxis()->SetTitle("i\\eta");
  EEPmap_channelStatus->GetXaxis()->SetTitle("ix");
  EEPmap_channelStatus->GetYaxis()->SetTitle("iy");
  EEMmap_channelStatus->GetXaxis()->SetTitle("ix");
  EEMmap_channelStatus->GetYaxis()->SetTitle("iy");

  EB_dead_category=histServ->make<TH1D>("EB_dead_category","EB dead category",4,0,4);
  EB_dead_category->GetXaxis()->SetBinLabel(1,"12");
  EB_dead_category->GetXaxis()->SetBinLabel(2,"13");
  EB_dead_category->GetXaxis()->SetBinLabel(3,"14");
  //   EB_dead_category->GetXaxis()->SetBinLabel(4,"78");
  //   EB_dead_category->GetXaxis()->SetBinLabel(5,"142");
  EB_dead_category->GetXaxis()->SetBinLabel(4,"all");

  EE_dead_category=histServ->make<TH1D>("EE_dead_category","EE dead category",4,0,4);
  EE_dead_category->GetXaxis()->SetBinLabel(1,"12");
  EE_dead_category->GetXaxis()->SetBinLabel(2,"13");
  EE_dead_category->GetXaxis()->SetBinLabel(3,"14");
  //   EE_dead_category->GetXaxis()->SetBinLabel(4,"78");
  //   EE_dead_category->GetXaxis()->SetBinLabel(5,"142");
  EE_dead_category->GetXaxis()->SetBinLabel(4,"all");


}


MyEcalChannelStatus::~MyEcalChannelStatus()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
MyEcalChannelStatus::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

  // retrieve the ECAL channel status
  edm::ESHandle<EcalChannelStatus> pChannelStatus;
  iSetup.get<EcalChannelStatusRcd>().get(pChannelStatus);
  const EcalChannelStatus* chStatus = pChannelStatus.product();

  // retrieve the ECAL RecHitCollections
  edm::Handle<EcalRecHitCollection> ebRecHits_h;
  iEvent.getByLabel(ecalEBInputTag, ebRecHits_h);
  const EcalRecHitCollection * ebRecHits = ebRecHits_h.product();

  edm::Handle<EcalRecHitCollection> eeRecHits_h;
  iEvent.getByLabel(ecalEEInputTag, eeRecHits_h);
  const EcalRecHitCollection * eeRecHits = eeRecHits_h.product();
    
  // retrieve geometry
  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<CaloGeometryRecord>().get(pG);  

  const CaloSubdetectorGeometry* geomEB ( dynamic_cast<const EcalBarrelGeometry*> (pG->getSubdetectorGeometry( DetId::Ecal, EcalBarrel )) );
  const CaloSubdetectorGeometry* geomEE ( dynamic_cast<const EcalEndcapGeometry*> (pG->getSubdetectorGeometry( DetId::Ecal, EcalEndcap )) );

  // Barrel
  int index_bar=0;	
  std::cerr << "BARREL --" << std::endl;
  for ( int i = 0; i < EBDetId::kSizeForDenseIndexing; ++i )
    {
      EBDetId id = EBDetId::unhashIndex( i );
      if ( id != EBDetId(0) ) {
	//ecal channel status
	EcalChannelStatusMap::const_iterator chit = chStatus->find( id );
	EcalChannelStatusCode chStatusCode = 1;
	if ( chit != chStatus->end() ) {
	  chStatusCode = *chit;
	} else {
	  edm::LogError("MyEcalChannelStatusError") << "No channel status found for xtal "
						    << id.rawId()
						    << "! something wrong with EcalChannelStatus in your DB? ";
	}
	int channelStatus = (chStatusCode.getStatusCode() & 0x001F);

	const CaloCellGeometry* cell ( geomEB->getGeometry(id) ) ;
	const GlobalPoint pos ( cell->getPosition() ) ;

	EBmap_channelStatus->Fill(  id.iphi()  , 
				  id.ieta()  , 
				  channelStatus );	    

	if ( channelStatus == 12 || channelStatus == 13 || channelStatus == 14)
	     //	     || channelStatus == 78 || channelStatus == 142 )
	  {
	    index_bar++;	    
	    std::cout << " idx " << index_bar 
		      << "  :  id " << id.rawId() 
		      << "  :  ieta,iphi " << id.ieta() << "," << id.iphi() 
		      << "  :  eta,phi " << pos.eta() << "," << pos.phi() 
		      << "  :  channelStatus " << channelStatus	     
		      << "\n";
	    //		      << EcalSeverityLevelAlgo::severityLevel(id, *ebRecHits, *chStatus )
	    //                << "  :  approxEta " << id.approxEta()   
	    EB_dead_category->AddBinContent(4);
	    ECALmap_EtaPhi_chstatus_12_13_14->Fill( pos.eta() , pos.phi() );

	  }	

	if( channelStatus == 12 )
	  EB_dead_category->AddBinContent(1);
	if( channelStatus == 13 )
	  EB_dead_category->AddBinContent(2);
	if( channelStatus == 14 )
	  EB_dead_category->AddBinContent(3);
// 	if( channelStatus == 78 )
// 	  EB_dead_category->AddBinContent(4);
// 	if( channelStatus == 142 )
// 	  EB_dead_category->AddBinContent(5);

      }
    }//barrel


  // Endcap
  int index_end=0;	
  std::cerr << "ENDCAP --" << std::endl;
  for ( int i = 0; i < EEDetId::kSizeForDenseIndexing; ++i )
    {
      EEDetId id = EEDetId::unhashIndex( i );
      if ( id != EEDetId(0) ) {
	//ecal channel status
	EcalChannelStatusMap::const_iterator chit = chStatus->find( id );
	EcalChannelStatusCode chStatusCode = 1;
	if ( chit != chStatus->end() ) {
	  chStatusCode = *chit;
	} else {
	  edm::LogError("MyEcalChannelStatusError") << "No channel status found for xtal "
						    << id.rawId()
						    << "! something wrong with EcalChannelStatus in your DB? ";
	}
	int channelStatus = (chStatusCode.getStatusCode() & 0x001F);

	const CaloCellGeometry* cell ( geomEE->getGeometry(id) ) ;
	const GlobalPoint pos ( cell->getPosition() ) ;

	if( id.zside()==1 )
	  EEPmap_channelStatus->Fill( id.ix(), id.iy(), channelStatus );
	else
	  EEMmap_channelStatus->Fill( id.ix(), id.iy(), channelStatus );

	if ( channelStatus == 12 || channelStatus == 13 || channelStatus == 14)
	     //	     || channelStatus == 78 || channelStatus == 142 )
	  {
	    index_end++;	      
	    std::cout << " idx " << index_end 
		      << "  :  id " << id.rawId() 
		      << "  : ix,iy " << id.ix() << "," << id.iy() 
		      << "  : eta,phi " << pos.eta() << "," << pos.phi() 
		      << "  : channelStatus " << channelStatus	     
		      << "\n";
	    //		  << EcalSeverityLevelAlgo::severityLevel(id, *eeRecHits, *chStatus )

	    EE_dead_category->AddBinContent(4);
	    ECALmap_EtaPhi_chstatus_12_13_14->Fill( pos.eta() , pos.phi() );
	  }

	if( channelStatus == 12 )
	  EE_dead_category->AddBinContent(1);
	if( channelStatus == 13 )
	  EE_dead_category->AddBinContent(2);
	if( channelStatus == 14 )
	  EE_dead_category->AddBinContent(3);
// 	if( channelStatus == 78 )
// 	  EE_dead_category->AddBinContent(4);
// 	if( channelStatus == 142 )
// 	  EE_dead_category->AddBinContent(5);

      }
    }//endcap

}


// ------------ method called once each job just before starting event loop  ------------
void 
MyEcalChannelStatus::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyEcalChannelStatus::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyEcalChannelStatus);
