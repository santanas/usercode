{
  gROOT->Reset();

  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.1f");
  gStyle->SetTextSize(50);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistFillColor(407);
  gROOT->ForceStyle();

  TFile *inputfile = TFile::Open("analyzeTree.root");

  TH1F *h_cat = (TH1F*)inputfile->Get("h_categories");

  TH2F *h_media_vs_test = (TH2F*)inputfile->Get("h2_MediaVotiEsami_vs_VotoTest");

  TH1F *h_diploma = (TH1F*)inputfile->Get("h1_VotoDiploma");
  TH1F *h_diploma_rescale = (TH1F*)inputfile->Get("h1_VotoDiploma_rescale");
  TH2F *h2_diploma_vs_test = (TH2F*)inputfile->Get("h2_VotoDiploma_rescale_vs_VotoTest");

  TH2F *h_cat_vs_test = (TH2F*)inputfile->Get("h2_categories_vs_VotoTest");
  TH2F *h_cat_vs_test_prob = (TH2F*)inputfile->Get("h2_categories_vs_VotoTest_prob");
  TH2F *h_cat_vs_test_combine_1_to_1_prob = (TH2F*)inputfile->Get("h2_categories_vs_VotoCombine_1_to_1_prob");
  TH2F *h_cat_vs_test_combine_2_to_1_prob = (TH2F*)inputfile->Get("h2_categories_vs_VotoCombine_2_to_1_prob");

  TH1F *h_test_LC = (TH1F*)inputfile->Get("h_VotoTest_LC");
  TH1F *h_test_LF = (TH1F*)inputfile->Get("h_VotoTest_LF");
  TH1F *h_test_FC = (TH1F*)inputfile->Get("h_VotoTest_FC");
  TH1F *h_test_A1 = (TH1F*)inputfile->Get("h_VotoTest_A1");
  TH1F *h_test_A2 = (TH1F*)inputfile->Get("h_VotoTest_A2");

  TH1F *h_combine_1_to_1_LC = (TH1F*)inputfile->Get("h_VotoCombine_1_to_1_LC");
  TH1F *h_combine_1_to_1_LF = (TH1F*)inputfile->Get("h_VotoCombine_1_to_1_LF");
  TH1F *h_combine_1_to_1_FC = (TH1F*)inputfile->Get("h_VotoCombine_1_to_1_FC");
  TH1F *h_combine_1_to_1_A1 = (TH1F*)inputfile->Get("h_VotoCombine_1_to_1_A1");
  TH1F *h_combine_1_to_1_A2 = (TH1F*)inputfile->Get("h_VotoCombine_1_to_1_A2");

  TH1F *h_combine_2_to_1_LC = (TH1F*)inputfile->Get("h_VotoCombine_2_to_1_LC");
  TH1F *h_combine_2_to_1_LF = (TH1F*)inputfile->Get("h_VotoCombine_2_to_1_LF");
  TH1F *h_combine_2_to_1_FC = (TH1F*)inputfile->Get("h_VotoCombine_2_to_1_FC");
  TH1F *h_combine_2_to_1_A1 = (TH1F*)inputfile->Get("h_VotoCombine_2_to_1_A1");
  TH1F *h_combine_2_to_1_A2 = (TH1F*)inputfile->Get("h_VotoCombine_2_to_1_A2");



  //===

  TCanvas c_cat;
  h_cat->Draw("texthist");
  h_cat->SetTitle("");
  h_cat->GetYaxis()->SetRangeUser(0,h_cat->GetMaximum()*1.3);
  h_cat->GetYaxis()->SetTitle("Numero studenti");
  h_cat->GetXaxis()->SetTitle("Stato");
  h_cat->SetMarkerSize(1.4);
  float Ntot = h_cat->GetEntries(); 
  char c_Ntot[100];
  sprintf(c_Ntot,"Numero totale = %d",Ntot);

  TPaveText *pt = new TPaveText(0.7,0.67,0.98,0.98,"brNDC");
  pt->SetFillColor(0);
  pt->AddText(c_Ntot);
  pt->AddText("LC = laureato in corso");
  pt->AddText("LF = laureato fuori corso");
  pt->AddText("FC = fuori corso, non laureato");
  pt->AddText("A1 = abbandono primo anno");
  pt->AddText("A2 = abbandono secondo anno in poi");
  pt->SetAllWith("","align",11);
  pt->Draw();

  //===
  
  TCanvas c_cat_prob;
  TH1F *h_cat_prob = (TH1F*)h_cat->Clone();
  h_cat_prob->Scale(100/Ntot);
  h_cat_prob->GetYaxis()->SetRangeUser(0,h_cat_prob->GetMaximum()*1.3);
  h_cat_prob->GetYaxis()->SetTitle("Percentuale studenti [%]");
  h_cat_prob->SetMarkerSize(1.4);
  h_cat_prob->Draw("texthist");
  pt->Draw();

  //===

  TCanvas c_cat_vs_test_prob;
  c_cat_vs_test_prob->SetLeftMargin(0.0761494);
  c_cat_vs_test_prob->SetRightMargin(0.359195);
  c_cat_vs_test_prob->SetBottomMargin(0.10148);
  c_cat_vs_test_prob->SetTopMargin(0.0930232);

  h_cat_vs_test_prob->SetTitle("");
  h_cat_vs_test_prob->GetXaxis()->SetTitle("Voto test ingresso");
  h_cat_vs_test_prob->GetYaxis()->SetTitle("Stato");
  h_cat_vs_test_prob->GetZaxis()->SetTitle("Percentuale studenti per ciascun intervallo di voto [%]");
  h_cat_vs_test_prob->SetMarkerSize(1.4);
  h_cat_vs_test_prob->Draw("colztext");
  TPaveText *pt_2 = new TPaveText(0.78,0.28,0.99,0.72,"brNDC");
  pt_2->SetFillColor(0);
  pt_2->AddText(c_Ntot);
  /*
  pt_2->AddText("LC = laureato in corso");
  pt_2->AddText("LF = laureato fuori corso");
  pt_2->AddText("FC = fuori corso, non laureato");
  pt_2->AddText("A1 = abbandono primo anno");
  pt_2->AddText("A2 = abbandono secondo anno in poi");
  */
  pt_2->AddText("L = laureato");
  pt_2->AddText("FC = fuori corso, non laureato");
  pt_2->AddText("A = abbandono");
  pt_2->SetAllWith("","align",11);
  pt_2->Draw();

  //===
  
  TCanvas c_cat_vs_test_h1;

  TH1F *h_test_L = (TH1F*)h_test_LC->Clone();
  h_test_L->Add(h_test_LF);
  TH1F *h_test_A = (TH1F*)h_test_A1->Clone();
  h_test_A->Add(h_test_A2);

  h_test_L->GetXaxis()->SetTitle("Voto test ingresso");
  h_test_L->GetYaxis()->SetTitle("Numero studenti");
  
  h_test_L->SetLineColor(2);
  h_test_A->SetLineColor(4);
  h_test_FC->SetLineColor(3);
  h_test_L->SetLineWidth(2);
  h_test_A->SetLineWidth(2);
  h_test_FC->SetLineWidth(2);
  h_test_L->SetFillColor(2);
  h_test_A->SetFillColor(0);
  h_test_FC->SetFillColor(3);
  h_test_L->SetFillStyle(3004);
  h_test_A->SetFillStyle(0);
  h_test_FC->SetFillStyle(3002);
  h_test_L->Draw("hist");
  h_test_A->Draw("histsame");
  h_test_FC->Draw("histsame");
  h_test_L->GetYaxis()->SetRangeUser(0,Ntot/5);
 
  TLegend *legend=new TLegend(0.132184,0.72093,0.551724,0.877378);
  legend->SetTextFont(72);
  legend->SetTextSize(0.035);
  legend->AddEntry(h_test_L,"Laureati (LC + LF)","f");
  legend->AddEntry(h_test_A,"Abbandoni (A1 + A2)","f");
  legend->AddEntry(h_test_FC,"Fuori corso non laureati (FC)","f");
  legend->Draw();

  //===

  TCanvas c_prob_cat;

  c_prob_cat->SetGridy();

  TH1D *pfx_prob_cat_L = h_cat_vs_test_prob->ProjectionX("pfx_prob_cat_L",1,1);
  pfx_prob_cat_L->GetYaxis()->SetRangeUser(0,100);
  pfx_prob_cat_L->GetYaxis()->SetTitle("Probabilita' =  P ( Stato | voto ) [%]");
  pfx_prob_cat_L->GetXaxis()->SetTitle("Voto test ingresso");
  pfx_prob_cat_L->SetFillColor(0);
  pfx_prob_cat_L->SetLineColor(2);
  pfx_prob_cat_L->SetLineWidth(2);
  //pfx_prob_cat_L->GetXaxis()->SetRangeUser(5,26);
  pfx_prob_cat_L->Draw();

  TH1D *pfx_prob_cat_FC = h_cat_vs_test_prob->ProjectionX("pfx_prob_cat_FC",2,2);
  pfx_prob_cat_FC->SetFillColor(0);
  pfx_prob_cat_FC->SetLineColor(3);
  pfx_prob_cat_FC->SetLineWidth(2);
  pfx_prob_cat_FC->Draw("same");

  TH1D *pfx_prob_cat_A = h_cat_vs_test_prob->ProjectionX("pfx_prob_cat_A",3,3);
  pfx_prob_cat_A->SetFillColor(0);
  pfx_prob_cat_A->SetLineColor(4);
  pfx_prob_cat_A->SetLineWidth(2);
  pfx_prob_cat_A->Draw("same");

  TLegend *legend0=new TLegend(0.57,0.82,0.99,0.98);
  legend0->SetTextFont(72);
  legend0->SetTextSize(0.035);
  legend0->AddEntry(pfx_prob_cat_L,"Laureato/a","f");
  legend0->AddEntry(pfx_prob_cat_FC,"Fuori corso","f");
  legend0->AddEntry(pfx_prob_cat_A,"Abbandono","f");
  legend0->Draw();

  //===

  TCanvas c_cat_vs_test_h1_LC_vs_A1;

  h_test_LC->SetLineColor(2);
  h_test_LC->SetLineWidth(2);
  h_test_LC->SetFillColor(2);
  h_test_LC->SetFillStyle(3004);
  h_test_LC->Draw("hist");

  h_test_A1->SetLineColor(4);
  h_test_A1->SetLineWidth(2);
  h_test_A1->SetFillColor(0);
  h_test_A1->SetFillStyle(0);
  h_test_A1->Draw("histsame");

  h_test_LC->GetYaxis()->SetRangeUser(0,Ntot/5);

  h_test_LC->GetXaxis()->SetTitle("Voto test ingresso");
  h_test_LC->GetYaxis()->SetTitle("Numero studenti");

  TLegend *legend1=new TLegend(0.132184,0.72093,0.551724,0.877378);
  legend1->SetTextFont(72);
  legend1->SetTextSize(0.035);
  legend1->AddEntry(h_test_L,"Laureati in corso (LC)","f");
  legend1->AddEntry(h_test_A,"Abbandoni primo anno (A1)","f");
  legend1->Draw();

  //==

  TCanvas c_media_vs_test;
  c_media_vs_test->SetRightMargin(0.17);
  c_media_vs_test->SetGridx();
  c_media_vs_test->SetGridy();  

  /* 
  h_media_vs_test->Draw("colz");
  h_media_vs_test->GetXaxis()->SetTitle("Voto test ingresso");
  h_media_vs_test->GetYaxis()->SetTitle("Media voti (pesata con CFU) / 30");
  h_media_vs_test->GetZaxis()->SetTitle("Numero studenti");
  */

  TProfile *pfx_media_vs_test = h_media_vs_test->ProfileX("pfx_media_vs_test",2,-1,"");
  pfx_media_vs_test->GetXaxis()->SetTitle("Voto test ingresso");
  pfx_media_vs_test->GetYaxis()->SetTitle("Media voti (pesata con CFU) / 30");
  pfx_media_vs_test->GetYaxis()->SetRangeUser(18,31);
  pfx_media_vs_test->SetMarkerStyle(20);
  pfx_media_vs_test->SetMarkerSize(0.7);
  pfx_media_vs_test->Draw();

  /*
  TProfile *pfx_media_vs_test_all = h_media_vs_test->ProfileX("pfx_media_vs_test_all",1,-1,"");
  pfx_media_vs_test_all->SetMarkerStyle(22);
  pfx_media_vs_test_all->SetMarkerSize(0.7);
  pfx_media_vs_test_all->SetMarkerColor(2);
  pfx_media_vs_test_all->Draw("same");
  */

  /*
  TLegend *legend2=new TLegend(0.50,0.19,0.80,0.30);
  legend2->SetTextFont(72);
  legend2->SetTextSize(0.035);
  legend2->AddEntry(pfx_media_vs_test,"Profilo X (media > 0)","lp");
  legend2->AddEntry(pfx_media_vs_test_all,"Profilo X (media >= 0)","lp");
  legend2->Draw();
  */

  //==

  TCanvas c_media;
  TH1D *h_media = h_media_vs_test->ProjectionY("h_media",1,-1);  
  h_media->GetYaxis()->SetTitle("Numero studenti");
  h_media->GetXaxis()->SetTitle("Media voti (pesata con CFU) / 30");
  h_media->GetXaxis()->SetRangeUser(17,31);
  h_media->SetFillColor(2);
  h_media->SetFillStyle(3004);
  h_media->Draw();

  TCanvas c_test;
  TH1D *h_test = h_media_vs_test->ProjectionX("h_test",2,-1);
  h_test->GetYaxis()->SetTitle("Numero studenti");
  h_test->GetXaxis()->SetTitle("Voto test ingresso");
  h_test->SetFillColor(4);
  h_test->SetFillStyle(3004);
  h_test->Draw();

  //==

  TCanvas c_diploma;
  h_diploma->GetYaxis()->SetTitle("Numero studenti");
  h_diploma->GetXaxis()->SetTitle("Voto diploma (60-101)");
  h_diploma->SetFillColor(2);
  h_diploma->SetFillStyle(3004);
  h_diploma->Draw();

  TCanvas c_diploma_rescale;
  h_diploma_rescale->GetYaxis()->SetTitle("Numero studenti");
  h_diploma_rescale->GetXaxis()->SetTitle("Voto diploma riscalato (0-25)");
  h_diploma_rescale->SetFillColor(4);
  h_diploma_rescale->SetFillStyle(3004);
  h_diploma_rescale->Draw();

  TCanvas c_diploma_vs_test;
  c_diploma_vs_test->SetGridx();
  c_diploma_vs_test->SetGridy();

  h2_diploma_vs_test->GetXaxis()->SetTitle("Voto test ingresso");
  h2_diploma_vs_test->GetYaxis()->SetTitle("Voto diploma riscalato");
  h2_diploma_vs_test->SetMarkerStyle(20);
  h2_diploma_vs_test->SetMarkerSize(0.4);
  h2_diploma_vs_test->Draw("colz");

  //

  TCanvas c_cat_vs_combine_1_to_1_h1;

  TH1F *h_combine_1_to_1_L = (TH1F*)h_combine_1_to_1_LC->Clone();
  h_combine_1_to_1_L->Add(h_combine_1_to_1_LF);
  TH1F *h_combine_1_to_1_A = (TH1F*)h_combine_1_to_1_A1->Clone();
  h_combine_1_to_1_A->Add(h_combine_1_to_1_A2);

  h_combine_1_to_1_L->GetXaxis()->SetTitle("(Voto test + Voto Diploma) / 2");
  h_combine_1_to_1_L->GetYaxis()->SetTitle("Numero studenti");
  
  h_combine_1_to_1_L->SetLineColor(2);
  h_combine_1_to_1_A->SetLineColor(4);
  h_combine_1_to_1_FC->SetLineColor(3);
  h_combine_1_to_1_L->SetLineWidth(2);
  h_combine_1_to_1_A->SetLineWidth(2);
  h_combine_1_to_1_FC->SetLineWidth(2);
  h_combine_1_to_1_L->SetFillColor(2);
  h_combine_1_to_1_A->SetFillColor(0);
  h_combine_1_to_1_FC->SetFillColor(3);
  h_combine_1_to_1_L->SetFillStyle(3004);
  h_combine_1_to_1_A->SetFillStyle(0);
  h_combine_1_to_1_FC->SetFillStyle(3002);
  h_combine_1_to_1_L->Draw("hist");
  h_combine_1_to_1_A->Draw("histsame");
  h_combine_1_to_1_FC->Draw("histsame");
  
  h_combine_1_to_1_L->GetYaxis()->SetRangeUser(0,Ntot/5);

  TLegend *legend_1_1=new TLegend(0.132184,0.72093,0.551724,0.877378);
  legend_1_1->SetTextFont(72);
  legend_1_1->SetTextSize(0.035);
  legend_1_1->AddEntry(h_combine_1_to_1_L,"Laureati (LC + LF)","f");
  legend_1_1->AddEntry(h_combine_1_to_1_A,"Abbandoni (A1 + A2)","f");
  legend_1_1->AddEntry(h_combine_1_to_1_FC,"Fuori corso non laureati (FC)","f");
  legend_1_1->Draw();

  //==

  TCanvas c_cat_vs_combine_2_to_1_h1;

  TH1F *h_combine_2_to_1_L = (TH1F*)h_combine_2_to_1_LC->Clone();
  h_combine_2_to_1_L->Add(h_combine_2_to_1_LF);
  TH1F *h_combine_2_to_1_A = (TH1F*)h_combine_2_to_1_A1->Clone();
  h_combine_2_to_1_A->Add(h_combine_2_to_1_A2);

  h_combine_2_to_1_L->GetXaxis()->SetTitle("(Voto test *2 + Voto Diploma * 1) / 3");
  h_combine_2_to_1_L->GetYaxis()->SetTitle("Numero studenti");
  
  h_combine_2_to_1_L->SetLineColor(2);
  h_combine_2_to_1_A->SetLineColor(4);
  h_combine_2_to_1_FC->SetLineColor(3);
  h_combine_2_to_1_L->SetLineWidth(2);
  h_combine_2_to_1_A->SetLineWidth(2);
  h_combine_2_to_1_FC->SetLineWidth(2);
  h_combine_2_to_1_L->SetFillColor(2);
  h_combine_2_to_1_A->SetFillColor(0);
  h_combine_2_to_1_FC->SetFillColor(3);
  h_combine_2_to_1_L->SetFillStyle(3004);
  h_combine_2_to_1_A->SetFillStyle(0);
  h_combine_2_to_1_FC->SetFillStyle(3002);
  h_combine_2_to_1_L->Draw("hist");
  h_combine_2_to_1_A->Draw("histsame");
  h_combine_2_to_1_FC->Draw("histsame");

  h_combine_2_to_1_L->GetYaxis()->SetRangeUser(0,Ntot/5);
  
  TLegend *legend_2_to_1=new TLegend(0.132184,0.72093,0.551724,0.877378);
  legend_2_to_1->SetTextFont(72);
  legend_2_to_1->SetTextSize(0.035);
  legend_2_to_1->AddEntry(h_combine_2_to_1_L,"Laureati (LC + LF)","f");
  legend_2_to_1->AddEntry(h_combine_2_to_1_A,"Abbandoni (A1 + A2)","f");
  legend_2_to_1->AddEntry(h_combine_2_to_1_FC,"Fuori corso non laureati (FC)","f");
  legend_2_to_1->Draw();

  //===

  TCanvas c_prob_cat_combine_2_to_1;

  c_prob_cat_combine_2_to_1->SetGridy();

  TH1D *pfx_prob_cat_combine_2_to_1_L = h_cat_vs_test_combine_2_to_1_prob->ProjectionX("pfx_prob_cat_combine_2_to_1_L",1,1);
  pfx_prob_cat_combine_2_to_1_L->GetYaxis()->SetRangeUser(0,100);
  pfx_prob_cat_combine_2_to_1_L->GetYaxis()->SetTitle("Probabilita' =  P ( Stato | voto ) [%]");
  pfx_prob_cat_combine_2_to_1_L->GetXaxis()->SetTitle("(Voto test *2 + Voto Diploma * 1) / 3");
  pfx_prob_cat_combine_2_to_1_L->SetFillColor(0);
  pfx_prob_cat_combine_2_to_1_L->SetLineColor(2);
  pfx_prob_cat_combine_2_to_1_L->SetLineWidth(2);
  pfx_prob_cat_combine_2_to_1_L->Draw();

  TH1D *pfx_prob_cat_combine_2_to_1_FC = h_cat_vs_test_combine_2_to_1_prob->ProjectionX("pfx_prob_cat_combine_2_to_1_FC",2,2);
  pfx_prob_cat_combine_2_to_1_FC->SetFillColor(0);
  pfx_prob_cat_combine_2_to_1_FC->SetLineColor(3);
  pfx_prob_cat_combine_2_to_1_FC->SetLineWidth(2);
  pfx_prob_cat_combine_2_to_1_FC->Draw("same");

  TH1D *pfx_prob_cat_combine_2_to_1_A = h_cat_vs_test_combine_2_to_1_prob->ProjectionX("pfx_prob_cat_combine_2_to_1_A",3,3);
  pfx_prob_cat_combine_2_to_1_A->SetFillColor(0);
  pfx_prob_cat_combine_2_to_1_A->SetLineColor(4);
  pfx_prob_cat_combine_2_to_1_A->SetLineWidth(2);
  pfx_prob_cat_combine_2_to_1_A->Draw("same");

  TLegend *legend0_2_to_1=new TLegend(0.57,0.82,0.99,0.98);
  legend0_2_to_1->SetTextFont(72);
  legend0_2_to_1->SetTextSize(0.035);
  legend0_2_to_1->AddEntry(pfx_prob_cat_combine_2_to_1_L,"Laureato/a","f");
  legend0_2_to_1->AddEntry(pfx_prob_cat_combine_2_to_1_FC,"Fuori corso","f");
  legend0_2_to_1->AddEntry(pfx_prob_cat_combine_2_to_1_A,"Abbandono","f");
  legend0_2_to_1->Draw();

  //===

  TCanvas c_prob_cat_combine_1_to_1;

  c_prob_cat_combine_1_to_1->SetGridy();

  TH1D *pfx_prob_cat_combine_1_to_1_L = h_cat_vs_test_combine_1_to_1_prob->ProjectionX("pfx_prob_cat_combine_1_to_1_L",1,1);
  pfx_prob_cat_combine_1_to_1_L->GetYaxis()->SetRangeUser(0,100);
  pfx_prob_cat_combine_1_to_1_L->GetYaxis()->SetTitle("Probabilita' =  P ( Stato | voto ) [%]");
  pfx_prob_cat_combine_1_to_1_L->GetXaxis()->SetTitle("(Voto test + Voto Diploma) / 2");

  pfx_prob_cat_combine_1_to_1_L->SetFillColor(0);
  pfx_prob_cat_combine_1_to_1_L->SetLineColor(2);
  pfx_prob_cat_combine_1_to_1_L->SetLineWidth(2);
  pfx_prob_cat_combine_1_to_1_L->Draw();

  TH1D *pfx_prob_cat_combine_1_to_1_FC = h_cat_vs_test_combine_1_to_1_prob->ProjectionX("pfx_prob_cat_combine_1_to_1_FC",2,2);
  pfx_prob_cat_combine_1_to_1_FC->SetFillColor(0);
  pfx_prob_cat_combine_1_to_1_FC->SetLineColor(3);
  pfx_prob_cat_combine_1_to_1_FC->SetLineWidth(2);
  pfx_prob_cat_combine_1_to_1_FC->Draw("same");

  TH1D *pfx_prob_cat_combine_1_to_1_A = h_cat_vs_test_combine_1_to_1_prob->ProjectionX("pfx_prob_cat_combine_1_to_1_A",3,3);
  pfx_prob_cat_combine_1_to_1_A->SetFillColor(0);
  pfx_prob_cat_combine_1_to_1_A->SetLineColor(4);
  pfx_prob_cat_combine_1_to_1_A->SetLineWidth(2);
  pfx_prob_cat_combine_1_to_1_A->Draw("same");

  TLegend *legend0_1_to_1=new TLegend(0.57,0.82,0.99,0.98);
  legend0_1_to_1->SetTextFont(72);
  legend0_1_to_1->SetTextSize(0.035);
  legend0_1_to_1->AddEntry(pfx_prob_cat_combine_1_to_1_L,"Laureato/a","f");
  legend0_1_to_1->AddEntry(pfx_prob_cat_combine_1_to_1_FC,"Fuori corso","f");
  legend0_1_to_1->AddEntry(pfx_prob_cat_combine_1_to_1_A,"Abbandono","f");
  legend0_1_to_1->Draw();

  //==
  
  c_cat.SaveAs("cat.pdf");
  c_cat_prob.SaveAs("cat_prob.pdf");
  c_cat_vs_test_prob.SaveAs("cat_vs_test_prob.pdf");
  c_cat_vs_test_h1.SaveAs("cat_vs_test_h1.pdf");
  c_cat_vs_test_h1_LC_vs_A1.SaveAs("cat_vs_test_h1_LC_vs_A1.pdf");

  c_prob_cat.SaveAs("prob_cat.pdf");
  c_prob_cat_combine_1_to_1.SaveAs("prob_cat_combine_1_to_1.pdf");
  c_prob_cat_combine_2_to_1.SaveAs("prob_cat_combine_2_to_1.pdf");

  c_media_vs_test.SaveAs("media_vs_test.pdf");
  c_media.SaveAs("media.pdf");
  c_test.SaveAs("test.pdf");
  c_diploma.SaveAs("diploma.pdf");
  c_diploma_rescale.SaveAs("diploma_rescale.pdf");
  c_diploma_vs_test.SaveAs("diploma_vs_test.pdf");

  c_cat_vs_combine_1_to_1_h1.SaveAs("cat_vs_combine_1_to_1_h1.pdf");
  c_cat_vs_combine_2_to_1_h1.SaveAs("cat_vs_combine_2_to_1_h1.pdf");

}
