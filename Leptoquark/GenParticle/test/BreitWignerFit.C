{
gROOT->Reset();

TFile f("GenParticle.root");
mygenplots.cd();
f.ls();

TCanvas c1;
c1->SetLeftMargin(0.117547);
c1->SetRightMargin(0.0817717);
c1->SetTopMargin(0.0837563);
c1->SetBottomMargin(0.116751);
c1->SetLogy();

TF1 bw("bw","[0]/( (x-[1])*(x-[1]) + 0.25*[2]*[2] )",200.,300.);
bw->SetParameter(0, 100);
bw->SetParameter(1, 250);
bw->SetParameter(2, 1);
h_invMass_LQ->Fit("bw","R");

// cout << bw->GetParameter(0) << endl;
// cout << bw->GetParameter(1) << endl;
// cout << bw->GetParameter(2) << endl;


h_invMass_LQ->GetXaxis()->SetRangeUser(240,260);

h_invMass_LQ->GetXaxis()->SetTitle("LQ mass (GeV)");
h_invMass_LQ->GetYaxis()->SetTitle("number of entries");


}
