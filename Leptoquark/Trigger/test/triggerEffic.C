{

  FILE *dataFile; 
  string inputFile = "triggerEffic__.dat";
  //inputFile=dcuHome+inputFile;
  dataFile = fopen(inputFile.c_str(),"r");
  if(!dataFile) {
    cout<<"Cannot open file "<< inputFile.c_str() <<endl;
    return -1;
  }  
  char line1[255], line2[255];  
  int ii=0;
  float f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10;   
  Double_t mass[20],massm2[20],massm1[20],massp1[20],massp2[20]; 
  Double_t massE[20]; 
  Double_t Rel[20]; 
  Double_t RelE[20]; 
  Double_t High[20]; 
  Double_t HighE[20]; 
  Double_t VHigh[20]; 
  Double_t VHighE[20];  
  Double_t HighOrVHigh[20]	;
  Double_t HighOrVHighE[20]	;
  Double_t HighOrVHighOrRel[20] ;
  Double_t HighOrVHighOrRelE[20];  
  
  fgets(line1,255,dataFile);  // skip header line

  while(  fgets(line1,62,dataFile) && fgets(line2,255,dataFile)) {
	//cout << line1 <<" - "<<line2<<" - " << line3 << endl;
    sscanf(&line1     ,"%f %f %f %f %f %f %f %f %f %f %f",&f0, &f1, &f2, &f3, &f4, &f5, &f6);
    sscanf(line2,"   %f %f %f %f ",       &f7, &f8, &f9,&f10);
    mass[ii]    =f0; massm2[ii]=f0-10; massm1[ii]=f0-5; massp1[ii]=f0+5; massp2[ii]=f0+10;
    Rel[ii]     =f1;
    RelE[ii]    =f2;
    High[ii]    =f3;
    HighE[ii]   =f4;
    VHigh[ii]   =f5;
    VHighE[ii]  =f6;
    HighOrVHigh[ii]        =f7;
    HighOrVHighE[ii]       =f8;
    HighOrVHighOrRel[ii]   =f9;
    HighOrVHighOrRelE[ii]  =f10;
        cout <<" ii=" << ii
    	 <<" mass   " << mass[ii] 
    	 <<" Rel    " << Rel[ii]
    	 <<" RelE   " << RelE[ii]
    	 <<" High   " << High[ii] 
    	 <<" HighE  " << HighE[ii]
    	 <<" VHigh  " << VHigh[ii]
    	 <<" VHighE " << VHighE[ii]
    	 <<" HighOrVHigh       " << HighOrVHigh[ii] 
    	 <<" HighOrVHighE      " << HighOrVHighE[ii]
    	 <<" HighOrVHighOrRel  " << HighOrVHighOrRel[ii]
    	 <<" HighOrVHighOrRelE " << HighOrVHighOrRelE[ii]
	 << endl; 
    ii++;
  }
  
  string TFileStr = "triggerEffic.root";
  TFile triggerEffic(TFileStr.c_str(), "RECREATE", "HLT efficiencies");
  

  TCanvas *c1 = new TCanvas("c1","HLT efficiencies",10,10,1000,700);
  c1->SetGridy();
  gr1 = new TGraphErrors(4,massm2,Rel, massE, RelE);
  gr1-> SetTitle("HLT Efficiencies") ;
  Double_t minY; minY = 0.2;
  Double_t maxY; maxY = 1.0 ;
  gr1-> SetMinimum(minY);
  gr1-> SetMaximum(maxY);
  gr1-> GetXaxis() -> SetTitle("Leptoquark Mass [GeV]");
  gr1-> GetYaxis() -> SetTitle("Efficiency");
  gr1->SetMarkerColor(1);
  gr1->SetLineColor(1);
  gr1->SetMarkerStyle(21);
  gr1->SetMarkerSize(1.5);
  gr1->Draw("AP");

  gr2 = new TGraphErrors(4,massm1,High, massE, HighE);
  gr2-> SetTitle("EM High Et") ;
  gr2->SetMarkerColor(2);
  gr2->SetLineColor(2);
  gr2->SetMarkerStyle(22);
  gr2->SetMarkerSize(1.5);
  gr2->Draw("P");

  gr3 = new TGraphErrors(4,mass,VHigh, massE, VHighE);
  gr3-> SetTitle("EM Very High Et") ;
  gr3->SetMarkerColor(3);
  gr3->SetLineColor(3);
  gr3->SetMarkerStyle(23);
  gr3->SetMarkerSize(1.5);
  gr3->Draw("P");

  gr4 = new TGraphErrors(4,massp1,HighOrVHigh, massE, HighOrVHighE);
  gr4-> SetTitle("EM High OR EM Very High Et") ;
  gr4->SetMarkerColor(4);
  gr4->SetLineColor(4);
  gr4->SetMarkerStyle(24);
  gr4->SetMarkerSize(1.5);
  gr4->Draw("P");

  gr5 = new TGraphErrors(4,massp2,HighOrVHighOrRel, massE, HighOrVHighOrRelE);
  gr5-> SetTitle("EM High OR EM Very High Et") ;
  gr5->SetMarkerColor(6);
  gr5->SetLineColor(5);
  gr5->SetMarkerStyle(26);
  gr5->SetMarkerSize(1.5);
  gr5->Draw("P");

  // legend
  //TLatex *t = new TLatex();
  //t->SetTextSize(0.03);
  //t->SetTextColor(6);t->DrawLatex(700.,0.45*maxY+0.10*minY,"HighEt OR VHighEt OR ElecRel");
  //t->SetTextColor(4);t->DrawLatex(700.,0.40*maxY+0.10*minY,"HighEt OR VHighEt");
  //t->SetTextColor(3);t->DrawLatex(700.,0.35*maxY+0.15*minY,"VeryHighEt");
  //t->SetTextColor(2);t->DrawLatex(700.,0.31*maxY+0.10*minY,"HighEt");
  //t->SetTextColor(1);t->DrawLatex(700.,0.27*maxY+0.05*minY,"ElecRel");


  TLegend* l=new TLegend(0.35,0.15,0.85,0.5);
  l->SetFillColor(kWhite);
  gr1->SetName("gr1");
  gr2->SetName("gr2");
  gr3->SetName("gr3");
  gr4->SetName("gr4");
  gr5->SetName("gr5");
  l->AddEntry("gr5","HighEt OR VeryHighEt OR SingleElecRel","p");
  l->AddEntry("gr4","HighEt OR VeryHighEt","p");
  l->AddEntry("gr3","VeryHighEt","p");
  l->AddEntry("gr2","HighEt","p");
  l->AddEntry("gr1","SingleElecRel","p");
  //l->SetHeader("Gas");
  l->Draw();

  c1->Print("triggerEffic.gif");
  c1->Print("triggerEffic.eps");
}
