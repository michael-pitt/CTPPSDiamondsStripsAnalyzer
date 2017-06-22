#define CTPPSSkimmerAnalyzer_cxx
#include "CTPPSSkimmerAnalyzer.h"
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
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch;
vector<vector<vector<TH1D*> > > hVector_h_pl;
vector<vector<TH1D*> > hVector_h_arm;
vector<string> Folders;

void CTPPSSkimmerAnalyzer::CreateHistos(){

  Folders.push_back("step0");
  Folders.push_back("step1");
  Folders.push_back("step2");

  for (std::vector<std::string>::size_type i=0; i<Folders.size(); i++){

    vector < vector< vector<TH1D*> > > vec_cut_ch;
    vector< vector<TH1D*> > vec_cut_pl;
    vector<TH1D*> vec_cut_arm;

    char name[300];
    sprintf(name,"CMSBX_%s",Folders.at(i).c_str());
    TH1I *histo_cms_bx = new TH1I(name,";CMS BX; N events",3600,0,3600);
    hVector_h_cms_bx.push_back(histo_cms_bx);

    TString htitle;
    for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){

      vector< vector<TH1D*> > vec_arm_per_ch;
      vector<TH1D*> vec_arm_per_pl;

      for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
	vector<TH1D*> vec_ch;
	for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	  htitle = ";Time [ns]; N events";
	  sprintf(name,"arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch = new TH1D(name,";Time [ns]; N events",1250,0,125);
	  vec_ch.push_back(histo_ch);
	}
	vec_arm_per_ch.push_back(vec_ch);

	htitle = ";Time [ns]; N events";
	sprintf(name,"arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl = new TH1D(name,";Time [ns]; N events",1250,0,125);
	vec_arm_per_pl.push_back(histo_pl);

      }

      htitle = ";Time [ns]; N events";
      sprintf(name,"arm%i_%s", arm_i, Folders.at(i).c_str());
      TH1D *histo_arm = new TH1D(name,";Time [ns]; N events",1250,0,125);

      vec_cut_ch.push_back( vec_arm_per_ch );
      vec_cut_pl.push_back( vec_arm_per_pl);
      vec_cut_arm.push_back(histo_arm);

    }
    hVector_h_ch.push_back( vec_cut_ch );
    hVector_h_pl.push_back( vec_cut_pl );
    hVector_h_arm.push_back( vec_cut_arm );
  }

}
void CTPPSSkimmerAnalyzer::Loop(){

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
      if(getBx->at(j)==7 || getBx->at(j)==47 || getBx->at(j)==87) isolatedBx = true;
    }

    bool isolatedBxCMS = false;
    if(getBxCMS == 7 || getBxCMS == 47 || getBxCMS==87) isolatedBxCMS = true;

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

void CTPPSSkimmerAnalyzer::FillHistos(int i){

  hVector_h_cms_bx[i]->Fill(getBxCMS);

  for (UInt_t j = 0; j < getT->size(); ++j){
    hVector_h_ch[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(getT->at(j)+25.*getOOTIndex->at(j));
    hVector_h_pl[i].at(arm->at(j)).at(plane->at(j))->Fill(getT->at(j)+25.*getOOTIndex->at(j));
    hVector_h_arm[i].at(arm->at(j))->Fill(getT->at(j)+25.*getOOTIndex->at(j));
  }

}

void CTPPSSkimmerAnalyzer::WriteHistos(){

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

    hVector_h_cms_bx[i]->Write();

    if(save_picture){
      hVector_h_cms_bx[i]->Draw();
      c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/%s.png", Folders.at(i).c_str(), hVector_h_cms_bx[i]->GetName()));
      c1->Modified();
      c1->Update();
    }

    for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){
      hVector_h_arm[i].at(arm_i)->Write();

      if(save_picture){
	hVector_h_arm[i].at(arm_i)->Draw();
	c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/%s.png", Folders.at(i).c_str(), hVector_h_arm[i].at(arm_i)->GetName()));
	c1->Modified();
	c1->Update();
      }

      for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
	hVector_h_pl[i].at(arm_i).at(pl_i)->Write();

	if(save_picture){
	  hVector_h_pl[i].at(arm_i).at(pl_i)->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/%s.png", Folders.at(i).c_str(),hVector_h_pl[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();
	}

	for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	  hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->Write();
	  if(hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){  
	    double max_x = hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.05*max_x,max_x+0.05*max_x);
	    int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	    if(migrad>1){
	      outstring << "\n\nArm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "Mean: " <<  hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1)<< " ns" << endl;
	      outstring << "Sigma: " <<  hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2)<< " ns" << endl;
	    }else{
	      outstring << "\n\nArm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "NOT CONVERGED FIT." << std::endl;
	    }
	  }
	  if(save_picture){
	    hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run295977/%s/%s.png",Folders.at(i).c_str(), hVector_h_ch[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();
	  }

	}
      }    
    }
  }

  if(save_picture) c1->Close();
  f->Close();
  outstring.close();

}

int CTPPSSkimmerAnalyzer::getFitStatus (char *Migrad) 
{
  int i ;
  char Status[][12]={"FAILED","PROBLEMS","CONVERGED","SUCCESSFUL"} ;

  for(i=0 ; i < 5 ; i++) 
    if (strstr(Migrad,Status[i]) != NULL) return i ;

  return -1  ;
}

int run()
{

  CTPPSSkimmerAnalyzer m;
  m.Loop();

  return 0;
}
