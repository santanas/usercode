
//#ifdef analyzeTree_cxx

#include <analyzeTree.h>

#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"

using namespace std;

analyzeTree::analyzeTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Richiesta1_Triennali_Update210717.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Richiesta1_Triennali_Update210717.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

analyzeTree::~analyzeTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t analyzeTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t analyzeTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void analyzeTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Matricola", &Matricola, &b_Matricola);
   fChain->SetBranchAddress("AnnoImmatr", &AnnoImmatr, &b_AnnoImmatr);
   fChain->SetBranchAddress("Corso", &Corso, &b_Corso);
   fChain->SetBranchAddress("AnnoUltimaIscr", &AnnoUltimaIscr, &b_AnnoUltimaIscr);
   fChain->SetBranchAddress("StatoOld", StatoOld, &b_StatoOld);
   fChain->SetBranchAddress("VotoTest", &VotoTest, &b_VotoTest);
   fChain->SetBranchAddress("VotoLaurea", &VotoLaurea, &b_VotoLaurea);
   fChain->SetBranchAddress("Sesso", Sesso, &b_Sesso);
   fChain->SetBranchAddress("AnnoNascita", &AnnoNascita, &b_AnnoNascita);
   fChain->SetBranchAddress("TipoDiploma", TipoDiploma, &b_TipoDiploma);
   fChain->SetBranchAddress("VotoDiploma", &VotoDiploma, &b_VotoDiploma);
   fChain->SetBranchAddress("ProvinciaIstituto", ProvinciaIstituto, &b_ProvinciaIstituto);
   fChain->SetBranchAddress("RegioneIstituto", RegioneIstituto, &b_RegioneIstituto);
   fChain->SetBranchAddress("CFU", &CFU, &b_CFU);
   fChain->SetBranchAddress("MediaPesataVoti", &MediaPesataVoti, &b_MediaPesataVoti);
   fChain->SetBranchAddress("StatoNew", StatoNew, &b_StatoNew);
   Notify();
}

