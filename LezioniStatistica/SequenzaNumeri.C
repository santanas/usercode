{

  gStyle->SetOptStat(0);

  gRandom->SetSeed(0);
  TRandom Ran;
  Ran.SetSeed(1);

  const int n = 1000; // n measurements
  const int step = 10; // a measurement every 10 seconds
  double smear = 1.;
  double gausRan=0;
  double t[n];
  double var1[n];
  double var2[n];
  double varmax = 28;
  double varmin = 12;

  //=== Graphs

  TF1 *f1 = new TF1("f1","[0]", 0, n*step);
  double var1_par0 = 20; 
  f1->SetParameter(0, var1_par0);

  TF1 *f2 = new TF1("f2","[0]+[1]*x", 0, n*step);
  double var2_par0 = 16; 
  double var2_par1 = 0.0008; 
  f2->SetParameter(0, var2_par0);
  f2->SetParameter(1, var2_par1);

  for(int k=0; k<n;k++)
    {
      //f1
      t[k]=step*k; 
      gausRan = Ran.Gaus(f1->Eval(t[k]),smear);
      var1[k]=gausRan;

      //f2
      gausRan = Ran.Gaus(f2->Eval(t[k]),smear);
      var2[k]=gausRan;
    }

  TCanvas c1;
  c1.SetGridx();
  c1.SetGridy();

  TGraph *g1 = new TGraph(n,t,var1);
  g1->SetTitle("");
  g1->GetYaxis()->SetTitle("variabile [unita' di misura = u]");
  g1->GetXaxis()->SetTitle("tempo [s]");
  g1->GetYaxis()->SetLimits(varmin, varmax);
  g1->GetYaxis()->SetRangeUser(varmin, varmax);
  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(0.5);
  g1->Draw("AP");
  c1.SaveAs("g_var1_vs_time_flat.pdf");

  TCanvas c2;
  c2.SetGridx();
  c2.SetGridy();

  TGraph *g2 = new TGraph(n,t,var2);
  g2->SetTitle("");
  g2->GetYaxis()->SetTitle("variabile [unita' di misura = u]");
  g2->GetXaxis()->SetTitle("tempo [s]");
  g2->GetYaxis()->SetLimits(varmin, varmax);
  g2->GetYaxis()->SetRangeUser(varmin, varmax);
  g2->SetMarkerStyle(20);
  g2->SetMarkerSize(0.5);
  g2->Draw("AP");
  c2.SaveAs("g_var2_vs_time_linear.pdf");

  //=== Histograms

  TCanvas c3;
  int Nbins1 = 4;
  TH1F *h1_largeBins = new TH1F("h1_largeBins","h1_largeBins",Nbins1,varmin,varmax);  
  h1_largeBins->SetTitle("");
  h1_largeBins->GetXaxis()->SetTitle("variabile [unita' di misura = u]");
  h1_largeBins->GetYaxis()->SetTitle("numero di misure = entries");
  for(int k=0; k<n;k++)
    {
      h1_largeBins->Fill(var1[k]);
    }
  h1_largeBins->Draw();
  float binSize1 = (varmax - varmin) / Nbins1; 
  TPaveText *pt1=new TPaveText(0.72,0.72,0.95,0.87,"brNDC");
  pt1->AddText(Form("N bins = %d",Nbins1));
  pt1->AddText(Form("Bin size = %3.4f [u]",binSize1));
  pt1->Draw();
  c3.SaveAs("h_var1_largeBins.pdf");

  TCanvas c4;
  int Nbins2 = 40;
  TH1F *h1_mediumBins = new TH1F("h1_mediumBins","h1_mediumBins",Nbins2,varmin,varmax);
  h1_mediumBins->SetTitle("");
  h1_mediumBins->GetXaxis()->SetTitle("variabile [unita' di misura = u]");
  h1_mediumBins->GetYaxis()->SetTitle("numero di misure = entries");
  for(int k=0; k<n;k++)
    {
      h1_mediumBins->Fill(var1[k]);
    }
  h1_mediumBins->Draw();
  float binSize2 = (varmax - varmin) / Nbins2; 
  TPaveText *pt2=new TPaveText(0.72,0.72,0.95,0.87,"brNDC");
  pt2->AddText(Form("N bins = %d",Nbins2));
  pt2->AddText(Form("Bin size = %3.4f [u]",binSize2));
  pt2->Draw();
  c4.SaveAs("h_var1_mediumBins.pdf");

  TCanvas c5;
  int Nbins3 = 10000;
  TH1F *h1_smallBins = new TH1F("h1_smallBins","h1_smallBins",Nbins3,varmin,varmax);
  h1_smallBins->SetTitle("");
  h1_smallBins->GetXaxis()->SetTitle("variabile [unita' di misura = u]");
  h1_smallBins->GetYaxis()->SetTitle("numero di misure = entries");
  for(int k=0; k<n;k++)
    {
      h1_smallBins->Fill(var1[k]);
    }
  h1_smallBins->Draw();
  float binSize3 = (varmax - varmin) / Nbins3; 
  TPaveText *pt3=new TPaveText(0.72,0.72,0.95,0.87,"brNDC");
  pt3->AddText(Form("N bins = %d",Nbins3));
  pt3->AddText(Form("Bin size = %3.4f [u]",binSize3));
  pt3->Draw();
  c5.SaveAs("h_var1_smallBins.pdf");

  //=== Media, Moda, Mediana

  TCanvas c6;
  h1_mediumBins->Draw();

  double median[1]={-99};
  double prob[1]={0.5};
  double mean = h1_mediumBins->GetMean();
  h1_mediumBins->GetQuantiles(1,median,prob);
  double mode = h1_mediumBins->GetBinCenter(h1_mediumBins->GetMaximumBin());

  TLine *line1_mean = new TLine(mean,0,mean,h1_mediumBins->GetMaximum());
  line1_mean->SetLineColor(kRed);
  line1_mean->Draw();
  TLine *line1_median = new TLine(median[0],0,median[0],h1_mediumBins->GetMaximum());
  line1_median->SetLineColor(kBlue);
  line1_median->Draw();
  TLine *line1_mode = new TLine(mode,0,mode,h1_mediumBins->GetMaximum());
  line1_mode->SetLineColor(kGreen);
  line1_mode->Draw();

  TLegend *legend1 = new TLegend(0.59,0.68,0.88,0.88);
  legend1->AddEntry(line1_mean,Form("Media = %3.2f [u]",mean),"l");
  legend1->AddEntry(line1_median,Form("Mediana = %3.2f [u]",median[0]),"l");
  legend1->AddEntry(line1_mode,Form("Moda = %3.2f [u]",mode),"l");
  legend1->Draw();  

  c6.SaveAs("h_var1_mediumBins_mean_median_mode.pdf");

  TCanvas c7;
  //  TF1 *fb = new TF1("fb","gaus(0)+gaus(3)+gaus(6)", 0, 50);  
  //  fb->SetParameters(10,4,0.5,5,12,8,8,40,0.5);
  TF1 *fb = new TF1("fb","[0]*TMath::Landau(x,[1],[2])",0,50); 
  fb->SetParameters(10,10,2);

  TH1F *h2 = new TH1F("h2","h2",250,-20,50);
  for(int k=0; k<100000;k++)
    {
      h2->Fill(fb->GetRandom());
    }
  h2->SetTitle("");
  h2->GetXaxis()->SetTitle("variabile [unita' di misura = u]");
  h2->GetYaxis()->SetTitle("numero di misure = entries");
  h2->Draw();
  mean = h2->GetMean();
  h2->GetQuantiles(1,median,prob);
  mode = h2->GetBinCenter(h2->GetMaximumBin());

  TLine *line2_mean = new TLine(mean,0,mean,h2->GetMaximum());
  line2_mean->SetLineColor(kRed);
  line2_mean->Draw();
  TLine *line2_median = new TLine(median[0],0,median[0],h2->GetMaximum());
  line2_median->SetLineColor(kBlue);
  line2_median->Draw();
  TLine *line2_mode = new TLine(mode,0,mode,h2->GetMaximum());
  line2_mode->SetLineColor(kGreen);
  line2_mode->Draw();

  TLegend *legend2 = new TLegend(0.59,0.68,0.88,0.88);
  legend2->AddEntry(line2_mean,Form("Media = %3.2f [u]",mean),"l");
  legend2->AddEntry(line2_median,Form("Mediana = %3.2f [u]",median[0]),"l");
  legend2->AddEntry(line2_mode,Form("Moda = %3.2f [u]",mode),"l");
  legend2->Draw();  

  c7.SaveAs("h2_mean_median_mode.pdf");

  //=== Standard Deviation

  TCanvas c8;
  h1_mediumBins->GetXaxis()->SetRangeUser(15,25);
  h1_mediumBins->Draw();
  mean = h1_mediumBins->GetMean();
  double stdDev = h1_mediumBins->GetStdDev();
  line1_mean->SetLineStyle(3);
  line1_mean->Draw();
  TLine *line1_1sigmadown = new TLine(mean-stdDev,0,mean-stdDev,h1_mediumBins->GetMaximum());
  line1_1sigmadown->SetLineColor(kRed);
  line1_1sigmadown->Draw();
  TLine *line1_1sigmaup = new TLine(mean+stdDev,0,mean+stdDev,h1_mediumBins->GetMaximum());
  line1_1sigmaup->SetLineColor(kRed);
  line1_1sigmaup->Draw();
  TArrow *ar1_1sigma = new TArrow(mean-stdDev,h1_mediumBins->GetMaximum()/8,mean+stdDev,h1_mediumBins->GetMaximum()/8,0.01,"<|>");
  ar1_1sigma->SetLineColor(kRed);
  ar1_1sigma->Draw();

  TLine *line1_3sigmadown = new TLine(mean-3*stdDev,0,mean-3*stdDev,h1_mediumBins->GetMaximum());
  line1_3sigmadown->SetLineColor(kBlack);
  line1_3sigmadown->Draw();
  TLine *line1_3sigmaup = new TLine(mean+3*stdDev,0,mean+3*stdDev,h1_mediumBins->GetMaximum());
  line1_3sigmaup->SetLineColor(kBlack);
  line1_3sigmaup->Draw();
  TArrow *ar1_3sigma = new TArrow(mean-3*stdDev,h1_mediumBins->GetMaximum()/3,mean+3*stdDev,h1_mediumBins->GetMaximum()/3,0.01,"<|>");
  ar1_3sigma->SetLineColor(kBlack);
  ar1_3sigma->Draw();

  TPaveText *pt1_sigma=new TPaveText(0.655,0.73,0.89,0.88,"brNDC");
  pt1_sigma->AddText(Form("Media = %3.2f [u]", mean));
  pt1_sigma->AddText(Form("#sigma_{x} = dev. std. = %3.2f [u]", stdDev));
  pt1_sigma->Draw();
  
  TLegend *legend1_sigma = new TLegend(0.6535,0.57,0.89,0.70);
  legend1_sigma->SetMargin(0.3);
  legend1_sigma->AddEntry(ar1_1sigma,"#pm 1 #sigma_{x}","l");
  legend1_sigma->AddEntry(ar1_3sigma,"#pm 3 #sigma_{x}","l");
  legend1_sigma->Draw();  

  c8.SaveAs("h_var1_mediumBins_stdDev.pdf");

  TCanvas c9;
  h2->GetXaxis()->SetRangeUser(-20,50);
  h2->Draw();
  mean = h2->GetMean();
  stdDev = h2->GetStdDev();
  line2_mean->SetLineStyle(3);
  line2_mean->Draw();
  TLine *line2_1sigmadown = new TLine(mean-stdDev,0,mean-stdDev,h2->GetMaximum());
  line2_1sigmadown->SetLineColor(kRed);
  line2_1sigmadown->Draw();
  TLine *line2_1sigmaup = new TLine(mean+stdDev,0,mean+stdDev,h2->GetMaximum());
  line2_1sigmaup->SetLineColor(kRed);
  line2_1sigmaup->Draw();
  TArrow *ar2_1sigma = new TArrow(mean-stdDev,h2->GetMaximum()/8,mean+stdDev,h2->GetMaximum()/8,0.01,"<|>");
  ar2_1sigma->SetLineColor(kRed);
  ar2_1sigma->Draw();

  TLine *line2_3sigmadown = new TLine(mean-3*stdDev,0,mean-3*stdDev,h2->GetMaximum());
  line2_3sigmadown->SetLineColor(kBlack);
  line2_3sigmadown->Draw();
  TLine *line2_3sigmaup = new TLine(mean+3*stdDev,0,mean+3*stdDev,h2->GetMaximum());
  line2_3sigmaup->SetLineColor(kBlack);
  line2_3sigmaup->Draw();
  TArrow *ar2_3sigma = new TArrow(mean-3*stdDev,h2->GetMaximum()/3,mean+3*stdDev,h2->GetMaximum()/3,0.01,"<|>");
  ar2_3sigma->SetLineColor(kBlack);
  ar2_3sigma->Draw();

  TPaveText *pt2_sigma=new TPaveText(0.655,0.73,0.89,0.88,"brNDC");
  pt2_sigma->AddText(Form("Media = %3.2f [u]", mean));
  pt2_sigma->AddText(Form("#sigma_{x} = dev. std. = %3.2f [u]", stdDev));
  pt2_sigma->Draw();

  TLegend *legend2_sigma = new TLegend(0.6535,0.57,0.89,0.70);
  legend2_sigma->SetMargin(0.3);
  legend2_sigma->AddEntry(ar2_1sigma,"#pm 1 #sigma_{x}","l");
  legend2_sigma->AddEntry(ar2_3sigma,"#pm 3 #sigma_{x}","l");
  legend2_sigma->Draw();  

  c9.SaveAs("h2_stdDev.pdf");

  //=== Stima intervalli

  const int M = 20;
  const int L = int(n/M);
  double mean1[L];
  double tmean1[L];
  for(int k=0;k<L;k++)
    {
      //cout << "=== bin " << k << endl;
      mean1[k]=0;
      for(int j=k*M;j<k*M+M;j++)
	{
	  //cout << "   === element " << j << endl;
	  //cout << var1[j] << endl;
	  mean1[k]+=var1[j];
	}      
      mean1[k]=double(mean1[k]/double(M));
      tmean1[k]=(t[k*M+M-1]+t[k*M])/2;
      //cout << "Mean = " << mean1[k] << endl;
      //cout << "Time = " << tmean1[k] << endl;
    }
  
  TCanvas c10;
  c10.SetGridx();
  c10.SetGridy();

  TGraph *gmean = new TGraph(L,tmean1,mean1);
  gmean->SetTitle("");
  gmean->GetYaxis()->SetTitle(Form("media variabile ogni %d misure [unita' di misura = u]",M));
  gmean->GetXaxis()->SetTitle("tempo [s]");
  gmean->GetYaxis()->SetLimits(varmin, varmax);
  gmean->GetYaxis()->SetRangeUser(varmin, varmax);
  gmean->SetMarkerStyle(20);
  gmean->SetMarkerSize(0.5);
  gmean->Draw("AP");
  c10.SaveAs("g_var1mean_vs_time_flat.pdf");

  TCanvas c11;
  TH1F *h1mean_mediumBins = new TH1F("h1mean_mediumBins","h1mean_mediumBins",Nbins2,varmin,varmax);
  h1mean_mediumBins->SetTitle("");
  h1mean_mediumBins->GetXaxis()->SetTitle(Form("media variabile ogni %d misure [unita' di misura = u]",M));
  h1mean_mediumBins->GetYaxis()->SetTitle("numero di misure = entries");
  for(int k=0; k<L;k++)
    {
      h1mean_mediumBins->Fill(mean1[k]);
    }
  h1mean_mediumBins->Draw();
  TPaveText *pt2mean=new TPaveText(0.72,0.72,0.95,0.87,"brNDC");
  pt2mean->AddText(Form("N bins = %d",Nbins2));
  pt2mean->AddText(Form("Bin size = %3.4f [u]",binSize2));
  pt2mean->Draw();
  c11.SaveAs("h_var1mean_mediumBins.pdf");

  TCanvas c12;
  TH1F *h1_mediumBins_firstGroup = new TH1F("h1_mediumBins_firstGroup","h1_mediumBins_firstGroup",Nbins2,varmin,varmax);
  h1_mediumBins_firstGroup->SetTitle("");
  h1_mediumBins_firstGroup->GetXaxis()->SetTitle("variabile [unita' di misura = u]");
  h1_mediumBins_firstGroup->GetYaxis()->SetTitle("numero di misure = entries");
  for(int k=0; k<n/10;k++)
    {
      h1_mediumBins_firstGroup->Fill(var1[k]);
    }
  h1_mediumBins_firstGroup->Draw();
  pt2->Draw();
  c12.SaveAs("h_var1_mediumBins_firstGroup.pdf");

  cout << h1_mediumBins_firstGroup->GetStdDev() << endl;

}
