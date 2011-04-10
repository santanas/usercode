{
//author A.Zabi
//This macro can be used to reproduce the tau benchmark plot
//for tau jet reconstruction studies
// 50 GeV taus desintegrating hadronically have been studied
gROOT->Reset();
//TFile *f = new TFile("pfjetBenchmark.root");
TFile *f = new TFile("pfjet_WithTracking_MCGLO_all.root");

//BRPt20_40->Fit("gaus","L");
//BRPt40_60->Fit("gaus","L");
//BRPt60_80->Fit("gaus","L");
//BRPt80_100->Fit("gaus","L");
//BRPt100_150->Fit("gaus","L");
//BRPt150_200->Fit("gaus","L");
//BRPt200_250->Fit("gaus","L");
//BRPt250_300->Fit("gaus","L");
//BRPt300_400->Fit("gaus","L");
//BRPt400_500->Fit("gaus","L");
//BRPt500_750->Fit("gaus","L");


TCanvas *c0 =new TCanvas("c0","Canvas",10,10,1800,700);
 TPaveStats *ptstats = new TPaveStats(0.5507035,0.52,0.9798995,0.9965035,"brNDC");
 ptstats->SetName("stats");
 ptstats->SetBorderSize(2);
 ptstats->SetTextAlign(12);
 ptstats->SetTextFont(42);  
  ptstats->SetFillColor(0);
  TText *text = ptstats->AddText("BRPt20_40");
  text->SetTextSize(0.04204795);
  BRPt20_40->GetListOfFunctions()->Add(ptstats);
  ptstats->SetParent(BRPt20_40->GetListOfFunctions());
  ptstats->SetOptStat(1111);
  //  ptstats->SetOptFit(10001);
  ptstats->Draw(); 
c0->Divide(6,2,.005,.005);
 c0->cd(1);
BRPt20_40->Fit("gaus");
 c0->cd(2);
 TText *text = ptstats->AddText("BRPt40_60");
 BRPt40_60->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt40_60->GetListOfFunctions());
BRPt40_60->Fit("gaus");
 c0->cd(3);
 TText *text = ptstats->AddText("BRPt60_80");
 BRPt60_80->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt60_80->GetListOfFunctions());
BRPt60_80->Fit("gaus");
 c0->cd(4);
 TText *text = ptstats->AddText("BRPt80_100");
 BRPt80_100->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt80_100->GetListOfFunctions());
BRPt80_100->Fit("gaus");
 c0->cd(5);
 TText *text = ptstats->AddText("BRPt100_150");
 BRPt100_150->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt100_150->GetListOfFunctions());
BRPt100_150->Fit("gaus");
 c0->cd(6);
 TText *text = ptstats->AddText("BRPt150_200");
 BRPt150_200->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt150_200->GetListOfFunctions());
BRPt150_200->Fit("gaus");
 c0->cd(7);
 TText *text = ptstats->AddText("BRPt200_250");
 BRPt200_250->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt200_250->GetListOfFunctions());
BRPt200_250->Fit("gaus");
 c0->cd(8);
 TText *text = ptstats->AddText("BRPt250_300");
 BRPt250_300->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt250_300->GetListOfFunctions());
BRPt250_300->Fit("gaus");
 c0->cd(9);
 TText *text = ptstats->AddText("BRPt300_400");
 BRPt300_400->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt300_400->GetListOfFunctions());
BRPt300_400->Fit("gaus");
 c0->cd(10);
 TText *text = ptstats->AddText("BRPt400_500");
 BRPt400_500->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt400_500->GetListOfFunctions());
BRPt400_500->Fit("gaus");
 c0->cd(11);
 TText *text = ptstats->AddText("BRPt500_750");
 BRPt500_750->GetListOfFunctions()->Add(ptstats);
 ptstats->SetParent(BRPt500_750->GetListOfFunctions());
BRPt500_750->Fit("gaus");

Int_t n = 11;
TF1* g[n];
Double_t mean[n], sigma[n], Mean[n], RMS[n], pt[n];

pt[0] = 30.;
pt[1] = 50.;
pt[2] = 70.;
pt[3] = 90.;
pt[4] = 125.;
pt[5] = 175.;
pt[6] = 225.;
pt[7] = 275.;
pt[8] = 350.;
pt[9] = 450.;
pt[10] = 600.;

