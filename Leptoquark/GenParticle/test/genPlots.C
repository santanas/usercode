{
gROOT->Reset();

TFile f("GenParticle.root");
mygenplots.cd();
f.ls();

int rebin=2;

//LQ pT
TCanvas c1;
c1->SetLeftMargin(0.117547);
c1->SetRightMargin(0.0817717);
c1->SetTopMargin(0.0837563);
c1->SetBottomMargin(0.116751);
c1->Divide(2,2);

c1.cd(1);
h_pT_LQ->Rebin(rebin);
h_pT_LQ->Sumw2();
h_pT_LQ->SetTitle(0);

h_pT_LQ->GetXaxis()->SetTitle("LQ pT (GeV)");
h_pT_LQ->GetYaxis()->SetTitle("number of entries");

h_pT_LQ->GetXaxis()->SetTitleOffset(1.2);
h_pT_LQ->GetYaxis()->SetTitleOffset(1.2);

h_pT_LQ->Draw("HISTE");


//lepton pT
c1.cd(2);
h_pT_lepton->Rebin(rebin);
h_pT_lepton->Sumw2();
h_pT_lepton->SetTitle(0);

h_pT_lepton->GetXaxis()->SetTitle("lepton pT (GeV)");
h_pT_lepton->GetYaxis()->SetTitle("number of entries");

h_pT_lepton->GetXaxis()->SetTitleOffset(1.2);
h_pT_lepton->GetYaxis()->SetTitleOffset(1.2);

h_pT_lepton->Draw("HISTE");


//quark pT
c1.cd(3);
h_pT_quark->Rebin(rebin);
h_pT_quark->Sumw2();
h_pT_quark->SetTitle(0);

h_pT_quark->GetXaxis()->SetTitle("quark pT (GeV)");
h_pT_quark->GetYaxis()->SetTitle("number of entries");

h_pT_quark->GetXaxis()->SetTitleOffset(1.2);
h_pT_quark->GetYaxis()->SetTitleOffset(1.2);

h_pT_quark->Draw("HISTE");


//LQ eta
TCanvas c2;
c2->SetLeftMargin(0.117547);
c2->SetRightMargin(0.0817717);
c2->SetTopMargin(0.0837563);
c2->SetBottomMargin(0.116751);
c2->Divide(2,2);

c2.cd(1);
h_eta_LQ->Rebin(rebin);
h_eta_LQ->Sumw2();
h_eta_LQ->SetTitle(0);

h_eta_LQ->GetXaxis()->SetTitle("LQ \\eta");
h_eta_LQ->GetYaxis()->SetTitle("number of entries");

h_eta_LQ->GetXaxis()->SetTitleOffset(1.2);
h_eta_LQ->GetYaxis()->SetTitleOffset(1.2);

h_eta_LQ->Draw("HISTE");


//lepton eta
c2.cd(2);
h_eta_lepton->Rebin(rebin);
h_eta_lepton->Sumw2();
h_eta_lepton->SetTitle(0);

h_eta_lepton->GetXaxis()->SetTitle("lepton \\eta");
h_eta_lepton->GetYaxis()->SetTitle("number of entries");

h_eta_lepton->GetXaxis()->SetTitleOffset(1.2);
h_eta_lepton->GetYaxis()->SetTitleOffset(1.2);

h_eta_lepton->Draw("HISTE");


//quark eta
c2.cd(3);
h_eta_quark->Rebin(rebin);
h_eta_quark->Sumw2();
h_eta_quark->SetTitle(0);

h_eta_quark->GetXaxis()->SetTitle("quark \\eta");
h_eta_quark->GetYaxis()->SetTitle("number of entries");

h_eta_quark->GetXaxis()->SetTitleOffset(1.2);
h_eta_quark->GetYaxis()->SetTitleOffset(1.2);

h_eta_quark->Draw("HISTE");


//LQ phi
TCanvas c3;
c3->SetLeftMargin(0.117547);
c3->SetRightMargin(0.0817717);
c3->SetTopMargin(0.0837563);
c3->SetBottomMargin(0.116751);
c3->Divide(2,2);

c3.cd(1);
h_phi_LQ->Rebin(rebin);
h_phi_LQ->Sumw2();
h_phi_LQ->SetTitle(0);

h_phi_LQ->GetXaxis()->SetTitle("LQ \\phi");
h_phi_LQ->GetYaxis()->SetTitle("number of entries");

h_phi_LQ->GetXaxis()->SetTitleOffset(1.2);
h_phi_LQ->GetYaxis()->SetTitleOffset(1.2);

h_phi_LQ->Draw("HISTE");


//lepton phi
c3.cd(2);
h_phi_lepton->Rebin(rebin);
h_phi_lepton->Sumw2();
h_phi_lepton->SetTitle(0);

h_phi_lepton->GetXaxis()->SetTitle("lepton \\phi");
h_phi_lepton->GetYaxis()->SetTitle("number of entries");

h_phi_lepton->GetXaxis()->SetTitleOffset(1.2);
h_phi_lepton->GetYaxis()->SetTitleOffset(1.2);

h_phi_lepton->Draw("HISTE");


//quark phi
c3.cd(3);
h_phi_quark->Rebin(rebin);
h_phi_quark->Sumw2();
h_phi_quark->SetTitle(0);

h_phi_quark->GetXaxis()->SetTitle("quark \\phi");
h_phi_quark->GetYaxis()->SetTitle("number of entries");

h_phi_quark->GetXaxis()->SetTitleOffset(1.2);
h_phi_quark->GetYaxis()->SetTitleOffset(1.2);

h_phi_quark->Draw("HISTE");




}
