#include "Selection.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> //serve per sprintf 
#include <iomanip>

#include <TTree.h>
#include <TChain.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>

using namespace std;


//1)usare array di float ed int per dumpare il numero di eventi selezionato dopo
//  ogni taglio. 

//==================
const int Nparam=4;   //--> NUMBER OF PARAMETERS 
//==================

float ECAL_barrel_limit = 1.479;

float ECAL_FR_1_cut = 1.4;
float ECAL_FR_2_cut = 1.6;
float ECAL_FR_3_cut = 2;

//     # cut value arrays of form {hoe, sigmaEtaEta, dPhiIn, dEtaIn}
//     PSet robustEleIDCuts =
//     {
//         vdouble barrel     = {0.115, 0.0140, 0.053, 0.0090}
//         vdouble endcap     = {0.150, 0.0275, 0.092, 0.0105}

float eleHoE_bar_cut=0.115; 
float eleSigmaEE_bar_cut=0.0140;
float eleDeltaPhiTrkSC_bar_cut=0.053;
float eleDeltaEtaTrkSC_bar_cut=0.0090;

float eleHoE_end_cut=0.150; 
float eleSigmaEE_end_cut=0.0275;
float eleDeltaPhiTrkSC_end_cut=0.092;
float eleDeltaEtaTrkSC_end_cut=0.0105;

// int eleNumTrkIso_cut=5;
// float eleTrkIso_cut=5.;
// float eleEcalIso_cut=5.;

int eleNumTrkIso_cut=1;
float eleTrkIso_cut=0.01;
float eleEcalIso_cut=0.01;

float elePt_cut=20.;

float ConeSizeJetCleaning_cut=0.5;
float jetPt_cut=20.;

float ele1stPt_cut=85.;
float ele2ndPt_cut=20.;

float jetEta_cut=3.;

float Mee_lowCut=80.;
float Mee_highCut=100.;

float St_Cut=400.;

int main(int argc, char* argv[])
{

  //=============== Check number of input parameters 
  if(argc!=Nparam+1) 
    
    {
      cout << "int main() >> ERROR : arcg = " << argc 
	   << " different from "              << Nparam+1 << endl;
      
      cout << "--- Correct Usage : exe eventProcess inputlist treename outputfilename" << endl;
      cout << "--- Correct Usage : eventProcess can be: all, ttbarjet, wjet, zjet, qcdjet, gammajet, leptoquark" << endl;
    }
  
  else 
    
    {
      //       cout << endl;
      //       cout << "***********************************" << endl;
      //       cout << "******* Launching the main  *******" << endl;
      //       cout << "***********************************" << endl;
      //       cout << endl;      

      //================ Parameters 

      // Event Number List
      TString *eventProcess= new TString(argv[1]);

      // Input list
      char listName[500];
      sprintf(listName,argv[2]); 
      
      // Name of input tree objects in (.root) files 
      char treeName[100];
      sprintf(treeName,argv[3]);
      
      // Output filename (.root)  
      TString *OutputFileName= new TString(argv[4]);


      //================ Creating chain 

      // A TChain is a collection of TFile objects.a
      // the first parameter "name" is the name of the TTree object
      // in the files added with Add.
      // Use TChain::Add to add a new element to this chain.
      
      //       cout << "Loading Data ............" << endl;
      //       cout << "Creating TChain ........." << endl;
      
      TChain *chain = new TChain(treeName);
      char pName[500];
      ifstream is(listName);
      if(is.good()) 
	{
	  cout << "Reading list : " << listName << " ......." << endl;
	  
	  while( is.getline(pName, 500, '\n') )
	    {
	      if (pName[0] == '#') continue;
	      //cout << "   Add: " << pName << endl;
	      chain->Add(pName);
	    }
	  
	  cout <<  "List read " << endl;

	  //================ Run analysis
 
	  Selection analysis(chain,OutputFileName,eventProcess);

	  //analysis.SetCuts(TrkIsoCut,HcalIsoCut,LoosePtCut,PT1Cut,PT2Cut,Eta1Cut,Eta2Cut,METCut,Ext_weight,Rescale_weight,isSoup);
	  analysis.Loop();
	 
	} // end check file is good
      
      else cout << "int main() >> ERROR : file " << listName << " not read" << endl;
      
      is.close();
      
      //cout << ".......... end of program" << endl;
    }// end check argc
}