TF1* g[0]  = BRPt20_40->GetFunction("gaus");
TF1* g[1]  = BRPt40_60->GetFunction("gaus");
TF1* g[2]  = BRPt60_80->GetFunction("gaus");
TF1* g[3]  = BRPt80_100->GetFunction("gaus");
TF1* g[4]  = BRPt100_150->GetFunction("gaus");
TF1* g[5]  = BRPt150_200->GetFunction("gaus");
TF1* g[6]  = BRPt200_250->GetFunction("gaus");
TF1* g[7]  = BRPt250_300->GetFunction("gaus");
TF1* g[8]  = BRPt300_400->GetFunction("gaus");
TF1* g[9]  = BRPt400_500->GetFunction("gaus");
TF1* g[10] = BRPt500_750->GetFunction("gaus");

Double_t Mean[0]  = BRPt20_40->GetMean();
Double_t Mean[1]  = BRPt40_60->GetMean();
Double_t Mean[2]  = BRPt60_80->GetMean();
Double_t Mean[3]  = BRPt80_100->GetMean();
Double_t Mean[4]  = BRPt100_150->GetMean();
Double_t Mean[5]  = BRPt150_200->GetMean();
Double_t Mean[6]  = BRPt200_250->GetMean();
Double_t Mean[7]  = BRPt250_300->GetMean();
Double_t Mean[8]  = BRPt300_400->GetMean();
Double_t Mean[9]  = BRPt400_500->GetMean();
Double_t Mean[10]  = BRPt500_750->GetMean();

Double_t RMS[0]  = BRPt20_40->GetRMS();
Double_t RMS[1]  = BRPt40_60->GetRMS();
Double_t RMS[2]  = BRPt60_80->GetRMS();
Double_t RMS[3]  = BRPt80_100->GetRMS();
Double_t RMS[4]  = BRPt100_150->GetRMS();
Double_t RMS[5]  = BRPt150_200->GetRMS();
Double_t RMS[6]  = BRPt200_250->GetRMS();
Double_t RMS[7]  = BRPt250_300->GetRMS();
Double_t RMS[8]  = BRPt300_400->GetRMS();
Double_t RMS[9]  = BRPt400_500->GetRMS();
Double_t RMS[10]  = BRPt500_750->GetRMS();

for (Int_t i=0; i<n; ++i) {
  mean[i] = g[i]->GetParameter(1);
  sigma[i] = g[i]->GetParameter(2);
}

TCanvas* c1 = new TCanvas;
c1->Divide(2,1);
c1->cd(1);
c1->cd(1)->SetGridx();
c1->cd(1)->SetGridy();
TGraph *gr1 = new TGraph ( n, pt, sigma );
TGraph *gr11 = new TGraph ( n, pt, RMS );
gr1->SetLineColor(kBlue);
gr1->SetLineWidth(2);
gr1->SetMinimum(0.02);
gr1->SetMaximum(0.16);
gr1->SetTitle("Resolution Delta(pT)/pT");
gr1->Draw("AC*");
gr11->SetMarkerStyle(21);
gr11->SetMarkerSize(0.5);
gr11->SetLineColor(kCyan);
gr11->Draw("CP");
// c1->Print("resolution.gif");
//TCanvas* c2 = new TCanvas;
//c2->SetGridx();
//c2->SetGridy();
c1->cd(2);
c1->cd(2)->SetGridx();
c1->cd(2)->SetGridy();
TGraph *gr2 = new TGraph ( n, pt, mean );
TGraph *gr12 = new TGraph ( n, pt, Mean );
gr2->SetMinimum(-0.25);
gr2->SetMaximum(0.01);
gr2->SetLineColor(kRed);
gr2->SetLineWidth(2);
gr2->SetTitle("Mean Delta(pT)/pT");
gr2->Draw("AC*");
gr12->SetMarkerStyle(21);
gr12->SetMarkerSize(0.5);
gr12->SetLineColor(kViolet);
gr12->Draw("CP");
//c2->Print("mean.gif");
c1->Print("resolution.gif");
}
