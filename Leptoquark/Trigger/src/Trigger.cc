// -*- C++ -*-
//
// Package:    Trigger
// Class:      Trigger
// 
/**\class Trigger Trigger.cc Leptoquark/Trigger/src/Trigger.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  pts/34
//         Created:  Sat Apr 26 17:19:59 CEST 2008
// $Id: Trigger.cc,v 1.1 2008/04/26 16:31:13 santanas Exp $
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

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include<vector>
#include<string>

#include <iomanip>

//
// class decleration
//

class Trigger : public edm::EDAnalyzer {
   public:
      explicit Trigger(const edm::ParameterSet&);
      ~Trigger();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      edm::InputTag hlTriggerResults_;  // Input tag for TriggerResults
      edm::TriggerNames triggerNames_;  // TriggerNames class

      unsigned int  nEvents_;           // number of events processed

      unsigned int  nWasRun_;           // # where at least one HLT was run
      unsigned int  nAccept_;           // # of accepted events
      unsigned int  nErrors_;           // # where at least one HLT had error

      std::vector<unsigned int> hlWasRun_; // # where HLT[i] was run
      std::vector<unsigned int> hlAccept_; // # of events accepted by HLT[i]
      std::vector<unsigned int> hlErrors_; // # of events with error in HLT[i]

      std::vector<std::string>  hlNames_;  // name of each HLT algorithm
      bool init_;                          // vectors initialised or not

      unsigned int  nPassHE;           // # number of events passing HLT1EMHighEt
      unsigned int  nPassVHE;           // # number of events passing HLT1EMVeryHighEt
      unsigned int  nPassSingleEleRel;           // # number of events passing HLT1ElectronRelaxed
      unsigned int  nPassHE_VHE;           // # number of events passing HLT1EMHighEt OR HLT1EMVeryHighEt
      unsigned int  nPassHE_VHE_SingleEleRel;           // # number of events passing HLT1EMHighEt OR HLT1EMVeryHighEt

      float  effHE;      
      float  effVHE;     
      float  effSingleEleRel; 
      float  effHE_VHE;       
      float  effHE_VHE_SingleEleRel;

      float  e_effHE;      
      float  e_effVHE;     
      float  e_effSingleEleRel; 
      float  e_effHE_VHE;       
      float  e_effHE_VHE_SingleEleRel;


  //TH1D * histo; 

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
Trigger::Trigger(const edm::ParameterSet& iConfig)

{

  hlTriggerResults_ = iConfig.getParameter<edm::InputTag>("HLTriggerResults");
  nEvents_ = 0;
  nWasRun_ = 0;
  nAccept_ = 0;
  nErrors_ = 0;
  init_ = false;

  nPassHE = 0;         
  nPassVHE = 0;        
  nPassSingleEleRel = 0;
  nPassHE_VHE = 0;           
  nPassHE_VHE_SingleEleRel = 0;

  effHE = 0;      
  effVHE = 0;     
  effSingleEleRel = 0; 
  effHE_VHE = 0;       
  effHE_VHE_SingleEleRel = 0;
  
  e_effHE = 0;      
  e_effVHE = 0;     
  e_effSingleEleRel = 0; 
  e_effHE_VHE = 0;       
  e_effHE_VHE_SingleEleRel = 0;

  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  //histo = fs->make<TH1D>("charge" , "Charges" , 200 , -2 , 2 );

}


Trigger::~Trigger()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
Trigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;
  using namespace edm;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif

   //**************************************************************
   
   nEvents_++;

   // get hold of TriggerResults
   Handle<TriggerResults> HLTR;
   try {iEvent.getByLabel(hlTriggerResults_,HLTR);} catch (...) {;}
   if (HLTR.isValid()) {
     if (HLTR->wasrun()) nWasRun_++;
     const bool accept(HLTR->accept());
     LogDebug("") << "HL TriggerResults decision: " << accept;
     if (accept) ++nAccept_;
     if (HLTR->error() ) nErrors_++;
   } else {
     LogDebug("") << "HL TriggerResults with label ["+hlTriggerResults_.encode()+"] not found!";
     nErrors_++;
     return;
   }
   
   // initialisation (could be made dynamic)
   if (!init_) {
     init_=true;
     triggerNames_.init(*HLTR);
     hlNames_=triggerNames_.triggerNames();
     const unsigned int n(hlNames_.size());
     hlWasRun_.resize(n);
     hlAccept_.resize(n);
     hlErrors_.resize(n);
     for (unsigned int i=0; i!=n; ++i) {
       hlWasRun_[i]=0;
       hlAccept_[i]=0;
       hlErrors_[i]=0;
     }
   }
   
   // decision for each HL algorithm
   const unsigned int n(hlNames_.size());
   for (unsigned int i=0; i!=n; ++i) {
     if (HLTR->wasrun(i)) hlWasRun_[i]++;
     if (HLTR->accept(i)) hlAccept_[i]++;
     if (HLTR->error(i) ) hlErrors_[i]++;

   }


   if(HLTR->accept(40)==true)  nPassHE++;  
   if(HLTR->accept(41)==true)  nPassVHE++;  
   if(HLTR->accept(33)==true)  nPassSingleEleRel++;  
   if(HLTR->accept(40)==true || HLTR->accept(41)==true) 
     nPassHE_VHE++;
   if(HLTR->accept(40)==true || HLTR->accept(41)==true || HLTR->accept(33)==true)
     nPassHE_VHE_SingleEleRel++;
   

   //end of analyze
   return;   
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
Trigger::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Trigger::endJob() {

  // final printout of accumulated statistics

  using namespace std;
  const unsigned int n(hlNames_.size());

  cout << dec << endl;
  cout << "HLT-Report " << "---------- Event  Summary ------------\n";
  cout << "HLT-Report"
       << " Events total = " << nEvents_
       << " wasrun = " << nWasRun_
       << " passed = " << nAccept_
       << " errors = " << nErrors_
       << "\n";

  cout << endl;
  cout << "HLT-Report " << "---------- HLTrig Summary ------------\n";
  cout << "HLT-Report "
       << right << setw(10) << "HLT  Bit#" << " "
       << right << setw(10) << "WasRun" << " "
       << right << setw(10) << "Passed" << " "
       << right << setw(10) << "Errors" << " "
       << "Name" << "\n";

  if (init_) {
    for (unsigned int i=0; i!=n; ++i) {
      cout << "HLT-Report "
	   << right << setw(10) << i << " "
	   << right << setw(10) << hlWasRun_[i] << " "
	   << right << setw(10) << hlAccept_[i] << " "
	   << right << setw(10) << hlErrors_[i] << " "
	   << hlNames_[i] << "\n";
    }
  } else {
    cout << "HLT-Report - No HL TriggerResults found!" << endl;
  }

  cout << endl;
  cout << "HLT-Report end!" << endl;
  cout << endl;


  //*************************************

  cout << "*****************************************" << endl;
  cout << "Trigger efficiency report for LQ analyzis" << endl;
  cout << "*****************************************" << endl;

  //   nEvents_;
  
  effHE = float(nPassHE) / float(nEvents_);
  e_effHE = sqrt(effHE*(1-effHE)/float(nEvents_));

  effVHE = float(nPassVHE) / float(nEvents_);
  e_effVHE = sqrt(effVHE*(1-effVHE)/float(nEvents_));

  effSingleEleRel = float(nPassSingleEleRel) / float(nEvents_);
  e_effSingleEleRel = sqrt(effSingleEleRel*(1-effSingleEleRel)/float(nEvents_));

  effHE_VHE = float(nPassHE_VHE) / float(nEvents_);
  e_effHE_VHE = sqrt(effHE_VHE*(1-effHE_VHE)/float(nEvents_));

  effHE_VHE_SingleEleRel = float(nPassHE_VHE_SingleEleRel) / float(nEvents_);
  e_effHE_VHE_SingleEleRel = sqrt(effHE_VHE_SingleEleRel*(1-effHE_VHE_SingleEleRel)/float(nEvents_));

  //   float  effVHE;     
  //   float  effSingleEleRel; 
  //   float  effHE_VHE;       
  //   float  effHE_VHE_SingleEleRel;
  
  //   float  e_effHE;      
  //   float  e_effVHE;     
  //   float  e_effSingleEleRel; 
  //   float  e_effHE_VHE;       
  //   float  e_effHE_VHE_SingleEleRel;

  cout << endl;
  cout << "nEvents: " << nEvents_ << endl; 
  cout << endl;  

  cout << "Eff-Report "
       << right << setw(10) << "Eff" << " "
       << right << setw(10) << "Err Eff" << " "
       << right << setw(10) << "Passed" << " "
       << "Name" << "\n";

  cout << "Eff-Report "
       << right << setw(10) << scientific << setprecision(2) << effHE << " "
       << right << setw(10) << scientific << setprecision(2) << e_effHE << " "
       << right << setw(10) << nPassHE << " "
       << hlNames_[40] << "(" << 40 << ")" <<"\n";

  cout << "Eff-Report "
       << right << setw(10) << scientific << setprecision(2) << effVHE << " "
       << right << setw(10) << scientific << setprecision(2) << e_effVHE << " "
       << right << setw(10) << nPassVHE << " "
       << hlNames_[41] << "(" << 41 << ")" <<"\n";

  cout << "Eff-Report "
       << right << setw(10) << scientific << setprecision(2) << effSingleEleRel << " "
       << right << setw(10) << scientific << setprecision(2) << e_effSingleEleRel << " "
       << right << setw(10) << nPassSingleEleRel << " "
       << hlNames_[33] << "(" << 33 << ")" <<"\n";

  cout << "Eff-Report "
       << right << setw(10) << scientific << setprecision(2) << effHE_VHE << " "
       << right << setw(10) << scientific << setprecision(2) << e_effHE_VHE << " "
       << right << setw(10) << nPassHE_VHE << " "
       << hlNames_[40] << "(" << 40 << ")" << "," 
       << hlNames_[41] << "(" << 41 << ")" << "\n";

  cout << "Eff-Report "
       << right << setw(10) << scientific << setprecision(2) << effHE_VHE_SingleEleRel << " "
       << right << setw(10) << scientific << setprecision(2) << e_effHE_VHE_SingleEleRel << " "
       << right << setw(10) << nPassHE_VHE_SingleEleRel << " "
       << hlNames_[40] << "(" << 40 << ")" << "," 
       << hlNames_[41] << "(" << 41 << ")" << ","
       << hlNames_[33] << "(" << 33 << ")" << "\n";

  return;

}

//define this as a plug-in
DEFINE_FWK_MODULE(Trigger);
