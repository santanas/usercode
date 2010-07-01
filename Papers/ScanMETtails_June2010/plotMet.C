{

  gROOT->Reset();
  gStyle->SetOptStat(111111);

  TFile f("results.root");

  TCanvas c;
  c->SetLogy();
  
  cleanedpfMET_new = (TH1F*)f.Get("cleanedpfMET");
  cleanedtcMET_new = (TH1F*)f.Get("cleanedtcMET");

  cleanedpfMET_new->SetTitle("");
  cleanedtcMET_new->SetTitle("");
    
  cleanedpfMET_new->SetLineColor(kBlue);
  cleanedpfMET_new->SetLineWidth(2);
  //cleanedpfMET_new->SetFillColor(kBlue);
  cleanedpfMET_new->SetFillStyle(1001);
  
  cleanedtcMET_new->SetLineColor(kRed);
  cleanedtcMET_new->SetFillColor(kRed);
  
  cleanedtcMET_new->SetStats(1111);
  cleanedpfMET_new->SetStats(1111);

  cleanedtcMET_new->GetYaxis()->SetTitle("Number of events / 3 GeV");

  cleanedtcMET_new->Draw("HISTs");
  cleanedpfMET_new->Draw("lsames");

  TLegend *legend = new TLegend(.4,.91,.75,.99);
  legend->SetBorderSize(1);
  legend->SetFillColor(0);
  //    legend->SetFillStyle(0);
  legend->AddEntry(cleanedtcMET_new,"tcMET","f");
  legend->AddEntry(cleanedpfMET_new,"pfMET","l");
  legend->Draw();

  c.SaveAs("met.C");

}
