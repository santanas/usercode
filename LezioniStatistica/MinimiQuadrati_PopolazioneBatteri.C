{

  const int n = 3; // n measurements

  double t[n]={0,1,2};
  double tmin = -1;
  double tmax = 50;
  double N[n]={153000,137000,128000};
  double Nmin = 0;
  double Nmax = 200000;

  TCanvas c1;
  c1.SetGridx();
  c1.SetGridy();

  TGraph *g1 = new TGraph(n,t,N);
  g1->SetTitle("Misura della vita media dei batteri di una popolazione");
  g1->GetXaxis()->SetTitle("t [giorni]");
  g1->GetXaxis()->SetTitleSize(0.05);
  g1->GetXaxis()->SetLimits(tmin,tmax);
  g1->GetYaxis()->SetTitle("N [numero batteri]");
  g1->GetYaxis()->SetTitleSize(0.05);
  g1->GetYaxis()->SetTitleOffset(0.75);
  g1->GetYaxis()->SetLimits(Nmin,Nmax);
  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(0.5);
  g1->Draw("AP");

  //points
  g1->GetXaxis()->SetRangeUser(-0.5,2.5);
  g1->GetYaxis()->SetRangeUser(120000,160000);
  c1.SaveAs("g_N_vs_t_points.pdf");
  
  //fit
  TF1 *f1 = new TF1("f1","expo", tmin, tmax);
  f1->SetLineStyle(2);
  g1->Fit("f1","r");
  cout << "N0: " << f1->GetParameter(0) << " +/- " << f1->GetParError(0) << endl;
  cout << "Tau: " << f1->GetParameter(1) << " +/- " << f1->GetParError(1) << endl;
  c1.SaveAs("g_N_vs_t_points_fit.pdf");

  //uncertainties on N (priori, assumendo errori statistici Poissoniani)
  double e_pre_N[n]={391.15,370.13,357.77};
  double e_pre_t[n]={0,0,0};
  TGraphErrors *g1_e_pre = new TGraphErrors(n,t,N,e_pre_t,e_pre_N);
  g1_e_pre->Draw("esame");
  c1.SaveAs("g_N_vs_t_points_fit_errorsPre.pdf");
  delete g1_e_pre;

  //uncertainties on N (a posteriori, assumendo andamento esponenziale)
  double sigma_N = 0;
  for(int k=0; k<n; k++)
    {
      double N_exp = exp(f1->GetParameter(0) + f1->GetParameter(1)*t[k]);
      sigma_N = sigma_N + TMath::Power( (N[k] - N_exp) , 2 ); 
    }
  sigma_N = sigma_N / (n-2) ;
  sigma_N = sqrt(sigma_N);
  cout << "sigma N: " << sigma_N << endl;

  double e_post_N[n]={sigma_N,sigma_N,sigma_N};
  double e_post_t[n]={0,0,0};
  TGraphErrors *g1_e_post = new TGraphErrors(n,t,N,e_post_t,e_post_N);
  g1_e_post->Draw("esame");
  c1.SaveAs("g_N_vs_t_points_fit_errorsPost.pdf");

  //uncertainties on parameters A and B
  g1->GetXaxis()->SetRangeUser(tmin,tmax);
  g1->GetYaxis()->SetRangeUser(Nmin,Nmax);
  c1.SaveAs("g_N_vs_t_points_fit_errors_unzoomToZero.pdf");

  g1->GetYaxis()->SetRangeUser(1000,Nmax);
  c1.SetLogy();
  c1.SaveAs("g_N_vs_t_points_fit_errors_unzoomToZero_logY.pdf");

}
