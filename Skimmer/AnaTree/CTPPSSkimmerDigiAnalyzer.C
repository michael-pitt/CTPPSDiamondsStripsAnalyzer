#define CTPPSSkimmerDigiAnalyzer_cxx
#include "CTPPSSkimmerDigiAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TH1.h>
#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLegend.h"

#include <TH2.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TSpline.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TF1.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TString.h>
#include <set>
#include <TString.h>
#include <TMinuit.h>
#include "TSystem.h"

#include <vector>
#include <stdlib.h>

using namespace std;

vector<TH1I*> hVector_h_cms_bx;
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch_getLeading;
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch_getTrailing;
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch_check;
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch_deltat;
vector<vector<vector<TH1D*> > > hVector_h_pl_getLeading;
vector<vector<vector<TH1D*> > > hVector_h_pl_getTrailing;
vector<vector<vector<TH1D*> > > hVector_h_pl_check;
vector<vector<vector<TH1D*> > > hVector_h_pl_result_leading;
vector<vector<vector<TH1D*> > > hVector_h_pl_result_trailing;
vector<vector<TH1D*> > hVector_h_arm_getLeading;
vector<vector<TH1D*> > hVector_h_arm_getTrailing;
vector<vector<TH1D*> > hVector_h_arm_check;
vector<string> Folders;

