#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH2F.h"
#include "TF1.h"
#include "TString.h"
#include "TMath.h"


void EffCurves(TString filename="EffstudiesMatchingDebugTestAllRunsAllTracksAllY_2017H_0_9999999.root")
{
  TFile *f1 = TFile::Open(filename);

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);

  TH1F *hn13 = (TH1F *)f1->Get("h1d3num");
  TH1F *hd13 = (TH1F *)f1->Get("h1d3den");

  TH1F *hn1103 = (TH1F *)f1->Get("h1d103num");
  TH1F *hd1103 = (TH1F *)f1->Get("h1d103den");

  hn13->Sumw2(); hd13->Sumw2(); 
  hn1103->Sumw2(); hd1103->Sumw2();

  hn13->Rebin(2); 
  hn1103->Rebin(2);
  hd13->Rebin(2);
  hd1103->Rebin(2);

  hn13->Divide(hd13); 
  hn1103->Divide(hd1103);

  auto effErf = [](double* x, double* p) {
    return (TMath::Erf((x[0] - p[0]) / p[1]) + 1) / 2. * p[2];
  };
  auto effErf2 = [](double* x, double* p) {
    return (TMath::Erf((x[0] - p[0]) / p[1]) + 1) / 2. * p[2];
  };

  TF1* myErf = new TF1("myErf", effErf, 0.04, 0.1, 3);
  TF1* myErf2 = new TF1("myErf2", effErf2, 0.04, 0.1, 3);

  hn13->SetMinimum(0); hn1103->SetMinimum(0);
  hn13->SetMaximum(1.05); hn1103->SetMaximum(1.05);
  c1->cd(1);
  hn13->SetStats(0); hn13->SetXTitle("#xi"); hn13->SetYTitle("Efficiency"); hn13->SetTitle("2017B, 45");
  hn13->GetXaxis()->SetRangeUser(0.02,0.15);
  hn13->SetMarkerColor(1); hn13->SetMarkerStyle(20); hn13->SetLineWidth(3); hn13->SetLineColor(1); hn13->Draw("e");
  //  hn13->Fit(myErf,"","",0.04,0.1); 
  c1->cd(2);
  hn1103->SetStats(0); hn1103->SetXTitle("#xi"); hn1103->SetYTitle("Efficiency"); hn1103->SetTitle("2017B, 56");
  hn1103->GetXaxis()->SetRangeUser(0.02,0.2);
  hn1103->SetMarkerColor(1); hn1103->SetMarkerStyle(20); hn1103->SetLineWidth(103); hn1103->SetLineColor(1); hn1103->Draw("e");
  //  hn1103->Fit(myErf2,"","",0.04,0.1);

  TFile *fz = new TFile("FinalOutputHistsPixelMethod.root","RECREATE");
  hn13->Write();
  hn1103->Write();
  fz->Close();

}

void Eff2D(TString filename="EffstudiesMatchingDebugTestAllRunsAllTracksAllY_2017H_0_9999999.root")
{
  TFile *f1 = TFile::Open(filename);

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);

  TH2F *hn13 = (TH2F *)f1->Get("h2d3num");
  TH2F *hd13 = (TH2F *)f1->Get("h2d3den");

  TH2F *hn1103 = (TH2F *)f1->Get("h2d103num");
  TH2F *hd1103 = (TH2F *)f1->Get("h2d103den");

  hn13->Sumw2(); hd13->Sumw2();
  hn1103->Sumw2(); hd1103->Sumw2();

  hn13->Divide(hd13);
  hn1103->Divide(hd1103);

  hn13->GetXaxis()->SetRangeUser(40,55);
  hn1103->GetXaxis()->SetRangeUser(40,55);
  hn13->GetYaxis()->SetRangeUser(-5,10);
  hn1103->GetYaxis()->SetRangeUser(-5,10);

  hn13->SetStats(0); 
  hn1103->SetStats(0);
  hn13->SetXTitle("x(pixels) [mm]");
  hn1103->SetXTitle("x(pixels) [mm]");
  hn13->SetYTitle("y(pixels) [mm]");
  hn1103->SetYTitle("y(pixels) [mm]");
  hn13->SetTitle("45");
  hn1103->SetTitle("56");

  hn13->SetMaximum(1.0);
  hn1103->SetMaximum(1.0);

  c1->cd(1);
  hn13->Draw("col2z");
  c1->cd(2);
  hn1103->Draw("col2z");
}

