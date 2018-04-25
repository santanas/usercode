{

  gStyle->SetOptStat(0);

  /*
  gRandom->SetSeed(0);
  TRandom Ran;
  Ran.SetSeed(10);
  const int n = 1000000; // n measurements  
  */

  int N=48;
  float p=0.25;

  float mean = N*p;
  float sigma = sqrt(N*p*(1-p));

  /*
  float h_binomial[n];
  for(int k=0; k<n;k++)
    {
      h_binomial[k]=Ran.Binomial(N,p);
    }
  */

  TCanvas c1;

  TH1F *h1_binomial = new TH1F("h1_binomial","h1_binomial",(N+1)*10,-0.5,N+0.5);  
  for(int k=0; k<=N; k++)
    {
      h1_binomial->Fill(k,TMath::Binomial(N,k)*TMath::Power(p,k)*TMath::Power(1-p,N-k));
    }
  h1_binomial->GetXaxis()->SetTitle("n = numero di successi in N prove");
  h1_binomial->GetXaxis()->SetRangeUser(mean-5*sigma,mean+5*sigma);
  h1_binomial->GetYaxis()->SetTitle("Probabilita' (binomiale) o densita' di probabilita' (gaussiana)");
  h1_binomial->GetYaxis()->SetRangeUser(0,h1_binomial->GetMaximum()*1.3);
  h1_binomial->SetTitle("");
  h1_binomial->SetFillColor(1);
  h1_binomial->Draw("HIST");

  TF1 *f_g_gaus = new TF1("f_g_gaus","gaus(0)",mean-5*sigma,mean+5*sigma);
  f_g_gaus->SetParameter(0,1/(sqrt(2*3.14)*sigma));
  f_g_gaus->SetParameter(1,mean);
  f_g_gaus->SetParameter(2,sigma);
  f_g_gaus->Draw("same");

  TLegend *legend2 = new TLegend(0.48,0.77,0.97,0.95);
  legend2->AddEntry(h1_binomial,Form("Binomiale, N=%d, p=%3.2f",N,p),"f");
  legend2->AddEntry(f_g_gaus,Form("Gaussiana, #mu=Np=%3.1f, #sigma=#sqrt{Np(1-p)}=%3.1f",mean,sigma),"l");
  legend2->Draw();  

  c1.SaveAs(Form("Binomiale_N%d_p%3.2f.pdf",N,p));

}