void Selection::Loop()
{

  char eventProcess[200];
  sprintf(eventProcess,"%s",&string(*eventProcess_)[0]);

  char output_root_title[200];
  sprintf(output_root_title,"%s%s",&string(*outputFileName_)[0],".root");

  // Create output file
  TFile *output_root = new TFile(&output_root_title[0],"RECREATE");
  
  // *** Book Histograms ***
  //TH1F(const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup)

  //weight
  TH1F *h_weight = new TH1F ("h_weight","",100,0,100);

  //pThat
  TH1F *h_pThat_unweight = new TH1F ("h_pThat_unweight","",100,0,1000);
  h_pThat_unweight->Sumw2();

  TH1F *h_pThat_weight = new TH1F ("h_pThat_weight","",100,0,1000);
  h_pThat_weight->Sumw2();

  TH1F *h_pThat_sel = new TH1F ("h_pThat_sel","",100,0,1000);
  h_pThat_sel->Sumw2();

  //----------------------------------------------------------------------

  //N electron
  TH1F *h_nEleID = new TH1F ("h_nEleID","",11,-0.5,10.5);
  h_nEleID->Sumw2();

  TH1F *h_nEleFinal = new TH1F ("h_nEleFinal","",11,-0.5,10.5);
  h_nEleFinal->Sumw2();

  //pT 1st ele
  TH1F *h_pT1stEle = new TH1F ("h_pT1stEle","",100,0,1000);
  h_pT1stEle->Sumw2();
  
  //pT 2nd ele
  TH1F *h_pT2ndEle = new TH1F ("h_pT2ndEle","",100,0,1000);
  h_pT2ndEle->Sumw2();
  
  //eta 1st ele
  TH1F *h_Eta1stEle = new TH1F ("h_Eta1stEle","",100,-3,3);
  h_Eta1stEle->Sumw2();
  
  //eta 2nd ele
  TH1F *h_Eta2ndEle = new TH1F ("h_Eta2ndEle","",100,-3,3);
  h_Eta2ndEle->Sumw2();

  //----------------------------------------------------------------------

  //N jets
  TH1F *h_nJetFinal = new TH1F ("h_nJetFinal","",11,-0.5,10.5);
  h_nJetFinal->Sumw2();

  //   TH1F *h_nJetFinal_pTcut = new TH1F ("h_nJetFinal_pTcut","",11,-0.5,10.5);
  //   h_nJetFinal_pTcut->Sumw2();

  //pT 1st jet
  TH1F *h_pT1stJet = new TH1F ("h_pT1stJet","",100,0,1000);
  h_pT1stJet->Sumw2();
  
  //pT 2nd jet
  TH1F *h_pT2ndJet = new TH1F ("h_pT2ndJet","",100,0,1000);
  h_pT2ndJet->Sumw2();
  
  //eta 1st jet
  TH1F *h_Eta1stJet = new TH1F ("h_Eta1stJet","",100,-5,5);
  h_Eta1stJet->Sumw2();
  
  //eta 2nd jet
  TH1F *h_Eta2ndJet = new TH1F ("h_Eta2ndJet","",100,-5,5);
  h_Eta2ndJet->Sumw2();

  //----------------------------------------------------------------------

  //S_t
  TH1F *h_St = new TH1F ("h_St","",200,0.,2000.);
  h_St->Sumw2();
  
  //----------------------------------------------------------------------

  //Mee
  TH1F *h_Mee = new TH1F ("h_Mee","",100,0.,300.);
  h_Mee->Sumw2();

  //Mej (all Comb)
  TH1F *h_Mej_allComb = new TH1F ("h_Mej_allComb","",100,0.,1000.);
  h_Mej_allComb->Sumw2();

  TH1F *h_Mej_allComb_Meecut = new TH1F ("h_Mej_allComb_Meecut","",100,0.,1000.);
  h_Mej_allComb_Meecut->Sumw2();

  TH1F *h_Mej_allComb_Meecut_Stcut = new TH1F ("h_Mej_allComb_Meecut_Stcut","",100,0.,1000.);
  h_Mej_allComb_Meecut_Stcut->Sumw2();

  //Mej (best Comb)
  TH1F *h_Mej_best = new TH1F ("h_Mej_best","",100,0.,1000.);
  h_Mej_best->Sumw2();

  TH1F *h_Mej_best_Meecut = new TH1F ("h_Mej_best_Meecut","",100,0.,1000.);
  h_Mej_best_Meecut->Sumw2();

  TH1F *h_Mej_best_Meecut_Stcut = new TH1F ("h_Mej_best_Meecut_Stcut","",100,0.,1000.);
  h_Mej_best_Meecut_Stcut->Sumw2();

  //Mej (wrong Comb)
  TH1F *h_Mej_wrong = new TH1F ("h_Mej_wrong","",100,0.,1000.);
  h_Mej_wrong->Sumw2();

  TH2F *h_2d_Mej_best_vs_wrong = new TH2F ("h_2d_Mej_best_vs_wrong","",100,0.,1000.,100,0.,1000.);
  h_2d_Mej_best_vs_wrong->Sumw2();

  //St vs Mej (Meecut)
  TH2F *h_2d_Mej_allComb_Meecut_vs_St = new TH2F ("h_2d_Mej_allComb_Meecut_vs_St","",200,0.,2000.,100,0.,1000.);
  h_2d_Mej_allComb_Meecut_vs_St->Sumw2();

  TH2F *h_2d_Mej_best_Meecut_vs_St = new TH2F ("h_2d_Mej_best_Meecut_vs_St","",200,0.,2000.,100,0.,1000.);
  h_2d_Mej_best_Meecut_vs_St->Sumw2();

  //----------------------------------------------------------------------

  // *** Inizialize variables ***

  //********** Loop over events *********
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  int myThousCounter=0;
  bool stdLoop=true;

  cout << "Analyzing process : " << eventProcess << " ......." << endl;
      
  //for (Long64_t jentry=0; jentry<2000;jentry++) {
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    
    //###### Standard loop over all the events #####
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    // Event counter
    if(myThousCounter==1000)
      {
	cout << "event "<< jentry << endl;
	myThousCounter=0;
      }
    myThousCounter++;
    
    //**************************************
    //** Write here your code inside loop **
    //**************************************
        
    //Select event process
    bool passEventProcessSelected=false;
    
    if( strcmp(eventProcess, "all")==0 )
      {    
	passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "wjet")==0 ) 
      {
	if(ALPGENprocessID>=1000 && ALPGENprocessID<=1999)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "zjet")==0 ) 
      {
	if(ALPGENprocessID>=2000 && ALPGENprocessID<=2999)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "ttbarjet")==0 ) 
      {
	if(ALPGENprocessID>=3000 && ALPGENprocessID<=3999)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "qcdjet")==0 ) 
      {
	if(processID == 11 || processID == 12 || processID == 13 || processID == 28 || processID == 68 || processID == 53)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "gammajet")==0 ) 
      {
	if(processID == 14 || processID == 18 || processID == 29)
	  passEventProcessSelected=true;
      }
    else if( strcmp(eventProcess, "leptoquark")==0 ) 
      {
	//temporary... to be changed after new FastSim production
	if(processID == -999 && ALPGENprocessID == -999)
	  passEventProcessSelected=true;
      }
    
    //skip event if not the desired process
    if(passEventProcessSelected==false)
      continue;

    // Skip signal events in gumbo (top secret)
    if (processID == 102 || processID == 123 || processID == 124 || processID == 141) 
      continue;

    
    // -------------- Candidates -------------------

    
    //### Electron definition
    vector<int> v_idx_ele_ID;
    vector<int> v_idx_ele_final;
    for(int iele=0;iele<eleCount;iele++)
      {

	bool pass_ECAL_FR=false;
	bool pass_eleID=false;
	bool pass_eleIso=false;
	bool pass_elePt=false;
	
	//ECAL fiducial region
	if(fabs(eleEta[iele]) < ECAL_FR_1_cut 
           || 
	   (fabs(eleEta[iele]) > ECAL_FR_2_cut 
	    && fabs(eleEta[iele]) < ECAL_FR_3_cut)
	   )
	  pass_ECAL_FR=true;

	if(fabs(eleEta[iele]) < ECAL_barrel_limit
	   && eleHoE[iele] < eleHoE_bar_cut 
	   && eleSigmaEE[iele] < eleSigmaEE_bar_cut
	   && eleDeltaPhiTrkSC[iele] < eleDeltaPhiTrkSC_bar_cut
	   && eleDeltaEtaTrkSC[iele] < eleDeltaEtaTrkSC_bar_cut
	   )
	  pass_eleID=true;

	if(fabs(eleEta[iele]) > ECAL_barrel_limit
	   && eleHoE[iele] < eleHoE_end_cut 
	   && eleSigmaEE[iele] < eleSigmaEE_end_cut
	   && eleDeltaPhiTrkSC[iele] < eleDeltaPhiTrkSC_end_cut
	   && eleDeltaEtaTrkSC[iele] < eleDeltaEtaTrkSC_end_cut
	   )
	  pass_eleID=true;

	if(eleNumTrkIso[iele] <= eleNumTrkIso_cut
	   && eleTrkIso[iele] < eleTrkIso_cut
	   && eleEcalIso[iele] < eleEcalIso_cut
	   )
	  pass_eleIso=true;

	if(elePt[iele] > elePt_cut)
	  {
	    pass_elePt=true;
	  }
	
	if(pass_eleID)
	  v_idx_ele_ID.push_back(iele);	  

	if(pass_ECAL_FR && pass_eleID && pass_eleIso && pass_elePt)
	  v_idx_ele_final.push_back(iele);	  

      }


    //### Jet definition
    vector<int> v_idx_jet_final;
    //vector<int> v_idx_jet_final_pTcut;
    for(int ijet=0;ijet<caloJetIC5Count;ijet++)
      {
	bool skipJet=false;

	TVector3 calojet;
	calojet.SetPtEtaPhi(caloJetIC5Pt[ijet],caloJetIC5Eta[ijet],caloJetIC5Phi[ijet]);	

	//for(int iele=0;iele<v_idx_ele_ID.size();iele++)
	for(int iele=0;iele<min(int(v_idx_ele_ID.size()),2);iele++)
	  {

	    TVector3 ele;
	    ele.SetPtEtaPhi(elePt[v_idx_ele_ID[iele]],
			    eleEta[v_idx_ele_ID[iele]],
			    elePhi[v_idx_ele_ID[iele]]);	

	    float DeltaR_ele_jet = calojet.DeltaR(ele);

	    if(DeltaR_ele_jet<ConeSizeJetCleaning_cut)
	      {
		skipJet=true;
		break;
	      }

	  }//end ele loop

	if(skipJet==true)
	  continue;


	if(caloJetIC5Pt[ijet]>jetPt_cut)
	  v_idx_jet_final.push_back(ijet);	  

      }


    // -------------  Selection ------------------------

    //## Booleans
    bool pass_twoEle=false;
    bool pass_twoJet=false;

    bool pass_1stElePtCut=false;
    bool pass_2ndElePtCut=false;

    bool pass_1stJetEtaCut=false;
    bool pass_2ndJetEtaCut=false;

    //## Selection criteria
    if(v_idx_ele_final.size()>=2)
      pass_twoEle=true;

    if(v_idx_jet_final.size()>=2)
      pass_twoJet=true;

    if(v_idx_ele_final.size()>=1)
      {
	if(elePt[v_idx_ele_final[0]] > ele1stPt_cut)
	  pass_1stElePtCut=true;
      }

    if(pass_twoEle==true)
      {
	if(elePt[v_idx_ele_final[1]] > ele2ndPt_cut)
	  pass_2ndElePtCut=true;
      }

    if(v_idx_jet_final.size()>=1)
      {
	if( fabs(caloJetIC5Eta[v_idx_jet_final[0]]) < jetEta_cut)
	  pass_1stJetEtaCut=true;
      }

    if(pass_twoJet==true)
      {
	if( fabs(caloJetIC5Eta[v_idx_jet_final[1]]) < jetEta_cut)
	  pass_2ndJetEtaCut=true;
      }


    //## M(ele-ele)
    TLorentzVector v_ee;

    if(pass_twoEle==true)
      {
	TLorentzVector ele1;
	ele1.SetPtEtaPhiM(elePt[v_idx_ele_final[0]],eleEta[v_idx_ele_final[0]],elePhi[v_idx_ele_final[0]],0);	
	TLorentzVector ele2;
	ele2.SetPtEtaPhiM(elePt[v_idx_ele_final[1]],eleEta[v_idx_ele_final[1]],elePhi[v_idx_ele_final[1]],0);	

	v_ee = ele1 + ele2;
      }


    //## M(jet-ele) 
    TLorentzVector LQ11;
    TLorentzVector LQ12;
    TLorentzVector LQ21;
    TLorentzVector LQ22;

    float bestLQMass1=0.;
    float bestLQMass2=0.;
    float wrongLQMass1=0.;
    float wrongLQMass2=0.;

    if(pass_twoEle==true && pass_twoJet==true)
      { 
	TLorentzVector ele1;
	ele1.SetPtEtaPhiM(elePt[v_idx_ele_final[0]],eleEta[v_idx_ele_final[0]],elePhi[v_idx_ele_final[0]],0);	
	TLorentzVector ele2;
	ele2.SetPtEtaPhiM(elePt[v_idx_ele_final[1]],eleEta[v_idx_ele_final[1]],elePhi[v_idx_ele_final[1]],0);	

	TLorentzVector jet1;
	jet1.SetPtEtaPhiM(caloJetIC5Pt[v_idx_jet_final[0]],caloJetIC5Eta[v_idx_jet_final[0]],caloJetIC5Phi[v_idx_jet_final[0]],0);	
	TLorentzVector jet2;
	jet2.SetPtEtaPhiM(caloJetIC5Pt[v_idx_jet_final[1]],caloJetIC5Eta[v_idx_jet_final[1]],caloJetIC5Phi[v_idx_jet_final[1]],0);	

	//comb1	
	LQ11 = ele1 + jet1;
	LQ22 = ele2 + jet2;

	//comb2
	LQ12 = ele1 + jet2;
	LQ21 = ele2 + jet1;

	//Best LQ mass algorithm
	float DeltaM_comb1=fabs(LQ11.M()-LQ22.M());
	float DeltaM_comb2=fabs(LQ21.M()-LQ12.M());

	if(DeltaM_comb1<DeltaM_comb2)
	  {
	    bestLQMass1=LQ11.M();
	    bestLQMass2=LQ22.M();
	    wrongLQMass1=LQ12.M();
	    wrongLQMass2=LQ21.M();
	  }
	else
	  {
	    bestLQMass1=LQ12.M();
	    bestLQMass2=LQ21.M();
	    wrongLQMass1=LQ11.M();
	    wrongLQMass2=LQ22.M();
	  }

      }



    //## Plots, number of selected events and efficiencies

    h_pThat_unweight->Fill(pthat);
    h_pThat_weight->Fill(pthat,weight);

    h_nEleID->Fill(v_idx_ele_ID.size(),weight);
    h_nEleFinal->Fill(v_idx_ele_final.size(),weight);

    h_nJetFinal->Fill(v_idx_jet_final.size(),weight);
    //h_nJetFinal_pTcut->Fill(v_idx_jet_final_pTcut.size(),weight);

    if(pass_twoEle==true && pass_twoJet==true 
       && pass_1stElePtCut==true && pass_2ndElePtCut==true
       && pass_1stJetEtaCut==true && pass_2ndJetEtaCut==true)
      { 
	float St = 
	  elePt[v_idx_ele_final[0]] 
	  + elePt[v_idx_ele_final[1]] 
	  + caloJetIC5Pt[v_idx_ele_final[0]]
	  + caloJetIC5Pt[v_idx_ele_final[1]];
	
	float Mee = v_ee.M();

	h_Mee->Fill(Mee,weight);

	h_Mej_allComb->Fill(LQ11.M(),weight);
	h_Mej_allComb->Fill(LQ12.M(),weight);
	h_Mej_allComb->Fill(LQ21.M(),weight);
	h_Mej_allComb->Fill(LQ22.M(),weight);

	h_Mej_best->Fill(bestLQMass1,weight);
	h_Mej_best->Fill(bestLQMass2,weight);

	h_Mej_wrong->Fill(wrongLQMass1,weight);
	h_Mej_wrong->Fill(wrongLQMass2,weight);

	h_2d_Mej_best_vs_wrong->Fill(bestLQMass1,wrongLQMass1);
	h_2d_Mej_best_vs_wrong->Fill(bestLQMass2,wrongLQMass2);

	//Remove real Zs
	if(Mee<Mee_lowCut || Mee>Mee_highCut)
	  {
	    h_Mej_allComb_Meecut->Fill(LQ11.M(),weight);
	    h_Mej_allComb_Meecut->Fill(LQ12.M(),weight);
	    h_Mej_allComb_Meecut->Fill(LQ21.M(),weight);
	    h_Mej_allComb_Meecut->Fill(LQ22.M(),weight);

	    h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ11.M());
	    h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ12.M());
	    h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ21.M());
	    h_2d_Mej_allComb_Meecut_vs_St->Fill(St,LQ22.M());

	    h_Mej_best_Meecut->Fill(bestLQMass1,weight);
	    h_Mej_best_Meecut->Fill(bestLQMass2,weight);

	    h_2d_Mej_best_Meecut_vs_St->Fill(St,bestLQMass1);
	    h_2d_Mej_best_Meecut_vs_St->Fill(St,bestLQMass2);

	    if(St>St_Cut)
	      {
		h_Mej_allComb_Meecut_Stcut->Fill(LQ11.M(),weight);
		h_Mej_allComb_Meecut_Stcut->Fill(LQ12.M(),weight);
		h_Mej_allComb_Meecut_Stcut->Fill(LQ21.M(),weight);
		h_Mej_allComb_Meecut_Stcut->Fill(LQ22.M(),weight);

		h_Mej_best_Meecut_Stcut->Fill(bestLQMass1,weight);
		h_Mej_best_Meecut_Stcut->Fill(bestLQMass2,weight);
	      }

	    h_pT1stEle->Fill(elePt[v_idx_ele_final[0]],weight);
	    h_pT2ndEle->Fill(elePt[v_idx_ele_final[1]],weight);
	    
	    h_Eta1stEle->Fill(eleEta[v_idx_ele_final[0]],weight);
	    h_Eta2ndEle->Fill(eleEta[v_idx_ele_final[1]],weight);
	    
	    h_pT1stJet->Fill(caloJetIC5Pt[v_idx_jet_final[0]],weight);
	    h_pT2ndJet->Fill(caloJetIC5Pt[v_idx_jet_final[1]],weight);
	    
	    h_Eta1stJet->Fill(caloJetIC5Eta[v_idx_jet_final[0]],weight);
	    h_Eta2ndJet->Fill(caloJetIC5Eta[v_idx_jet_final[1]],weight);

	    h_St->Fill(St,weight);

	    h_pThat_sel->Fill(pthat,weight);

	  }

      }

  }// end loop over events


  //Write histograms
  h_pThat_unweight->Write();
  h_pThat_weight->Write();
  h_pThat_sel->Write();

  h_nEleID->Write();
  h_nEleFinal->Write();

  h_pT1stEle->Write();
  h_pT2ndEle->Write();
  h_Eta1stEle->Write();
  h_Eta2ndEle->Write();

  h_nJetFinal->Write();
  //h_nJetFinal_pTcut->Write();

  h_pT1stJet->Write();
  h_pT2ndJet->Write();
  h_Eta1stJet->Write();
  h_Eta2ndJet->Write();

  h_St->Write();

  h_Mee->Write();
  h_Mej_allComb->Write();
  h_Mej_allComb_Meecut->Write();
  h_Mej_allComb_Meecut_Stcut->Write();
  h_Mej_best->Write();
  h_Mej_best_Meecut->Write();
  h_Mej_best_Meecut_Stcut->Write();
  h_Mej_wrong->Write();
  h_2d_Mej_best_vs_wrong->Write();

  h_2d_Mej_allComb_Meecut_vs_St->Write();
  h_2d_Mej_best_Meecut_vs_St->Write();
  
}