void EffByPeriod()
{
  TFile *f1, *f2, *f3, *f4, *f5;
  f1 = TFile::Open("EffstudiesMatchingDebugTest_2017B_0_999999.root");
  f2 = TFile::Open("EffstudiesMatchingDebugTest_2017C_0_999999_NoRunsRemoved.root");
  f3 = TFile::Open("EffstudiesMatchingDebugTest_2017D_0_999999_NoRunsRemoved.root");
  //  f2 = TFile::Open("EffstudiesMatchingDebugTest_2017C_0_999999.root");
  //  f3 = TFile::Open("EffstudiesMatchingDebugTest_2017D_0_999999.root");
  f4 = TFile::Open("EffstudiesMatchingDebugTest_2017E_0_999999.root");
  //  f4 = TFile::Open("EffstudiesMatchingDebugTest_2017E_0_999999.root");
  f5 = TFile::Open("EffstudiesMatchingDebugTest_2017F_0_999999_NoRunsRemoved.root");
  //  f5 = TFile::Open("EffstudiesMatchingDebugTest_2017F_0_999999.root");

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);

  TH1F *hn13b = (TH1F *)f1->Get("h1d3num");
  TH1F *hd13b = (TH1F *)f1->Get("h1d3den");
  TH1F *hn1103b = (TH1F *)f1->Get("h1d103num");
  TH1F *hd1103b = (TH1F *)f1->Get("h1d103den");

  TH1F *hn13c = (TH1F *)f2->Get("h1d3num");
  TH1F *hd13c = (TH1F *)f2->Get("h1d3den");
  TH1F *hn1103c = (TH1F *)f2->Get("h1d103num");
  TH1F *hd1103c = (TH1F *)f2->Get("h1d103den");

  TH1F *hn13d = (TH1F *)f3->Get("h1d3num");
  TH1F *hd13d = (TH1F *)f3->Get("h1d3den");
  TH1F *hn1103d = (TH1F *)f3->Get("h1d103num");
  TH1F *hd1103d = (TH1F *)f3->Get("h1d103den");

  TH1F *hn13e = (TH1F *)f4->Get("h1d3num");
  TH1F *hd13e = (TH1F *)f4->Get("h1d3den");
  TH1F *hn1103e = (TH1F *)f4->Get("h1d103num");
  TH1F *hd1103e = (TH1F *)f4->Get("h1d103den");

  TH1F *hn13f = (TH1F *)f5->Get("h1d3num");
  TH1F *hd13f = (TH1F *)f5->Get("h1d3den");
  TH1F *hn1103f = (TH1F *)f5->Get("h1d103num");
  TH1F *hd1103f = (TH1F *)f5->Get("h1d103den");


  hn13b->Sumw2(); hd13b->Sumw2();
  hn1103b->Sumw2(); hd1103b->Sumw2();
  hn13b->Rebin(2);
  hn1103b->Rebin(2);
  hd13b->Rebin(2);
  hd1103b->Rebin(2);
  hn13b->Divide(hd13b);
  hn1103b->Divide(hd1103b);

  hn13c->Sumw2(); hd13c->Sumw2();
  hn1103c->Sumw2(); hd1103c->Sumw2();
  hn13c->Rebin(2);
  hn1103c->Rebin(2);
  hd13c->Rebin(2);
  hd1103c->Rebin(2);
  hn13c->Divide(hd13c);
  hn1103c->Divide(hd1103c);
  
  hn13d->Sumw2(); hd13d->Sumw2();
  hn1103d->Sumw2(); hd1103d->Sumw2();
  hn13d->Rebin(2);
  hn1103d->Rebin(2);
  hd13d->Rebin(2);
  hd1103d->Rebin(2);
  hn13d->Divide(hd13d);
  hn1103d->Divide(hd1103d);

  hn13e->Sumw2(); hd13e->Sumw2();
  hn1103e->Sumw2(); hd1103e->Sumw2();
  hn13e->Rebin(2);
  hn1103e->Rebin(2);
  hd13e->Rebin(2);
  hd1103e->Rebin(2);
  hn13e->Divide(hd13e);
  hn1103e->Divide(hd1103e);

  hn13f->Sumw2(); hd13f->Sumw2();
  hn1103f->Sumw2(); hd1103f->Sumw2();
  hn13f->Rebin(2);
  hn1103f->Rebin(2);
  hd13f->Rebin(2);
  hd1103f->Rebin(2);
  hn13f->Divide(hd13f);
  hn1103f->Divide(hd1103f);

  hn13b->GetXaxis()->SetRangeUser(0.02,0.15);
  hn1103b->GetXaxis()->SetRangeUser(0.02,0.2);
  hn13c->GetXaxis()->SetRangeUser(0.02,0.15);
  hn1103c->GetXaxis()->SetRangeUser(0.02,0.2);
  hn13d->GetXaxis()->SetRangeUser(0.02,0.15);
  hn1103d->GetXaxis()->SetRangeUser(0.02,0.2);
  hn13e->GetXaxis()->SetRangeUser(0.02,0.15);
  hn1103e->GetXaxis()->SetRangeUser(0.02,0.2);
  hn13f->GetXaxis()->SetRangeUser(0.02,0.15);
  hn1103f->GetXaxis()->SetRangeUser(0.02,0.2);


  hn13b->SetMarkerColor(1); hn13b->SetLineColor(1); hn13b->SetMarkerStyle(20); hn13b->SetLineWidth(3);
  hn13c->SetMarkerColor(2); hn13c->SetLineColor(2); hn13c->SetMarkerStyle(21); hn13c->SetLineWidth(3);
  hn13d->SetMarkerColor(3); hn13d->SetLineColor(3); hn13d->SetMarkerStyle(22); hn13d->SetLineWidth(3);
  hn13e->SetMarkerColor(4); hn13e->SetLineColor(4); hn13e->SetMarkerStyle(23); hn13e->SetLineWidth(3);
  hn13f->SetMarkerColor(6); hn13f->SetLineColor(6); hn13f->SetMarkerStyle(24); hn13f->SetLineWidth(3);

  hn1103b->SetMarkerColor(1); hn1103b->SetLineColor(1); hn1103b->SetMarkerStyle(20); hn1103b->SetLineWidth(3);
  hn1103c->SetMarkerColor(2); hn1103c->SetLineColor(2); hn1103c->SetMarkerStyle(21); hn1103c->SetLineWidth(3);
  hn1103d->SetMarkerColor(3); hn1103d->SetLineColor(3); hn1103d->SetMarkerStyle(22); hn1103d->SetLineWidth(3);
  hn1103e->SetMarkerColor(4); hn1103e->SetLineColor(4); hn1103e->SetMarkerStyle(23); hn1103e->SetLineWidth(3);
  hn1103f->SetMarkerColor(6); hn1103f->SetLineColor(6); hn1103f->SetMarkerStyle(24); hn1103f->SetLineWidth(3);

  hn13b->SetMaximum(1.0); 
  hn1103b->SetMaximum(1.0);
  hn13b->SetStats(0);
  hn1103b->SetStats(0);
  hn13b->SetXTitle("#xi");
  hn1103b->SetXTitle("#xi");

  c1->cd(1);
  hn13b->Draw("e");
  hn13c->Draw("esame");
  hn13d->Draw("esame");
  hn13e->Draw("esame");
  hn13f->Draw("esame");

  c1->cd(2);
  hn1103b->Draw("e");
  hn1103c->Draw("esame");
  hn1103d->Draw("esame");
  hn1103e->Draw("esame");
  hn1103f->Draw("esame");

  TLegend *l1 = new TLegend(0.6,0.2,0.9,0.5);
  l1->AddEntry(hn13b,"2017B, excluding runs with 45 RP extracted");
  l1->AddEntry(hn13c,"2017C");
  l1->AddEntry(hn13d,"2017D");
  l1->AddEntry(hn13e,"2017E, excluding runs with 45 RP extracted");
  l1->AddEntry(hn13f,"2017F");
  l1->Draw("same");

}

