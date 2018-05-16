{

  const int n = 5; // n measurements

  double P[n]={65,75,85,95,105};
  double Pmin = -20;
  double Pmax = 120;
  double T[n]={-20,17,42,94,127};
  double Tmin = -300;
  double Tmax = 150;

  TCanvas c1;
  c1.SetGridx();
  c1.SetGridy();

  TGraph *g1 = new TGraph(n,P,T);
  g1->SetTitle("Misura dello zero assoluto");
  g1->GetXaxis()->SetTitle("P [mmHg]");
  g1->GetXaxis()->SetTitleSize(0.05);
  g1->GetXaxis()->SetLimits(Pmin,Pmax);
  g1->GetYaxis()->SetTitle("T [#circC]");
  g1->GetYaxis()->SetTitleSize(0.05);
  g1->GetYaxis()->SetTitleOffset(0.75);
  g1->GetYaxis()->SetLimits(Tmin,Tmax);
  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(0.5);
  g1->Draw("AP");

  //points
  g1->GetXaxis()->SetRangeUser(60,110);
  g1->GetYaxis()->SetRangeUser(-50,150);
  c1.SaveAs("g_T_vs_P_points.pdf");
  
  //fit
  TF1 *f1 = new TF1("f1","[0]+[1]*x", Pmin, Pmax);
  f1->SetLineStyle(2);
  g1->Fit("f1","r");
  cout << "A: " << f1->GetParameter(0) << " +/- " << f1->GetParError(0) << endl;
  cout << "B: " << f1->GetParameter(1) << " +/- " << f1->GetParError(1) << endl;
  c1.SaveAs("g_T_vs_P_points_fit.pdf");

  //uncertainties on T
  double sigma_T = 0;
  for(int k=0; k<n; k++)
    {
      double T_exp = f1->GetParameter(0) + f1->GetParameter(1)*P[k];
      sigma_T = sigma_T + TMath::Power( (T[k] - T_exp) , 2 ); 
    }
  sigma_T = sigma_T / (n-2) ;
  sigma_T = sqrt(sigma_T);
  cout << "sigma T: " << sigma_T << endl;

  double e_T[n]={sigma_T,sigma_T,sigma_T,sigma_T,sigma_T};
  double e_P[n]={0,0,0,0,0};
  TGraphErrors *g1_e = new TGraphErrors(n,P,T,e_P,e_T);
  g1_e->Draw("esame");
  c1.SaveAs("g_T_vs_P_points_fit_errors.pdf");

  //uncertainties on parameters A and B
  double zerox[n]={0};
  double e_zerox[n]={0};
  double zeroy[n]={f1->GetParameter(0)};
  double e_zeroy[n]={f1->GetParError(0)};
  TGraphErrors *zero = new TGraphErrors(1,zerox,zeroy,e_zerox,e_zeroy);
  zero->SetMarkerStyle(20);
  zero->SetMarkerColor(2);
  zero->SetLineColor(2);
  zero->Draw("Psame"); 

  g1->GetXaxis()->SetRangeUser(Pmin,Pmax);
  g1->GetYaxis()->SetRangeUser(Tmin,Tmax);
    c1.SaveAs("g_T_vs_P_points_fit_errors_unzoomToZero.pdf");
 

}
