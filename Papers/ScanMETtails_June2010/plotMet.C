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
    
  cleanedpfMET_new->SetLineColor(38);
  //cleanedpfMET_new->SetLineWidth(2);
  cleanedpfMET_new->SetFillColor(38);
  //cleanedpfMET_new->SetFillStyle(1001);
  
  cleanedtcMET_new->SetLineColor(kRed);
  cleanedtcMET_new->SetLineWidth(2);
  //cleanedtcMET_new->SetFillColor(kRed);
  cleanedtcMET_new->SetFillStyle(1001);  

  cleanedtcMET_new->SetStats(1111);
  cleanedpfMET_new->SetStats(1111);

  cleanedtcMET_new->GetYaxis()->SetTitle("Number of events / 3 GeV");
  cleanedtcMET_new->SetName("cleaned tcMET");
  cleanedpfMET_new->SetName("cleaned pfMET");

  TPaveStats *ptstats = new TPaveStats(0.436782,0.608051,0.637931,0.847458,"brNDC");
  ptstats->SetName("stats");
  ptstats->SetFillColor(0);
  ptstats->SetTextColor(kRed);
  ptstats->SetLineColor(kRed);
  ptstats->SetBorderSize(1);
  ptstats->SetTextAlign(12);
  TText *text = ptstats->AddText("cleaned tcMET");
  text->SetTextSize(0.036709);
  ptstats->Draw();
  cleanedtcMET_new->GetListOfFunctions()->Add(ptstats);
  ptstats->SetParent(cleanedtcMET_new->GetListOfFunctions());

  cleanedtcMET_new->Draw("ls");  

  ptstats = new TPaveStats(0.656609,0.610169,0.857759,0.849576,"brNDC");
  ptstats->SetName("stats");
  ptstats->SetFillColor(0);
  ptstats->SetTextColor(35);
  ptstats->SetLineColor(35);
  ptstats->SetBorderSize(1);
  ptstats->SetTextAlign(12);
  text = ptstats->AddText("cleaned pfMET");
  text->SetTextSize(0.036709);
  ptstats->Draw();
  cleanedpfMET_new->GetListOfFunctions()->Add(ptstats);
  ptstats->SetParent(cleanedpfMET_new->GetListOfFunctions());
 
  cleanedpfMET_new->Draw("HISTsames");

  TLegend *legend = new TLegend(0.517241,0.438559,0.751437,0.57839,NULL,"brNDC");
  legend->SetBorderSize(1);
  legend->SetMargin(0.25);
  legend->SetFillColor(0);
  //    legend->SetFillStyle(0);
  legend->AddEntry(cleanedtcMET_new,"cleaned tcMET","l");
  legend->AddEntry(cleanedpfMET_new,"cleaned pfMET","f");
  legend->Draw();

  gPad->RedrawAxis();

  c.SaveAs("met.C");

}