void EffByDataset()
{
  TFile *f1;
  TFile *f2;
  TFile *f3;
  //  f1 = TFile::Open("EffstudiesMatchingDebugTest_2017E_0_999999.root");
  //  f2 = TFile::Open("EffstudiesMatchingDebugTest_BTagCSV_2017E_0_999999.root");
  f1 = TFile::Open("EffstudiesMatchingDebugTestAllRunsAllTracksAllY_NOTrigger_2017H_0_9999999.root");
  f2 = TFile::Open("EffstudiesMatchingDebugTestAllRunsAllTracksAllY_Run2_2017_2017H_0_9999999.root");
  f3 = TFile::Open("EffstudiesMatchingDebugTestAllRunsAllTracksAllY_AllTrk_2017H_0_9999999.root");

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);

  TH1F *hn13el = (TH1F *)f1->Get("h1d3num");
  TH1F *hd13el = (TH1F *)f1->Get("h1d3den");

  TH1F *hn1103el = (TH1F *)f1->Get("h1d103num");
  TH1F *hd1103el = (TH1F *)f1->Get("h1d103den");
  hn13el->Sumw2(); hd13el->Sumw2();
  hn1103el->Sumw2(); hd1103el->Sumw2();
  hn13el->Rebin(2);
  hn1103el->Rebin(2);
  hd13el->Rebin(2);
  hd1103el->Rebin(2);
  hn13el->Divide(hd13el);
  hn1103el->Divide(hd1103el);

  TH1F *hn13b = (TH1F *)f2->Get("h1d3num");
  TH1F *hd13b = (TH1F *)f2->Get("h1d3den");
  TH1F *hn1103b = (TH1F *)f2->Get("h1d103num");
  TH1F *hd1103b = (TH1F *)f2->Get("h1d103den");
  hn13b->Sumw2(); hd13b->Sumw2();
  hn1103b->Sumw2(); hd1103b->Sumw2();
  hn13b->Rebin(2);
  hn1103b->Rebin(2);
  hd13b->Rebin(2);
  hd1103b->Rebin(2);
  hn13b->Divide(hd13b);
  hn1103b->Divide(hd1103b);

  TH1F *hn13j = (TH1F *)f3->Get("h1d3num");
  TH1F *hd13j = (TH1F *)f3->Get("h1d3den");
  TH1F *hn1103j = (TH1F *)f3->Get("h1d103num");
  TH1F *hd1103j = (TH1F *)f3->Get("h1d103den");
  hn13j->Sumw2(); hd13j->Sumw2();
  hn1103j->Sumw2(); hd1103j->Sumw2();
  hn13j->Rebin(2);
  hn1103j->Rebin(2);
  hd13j->Rebin(2);
  hd1103j->Rebin(2);
  hn13j->Divide(hd13j);
  hn1103j->Divide(hd1103j);


  hn13el->SetMinimum(0); hn1103el->SetMinimum(0);
  hn13el->SetMaximum(1.05); hn1103el->SetMaximum(1.05);
  c1->cd(1);
  hn13el->SetStats(0); hn13el->SetXTitle("#xi"); hn13el->SetYTitle("Efficiency"); hn13el->SetTitle("2017H, 45");
  hn13el->GetXaxis()->SetRangeUser(0.02,0.15);
  hn13el->SetMarkerColor(1); hn13el->SetMarkerStyle(20); hn13el->SetLineWidth(3); hn13el->SetLineColor(1); hn13el->Draw("e");

  hn13b->SetMarkerColor(9); hn13b->SetMarkerStyle(21); hn13b->SetLineWidth(3); hn13b->SetLineColor(9); hn13b->Draw("esame");
  //  hn13j->SetMarkerColor(8); hn13j->SetMarkerStyle(22); hn13j->SetLineWidth(3); hn13j->SetLineColor(8); hn13j->Draw("esame");


  c1->cd(2);
  hn1103el->SetStats(0); hn1103el->SetXTitle("#xi"); hn1103el->SetYTitle("Efficiency"); hn1103el->SetTitle("2017H, 56");
  hn1103el->GetXaxis()->SetRangeUser(0.02,0.2);
  hn1103el->SetMarkerColor(1); hn1103el->SetMarkerStyle(20); hn1103el->SetLineWidth(3); hn1103el->SetLineColor(1); hn1103el->Draw("e");

  hn1103b->SetMarkerColor(9); hn1103b->SetMarkerStyle(21); hn1103b->SetLineWidth(3); hn1103b->SetLineColor(9); hn1103b->Draw("esame");
  //  hn1103j->SetMarkerColor(8); hn1103j->SetMarkerStyle(22); hn1103j->SetLineWidth(3); hn1103j->SetLineColor(8); hn1103j->Draw("esame");



  TLegend *l1 = new TLegend(0.6,0.2,0.9,0.5);
  l1->AddEntry(hn1103el,"2017H, NoTrigger");
  l1->AddEntry(hn1103b,"2017H, Trigger");
  l1->AddEntry(hn1103j,"2017H, All tracks");
  //  l1->Draw("same");


}