void CTPPSSkimmerDigiAnalyzer::CreateHistos(){

  Folders.push_back("step0");
  Folders.push_back("step1");
  Folders.push_back("step2");

  for (std::vector<std::string>::size_type i=0; i<Folders.size(); i++){

    vector < vector< vector<TH1D*> > > vec_cut_ch_getLeading;
    vector< vector<TH1D*> > vec_cut_pl_getLeading;
    vector< vector<TH1D*> > vec_cut_pl_result_leading;
    vector< vector<TH1D*> > vec_cut_pl_result_trailing;
    vector<TH1D*> vec_cut_arm_getLeading;

    vector < vector< vector<TH1D*> > > vec_cut_ch_getTrailing;
    vector< vector<TH1D*> > vec_cut_pl_getTrailing;
    vector<TH1D*> vec_cut_arm_getTrailing;

    vector < vector< vector<TH1D*> > > vec_cut_ch_deltat;
    vector < vector< vector<TH1D*> > > vec_cut_ch_check;
    vector< vector<TH1D*> > vec_cut_pl_check;
    vector<TH1D*> vec_cut_arm_check;

    char name[300];
    sprintf(name,"CMSBX_%s",Folders.at(i).c_str());
    TH1I *histo_cms_bx = new TH1I(name,";CMS BX; N events",3600,0,3600);
    hVector_h_cms_bx.push_back(histo_cms_bx);

    for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){

      vector< vector<TH1D*> > vec_arm_per_ch_getLeading;
      vector<TH1D*> vec_arm_per_pl_getLeading;

      vector< vector<TH1D*> > vec_arm_per_ch_getTrailing;
      vector<TH1D*> vec_arm_per_pl_getTrailing;

      vector< vector<TH1D*> > vec_arm_per_ch_deltat;
      vector< vector<TH1D*> > vec_arm_per_ch_check;
      vector<TH1D*> vec_arm_per_pl_check;
      vector<TH1D*> vec_arm_per_pl_result_leading;
      vector<TH1D*> vec_arm_per_pl_result_trailing;

      for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
	vector<TH1D*> vec_ch_getLeading;
	vector<TH1D*> vec_ch_getTrailing;
	vector<TH1D*> vec_ch_check;
	vector<TH1D*> vec_ch_deltat;
	for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	  sprintf(name,"getLeading_arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch_getLeading = new TH1D(name,";[ns]; N events",1250,0,125);
	  vec_ch_getLeading.push_back(histo_ch_getLeading);

	  sprintf(name,"getTrailing_arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch_getTrailing = new TH1D(name,";[ns]; N events",1500,0,200);
	  vec_ch_getTrailing.push_back(histo_ch_getTrailing);

	  sprintf(name,"check_arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch_check = new TH1D(name,"; ; N events",16,0,16);
	  vec_ch_check.push_back(histo_ch_check);

	  sprintf(name,"deltat_arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch_deltat = new TH1D(name,"; [ns]; N events",100,-50.,50.);
	  vec_ch_deltat.push_back(histo_ch_deltat);

	}
	vec_arm_per_ch_getLeading.push_back(vec_ch_getLeading);
	vec_arm_per_ch_getTrailing.push_back(vec_ch_getTrailing);
	vec_arm_per_ch_check.push_back(vec_ch_check);
	vec_arm_per_ch_deltat.push_back(vec_ch_deltat);

	sprintf(name,"getLeading_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_getLeading = new TH1D(name,";[ns]; N events",1250,0,125);
	vec_arm_per_pl_getLeading.push_back(histo_pl_getLeading);

	sprintf(name,"getTrailing_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_getTrailing = new TH1D(name,";[ns]; N events",1500,0,200);
	vec_arm_per_pl_getTrailing.push_back(histo_pl_getTrailing);

	sprintf(name,"check_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_check = new TH1D(name,"; ; N events",16,0,16);
	vec_arm_per_pl_check.push_back(histo_pl_check);

	sprintf(name,"result_leading_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_result_leading = new TH1D(name,"; Channel; Signal Fit, Mean [ns]",16,0,16);
	vec_arm_per_pl_result_leading.push_back(histo_pl_result_leading);

	sprintf(name,"result_trailing_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_result_trailing = new TH1D(name,"; Channel; Signal Fit, Mean [ns]",16,0,16);
	vec_arm_per_pl_result_trailing.push_back(histo_pl_result_trailing);

      }

      sprintf(name,"getLeading_arm%i_%s", arm_i, Folders.at(i).c_str());
      TH1D *histo_arm_getLeading = new TH1D(name,";[ns]; N events",1250,0,125);

      sprintf(name,"getTrailing_arm%i_%s", arm_i, Folders.at(i).c_str());
      TH1D *histo_arm_getTrailing = new TH1D(name,";[ns]; N events",1500,0,200);

      sprintf(name,"check_arm%i_%s", arm_i, Folders.at(i).c_str());
      TH1D *histo_arm_check = new TH1D(name,"; ; N events",16,0,16);

      vec_cut_ch_getLeading.push_back( vec_arm_per_ch_getLeading);
      vec_cut_pl_getLeading.push_back( vec_arm_per_pl_getLeading);
      vec_cut_pl_result_leading.push_back( vec_arm_per_pl_result_leading);
      vec_cut_pl_result_trailing.push_back( vec_arm_per_pl_result_trailing);
      vec_cut_arm_getLeading.push_back(histo_arm_getLeading);

      vec_cut_ch_getTrailing.push_back( vec_arm_per_ch_getTrailing);
      vec_cut_pl_getTrailing.push_back( vec_arm_per_pl_getTrailing);
      vec_cut_arm_getTrailing.push_back(histo_arm_getTrailing);

      vec_cut_ch_check.push_back( vec_arm_per_ch_check);
      vec_cut_pl_check.push_back( vec_arm_per_pl_check);
      vec_cut_arm_check.push_back(histo_arm_check);

      vec_cut_ch_deltat.push_back( vec_arm_per_ch_deltat);

    }
    hVector_h_ch_getLeading.push_back( vec_cut_ch_getLeading );
    hVector_h_pl_getLeading.push_back( vec_cut_pl_getLeading );
    hVector_h_pl_result_leading.push_back( vec_cut_pl_result_leading );
    hVector_h_pl_result_trailing.push_back( vec_cut_pl_result_trailing );
    hVector_h_arm_getLeading.push_back( vec_cut_arm_getLeading );

    hVector_h_ch_getTrailing.push_back( vec_cut_ch_getTrailing );
    hVector_h_pl_getTrailing.push_back( vec_cut_pl_getTrailing );
    hVector_h_arm_getTrailing.push_back( vec_cut_arm_getTrailing );

    hVector_h_ch_deltat.push_back( vec_cut_ch_deltat );
    hVector_h_ch_check.push_back( vec_cut_ch_check );
    hVector_h_pl_check.push_back( vec_cut_pl_check );
    hVector_h_arm_check.push_back( vec_cut_arm_check );

  }

}
void CTPPSSkimmerDigiAnalyzer::Loop(){

  CreateHistos();

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  // Loop over all events...
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%10000 == 0) cout << jentry << " out of "<<nentries << endl; 

    bool HLT_trigger = false;
    for (UInt_t j = 0; j < HLT->size(); ++j) {
      if(HLT->at(j)==1) HLT_trigger = true;
    }

    bool isolatedBx = false;
    for (UInt_t j = 0; j < getBx->size(); ++j) {
      //if(getBx->at(j)==7 || getBx->at(j)==47 || getBx->at(j)==87) isolatedBx = true;
        if(getBx->at(j)==1) isolatedBx = true;
    }

    bool isolatedBxCMS = false;
    //if(getBxCMS == 7 || getBxCMS == 47 || getBxCMS==87) isolatedBxCMS = true;
    if(getBxCMS == 1) isolatedBxCMS = true;

    FillHistos(0); 
    if(isolatedBxCMS){
      FillHistos(1); 
    }
    if(isolatedBxCMS && HLT_trigger){
      FillHistos(2);
    }

  } // Loop over all events...

  WriteHistos();

}

void CTPPSSkimmerDigiAnalyzer::FillHistos(int i){

  hVector_h_cms_bx[i]->Fill(getBxCMS);

  for (UInt_t j = 0; j < getLeading->size(); ++j){
    if(multiplehits->at(j)==0){

      if(channel->at(j)>20) continue;

      hVector_h_ch_getLeading[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(getLeading->at(j)*HPTDC_BIN_WIDTH_NS);
      hVector_h_pl_getLeading[i].at(arm->at(j)).at(plane->at(j))->Fill(getLeading->at(j)*HPTDC_BIN_WIDTH_NS);
      hVector_h_arm_getLeading[i].at(arm->at(j))->Fill(getLeading->at(j)*HPTDC_BIN_WIDTH_NS);

      hVector_h_ch_getTrailing[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(getTrailing->at(j)*HPTDC_BIN_WIDTH_NS);
      hVector_h_pl_getTrailing[i].at(arm->at(j)).at(plane->at(j))->Fill(getTrailing->at(j)*HPTDC_BIN_WIDTH_NS);
      hVector_h_arm_getTrailing[i].at(arm->at(j))->Fill(getTrailing->at(j)*HPTDC_BIN_WIDTH_NS);

      hVector_h_ch_deltat[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill((getLeading->at(j)-getTrailing->at(j))*HPTDC_BIN_WIDTH_NS);

      if(getLeading->at(j) != 0 && getTrailing->at(j) != 0){
	hVector_h_ch_check[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill("Both",1);
	hVector_h_pl_check[i].at(arm->at(j)).at(plane->at(j))->Fill("Both",1);
	hVector_h_arm_check[i].at(arm->at(j))->Fill("Both",1);
      }
      if(getLeading->at(j) != 0 && getTrailing->at(j) == 0){
	hVector_h_ch_check[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill("Only Leading Edge",1);
	hVector_h_pl_check[i].at(arm->at(j)).at(plane->at(j))->Fill("Only Leading Edge",1);
	hVector_h_arm_check[i].at(arm->at(j))->Fill("Only Leading Edge",1);
      }
      if(getLeading->at(j) == 0 && getTrailing->at(j) != 0){
	hVector_h_ch_check[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill("Only Trailing Edge",1);
	hVector_h_pl_check[i].at(arm->at(j)).at(plane->at(j))->Fill("Only Trailing Edge",1);
	hVector_h_arm_check[i].at(arm->at(j))->Fill("Only Trailing Edge",1);
      }

    }
  }


}

void CTPPSSkimmerDigiAnalyzer::WriteHistos(){

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();

  gStyle->SetStatY(0.9); // Set y-position (fraction of pad size)
  gStyle->SetStatX(0.4); // Set x-position (fraction of pad size)
  gStyle->SetStatW(0.15); // Set width of stat-box (fraction of pad size)
  gStyle->SetStatH(0.15);  // Set height of stat-box (fraction of pad size)

  std::ofstream outstring("fit_results.txt");

  bool save_picture = true;
  TCanvas *c1 = new TCanvas("Calibration","",200,10,600,400);

  TFile* f = new TFile("histo_run295977.root", "RECREATE");

  for (std::vector<std::string>::size_type i=0; i<Folders.size(); i++){

    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/", Folders.at(i).c_str()));
    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getLeading", Folders.at(i).c_str()));
    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getTrailing", Folders.at(i).c_str()));
    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/together", Folders.at(i).c_str()));
    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/check", Folders.at(i).c_str()));
    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/result", Folders.at(i).c_str()));
    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/deltat", Folders.at(i).c_str()));

    hVector_h_cms_bx[i]->Write();

    if(save_picture){
      hVector_h_cms_bx[i]->Draw();
      c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/%s.png", Folders.at(i).c_str(), hVector_h_cms_bx[i]->GetName()));
      c1->Modified();
      c1->Update();
    }

    for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){
      hVector_h_arm_getLeading[i].at(arm_i)->Write();
      hVector_h_arm_getTrailing[i].at(arm_i)->Write();

      if(save_picture){
	hVector_h_arm_getLeading[i].at(arm_i)->Draw();
	c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getLeading/%s.png", Folders.at(i).c_str(), hVector_h_arm_getLeading[i].at(arm_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_arm_getTrailing[i].at(arm_i)->Draw();
	c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getTrailing/%s.png", Folders.at(i).c_str(), hVector_h_arm_getTrailing[i].at(arm_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_arm_check[i].at(arm_i)->Draw();
	c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/check/%s.png", Folders.at(i).c_str(), hVector_h_arm_check[i].at(arm_i)->GetName()));
	c1->Modified();
	c1->Update();

      }

      for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
	hVector_h_pl_getLeading[i].at(arm_i).at(pl_i)->Write();
	hVector_h_pl_getTrailing[i].at(arm_i).at(pl_i)->Write();

	if(save_picture){
	  hVector_h_pl_getLeading[i].at(arm_i).at(pl_i)->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getLeading/%s.png", Folders.at(i).c_str(),hVector_h_pl_getLeading[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();

	  hVector_h_pl_getTrailing[i].at(arm_i).at(pl_i)->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getTrailing/%s.png", Folders.at(i).c_str(),hVector_h_pl_getTrailing[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();

	  hVector_h_pl_check[i].at(arm_i).at(pl_i)->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/check/%s.png", Folders.at(i).c_str(),hVector_h_pl_check[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();

	}

	for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){

	  hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->Write();
	  hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->Write();
	  hVector_h_ch_deltat[i].at(arm_i).at(pl_i).at(ch_i)->Write();

	  if(save_picture){
	    TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");
	    leg->SetFillColor(kWhite);
	    leg->SetLineColor(kWhite);
	    leg->AddEntry(hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i),"Leading Edge","LF");
	    leg->AddEntry(hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i),"Trailing Edge","LF");
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->SetStats(0);
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->SetFillColor(kBlack);
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->SetLineColor(kBlack);
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->SetLineWidth(1.);
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->SetStats(0);
	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->SetFillColor(kOrange);
	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->SetLineColor(kOrange);
	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->SetLineWidth(1.);
	    leg->Draw();
	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->Draw("SAME");
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/together/LeadingAndTrailing_arm%i_pl%i_ch%i.png", Folders.at(i).c_str(), arm_i, pl_i, ch_i));
	    c1->Modified();
	    c1->Update();
	    delete leg;
	  }

	  if(hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){  
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->SetStats(111111);
	    double max_x = hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.02*max_x,max_x+0.02*max_x);
	    int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	    if(migrad>1){
	      outstring << "\n\nLeading Edge ---> Arm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "Mean: " <<  hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1)<< " ns" << endl;
	      outstring << "Sigma: " <<  hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2)<< " ns" << endl;
	      hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->SetBinContent(ch_i+1,hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1));
	      hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->SetBinError(ch_i+1,hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2));
	    }else{
	      outstring << "\n\nArm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "NOT CONVERGED FIT." << std::endl;
	    }
	  }

	  if(hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->SetStats(111111);
	    double max_x = hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.02*max_x,max_x+0.02*max_x);
	    int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	    if(migrad>1){
	      outstring << "\n\nTrailing Edge ---> Arm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "Mean: " <<  hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1)<< " ns" << endl;
	      outstring << "Sigma: " <<  hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2)<< " ns" << endl;
	      hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->SetBinContent(ch_i+1,hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1));
	      hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->SetBinError(ch_i+1,hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2));
	    }else{
	      outstring << "\n\nArm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "NOT CONVERGED FIT." << std::endl;
	    }
	  }

	  if( hVector_h_ch_deltat[i].at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	    double max_x =  hVector_h_ch_deltat[i].at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter( hVector_h_ch_deltat[i].at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_deltat[i].at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.02*max_x,max_x+0.02*max_x);
	  }

	  if(save_picture){
	    hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getLeading/%s.png",Folders.at(i).c_str(), hVector_h_ch_getLeading[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();

	    hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/getTrailing/%s.png",Folders.at(i).c_str(), hVector_h_ch_getTrailing[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();

	    hVector_h_ch_check[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/check/%s.png",Folders.at(i).c_str(), hVector_h_ch_check[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();

	    hVector_h_ch_deltat[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/deltat/%s.png",Folders.at(i).c_str(), hVector_h_ch_deltat[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();

	  }

	}
	hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->Write();

	if(save_picture){
	  TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");
	  leg->SetFillColor(kWhite);
	  leg->SetLineColor(kWhite);
	  leg->AddEntry(hVector_h_pl_result_leading[i].at(arm_i).at(pl_i),"Leading Edge","LF");
	  leg->AddEntry(hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i),"Trailing Edge","LF");
	  hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->GetYaxis()->SetRangeUser(0.,200.);
	  hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->SetStats(0);
	  hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->SetLineColor(kBlack);
	  hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->SetMarkerColor(kBlack);
	  hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->SetMarkerSize(2);
	  hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->SetMarkerStyle(2);
	  hVector_h_pl_result_leading[i].at(arm_i).at(pl_i)->Draw("E1P");
	  hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->GetYaxis()->SetRangeUser(0.,200.);
	  hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->SetStats(0);
	  hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->SetLineColor(kOrange);
	  hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->SetMarkerColor(kOrange);
	  hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->SetMarkerSize(2);
	  hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->SetMarkerStyle(2);
	  hVector_h_pl_result_trailing[i].at(arm_i).at(pl_i)->Draw("ESAMEEX0");
	  leg->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/result/fit_results_arm%i_pl_%i.png",Folders.at(i).c_str(), arm_i, pl_i));
	  c1->Modified();
	  c1->Update();
	  delete leg;
	}

      }    
    }
  }

  if(save_picture) c1->Close();
  f->Close();
  outstring.close();

}

int CTPPSSkimmerDigiAnalyzer::getFitStatus (char *Migrad) 
{
  int i ;
  char Status[][12]={"FAILED","PROBLEMS","CONVERGED","SUCCESSFUL"} ;

  for(i=0 ; i < 5 ; i++) 
    if (strstr(Migrad,Status[i]) != NULL) return i ;

  return -1  ;
}

int run()
{

  CTPPSSkimmerDigiAnalyzer m;
  m.Loop();

  return 0;
}