Bool_t analyzeTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void analyzeTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t analyzeTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void analyzeTree::Loop()
{
  //Book histograms

  //Media vs Voto Test
  const int N_bins_MediaVotiEsami=16; 
  double bins_MediaVotiEsami[N_bins_MediaVotiEsami+1]={0,1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31}; 
  //  const int N_bins_VotoTest=13; 
  //  double bins_VotoTest[N_bins_VotoTest+1]={0,2,4,6,8,10,12,14,16,18,20,22,24,26}; 
  const int N_bins_VotoTest=8; 
  double bins_VotoTest[N_bins_VotoTest+1]={0,5,8,11,14,17,20,23,26}; 
  //const int N_bins_VotoTest=5; 
  //double bins_VotoTest[N_bins_VotoTest+1]={0,5,10,15,20,26}; 
  TH2F *h2_MediaVotiEsami_vs_VotoTest = new TH2F("h2_MediaVotiEsami_vs_VotoTest","",N_bins_VotoTest,bins_VotoTest,N_bins_MediaVotiEsami,bins_MediaVotiEsami);

  TH2F *h2_MediaVotiEsami_vs_VotoCombine_1_to_1 = new TH2F("h2_MediaVotiEsami_vs_VotoCombine_1_to_1","",N_bins_VotoTest,bins_VotoTest,N_bins_MediaVotiEsami,bins_MediaVotiEsami);
  TH2F *h2_MediaVotiEsami_vs_VotoCombine_2_to_1 = new TH2F("h2_MediaVotiEsami_vs_VotoCombine_2_to_1","",N_bins_VotoTest,bins_VotoTest,N_bins_MediaVotiEsami,bins_MediaVotiEsami);
  TH1F *h1_VotoDiploma = new TH1F("h1_VotoDiploma","",42,60,102);
  TH1F *h1_VotoDiploma_rescale = new TH1F("h1_VotoDiploma_rescale","",N_bins_VotoTest,bins_VotoTest);

  TH2F *h2_VotoDiploma_rescale_vs_VotoTest = new TH2F("h2_VotoDiploma_rescale_vs_VotoTest","",N_bins_VotoTest,bins_VotoTest,N_bins_VotoTest,bins_VotoTest);

  //Categories
  TH1F *h_categories = new TH1F("h_categories","h_categories",5,0,5);
  h_categories->GetXaxis()->SetBinLabel(1,"LC");
  h_categories->GetXaxis()->SetBinLabel(2,"LF");
  h_categories->GetXaxis()->SetBinLabel(3,"FC");
  h_categories->GetXaxis()->SetBinLabel(4,"A1");
  h_categories->GetXaxis()->SetBinLabel(5,"A2");

  /*
  const int N_bins_VotoTest2=5; 
  double bins_VotoTest2[N_bins_VotoTest2+1]={0,5,10,15,20,26}; 
  TH2F *h2_categories_vs_VotoTest = new TH2F("h2_categories_vs_VotoTest","",N_bins_VotoTest2,bins_VotoTest2,5,0,5);
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(1,"LC");
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(2,"LF");
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(3,"FC");
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(4,"A1");
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(5,"A2");
  */

  //Categories vs test

  TH2F *h2_categories_vs_VotoTest = new TH2F("h2_categories_vs_VotoTest","",N_bins_VotoTest,bins_VotoTest,3,0,3);
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(1,"L");
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(2,"FC");
  h2_categories_vs_VotoTest->GetYaxis()->SetBinLabel(3,"A");

  TH1F *h_VotoTest_LC = new TH1F("h_VotoTest_LC","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoTest_LF = new TH1F("h_VotoTest_LF","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoTest_FC = new TH1F("h_VotoTest_FC","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoTest_A1 = new TH1F("h_VotoTest_A1","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoTest_A2 = new TH1F("h_VotoTest_A2","",N_bins_VotoTest,bins_VotoTest);

  //Categories vs test-diploma combined

  TH2F *h2_categories_vs_VotoCombine_2_to_1 = new TH2F("h2_categories_vs_VotoCombine_2_to_1","",N_bins_VotoTest,bins_VotoTest,3,0,3);
  h2_categories_vs_VotoCombine_2_to_1->GetYaxis()->SetBinLabel(1,"L");
  h2_categories_vs_VotoCombine_2_to_1->GetYaxis()->SetBinLabel(2,"FC");
  h2_categories_vs_VotoCombine_2_to_1->GetYaxis()->SetBinLabel(3,"A");

  TH1F *h_VotoCombine_2_to_1_LC = new TH1F("h_VotoCombine_2_to_1_LC","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_2_to_1_LF = new TH1F("h_VotoCombine_2_to_1_LF","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_2_to_1_FC = new TH1F("h_VotoCombine_2_to_1_FC","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_2_to_1_A1 = new TH1F("h_VotoCombine_2_to_1_A1","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_2_to_1_A2 = new TH1F("h_VotoCombine_2_to_1_A2","",N_bins_VotoTest,bins_VotoTest);

  TH2F *h2_categories_vs_VotoCombine_1_to_1 = new TH2F("h2_categories_vs_VotoCombine_1_to_1","",N_bins_VotoTest,bins_VotoTest,3,0,3);
  h2_categories_vs_VotoCombine_1_to_1->GetYaxis()->SetBinLabel(1,"L");
  h2_categories_vs_VotoCombine_1_to_1->GetYaxis()->SetBinLabel(2,"FC");
  h2_categories_vs_VotoCombine_1_to_1->GetYaxis()->SetBinLabel(3,"A");

  TH1F *h_VotoCombine_1_to_1_LC = new TH1F("h_VotoCombine_1_to_1_LC","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_1_to_1_LF = new TH1F("h_VotoCombine_1_to_1_LF","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_1_to_1_FC = new TH1F("h_VotoCombine_1_to_1_FC","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_1_to_1_A1 = new TH1F("h_VotoCombine_1_to_1_A1","",N_bins_VotoTest,bins_VotoTest);
  TH1F *h_VotoCombine_1_to_1_A2 = new TH1F("h_VotoCombine_1_to_1_A2","",N_bins_VotoTest,bins_VotoTest);


  //Loop over events

  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    //write your code for each event here:

    if(VotoTest>0 && VotoDiploma>=60)
      {
	//diploma
 	float VotoDiploma_rescale = 0.60975*VotoDiploma - 36.585;
	float Voto_combine_1_to_1 = (VotoTest * 1 + VotoDiploma_rescale * 1) / 2;
	float Voto_combine_2_to_1 = (VotoTest * 2 + VotoDiploma_rescale * 1) / 3;
	//

	h2_MediaVotiEsami_vs_VotoTest->Fill(VotoTest,MediaPesataVoti);	
	h2_VotoDiploma_rescale_vs_VotoTest->Fill(VotoTest,VotoDiploma_rescale);	
	h_categories->Fill(StatoNew,1);	
	//h2_categories_vs_VotoTest->Fill(VotoTest,StatoNew,1);

	h1_VotoDiploma->Fill(VotoDiploma);
	h1_VotoDiploma_rescale->Fill(VotoDiploma_rescale);
	h2_MediaVotiEsami_vs_VotoCombine_1_to_1->Fill(Voto_combine_1_to_1,MediaPesataVoti);	
	h2_MediaVotiEsami_vs_VotoCombine_2_to_1->Fill(Voto_combine_2_to_1,MediaPesataVoti);

	if(strcmp(StatoNew,"LC")==0 || strcmp(StatoNew,"LF")==0)
	  {
	    h2_categories_vs_VotoTest->Fill(VotoTest,"L",1);
	    h2_categories_vs_VotoCombine_1_to_1->Fill(Voto_combine_1_to_1,"L",1);
	    h2_categories_vs_VotoCombine_2_to_1->Fill(Voto_combine_2_to_1,"L",1);
	  }
	if(strcmp(StatoNew,"FC")==0)
	  {
	    h2_categories_vs_VotoTest->Fill(VotoTest,"FC",1);
	    h2_categories_vs_VotoCombine_1_to_1->Fill(Voto_combine_1_to_1,"FC",1);
	    h2_categories_vs_VotoCombine_2_to_1->Fill(Voto_combine_2_to_1,"FC",1);
	  }
	if(strcmp(StatoNew,"A1")==0 || strcmp(StatoNew,"A2")==0)
	  {
	    h2_categories_vs_VotoTest->Fill(VotoTest,"A",1);
	    h2_categories_vs_VotoCombine_1_to_1->Fill(Voto_combine_1_to_1,"A",1);
	    h2_categories_vs_VotoCombine_2_to_1->Fill(Voto_combine_2_to_1,"A",1);	    
	  }

	if(strcmp(StatoNew,"LC")==0)
	  {
	    h_VotoTest_LC->Fill(VotoTest);	
	    h_VotoCombine_1_to_1_LC->Fill(Voto_combine_1_to_1);	
	    h_VotoCombine_2_to_1_LC->Fill(Voto_combine_2_to_1);	
	  }
	if(strcmp(StatoNew,"LF")==0)
	  {
	    h_VotoTest_LF->Fill(VotoTest);	
	    h_VotoCombine_1_to_1_LF->Fill(Voto_combine_1_to_1);	
	    h_VotoCombine_2_to_1_LF->Fill(Voto_combine_2_to_1);		    
	  }
	if(strcmp(StatoNew,"FC")==0)
	  {
	    h_VotoTest_FC->Fill(VotoTest);	
	    h_VotoCombine_1_to_1_FC->Fill(Voto_combine_1_to_1);	
	    h_VotoCombine_2_to_1_FC->Fill(Voto_combine_2_to_1);		    	    
	  }
	if(strcmp(StatoNew,"A1")==0)
	  {
	    h_VotoTest_A1->Fill(VotoTest);	
	    h_VotoCombine_1_to_1_A1->Fill(Voto_combine_1_to_1);	
	    h_VotoCombine_2_to_1_A1->Fill(Voto_combine_2_to_1);		    	    
	  }
	if(strcmp(StatoNew,"A2")==0)
	  {
	    h_VotoTest_A2->Fill(VotoTest);	
	    h_VotoCombine_1_to_1_A2->Fill(Voto_combine_1_to_1);	
	    h_VotoCombine_2_to_1_A2->Fill(Voto_combine_2_to_1);		    	    
	  }
		
      }

  }// end loop over events

  //Probability
  TH2F *h2_categories_vs_VotoTest_prob = (TH2F*)h2_categories_vs_VotoTest->Clone();
  h2_categories_vs_VotoTest_prob->SetName("h2_categories_vs_VotoTest_prob");
  h2_categories_vs_VotoTest_prob->SetTitle("");
  h2_categories_vs_VotoTest_prob->GetZaxis()->SetRangeUser(0,100);
  for(int x=1;x<=h2_categories_vs_VotoTest_prob->GetNbinsX();x++)
    {
      //      int integral_X = h2_categories_vs_VotoTest_prob->Integral(x,x,1,5);
      int integral_X = h2_categories_vs_VotoTest_prob->Integral(x,x,1,3);
      cout << integral_X << endl;
      
      for (int y=1; y<=h2_categories_vs_VotoTest_prob->GetNbinsY();y++)
	{
	  if(integral_X>0)
	    {
	      float prob = float(100*h2_categories_vs_VotoTest_prob->GetBinContent(x,y))/float(integral_X);
	      h2_categories_vs_VotoTest_prob->SetBinContent(x,y,prob);
	    }
	}
    }

  TH2F *h2_categories_vs_VotoCombine_1_to_1_prob = (TH2F*)h2_categories_vs_VotoCombine_1_to_1->Clone();
  h2_categories_vs_VotoCombine_1_to_1_prob->SetName("h2_categories_vs_VotoCombine_1_to_1_prob");
  h2_categories_vs_VotoCombine_1_to_1_prob->SetTitle("");
  h2_categories_vs_VotoCombine_1_to_1_prob->GetZaxis()->SetRangeUser(0,100);
  for(int x=1;x<=h2_categories_vs_VotoCombine_1_to_1_prob->GetNbinsX();x++)
    {
      //      int integral_X = h2_categories_vs_VotoCombine_1_to_1_prob->Integral(x,x,1,5);
      int integral_X = h2_categories_vs_VotoCombine_1_to_1_prob->Integral(x,x,1,3);
      
      for (int y=1; y<=h2_categories_vs_VotoCombine_1_to_1_prob->GetNbinsY();y++)
	{
	  if(integral_X>0)
	    {
	      float prob = float(100*h2_categories_vs_VotoCombine_1_to_1_prob->GetBinContent(x,y))/float(integral_X);
	      h2_categories_vs_VotoCombine_1_to_1_prob->SetBinContent(x,y,prob);
	    }
	}
    }

  TH2F *h2_categories_vs_VotoCombine_2_to_1_prob = (TH2F*)h2_categories_vs_VotoCombine_2_to_1->Clone();
  h2_categories_vs_VotoCombine_2_to_1_prob->SetName("h2_categories_vs_VotoCombine_2_to_1_prob");
  h2_categories_vs_VotoCombine_2_to_1_prob->SetTitle("");
  h2_categories_vs_VotoCombine_2_to_1_prob->GetZaxis()->SetRangeUser(0,100);
  for(int x=1;x<=h2_categories_vs_VotoCombine_2_to_1_prob->GetNbinsX();x++)
    {
      //      int integral_X = h2_categories_vs_VotoCombine_2_to_1_prob->Integral(x,x,1,5);
      int integral_X = h2_categories_vs_VotoCombine_2_to_1_prob->Integral(x,x,1,3);
      
      for (int y=1; y<=h2_categories_vs_VotoCombine_2_to_1_prob->GetNbinsY();y++)
	{
	  if(integral_X>0)
	    {
	      float prob = float(100*h2_categories_vs_VotoCombine_2_to_1_prob->GetBinContent(x,y))/float(integral_X);
	      h2_categories_vs_VotoCombine_2_to_1_prob->SetBinContent(x,y,prob);
	    }
	}
    }



  //write tree
  TFile outfile("analyzeTree.root","recreate");  
  h_categories->Write();

  h1_VotoDiploma->Write();
  h1_VotoDiploma_rescale->Write();

  h2_MediaVotiEsami_vs_VotoTest->Write();
  h2_VotoDiploma_rescale_vs_VotoTest->Write();
  h2_MediaVotiEsami_vs_VotoCombine_1_to_1->Write();
  h2_MediaVotiEsami_vs_VotoCombine_2_to_1->Write();

  h2_categories_vs_VotoTest->Write();
  h2_categories_vs_VotoTest_prob->Write();
  h_VotoTest_LC->Write();
  h_VotoTest_LF->Write();
  h_VotoTest_FC->Write();
  h_VotoTest_A1->Write();
  h_VotoTest_A2->Write();

  h2_categories_vs_VotoCombine_1_to_1->Write();
  h2_categories_vs_VotoCombine_1_to_1_prob->Write();
  h_VotoCombine_1_to_1_LC->Write();
  h_VotoCombine_1_to_1_LF->Write();
  h_VotoCombine_1_to_1_FC->Write();
  h_VotoCombine_1_to_1_A1->Write();
  h_VotoCombine_1_to_1_A2->Write();

  h2_categories_vs_VotoCombine_2_to_1->Write();
  h2_categories_vs_VotoCombine_2_to_1_prob->Write();
  h_VotoCombine_2_to_1_LC->Write();
  h_VotoCombine_2_to_1_LF->Write();
  h_VotoCombine_2_to_1_FC->Write();
  h_VotoCombine_2_to_1_A1->Write();
  h_VotoCombine_2_to_1_A2->Write();

  outfile.Close();

  //delete objects
  delete h_categories;
  delete h1_VotoDiploma;
  delete h1_VotoDiploma_rescale;

  delete h2_MediaVotiEsami_vs_VotoTest;
  delete h2_VotoDiploma_rescale_vs_VotoTest;
  delete h2_MediaVotiEsami_vs_VotoCombine_1_to_1;
  delete h2_MediaVotiEsami_vs_VotoCombine_2_to_1;

  delete h2_categories_vs_VotoTest;
  delete h2_categories_vs_VotoTest_prob;
  delete h_VotoTest_LC;
  delete h_VotoTest_LF;
  delete h_VotoTest_FC;
  delete h_VotoTest_A1;
  delete h_VotoTest_A2;

  delete h2_categories_vs_VotoCombine_1_to_1;
  delete h2_categories_vs_VotoCombine_1_to_1_prob;
  delete h_VotoCombine_1_to_1_LC;
  delete h_VotoCombine_1_to_1_LF;
  delete h_VotoCombine_1_to_1_FC;
  delete h_VotoCombine_1_to_1_A1;
  delete h_VotoCombine_1_to_1_A2;

  delete h2_categories_vs_VotoCombine_2_to_1;
  delete h2_categories_vs_VotoCombine_2_to_1_prob;
  delete h_VotoCombine_2_to_1_LC;
  delete h_VotoCombine_2_to_1_LF;
  delete h_VotoCombine_2_to_1_FC;
  delete h_VotoCombine_2_to_1_A1;
  delete h_VotoCombine_2_to_1_A2;
}

//#endif // #ifdef analyzeTree_cxx

int main ()
{
  analyzeTree t;
  t.Loop();
  return 0;
}


