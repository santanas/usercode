#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>

analysisClass::analysisClass(string * inputList, string * cutFile, string * treeName, TString * outputFileName, string * cutEfficFile)
  :baseClass(inputList, cutFile, treeName, outputFileName, cutEfficFile)
{
  std::cout << "analysisClass::analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::analysisClass(): ends " << std::endl;
}

analysisClass::~analysisClass()
{
  std::cout << "analysisClass::~analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::~analysisClass(): ends " << std::endl;
}

void analysisClass::Loop()
{
   std::cout << "analysisClass::Loop() begins" <<std::endl;   
    
   if (fChain == 0) return;
   
   //////////book histos here
   TH2F *h_RelDeltaP_vs_trkP_top = new TH2F ("h_RelDeltaP_vs_trkP_top","h_RelDeltaP_vs_trkP_top",500,0,500,500,-10,10);
   TH2F *h_RelDeltaP_vs_trkP_bottom = new TH2F ("h_RelDeltaP_vs_trkP_bottom","h_RelDeltaP_vs_trkP_bottom",500,0,500,500,-10,10);

   TH2F *h_muonDTP_vs_trkP_top = new TH2F ("h_muonDTP_vs_trkP_top","h_muonDTP_vs_trkP_top",500,0,500,500,0,500);
   TH2F *h_muonDTP_vs_trkP_bottom = new TH2F ("h_muonDTP_vs_trkP_bottom","h_muonDTP_vs_trkP_bottom",500,0,500,500,0,500);

#ifdef USE_EXAMPLE
   STDOUT("WARNING: using example code. In order NOT to use it, comment line that defines USE_EXAMPLE flag in Makefile.");   
   // number of electrons
   TH1F *h_nEleFinal = new TH1F ("h_nEleFinal","",11,-0.5,10.5);
   h_nEleFinal->Sumw2();
   //pT 1st ele
   TH1F *h_pT1stEle = new TH1F ("h_pT1stEle","",100,0,1000);
   h_pT1stEle->Sumw2();
   //pT 2nd ele
   TH1F *h_pT2ndEle = new TH1F ("h_pT2ndEle","",100,0,1000);
   h_pT2ndEle->Sumw2();

#endif //end of USE_EXAMPLE

   /////////initialize variables

   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   ////// The following ~7 lines have been taken from rootNtupleClass->Loop() /////
   ////// If the root version is updated and rootNtupleClass regenerated,     /////
   ////// these lines may need to be updated.                                 /////    
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
     // if (Cut(ientry) < 0) continue;

     ////////////////////// User's code starts here ///////////////////////

     ///Stuff to be done every event

#ifdef USE_EXAMPLE
     // Electrons
     vector<int> v_idx_ele_final;
     for(int iele=0;iele<eleCount;iele++)
       {
	 // ECAL barrel fiducial region
	 bool pass_ECAL_FR=false;
	 if( fabs(eleEta[iele]) < getPreCutValue1("eleFidRegion") )	v_idx_ele_final.push_back(iele);
       }     

     // Set the evaluation of the cuts to false and clear the variable values and filled status
     resetCuts();
     
     // Set the value of the variableNames listed in the cutFile to their current value
     fillVariableWithValue("nEleFinal", v_idx_ele_final.size()) ;
     if( v_idx_ele_final.size() >= 1 ) 
       {
	 fillVariableWithValue( "pT1stEle", elePt[v_idx_ele_final[0]] );
       }
     if( v_idx_ele_final.size() >= 2 ) 
       {
	 fillVariableWithValue( "pT2ndEle", elePt[v_idx_ele_final[1]] );
	 // Calculate Mee
	 TLorentzVector v_ee, ele1, ele2;
	 ele1.SetPtEtaPhiM(elePt[v_idx_ele_final[0]],eleEta[v_idx_ele_final[0]],elePhi[v_idx_ele_final[0]],0);
	 ele2.SetPtEtaPhiM(elePt[v_idx_ele_final[1]],eleEta[v_idx_ele_final[1]],elePhi[v_idx_ele_final[1]],0);
	 v_ee = ele1 + ele2;
	 fillVariableWithValue( "invMass_ee", v_ee.M() ) ;
       }

     // Evaluate cuts (but do not apply them)
     evaluateCuts();
     
     // Fill histograms and do analysis based on cut evaluation
     h_nEleFinal->Fill(v_idx_ele_final.size());
     //if( v_idx_ele_final.size()>=1 ) h_pT1stEle->Fill(elePt[v_idx_ele_final[0]]);
     //if( v_idx_ele_final.size()>=2 && (elePt[v_idx_ele_final[0]])>85 ) h_pT2ndEle->Fill(elePt[v_idx_ele_final[1]]);
     if( passedCut("pT1stEle") ) h_pT1stEle->Fill(elePt[v_idx_ele_final[0]]);
     if( passedCut("pT2ndEle") ) h_pT2ndEle->Fill(elePt[v_idx_ele_final[1]]);
     
     // reject events that did not pass level 0 cuts
     if( !passedCut("0") ) continue;
     // ......
     
     // reject events that did not pass level 1 cuts
     if( !passedCut("1") ) continue;
     // ......

     // reject events that did not pass the full cut list
     if( !passedCut("all") ) continue;
     // ......

#endif  // end of USE_EXAMPLE

     //########### My code here #############
     // Set the evaluation of the cuts to false and clear the variable values and filled status
     resetCuts();

     double LenghtHB = getPreCutValue2("radiusHB") - getPreCutValue1("radiusHB");

     int n_DTtracksT=0;
     int n_DTtracksB=0;
     int n_DTtracksValidProp=0;
     for(int muon=0; muon<muonDTCount; muon++)
       {
	 if(muonDTYin[muon]>0 && muonDTYout[muon]>0)
	   n_DTtracksT++;

	 if(muonDTYin[muon]<0 && muonDTYout[muon]<0)
	   n_DTtracksB++;

	 if(muonDTisValidProp[muon]==1)
	   n_DTtracksValidProp++;
       }

     fillVariableWithValue( "N_DTtracks", muonDTCount );
     fillVariableWithValue( "N_DTtracksProp", n_DTtracksValidProp );
     fillVariableWithValue( "N_DTtracksT", n_DTtracksT );
     fillVariableWithValue( "N_DTtracksB", n_DTtracksB );
     fillVariableWithValue( "N_TRKtracks", trackCount );

     if( muonDTCount >=2  )
       {

	 int indexTOP=-1;
	 int indexBOTTOM=-1;
	 
	 if(muonDTYin[0]>0 && muonDTYout[0]>0)
	   {
	     indexTOP = 0;
	   }
	 else
	   if(muonDTYin[0]<0 && muonDTYout[0]<0)
	     {
	       indexBOTTOM = 0;
	     }

	 if(muonDTYin[1]>0 && muonDTYout[1]>0)
	   {
	     indexTOP = 1;
	   }
	 else
	   if(muonDTYin[1]<0 && muonDTYout[1]<0)
	     {
	       indexBOTTOM = 1;
	     }
	
	 if(indexTOP >= 0 && indexBOTTOM >= 0)
	   {
	     //top
	     fillVariableWithValue( "muonTDXY_DT_TRK", muonDTnrstTrkDXY[indexTOP] );
	     fillVariableWithValue( "muonTDZ_DT_TRK", muonDTnrstTrkDZ[indexTOP] );
	     fillVariableWithValue( "muonTDTNHits", muonDTNumRecHits[indexTOP] );
	     fillVariableWithValue( "muonTDTNValidHits", muonDTNumValidRecHits[indexTOP] );
	     fillVariableWithValue( "muonTDTEtaAtHB", max(muonDTEtaAtInHB[indexTOP],muonDTEtaAtOutHB[indexTOP]) );
	     fillVariableWithValue( "muonTDTPhiAtHB", max(muonDTPhiAtInHB[indexTOP],muonDTPhiAtOutHB[indexTOP]) );
	     fillVariableWithValue( "muonTDTLenghtHB", muonDTLengthInHB[indexTOP] );
	     fillVariableWithValue( "muonTDTdRHitMuInHB", muonDTdeltaRatInHB[indexTOP] );
	     fillVariableWithValue( "muonTDTdRHitMuOutHB", muonDTdeltaRatOutHB[indexTOP] );
	     fillVariableWithValue( "muonTDTEnergy", muonDTTotEnergy[indexTOP] );
	     double normEnergyTOP = (muonDTTotEnergy[indexTOP] / muonDTLengthInHB[indexTOP]) * LenghtHB; 
	     fillVariableWithValue( "muonTDTEnergyNorm", normEnergyTOP );
	     fillVariableWithValue( "muonTDTNCells", muonDTNumCells[indexTOP] );
	     fillVariableWithValue( "muonTDTCellSameEta", muonDTHaveCellsSameEta[indexTOP] );
	     fillVariableWithValue( "muonTDTCellSamePhi", muonDTHaveCellsSamePhi[indexTOP] );
	     if(muonDTHaveCellsSameEta[indexTOP] || muonDTHaveCellsSamePhi[indexTOP])
	       fillVariableWithValue( "muonTDTCellSmEtaOrPhi", 1 );
	     fillVariableWithValue( "muonTDTP", muonDTP[indexTOP] );
	     fillVariableWithValue( "muonTDTPt", muonDTPt[indexTOP] );

	     if(muonDTnrstTrkIdx[indexTOP]>=0)
	       {
		 fillVariableWithValue( "muonTDTtrkChi2", trackChi2[muonDTnrstTrkIdx[indexTOP]] );
		 fillVariableWithValue( "muonTDTtrkNdof", trackNdof[muonDTnrstTrkIdx[indexTOP]] );
		 fillVariableWithValue( "muonTDTtrkNrmChi2", trackChi2[muonDTnrstTrkIdx[indexTOP]]/trackNdof[muonDTnrstTrkIdx[indexTOP]] );
		 fillVariableWithValue( "muonTDTtrkP", trackP[muonDTnrstTrkIdx[indexTOP]] );
		 fillVariableWithValue( "muonTDTtrkPt", trackPt[muonDTnrstTrkIdx[indexTOP]] );
		 double relDeltaP = ( muonDTP[indexTOP] - trackP[muonDTnrstTrkIdx[indexTOP]] ) / trackP[muonDTnrstTrkIdx[indexTOP]];
		 double relDeltaPt = ( muonDTPt[indexTOP] - trackPt[muonDTnrstTrkIdx[indexTOP]] ) / trackPt[muonDTnrstTrkIdx[indexTOP]];
		 fillVariableWithValue( "muonTDTtrkRelDeltaP", relDeltaP );
		 fillVariableWithValue( "muonTDTtrkRelDeltaPt", relDeltaPt );
	       }

	     //bottom
	     fillVariableWithValue( "muonBDXY_DT_TRK", muonDTnrstTrkDXY[indexBOTTOM] );
	     fillVariableWithValue( "muonBDZ_DT_TRK", muonDTnrstTrkDZ[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTNHits", muonDTNumRecHits[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTNValidHits", muonDTNumValidRecHits[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTEtaAtHB", max(muonDTEtaAtInHB[indexBOTTOM],muonDTEtaAtOutHB[indexBOTTOM]) );
	     fillVariableWithValue( "muonBDTPhiAtHB", max(muonDTPhiAtInHB[indexBOTTOM],muonDTPhiAtOutHB[indexBOTTOM]) );
	     fillVariableWithValue( "muonBDTLenghtHB", muonDTLengthInHB[indexBOTTOM] );	     
	     fillVariableWithValue( "muonBDTdRHitMuInHB", muonDTdeltaRatInHB[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTdRHitMuOutHB", muonDTdeltaRatOutHB[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTEnergy", muonDTTotEnergy[indexBOTTOM] );
	     double normEnergyBOTTOM = (muonDTTotEnergy[indexBOTTOM] / muonDTLengthInHB[indexBOTTOM]) * LenghtHB; 
	     fillVariableWithValue( "muonBDTEnergyNorm", normEnergyBOTTOM );
	     fillVariableWithValue( "muonBDTNCells", muonDTNumCells[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTCellSameEta", muonDTHaveCellsSameEta[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTCellSamePhi", muonDTHaveCellsSamePhi[indexBOTTOM] );
	     if(muonDTHaveCellsSameEta[indexBOTTOM] || muonDTHaveCellsSamePhi[indexBOTTOM])
	       fillVariableWithValue( "muonBDTCellSmEtaOrPhi", 1 );
	     fillVariableWithValue( "muonBDTP", muonDTP[indexBOTTOM] );
	     fillVariableWithValue( "muonBDTPt", muonDTPt[indexBOTTOM] );

	     if(muonDTnrstTrkIdx[indexBOTTOM]>=0)
	       {
		 fillVariableWithValue( "muonBDTtrkChi2", trackChi2[muonDTnrstTrkIdx[indexBOTTOM]] );
		 fillVariableWithValue( "muonBDTtrkNdof", trackNdof[muonDTnrstTrkIdx[indexBOTTOM]] );
		 fillVariableWithValue( "muonBDTtrkNrmChi2", trackChi2[muonDTnrstTrkIdx[indexBOTTOM]]/trackNdof[muonDTnrstTrkIdx[indexBOTTOM]] );
		 fillVariableWithValue( "muonBDTtrkP", trackP[muonDTnrstTrkIdx[indexBOTTOM]] );
		 fillVariableWithValue( "muonBDTtrkPt", trackPt[muonDTnrstTrkIdx[indexBOTTOM]] );
		 double relDeltaP = ( muonDTP[indexBOTTOM] - trackP[muonDTnrstTrkIdx[indexBOTTOM]] ) / trackP[muonDTnrstTrkIdx[indexBOTTOM]];
		 double relDeltaPt = ( muonDTPt[indexBOTTOM] - trackPt[muonDTnrstTrkIdx[indexBOTTOM]] ) / trackPt[muonDTnrstTrkIdx[indexBOTTOM]];
		 fillVariableWithValue( "muonBDTtrkRelDeltaP", relDeltaP );
		 fillVariableWithValue( "muonBDTtrkRelDeltaPt", relDeltaPt );
	       }
	   }
       
       }

     // Evaluate cuts (but do not apply them)
     evaluateCuts();

     if( passedCut("0") && passedCut("1") &&  passedCut("2") && passedCut("3") && passedCut("4") ) 
       { 
	 if( muonDTCount >=2  )
	   {
	   
	     int indexTOP=-1;
	     int indexBOTTOM=-1;
	   
	     if(muonDTYin[0]>0 && muonDTYout[0]>0)
	       indexTOP = 0;
	     else
	       if(muonDTYin[0]<0 && muonDTYout[0]<0)
		 indexBOTTOM = 0;
	     if(muonDTYin[1]>0 && muonDTYout[1]>0)
	       indexTOP = 1;
	     else
	       if(muonDTYin[1]<0 && muonDTYout[1]<0)
		 indexBOTTOM = 1;

	     if(indexTOP >= 0 && indexBOTTOM >= 0)
	       {
		 if(muonDTnrstTrkIdx[indexTOP]>=0)
		   {
		     double relDeltaP = ( muonDTP[indexTOP] - trackP[muonDTnrstTrkIdx[indexTOP]] ) / trackP[muonDTnrstTrkIdx[indexTOP]];
		     h_RelDeltaP_vs_trkP_top->Fill(trackP[muonDTnrstTrkIdx[indexTOP]] , relDeltaP);
		     h_muonDTP_vs_trkP_top->Fill(trackP[muonDTnrstTrkIdx[indexTOP]] , muonDTP[indexTOP]);
		   }
	     
		 if(muonDTnrstTrkIdx[indexBOTTOM]>=0)
		   {
		     double relDeltaP = ( muonDTP[indexBOTTOM] - trackP[muonDTnrstTrkIdx[indexBOTTOM]] ) / trackP[muonDTnrstTrkIdx[indexBOTTOM]];
		     h_RelDeltaP_vs_trkP_bottom->Fill(trackP[muonDTnrstTrkIdx[indexBOTTOM]] , relDeltaP);
		     h_muonDTP_vs_trkP_bottom->Fill(trackP[muonDTnrstTrkIdx[indexBOTTOM]] , muonDTP[indexBOTTOM]);
		   }
	       }
	   }
       }
     
     ////////////////////// User's code ends here ///////////////////////

   } // End loop over events

   //////////write histos 


#ifdef USE_EXAMPLE
   STDOUT("WARNING: using example code. In order NOT to use it, comment line that defines USE_EXAMPLE flag in Makefile.");   

   h_nEleFinal->Write();
   h_pT1stEle->Write();
   h_pT2ndEle->Write();

   //pT of both electrons, to be built using the histograms produced automatically by baseClass
   TH1F * h_pTElectrons = new TH1F ("h_pTElectrons","", getHistoNBins("pT1stEle"), getHistoMin("pT1stEle"), getHistoMax("pT1stEle"));
   h_pTElectrons->Add( & getHisto_noCuts_or_skim("pT1stEle") ); // all histos can be retrieved, see other getHisto_xxxx methods in baseClass.h
   h_pTElectrons->Add( & getHisto_noCuts_or_skim("pT2ndEle") );
   //one could also do:  *h_pTElectrons = getHisto_noCuts_or_skim("pT1stEle") + getHisto_noCuts_or_skim("pT2ndEle");
   h_pTElectrons->Write();
   //one could also do:   const TH1F& h = getHisto_noCuts_or_skim// and use h
#endif // end of USE_EXAMPLE

   h_RelDeltaP_vs_trkP_top->Write();
   h_RelDeltaP_vs_trkP_bottom->Write();
   h_muonDTP_vs_trkP_top->Write();
   h_muonDTP_vs_trkP_bottom->Write();
		 
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
