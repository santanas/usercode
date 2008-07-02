{

gROOT->Reset();

TFile FileTrueEle("EleIsoID_trueEle_LQ.root");
TFile FileFakeEle("EleIsoID_fakeEle_QCD.root");

//eleHoE
TCanvas c1;
c1.SetLogy();

TLegend *legend1 = new TLegend(0.56,0.62,0.86,0.85);
legend1->SetFillColor(kWhite);

FileTrueEle.cd();

h_eleHoE_matchLQ->SetStats(0);
h_eleHoE_matchLQ->SetLineColor(kBlue);
h_eleHoE_matchLQ->SetLineWidth(2);
h_eleHoE_matchLQ->GetXaxis()->SetTitle("H/E");
h_eleHoE_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleHoE_matchLQ->DrawNormalized("HIST");

legend1->AddEntry(h_eleHoE_matchLQ,"electrons from LQ","f");

FileFakeEle.cd();
h_eleHoE_NOmatchLQ->SetFillStyle(3003);
h_eleHoE_NOmatchLQ->SetFillColor(kRed);
h_eleHoE_NOmatchLQ->DrawNormalized("sameHIST");

legend1->AddEntry(h_eleHoE_NOmatchLQ,"fake electrons from QCD jets","f");

legend1->Draw();

c1.SaveAs("h_eleHoE.gif");

//h_eleSigmaEE
TCanvas c2;
c2.SetLogy();

TLegend *legend2 = new TLegend(0.56,0.62,0.86,0.85);
legend2->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleSigmaEE_matchLQ->SetStats(0);
h_eleSigmaEE_matchLQ->SetLineColor(kBlue);
h_eleSigmaEE_matchLQ->SetLineWidth(2);
h_eleSigmaEE_matchLQ->GetXaxis()->SetTitle("\\sigma_{\\eta\\eta}");
h_eleSigmaEE_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleSigmaEE_matchLQ->DrawNormalized("HIST");

legend2->AddEntry(h_eleSigmaEE_matchLQ,"electrons from LQ","f");

FileFakeEle.cd();
h_eleSigmaEE_NOmatchLQ->SetFillStyle(3003);
h_eleSigmaEE_NOmatchLQ->SetFillColor(kRed);
h_eleSigmaEE_NOmatchLQ->DrawNormalized("sameHIST");

legend2->AddEntry(h_eleSigmaEE_NOmatchLQ,"fake electrons from QCD jets","f");

legend2->Draw();

c2.SaveAs("h_eleSigmaEE.gif");

//h_eleDeltaPhiTrkSC_matchLQ
TCanvas c3;
c3.SetLogy();

TLegend *legend3 = new TLegend(0.56,0.62,0.86,0.85);
legend3->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleDeltaPhiTrkSC_matchLQ->SetStats(0);
h_eleDeltaPhiTrkSC_matchLQ->SetLineColor(kBlue);
h_eleDeltaPhiTrkSC_matchLQ->SetLineWidth(2);
h_eleDeltaPhiTrkSC_matchLQ->GetXaxis()->SetTitle("\\Delta\\phi Trk-SC @ ECAL");
h_eleDeltaPhiTrkSC_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleDeltaPhiTrkSC_matchLQ->DrawNormalized("HIST");

legend3->AddEntry(h_eleDeltaPhiTrkSC_matchLQ,"electrons from LQ","f");

FileFakeEle.cd();
h_eleDeltaPhiTrkSC_NOmatchLQ->SetFillStyle(3003);
h_eleDeltaPhiTrkSC_NOmatchLQ->SetFillColor(kRed);
h_eleDeltaPhiTrkSC_NOmatchLQ->DrawNormalized("sameHIST");

legend3->AddEntry(h_eleDeltaPhiTrkSC_NOmatchLQ,"fake electrons from QCD jets","f");

legend3->Draw();

c3.SaveAs("h_eleDeltaPhiTrkSC.gif");

//h_eleDeltaEtaTrkSC_matchLQ
TCanvas c4;
c4.SetLogy();

TLegend *legend4 = new TLegend(0.56,0.62,0.86,0.85);
legend4->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleDeltaEtaTrkSC_matchLQ->SetStats(0);
h_eleDeltaEtaTrkSC_matchLQ->SetLineColor(kBlue);
h_eleDeltaEtaTrkSC_matchLQ->SetLineWidth(2);
h_eleDeltaEtaTrkSC_matchLQ->GetXaxis()->SetTitle("\\Delta\\eta Trk-SC @ ECAL");
h_eleDeltaEtaTrkSC_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleDeltaEtaTrkSC_matchLQ->DrawNormalized("HIST");

legend4->AddEntry(h_eleDeltaEtaTrkSC_matchLQ,"electrons from LQ","f");

FileFakeEle.cd();
h_eleDeltaEtaTrkSC_NOmatchLQ->SetFillStyle(3003);
h_eleDeltaEtaTrkSC_NOmatchLQ->SetFillColor(kRed);
h_eleDeltaEtaTrkSC_NOmatchLQ->DrawNormalized("sameHIST");

legend4->AddEntry(h_eleDeltaEtaTrkSC_NOmatchLQ,"fake electrons from QCD jets","f");

legend4->Draw();

c4.SaveAs("h_eleDeltaEtaTrkSC.gif");

//h_eleNumTrkIso_NOmatchLQ
TCanvas c5;
//c5.SetLogy();

TLegend *legend5 = new TLegend(0.56,0.62,0.86,0.85);
legend5->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleNumTrkIso_matchLQ->SetStats(0);
h_eleNumTrkIso_matchLQ->SetLineColor(kBlue);
h_eleNumTrkIso_matchLQ->SetLineWidth(2);
h_eleNumTrkIso_matchLQ->GetXaxis()->SetTitle("NumTrkIso");
h_eleNumTrkIso_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleNumTrkIso_matchLQ->DrawNormalized("HIST");

legend5->AddEntry(h_eleNumTrkIso_matchLQ,"electrons from LQ","f");

FileFakeEle.cd();
h_eleNumTrkIso_NOmatchLQ->SetFillStyle(3003);
h_eleNumTrkIso_NOmatchLQ->SetFillColor(kRed);
h_eleNumTrkIso_NOmatchLQ->DrawNormalized("sameHIST");

legend5->AddEntry(h_eleNumTrkIso_NOmatchLQ,"fake electrons from QCD jets","f");

legend5->Draw();

c5.SaveAs("h_eleNumTrkIso.gif");

//h_eleTrkIso_matchLQ_matchLQ
TCanvas c6;
c6.SetLogy();

TLegend *legend6 = new TLegend(0.56,0.62,0.86,0.85);
legend6->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleTrkIso_matchLQ->SetStats(0);
h_eleTrkIso_matchLQ->SetLineColor(kBlue);
h_eleTrkIso_matchLQ->SetLineWidth(2);
h_eleTrkIso_matchLQ->GetXaxis()->SetTitle("TrkIso");
h_eleTrkIso_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleTrkIso_matchLQ->DrawNormalized("HIST");

legend6->AddEntry(h_eleTrkIso_matchLQ,"electrons from LQ","f");

FileFakeEle.cd();
h_eleTrkIso_NOmatchLQ->SetFillStyle(3003);
h_eleTrkIso_NOmatchLQ->SetFillColor(kRed);
h_eleTrkIso_NOmatchLQ->DrawNormalized("sameHIST");

legend6->AddEntry(h_eleTrkIso_NOmatchLQ,"fake electrons from QCD jets","f");

legend6->Draw();

c6.SaveAs("h_eleTrkIso.gif");

//h_eleEcalIso_matchLQ_matchLQ
TCanvas c7;
c7.SetLogy();

TLegend *legend7 = new TLegend(0.56,0.62,0.86,0.85);
legend7->SetFillColor(kWhite);

FileTrueEle.cd(); 

h_eleEcalIso_matchLQ->SetStats(0);
h_eleEcalIso_matchLQ->SetLineColor(kBlue);
h_eleEcalIso_matchLQ->SetLineWidth(2);
h_eleEcalIso_matchLQ->GetXaxis()->SetTitle("EcalIso");
h_eleEcalIso_matchLQ->GetYaxis()->SetTitle("a.u.");
h_eleEcalIso_matchLQ->DrawNormalized("HIST");

legend7->AddEntry(h_eleEcalIso_matchLQ,"electrons from LQ","f");

FileFakeEle.cd();
h_eleEcalIso_NOmatchLQ->SetFillStyle(3003);
h_eleEcalIso_NOmatchLQ->SetFillColor(kRed);
h_eleEcalIso_NOmatchLQ->DrawNormalized("sameHIST");

legend7->AddEntry(h_eleEcalIso_NOmatchLQ,"fake electrons from QCD jets","f");

legend7->Draw();

c7.SaveAs("h_eleEcalIso.gif");

}
