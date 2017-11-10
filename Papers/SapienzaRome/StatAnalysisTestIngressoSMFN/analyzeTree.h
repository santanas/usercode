//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb  7 17:06:02 2017 by ROOT version 5.34/19
// from TTree tree/dati studenti
// found on file: Fisica2013.root
//////////////////////////////////////////////////////////

//#ifndef analyzeTree_h
//#define analyzeTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class analyzeTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           Matricola;
   Int_t           AnnoImmatr;
   Int_t           Corso;
   Int_t           AnnoUltimaIscr;
   Char_t          StatoOld[10];
   Int_t           VotoTest;
   Int_t           VotoLaurea;
   Char_t          Sesso[2];
   Int_t           AnnoNascita;
   Char_t          TipoDiploma[300];
   Int_t           VotoDiploma;
   Char_t          ProvinciaIstituto[300];
   Char_t          RegioneIstituto[300];
   Int_t           CFU;
   Float_t         MediaPesataVoti;
   Char_t          StatoNew[10];

   // List of branches
   TBranch        *b_Matricola;   //!
   TBranch        *b_AnnoImmatr;   //!
   TBranch        *b_Corso;   //!
   TBranch        *b_AnnoUltimaIscr;   //!
   TBranch        *b_StatoOld;   //!
   TBranch        *b_VotoTest;   //!
   TBranch        *b_VotoLaurea;   //!
   TBranch        *b_Sesso;   //!
   TBranch        *b_AnnoNascita;   //!
   TBranch        *b_TipoDiploma;   //!
   TBranch        *b_VotoDiploma;   //!
   TBranch        *b_ProvinciaIstituto;   //!
   TBranch        *b_RegioneIstituto;   //!
   TBranch        *b_CFU;   //!
   TBranch        *b_MediaPesataVoti;   //!
   TBranch        *b_StatoNew;   //!

   analyzeTree(TTree *tree=0);
   virtual ~analyzeTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

//#endif

