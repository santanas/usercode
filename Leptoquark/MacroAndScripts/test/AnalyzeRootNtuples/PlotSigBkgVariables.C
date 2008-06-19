PlotSigBkgVariable(char *histoname_, char* outputfilename_, char* xtitle_, char* ytitle_, float Ymin_=0.1, float Ymax_=1000, float Xmin_=0., float Xmax_=1000., int rebin_, int logY_){
  
  gROOT->Reset();
  gStyle->SetOptStat(0);

  //Which histogram you want to plot?
  char histoname[200];
  sprintf(histoname,"%s",&histoname_[0]);

  //Which sig/bkg files you want to consider?
  TFile L250("lq1stgen_M250_fast169_100pb.root");
  //TFile L650("LQ1stgenM650_fast169_100pbScenario.root");
  TFile Z("zjet.root");
  TFile W("wjet.root");
  TFile ttbar("ttbar.root");
  TFile QCD("qcdjet.root");
  TFile gj("gammajet.root");

  TLegend *legend = new TLegend(0.56,0.62,0.86,0.85);
  legend->SetFillColor(kWhite);

  int rebin=rebin_;

  TCanvas c1;
  if(logY_==1)
    c1.SetLogy();

  THStack h_BgkStack;

  //~~~~~~~~~~~~~~~~~~~~~~
  Z.cd();
  TH1F h1;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h1); 
  h1.GetXaxis()->SetTitle(xtitle_);
  h1.GetYaxis()->SetTitle(ytitle_);

  h1.SetMinimum(Ymin_);

  if(Ymax_>0)
    {
      h1.SetMaximum(Ymax_);
      h1.GetYaxis()->SetRangeUser(Ymin_,Ymax_);
    }

  h1.SetLineColor(1);
  h1.SetLineWidth(2);
  h1.SetFillColor(1);
  h1.SetFillStyle(3003);
  h1.Rebin(rebin);
  h1.SetName("Z+jet");
  h1.DrawClone("HISTE");

  legend->AddEntry(&h1,"Z+jet","f");

  //~~~~~~~~~~~~~~~~~~~~~~
  W.cd();
  TH1F h2;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h2); 
  h2.SetLineColor(2);
  h2.SetLineWidth(2);
  h2.SetFillColor(2);
  h2.SetFillStyle(3001);
  h2.Rebin(rebin);
  h2.SetName("W+jet");
  h2.DrawClone("HISTEsame");

  legend->AddEntry(&h2,"W+jet","f");


  //~~~~~~~~~~~~~~~~~~~~~~
  ttbar.cd();
  TH1F h3;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h3); 
  h3.SetLineColor(3);
  h3.SetLineWidth(2);
  h3.SetFillColor(3);
  h3.SetFillStyle(3003);
  h3.SetName("ttbar");
  //h3.Scale(1.8);
  h3.Rebin(rebin);
  h3.DrawClone("HISTEsame");

  legend->AddEntry(&h3,"ttbar","f");

  //~~~~~~~~~~~~~~~~~~~~~~
  QCD.cd();
  TH1F h4;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h4); 
  h4.SetLineColor(6);
  h4.SetLineWidth(2);
  h4.SetFillColor(6);
  h4.SetFillStyle(3004);
  h4.Rebin(rebin);
  h4.SetName("QCDjets");
  h4.DrawClone("HISTEsame");

  legend->AddEntry(&h4,"QCDjets","f");


  //~~~~~~~~~~~~~~~~~~~~~~
  gj.cd();
  TH1F h5;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h5); 
  h5.SetLineColor(5);
  h5.SetLineWidth(2);
  h5.SetFillColor(5);
  h5.SetFillStyle(3001);
  h5.Rebin(rebin);
  h5.SetName("\\gamma+jet");
  h5.DrawClone("HISTEsame");

  legend->AddEntry(&h5,"\\gamma+jet","f");


  // ~~~~~~~~~~~~~~~~~~~~~~
  //      Draw Stack
  // ~~~~~~~~~~~~~~~~~~~~~~
  h_BgkStack.Add(&h2);
  h_BgkStack.Add(&h3);
  h_BgkStack.Add(&h4);
  h_BgkStack.Add(&h5);
  h_BgkStack.Add(&h1);

  h_BgkStack.Draw("HISTE");

  h_BgkStack.GetXaxis()->SetTitle(xtitle_);
  h_BgkStack.GetXaxis()->SetRangeUser(Xmin_,Xmax_);
  h_BgkStack.GetYaxis()->SetTitle(ytitle_);

  h_BgkStack.SetMinimum(Ymin_);

  if(Ymax_>0)
    {
      h_BgkStack.SetMaximum(Ymax_);
      h_BgkStack.GetYaxis()->SetRangeUser(Ymin_,Ymax_);
    }

  //~~~~~~~~~~~~~~~~~~~~~~
  L250.cd();
  TH1F h6;
  ((TH1D*)gDirectory->Get(histoname))->Copy(h6); 
  h6.SetLineColor(4);
  h6.SetLineWidth(3);
  h6.Scale(0.85);
  h6.Rebin(rebin);
  h6.SetName("LQ (250 GeV)");
  h6.DrawClone("HISTEsame");

  legend->AddEntry(&h6,"LQ (250 GeV)","f");

  //   L650.cd();
  //  TH1F h7;
  //  ((TH1D*)gDirectory->Get(histoname))->Copy(h7); 
  //   h7.SetLineColor(4);
  //   h7.SetLineWidth(3);
  //   h7.Scale(0.85);
  //   h7.Rebin(rebin);
  //   h7.SetName("LQ (650 GeV)");
  //   h7.DrawClone("HISTEsame");
  
  //   legend->AddEntry(&h7,"LQ (650 GeV)","f");
  
  legend->SetMargin(0.5);
  legend->Draw();

  c1.SaveAs(outputfilename_);

}
