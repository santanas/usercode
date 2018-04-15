{

  //gStyle->SetOptStat(0);
  
  gRandom->SetSeed(0);
  TRandom Ran;
  Ran.SetSeed(10);

  //h = 1/2 g t^2
  //g = 2h/t^2
  
  const int n = 1000000; // n measurements

  float g_true = 9.8;//m/s^2 (estimate)
  float h_true = 1.0000;//m
  float t_true = 0.4517;//s
  float sigma_g = 0.2;//m/s^2 (estimate)
  float sigma_h = 0.0050;//m
  float sigma_t = 0.0050;//s

  float binSize_h = sigma_h/10;
  float h_min = h_true - 7*sigma_h;
  float h_max = h_true + 7*sigma_h;
  float binSize_t = sigma_t/10;
  float t_min = t_true - 7*sigma_t;
  float t_max = t_true + 7*sigma_t;
  float binSize_g = sigma_g/10;
  float g_min = g_true - 7*sigma_g;
  float g_max = g_true + 7*sigma_g;

  float h_gaus[n];
  float t_gaus[n];
  float h_uni[n];
  float t_uni[n];
  float g_gaus[n];
  float g_uni[n];

  for(int k=0; k<n;k++)
    {
      h_gaus[k]=Ran.Gaus(h_true,sigma_h);
      t_gaus[k]=Ran.Gaus(t_true,sigma_t);
      g_gaus[k]=2*h_gaus[k]/(t_gaus[k]*t_gaus[k]);

      h_uni[k]=Ran.Uniform(h_true-(sqrt(12)/2)*sigma_h,h_true+(sqrt(12)/2)*sigma_h);
      t_uni[k]=Ran.Uniform(t_true-(sqrt(12)/2)*sigma_t,t_true+(sqrt(12)/2)*sigma_t);
      g_uni[k]=2*h_uni[k]/(t_uni[k]*t_uni[k]);
    }

  // altezza h (gaus)
  TH1F *h1_h_gaus = new TH1F("h1_h_gaus","h1_h_gaus",int((h_max-h_min)/binSize_h),h_min,h_max);  
  TH1F *h1_h_gaus_norm = new TH1F("h1_h_gaus_norm","h1_h_gaus_norm",int((h_max-h_min)/binSize_h),h_min,h_max);  
  h1_h_gaus->Sumw2(); 
  h1_h_gaus->SetTitle("Istogramma dell'altezza h");
  h1_h_gaus->GetXaxis()->SetTitle("h [m]");
  h1_h_gaus->GetYaxis()->SetTitle("Entries");
  h1_h_gaus_norm->Sumw2();
  h1_h_gaus_norm->SetTitle("Distribuzione di probabilita' dell'altezza h");
  h1_h_gaus_norm->GetXaxis()->SetTitle("h [m]");
  h1_h_gaus_norm->GetYaxis()->SetTitle("f(h)");

  for(int k=0; k<n;k++)
    {
      h1_h_gaus->Fill(h_gaus[k]);
      h1_h_gaus_norm->Fill(h_gaus[k]);
    }

  h1_h_gaus_norm->Scale(1/(h1_h_gaus_norm->Integral()*binSize_h));

  TCanvas c1;
  h1_h_gaus->Draw("HIST");
  TCanvas c2;
  h1_h_gaus_norm->Draw("HIST");

  c1.SaveAs("h1_h_gaus.pdf");
  c2.SaveAs("h1_h_gaus_norm.pdf");

  // tempo t (gaus)
  TH1F *h1_t_gaus = new TH1F("h1_t_gaus","h1_t_gaus",int((t_max-t_min)/binSize_t),t_min,t_max);  
  TH1F *h1_t_gaus_norm = new TH1F("h1_t_gaus_norm","h1_t_gaus_norm",int((t_max-t_min)/binSize_t),t_min,t_max);  
  h1_t_gaus->Sumw2(); 
  h1_t_gaus->SetTitle("Istogramma del tempo t");
  h1_t_gaus->GetXaxis()->SetTitle("t [s]");
  h1_t_gaus->GetYaxis()->SetTitle("Entries");
  h1_t_gaus_norm->Sumw2();
  h1_t_gaus_norm->SetTitle("Distribuzione di probabilita' del tempo t");
  h1_t_gaus_norm->GetXaxis()->SetTitle("t [s]");
  h1_t_gaus_norm->GetYaxis()->SetTitle("f(t)");

  for(int k=0; k<n;k++)
    {
      h1_t_gaus->Fill(t_gaus[k]);
      h1_t_gaus_norm->Fill(t_gaus[k]);
    }

  h1_t_gaus_norm->Scale(1/(h1_t_gaus_norm->Integral()*binSize_t));

  TCanvas c3;
  h1_t_gaus->Draw("HIST");
  TCanvas c4;
  h1_t_gaus_norm->Draw("HIST");

  c3.SaveAs("h1_t_gaus.pdf");
  c4.SaveAs("h1_t_gaus_norm.pdf");

  // accelerazione g (gaus)
  TH1F *h1_g_gaus = new TH1F("h1_g_gaus","h1_g_gaus",int((g_max-g_min)/binSize_g),g_min,g_max);  
  TH1F *h1_g_gaus_norm = new TH1F("h1_g_gaus_norm","h1_g_gaus_norm",int((g_max-g_min)/binSize_g),g_min,g_max);  
  h1_g_gaus->Sumw2(); 
  h1_g_gaus->SetTitle("Istogramma dell'accelerazione g = 2h/t^{2}");
  h1_g_gaus->GetXaxis()->SetTitle("g [m/s^2]");
  h1_g_gaus->GetYaxis()->SetTitle("Entries");
  h1_g_gaus_norm->Sumw2();
  h1_g_gaus_norm->SetTitle("Distribuzione di probabilita' dell'accelerazione g  = 2h/t^{2}");
  h1_g_gaus_norm->GetXaxis()->SetTitle("g [m/s^2]");
  h1_g_gaus_norm->GetYaxis()->SetTitle("f(g)");

  for(int k=0; k<n;k++)
    {
      h1_g_gaus->Fill(g_gaus[k]);
      h1_g_gaus_norm->Fill(g_gaus[k]);
    }

  h1_g_gaus_norm->Scale(1/(h1_g_gaus_norm->Integral()*binSize_g));

  TCanvas c5;
  h1_g_gaus->Draw("HIST");
  TCanvas c6;
  h1_g_gaus_norm->Draw("HIST");
  float integral_g_gaus_norm_1sigma = -1;
  integral_g_gaus_norm_1sigma = h1_g_gaus_norm->Integral(
							 h1_g_gaus_norm->GetXaxis()->FindBin(h1_g_gaus_norm->GetMean()-h1_g_gaus_norm->GetRMS()),
							 h1_g_gaus_norm->GetXaxis()->FindBin(h1_g_gaus_norm->GetMean()+h1_g_gaus_norm->GetRMS()),
							 "width");							 
  cout << "integral_g_gaus_norm_1sigma = " << integral_g_gaus_norm_1sigma << endl;

  TF1 *f_g_gaus = new TF1("f_g_gaus","gaus(0)",g_min,g_max);
  f_g_gaus->SetParameter(0,1/(sqrt(2*3.14)*h1_g_gaus_norm->GetRMS()));
  f_g_gaus->SetParameter(1,h1_g_gaus_norm->GetMean());
  f_g_gaus->SetParameter(2,h1_g_gaus_norm->GetRMS());
  f_g_gaus->Draw("same");

  c5.SaveAs("h1_g_gaus.pdf");
  c6.SaveAs("h1_g_gaus_norm.pdf");

  // altezza h (uni)
  TH1F *h1_h_uni = new TH1F("h1_h_uni","h1_h_uni",int((h_max-h_min)/binSize_h),h_min,h_max);  
  TH1F *h1_h_uni_norm = new TH1F("h1_h_uni_norm","h1_h_uni_norm",int((h_max-h_min)/binSize_h),h_min,h_max);  
  h1_h_uni->Sumw2(); 
  h1_h_uni->SetTitle("Istogramma dell'altezza h");
  h1_h_uni->GetXaxis()->SetTitle("h [m]");
  h1_h_uni->GetYaxis()->SetTitle("Entries");
  h1_h_uni_norm->Sumw2();
  h1_h_uni_norm->SetTitle("Distribuzione di probabilita' dell'altezza h");
  h1_h_uni_norm->GetXaxis()->SetTitle("h [m]");
  h1_h_uni_norm->GetYaxis()->SetTitle("f(h)");

  for(int k=0; k<n;k++)
    {
      h1_h_uni->Fill(h_uni[k]);
      h1_h_uni_norm->Fill(h_uni[k]);
    }

  h1_h_uni_norm->Scale(1/(h1_h_uni_norm->Integral()*binSize_h));

  TCanvas c7;
  h1_h_uni->Draw("HIST");
  TCanvas c8;
  h1_h_uni_norm->Draw("HIST");

  c7.SaveAs("h1_h_uni.pdf");
  c8.SaveAs("h1_h_uni_norm.pdf");

  // tempo t (uni)
  TH1F *h1_t_uni = new TH1F("h1_t_uni","h1_t_uni",int((t_max-t_min)/binSize_t),t_min,t_max);  
  TH1F *h1_t_uni_norm = new TH1F("h1_t_uni_norm","h1_t_uni_norm",int((t_max-t_min)/binSize_t),t_min,t_max);  
  h1_t_uni->Sumw2(); 
  h1_t_uni->SetTitle("Istogramma del tempo t");
  h1_t_uni->GetXaxis()->SetTitle("t [s]");
  h1_t_uni->GetYaxis()->SetTitle("Entries");
  h1_t_uni_norm->Sumw2();
  h1_t_uni_norm->SetTitle("Distribuzione di probabilita' del tempo t");
  h1_t_uni_norm->GetXaxis()->SetTitle("t [s]");
  h1_t_uni_norm->GetYaxis()->SetTitle("f(t)");

  for(int k=0; k<n;k++)
    {
      h1_t_uni->Fill(t_uni[k]);
      h1_t_uni_norm->Fill(t_uni[k]);
    }

  h1_t_uni_norm->Scale(1/(h1_t_uni_norm->Integral()*binSize_t));

  TCanvas c9;
  h1_t_uni->Draw("HISTE");
  TCanvas c10;
  h1_t_uni_norm->Draw("HISTE");

  c9.SaveAs("h1_t_uni.pdf");
  c10.SaveAs("h1_t_uni_norm.pdf");

  // accelerazione g (uni)
  TH1F *h1_g_uni = new TH1F("h1_g_uni","h1_g_uni",int((g_max-g_min)/binSize_g),g_min,g_max);  
  TH1F *h1_g_uni_norm = new TH1F("h1_g_uni_norm","h1_g_uni_norm",int((g_max-g_min)/binSize_g),g_min,g_max);  
  h1_g_uni->Sumw2(); 
  h1_g_uni->SetTitle("Istogramma dell'accelerazione g = 2h/t^{2}");
  h1_g_uni->GetXaxis()->SetTitle("g [m/s^2]");
  h1_g_uni->GetYaxis()->SetTitle("Entries");
  h1_g_uni_norm->Sumw2();
  h1_g_uni_norm->SetTitle("Distribuzione di probabilita' dell'accelerazione g = 2h/t^{2}");
  h1_g_uni_norm->GetXaxis()->SetTitle("g [m/s^2]");
  h1_g_uni_norm->GetYaxis()->SetTitle("f(g)");

  for(int k=0; k<n;k++)
    {
      h1_g_uni->Fill(g_uni[k]);
      h1_g_uni_norm->Fill(g_uni[k]);
    }

  h1_g_uni_norm->Scale(1/(h1_g_uni_norm->Integral()*binSize_g));

  TCanvas c11;
  h1_g_uni->Draw("HISTE");
  TCanvas c12;
  h1_g_uni_norm->Draw("HISTE");
  float integral_g_uni_norm_1sigma = -1;
  integral_g_uni_norm_1sigma = h1_g_uni_norm->Integral(
							 h1_g_uni_norm->GetXaxis()->FindBin(h1_g_uni_norm->GetMean()-h1_g_uni_norm->GetRMS()),
							 h1_g_uni_norm->GetXaxis()->FindBin(h1_g_uni_norm->GetMean()+h1_g_uni_norm->GetRMS()),
							 "width");							 
  cout << "integral_g_uni_norm_1sigma = " << integral_g_uni_norm_1sigma << endl;
  h1_g_uni_norm->GetYaxis()->SetRangeUser(0,h1_g_uni_norm->GetMaximum()*1.5);

  TF1 *f_g_uni = new TF1("f_g_uni","gaus(0)",g_min,g_max);
  f_g_uni->SetParameter(0,1/(sqrt(2*3.14)*h1_g_uni_norm->GetRMS()));
  f_g_uni->SetParameter(1,h1_g_uni_norm->GetMean());
  f_g_uni->SetParameter(2,h1_g_uni_norm->GetRMS());
  f_g_uni->Draw("same");

  c11.SaveAs("h1_g_uni.pdf");
  c12.SaveAs("h1_g_uni_norm.pdf");

  
}
