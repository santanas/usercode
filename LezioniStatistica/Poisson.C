{

  gStyle->SetOptStat(0);

  float Lambda=3;

  float mean = Lambda;
  float sigma = sqrt(Lambda);

  TCanvas c1;

  TH1F *h1_poisson = new TH1F("h1_poisson","h1_poisson",(int(Lambda+7*sigma-TMath::Max(float(-0.5),float(Lambda-7*sigma)))+1)*10,TMath::Max(float(-0.5),float(Lambda-7*sigma)),Lambda+7*sigma);  
  for(int k=0; k<=Lambda*30*sigma; k++)
    {
      h1_poisson->Fill(k,TMath::Poisson(k,Lambda));
    }
  h1_poisson->GetXaxis()->SetTitle("n = numero di conteggi");
  h1_poisson->GetXaxis()->SetRangeUser(mean-5*sigma,mean+5*sigma);
  h1_poisson->GetYaxis()->SetTitle("Probabilita' (poissoniana) o densita' di probabilita' (gaussiana)");
  h1_poisson->GetYaxis()->SetRangeUser(0,h1_poisson->GetMaximum()*1.3);
  h1_poisson->SetTitle("");
  h1_poisson->SetFillColor(1);
  h1_poisson->Draw("HIST");

  TF1 *f_g_gaus = new TF1("f_g_gaus","gaus(0)",mean-5*sigma,mean+5*sigma);
  f_g_gaus->SetParameter(0,1/(sqrt(2*3.14)*sigma));
  f_g_gaus->SetParameter(1,mean);
  f_g_gaus->SetParameter(2,sigma);
  f_g_gaus->Draw("same");

  TLegend *legend2 = new TLegend(0.48,0.77,0.97,0.95);
  legend2->AddEntry(h1_poisson,Form("Poisson, #lambda=%3.1f",Lambda),"f");
  legend2->AddEntry(f_g_gaus,Form("Gaussiana, #mu=#lambda=%3.1f, #sigma=#sqrt{#lambda}=%3.1f",mean,sigma),"l");
  legend2->Draw();  

  c1.SaveAs(Form("Poisson_Lambda%3.1f.pdf",Lambda));

}
