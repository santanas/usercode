// Author: Francesco Santanastasio 09/11/2017
// Description: Create root tree from csv file

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
   
void makeTree()
{
  //---------
  
  ifstream infile ( "data/SANTANASTASIO_RICHIESTA1_TRIENNALI_210717_mod.csv" ); string delimiter = ",";  
  TString outfilename = "Richiesta1_Triennali_Update210717.root";

  //Per modificare un csv/excel:
  // 1) parti da excel ed aggiungi nell'ultima colonna -9 in tutti i campi vuoti, quindi create csv
  // 2) replace ^M with endline
  // vi
  // :
  // %s/^M/\r/g
  // 3) rimuovi prima riga con nomi colonne e rimuovi ultima riga vuota
  // 4) replace ,, con ,-9, (ripetere piu' volte per casi come ",,,,,")
  // 5) replace , , con ,-9, (ripetere piu' volte)


  //---------

  //MATRICOLA, ANNO_ACCADEMICO_IMMATRICOLAZIONE, CODICE_CORSO, ANNO_ACCADEMICO_ULTIMA_ISCRIZIONE,STATO_LAUREA,PUNTEGGIO_TEST_DI_INGRESSO,VOTO_LAUREA,SESSO,ANNO_NASCITA,TIPOLOGIA_DIPLOMA,VOTO_DIPLOMA,PROVINCIA_ISTITUTO_DIPLOMA,REGIONE_ISTITUTO_DIPLOMA,CFU_SOSTENUTI,MEDIA_DEI_VOTI_PESATA_PER_I_CFU

  //OLD: Matricola,Anno  Immatricolazione, Codice Del corso,Anno Ultima Iscrizione,"Stato (A: abbandono L:laureato T:trasferito)",Punteggio test,Voto laurea,Sesso,Anno di Nascita,Tipologia Diploma,Voto Diploma,Provincia Istituto superiore,Regione Istituto superiore,Cfu sostenuti,Media ponderata

  int          Matricola=-999;
  int          AnnoImmatr=-999; //se AA = 2014/2015 questo numero e' 2015
  int          Corso=-999;
  int          AnnoUltimaIscr=-999;
  char         StatoOld[20]="";
  char         StatoNew[20]="";
  int          VotoTest=-999;
  int          VotoLaurea=-999;
  char         Sesso[10]="";
  int          AnnoNascita=-999;
  char         TipoDiploma[300]="";
  int          VotoDiploma=-999;
  char         ProvinciaIstituto[300]="";
  char         RegioneIstituto[300]="";
  int          CFU=-999;
  float        MediaPesataVoti=-999.;

  TTree tree("tree","dati studenti");
  tree.Branch("Matricola",&Matricola,"Matricola/I");//0  
  tree.Branch("AnnoImmatr",&AnnoImmatr,"AnnoImmatr/I");//1
  tree.Branch("Corso",&Corso,"Corso/I");//2
  tree.Branch("AnnoUltimaIscr",&AnnoUltimaIscr,"AnnoUltimaIscr/I");//3
  tree.Branch("StatoOld",&StatoOld,"StatoOld/C");//4
  tree.Branch("VotoTest",&VotoTest,"VotoTest/I");//5
  tree.Branch("VotoLaurea",&VotoLaurea,"VotoLaurea/I");//6
  tree.Branch("Sesso",&Sesso,"Sesso/C");//7
  tree.Branch("AnnoNascita",&AnnoNascita,"AnnoNascita/I");//8
  tree.Branch("TipoDiploma",&TipoDiploma,"TipoDiploma/C");//9
  tree.Branch("VotoDiploma",&VotoDiploma,"VotoDiploma/I");//10
  tree.Branch("ProvinciaIstituto",&ProvinciaIstituto,"ProvinciaIstituto/C");//11
  tree.Branch("RegioneIstituto",&RegioneIstituto,"RegioneIstituto/C");//12
  tree.Branch("CFU",&CFU,"CFU/I");//13
  tree.Branch("MediaPesataVoti",&MediaPesataVoti,"MediaPesataVoti/F");//14

  tree.Branch("StatoNew",&StatoNew,"StatoNew/C");

  //---------

  string line;  
  TString line1;  
  int counter=0;
  while ( infile.good() )
    {     
      getline(infile, line);      
      line1 = TString(line.c_str());
      TObjArray *splitLine = line1.Tokenize(',');
      int N_items = splitLine->GetEntries();
      if(N_items ==15)
	{
	  /*
	  cout << "============ Next student ============= " << endl ;
	  cout << counter+1 << " : " << line1 << endl;
	  cout << N_items << endl;
	  cout << endl;
	  */

	  for (Int_t i = 0; i < splitLine->GetEntries(); i++) 
	    {
	      TString value;
	      value = ((TObjString *)(splitLine->At(i)))->String();
	      //std::cout << value << std::endl;
	      if(i==0) Matricola = value.Atoi();
	      if(i==1) AnnoImmatr = value.Atoi();
	      if(i==2) Corso = value.Atoi();
	      if(i==3) AnnoUltimaIscr = value.Atoi();
	      if(i==4) strcpy(StatoOld, value.Data());
	      if(i==5) 
		{
		  if(value.Atoi()>25)
		    VotoTest = value.Atoi()*(25/30);
		  else
		    VotoTest = value.Atoi();
		}
	      if(i==6) VotoLaurea = value.Atoi();
	      if(i==7) strcpy(Sesso, value.Data());
	      if(i==8) AnnoNascita = value.Atoi();
	      if(i==9) strcpy(TipoDiploma, value.Data());
	      if(i==10) VotoDiploma = value.Atoi();
	      if(i==11) strcpy(ProvinciaIstituto, value.Data());
	      if(i==12) strcpy(RegioneIstituto, value.Data());
	      if(i==13) CFU = value.Atoi();
	      if(i==14) MediaPesataVoti = value.Atof();
	    }


	  //-- Edit "Stato"

	  //Anno di laurea
	  int annoCoorte=AnnoImmatr;
	  int annoLaureaAtteso=AnnoImmatr+2;
	  int annoCorrente=2017;

	  //if( (StatoOld=="L" || (StatoOld=="-1" && VotoLaurea>0) ) && CFU>=180 )
	  if( VotoLaurea>0 && AnnoUltimaIscr==annoLaureaAtteso )
	    {
	      strcpy(StatoNew, "LC"); //laureato in corso
	    } 
	  else if( VotoLaurea>0 && AnnoUltimaIscr>annoLaureaAtteso )
	    {
	      strcpy(StatoNew, "LF"); //laureato fuori corso
	    } 
	  else if( strcmp(StatoOld,"ABBANDONO")==0 
		   || strcmp(StatoOld,"TRASFERIMENTO IN USCITA")==0 
		   || ( strcmp(StatoOld,"-9")==0 && AnnoUltimaIscr<annoCorrente )
		   ) 
	    {
	      if(AnnoUltimaIscr==annoCoorte)
		{
		  strcpy(StatoNew, "A1"); //abbandono primo anno
		} 
	      else 
		{
		  strcpy(StatoNew, "A2"); //abbandono dal secondo anno in poi
		}
	    } 
	  else 
	    { 
	      strcpy(StatoNew, "FC"); //fuori corso, non laureato 
	    }

	  tree.Fill(); 
	  counter++;
	}// require 15 items for each line
    }//loop over lines
  tree.Print();
     
  TFile outfile(outfilename,"recreate");  
  tree.Write();
  outfile.Close();  

}

int  main ()
{
  makeTree();
  return 0;
}
