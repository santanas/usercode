{

  //  gStyle->SetOptStat(0);

  const int n = 20;
  double T1[n]={11.3 , 11.6 , 11.4 , 12.1 , 11.1 , 12.2 , 11.5 , 11.8 , 11.7 , 12.0 , 11.1 , 12.1 , 11.9 , 11.4 , 11.7 , 11.6 , 12.0 , 11.4 , 11.7 , 11.3 };
  double T2[n]={11.1 , 10.9 , 11.4 , 11.3 , 10.8 , 11.1 , 11.3 , 11.4 , 11.0 , 11.3 , 11.0 , 11.0 , 11.4 , 10.9 , 11.2 , 11.3 , 11.3 , 11.1 , 11.0 , 11.3 };
  float Tmin = 10.45;
  float Tmax = 12.55;
  float binSize = 0.1; 
  int Nbins = int((Tmax - Tmin) / binSize); 

  TCanvas c1;
  TH1F *h1_T1 = new TH1F("h1_T1","h1_T1",Nbins,Tmin,Tmax);  
  h1_T1->SetTitle("");
  h1_T1->GetXaxis()->SetTitle("Temperatura [C]");
  h1_T1->GetYaxis()->SetTitle("numero di misure = entries");
  TH1F *h1_T2 = new TH1F("h1_T2","h1_T2",Nbins,Tmin,Tmax);  
  h1_T2->SetTitle("");
  h1_T2->SetLineColor(2);
  h1_T2->GetXaxis()->SetTitle("Temperatura [C]");
  h1_T2->GetYaxis()->SetTitle("numero di misure = entries");
  for(int k=0; k<n;k++)
    {
      h1_T1->Fill(T1[k]);
      h1_T2->Fill(T2[k]);
    }
  h1_T1->Draw();
  h1_T1->GetYaxis()->SetRangeUser(0,8);
  h1_T2->Draw("sames");
  binSize = (Tmax - Tmin) / Nbins; 
  TPaveText *pt1=new TPaveText(0.72,0.72,0.95,0.87,"brNDC");
  pt1->AddText(Form("N bins = %d",Nbins));
  pt1->AddText(Form("Bin size = %3.4f [C]",binSize));
  pt1->Draw();
  c1.SaveAs("h_Temperatures.pdf");

  //-------

  const int N = 20;
  double t[N]={6.2 , 6.6 , 6.4 , 6.7 , 6.2 , 6.3 , 5.9 , 6.4 , 6.5 , 6.2 , 6.3 , 6.4 , 6.0 , 6.3 , 6.2 , 6.6 , 6.1 , 6.3 , 6.5 , 7.0 };
  float tmin = 5.85;
  float tmax = 7.05;
  float binSizet = 0.1; 
  int Nbinst = int((tmax - tmin) / binSizet); 

  TCanvas c2;
  TH1F *h1_t = new TH1F("h1_t","h1_t",Nbinst,tmin,tmax);  
  h1_t->SetTitle("");
  h1_t->GetXaxis()->SetTitle("Tempo di caduta [s]");
  h1_t->GetYaxis()->SetTitle("Numero di misure = entries");
  for(int k=0; k<N;k++)
    {
      h1_t->Fill(t[k]);
    }
  h1_t->Draw();
  binSizet = (tmax - tmin) / Nbinst; 
  TPaveText *pt2=new TPaveText(0.72,0.72,0.95,0.87,"brNDC");
  pt2->AddText(Form("N bins = %d",Nbinst));
  pt2->AddText(Form("Bin size = %3.4f [C]",binSizet));
  pt2->Draw();
  c2.SaveAs("h_TempoCaduta.pdf");

}