void EffCategories(TString filename="EffstudiesMatchingDebugTestAllRunsAllTracksAllY_2017H_0_9999999.root")
{
  TFile * f1 = TFile::Open(filename);


  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);

  TH1F *hn13 = (TH1F *)f1->Get("h1d3num");
  TH1F *hd13 = (TH1F *)f1->Get("h1d3den");
  TH1F *hn13toofull = (TH1F *)f1->Get("h1d3toofull");
  TH1F *hn13planes = (TH1F *)f1->Get("h1d3planes");
  TH1F *hn13nomatch = (TH1F *)f1->Get("h1d3matchfail");

  TH1F *hfail3 = (TH1F *)hd13->Clone("hfail3");

  c1->cd(1);
  hd13->Rebin(5); hn13toofull->Rebin(5); hn13planes->Rebin(5); hn13->Rebin(5); hn13nomatch->Rebin(5);

  hfail3->Rebin(5);
  hfail3->Add(hn13,-1);


  hd13->Sumw2(); hd13->Add(hn13toofull); hd13->Add(hn13planes); 
  hd13->SetLineWidth(3); hd13->SetLineColor(1); hd13->Draw("hist");  
  hn13->SetLineWidth(3); hn13->SetLineColor(6); hn13->Draw("histsame");
  hn13nomatch->SetLineWidth(3); hn13nomatch->SetLineColor(3); hn13nomatch->Draw("histsame");
  hn13toofull->SetLineWidth(3); hn13toofull->Sumw2(); hn13toofull->Add(hn13planes); hn13toofull->SetLineColor(2); hn13toofull->Draw("histsame");
  hfail3->SetLineWidth(3); hfail3->SetLineColor(4); hfail3->Draw("histsame");

  hd13->SetStats(0); hd13->SetXTitle("#xi"); hd13->SetYTitle("Events"); hd13->SetTitle("2017B, 45");

  TLegend *l1 = new TLegend(0.6,0.6,0.9,0.9);
  l1->AddEntry(hd13,"All single pixel tracks");
  l1->AddEntry(hn13,"Matching strip tracks");
  l1->AddEntry(hn13nomatch,"Strip tracks failing #xi match");
  l1->AddEntry(hn13toofull,"Strip multi-track events");
  l1->AddEntry(hfail3,"Failing events");
  l1->Draw("same");

  c1->cd(2);
  TH1F *hn1103 = (TH1F *)f1->Get("h1d103num");
  TH1F *hd1103 = (TH1F *)f1->Get("h1d103den");
  TH1F *hn1103toofull = (TH1F *)f1->Get("h1d103toofull");
  TH1F *hn1103planes = (TH1F *)f1->Get("h1d103planes");
  TH1F *hn1103nomatch = (TH1F *)f1->Get("h1d103matchfail");

  TH1F *hfail103 = (TH1F *)hd1103->Clone("hfail103");

  hd1103->Rebin(5); hn1103toofull->Rebin(5); hn1103planes->Rebin(5); hn1103->Rebin(5); hn1103nomatch->Rebin(5);

  hfail103->Rebin(5);
  hfail103->Add(hn1103,-1);


  hd1103->Sumw2(); hd1103->Add(hn1103toofull); hd1103->Add(hn1103planes);
  hd1103->SetLineWidth(3); hd1103->SetLineColor(1); hd1103->Draw("hist");
  hn1103->SetLineWidth(3); hn1103->SetLineColor(6); hn1103->Draw("histsame");
  hn1103nomatch->SetLineWidth(3); hn1103nomatch->SetLineColor(3); hn1103nomatch->Draw("histsame");
  hn1103toofull->SetLineWidth(3); hn1103toofull->Sumw2(); hn1103toofull->Add(hn1103planes); hn1103toofull->SetLineColor(2); hn1103toofull->Draw("histsame");
  hfail103->SetLineWidth(3); hfail103->SetLineColor(4); hfail103->Draw("histsame");

  hd1103->SetStats(0); hd1103->SetXTitle("#xi"); hd1103->SetYTitle("Events"); hd1103->SetTitle("2017B, 56");

}
