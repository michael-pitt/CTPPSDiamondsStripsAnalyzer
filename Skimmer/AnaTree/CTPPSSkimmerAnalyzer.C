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
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch_getT;
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch_getToT;
vector<vector<vector<vector<TH1D*> > > >hVector_h_ch_check;
vector<vector<vector<TH1D*> > > hVector_h_pl_getT;
vector<vector<vector<TH1D*> > > hVector_h_pl_getToT;
vector<vector<vector<TH1D*> > > hVector_h_pl_check;
vector<vector<vector<TH1D*> > > hVector_h_pl_result;
vector<vector<TH1D*> > hVector_h_arm_getT;
vector<vector<TH1D*> > hVector_h_arm_getToT;
vector<vector<TH1D*> > hVector_h_arm_check;
vector<string> Folders;

void CTPPSSkimmerAnalyzer::CreateHistos(){

  Folders.push_back("step0");
  Folders.push_back("step1");
  Folders.push_back("step2");

  for (std::vector<std::string>::size_type i=0; i<Folders.size(); i++){

    vector < vector< vector<TH1D*> > > vec_cut_ch_getT;
    vector< vector<TH1D*> > vec_cut_pl_getT;
    vector< vector<TH1D*> > vec_cut_pl_result;
    vector<TH1D*> vec_cut_arm_getT;

    vector < vector< vector<TH1D*> > > vec_cut_ch_getToT;
    vector< vector<TH1D*> > vec_cut_pl_getToT;
    vector<TH1D*> vec_cut_arm_getToT;

    vector < vector< vector<TH1D*> > > vec_cut_ch_check;
    vector< vector<TH1D*> > vec_cut_pl_check;
    vector<TH1D*> vec_cut_arm_check;

    char name[300];
    sprintf(name,"CMSBX_%s",Folders.at(i).c_str());
    TH1I *histo_cms_bx = new TH1I(name,";CMS BX; N events",3600,0,3600);
    hVector_h_cms_bx.push_back(histo_cms_bx);

    for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){

      vector< vector<TH1D*> > vec_arm_per_ch_getT;
      vector<TH1D*> vec_arm_per_pl_getT;

      vector< vector<TH1D*> > vec_arm_per_ch_getToT;
      vector<TH1D*> vec_arm_per_pl_getToT;

      vector< vector<TH1D*> > vec_arm_per_ch_check;
      vector<TH1D*> vec_arm_per_pl_check;
      vector<TH1D*> vec_arm_per_pl_result;

      for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
	vector<TH1D*> vec_ch_getT;
	vector<TH1D*> vec_ch_getToT;
	vector<TH1D*> vec_ch_check;
	for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	  sprintf(name,"getT_arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch_getT = new TH1D(name,";Leading Edge [ns]; N events",1250,0,125);
	  vec_ch_getT.push_back(histo_ch_getT);

	  sprintf(name,"getToT_arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch_getToT = new TH1D(name,";Time Over Threshold [ns]; N events",1500,0,200);
	  vec_ch_getToT.push_back(histo_ch_getToT);

	  sprintf(name,"check_arm%i_pl%i_ch%i_%s", arm_i, pl_i, ch_i, Folders.at(i).c_str());
	  TH1D *histo_ch_check = new TH1D(name,"; ; N events",16,0,16);
	  vec_ch_check.push_back(histo_ch_check);
	}
	vec_arm_per_ch_getT.push_back(vec_ch_getT);
	vec_arm_per_ch_getToT.push_back(vec_ch_getToT);
	vec_arm_per_ch_check.push_back(vec_ch_check);

	sprintf(name,"getT_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_getT = new TH1D(name,";Leading Edge [ns]; N events",1250,0,125);
	vec_arm_per_pl_getT.push_back(histo_pl_getT);

	sprintf(name,"getToT_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_getToT = new TH1D(name,";Time Over Threshold [ns]; N events",1500,0,200);
	vec_arm_per_pl_getToT.push_back(histo_pl_getToT);

	sprintf(name,"check_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_check = new TH1D(name,"; ; N events",16,0,16);
	vec_arm_per_pl_check.push_back(histo_pl_check);

	sprintf(name,"result_arm%i_pl%i_%s", arm_i, pl_i, Folders.at(i).c_str());
	TH1D *histo_pl_result = new TH1D(name,"; Channel; <Leading Edge> [ns]",16,0,16);
	vec_arm_per_pl_result.push_back(histo_pl_result);

      }

      sprintf(name,"getT_arm%i_%s", arm_i, Folders.at(i).c_str());
      TH1D *histo_arm_getT = new TH1D(name,";Leading Edge [ns]; N events",1250,0,125);

      sprintf(name,"getToT_arm%i_%s", arm_i, Folders.at(i).c_str());
      TH1D *histo_arm_getToT = new TH1D(name,";Time Over Threshold [ns]; N events",1500,0,200);

      sprintf(name,"check_arm%i_%s", arm_i, Folders.at(i).c_str());
      TH1D *histo_arm_check = new TH1D(name,"; ; N events",16,0,16);

      vec_cut_ch_getT.push_back( vec_arm_per_ch_getT);
      vec_cut_pl_getT.push_back( vec_arm_per_pl_getT);
      vec_cut_pl_result.push_back( vec_arm_per_pl_result);
      vec_cut_arm_getT.push_back(histo_arm_getT);

      vec_cut_ch_getToT.push_back( vec_arm_per_ch_getToT);
      vec_cut_pl_getToT.push_back( vec_arm_per_pl_getToT);
      vec_cut_arm_getToT.push_back(histo_arm_getToT);

      vec_cut_ch_check.push_back( vec_arm_per_ch_check);
      vec_cut_pl_check.push_back( vec_arm_per_pl_check);
      vec_cut_arm_check.push_back(histo_arm_check);

    }
    hVector_h_ch_getT.push_back( vec_cut_ch_getT );
    hVector_h_pl_getT.push_back( vec_cut_pl_getT );
    hVector_h_pl_result.push_back( vec_cut_pl_result );
    hVector_h_arm_getT.push_back( vec_cut_arm_getT );

    hVector_h_ch_getToT.push_back( vec_cut_ch_getToT );
    hVector_h_pl_getToT.push_back( vec_cut_pl_getToT );
    hVector_h_arm_getToT.push_back( vec_cut_arm_getToT );

    hVector_h_ch_check.push_back( vec_cut_ch_check );
    hVector_h_pl_check.push_back( vec_cut_pl_check );
    hVector_h_arm_check.push_back( vec_cut_arm_check );

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

void CTPPSSkimmerAnalyzer::FillHistos(int i){

  char slice[300];
  hVector_h_cms_bx[i]->Fill(getBxCMS);

  for (UInt_t j = 0; j < getT->size(); ++j){
    if(multiplehits->at(j)==0){

      hVector_h_ch_getT[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(getT->at(j)+25.*getOOTIndex->at(j));
      hVector_h_pl_getT[i].at(arm->at(j)).at(plane->at(j))->Fill(getT->at(j)+25.*getOOTIndex->at(j));
      hVector_h_arm_getT[i].at(arm->at(j))->Fill(getT->at(j)+25.*getOOTIndex->at(j));
      hVector_h_ch_getToT[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(getToT->at(j)+25.*getOOTIndex->at(j));
      hVector_h_pl_getToT[i].at(arm->at(j)).at(plane->at(j))->Fill(getToT->at(j)+25.*getOOTIndex->at(j));
      hVector_h_arm_getToT[i].at(arm->at(j))->Fill(getToT->at(j)+25.*getOOTIndex->at(j));

      if(getT->at(j) != 0 && getToT->at(j) != 0){
	sprintf(slice,"Both [%i]", getOOTIndex->at(j));
	hVector_h_ch_check[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(slice,1);
	hVector_h_pl_check[i].at(arm->at(j)).at(plane->at(j))->Fill(slice,1);
	hVector_h_arm_check[i].at(arm->at(j))->Fill(slice,1);
      }
      if(getT->at(j) != 0 && getToT->at(j) == 0){
	sprintf(slice,"Only Leading [%i]", getOOTIndex->at(j));
	hVector_h_ch_check[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(slice,1);
	hVector_h_pl_check[i].at(arm->at(j)).at(plane->at(j))->Fill(slice,1);
	hVector_h_arm_check[i].at(arm->at(j))->Fill(slice,1);
      }
      if(getT->at(j) == 0 && getToT->at(j) != 0){
	sprintf(slice,"Only Trailing [%i]", getOOTIndex->at(j));
	hVector_h_ch_check[i].at(arm->at(j)).at(plane->at(j)).at(channel->at(j))->Fill(slice,1);
	hVector_h_pl_check[i].at(arm->at(j)).at(plane->at(j))->Fill(slice,1);
	hVector_h_arm_check[i].at(arm->at(j))->Fill(slice,1);
      }

    }
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

  TFile* f = new TFile("histo_run296173.root", "RECREATE");

  for (std::vector<std::string>::size_type i=0; i<Folders.size(); i++){

    gSystem->mkdir("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173");
    gSystem->CopyFile("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/index.php","/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/index.php");
    gSystem->CopyFile("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/index.php",Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/index.php", Folders.at(i).c_str()));

    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296176/%s/getT", Folders.at(i).c_str()));
    gSystem->CopyFile("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/index.php",Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getT/index.php", Folders.at(i).c_str()));

    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296176/%s/getToT", Folders.at(i).c_str()));
    gSystem->CopyFile("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/index.php",Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getToT/index.php", Folders.at(i).c_str()));

    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296176/%s/check", Folders.at(i).c_str()));
    gSystem->CopyFile("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/index.php",Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/check/index.php", Folders.at(i).c_str()));

    gSystem->mkdir(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296176/%s/result", Folders.at(i).c_str()));
    gSystem->CopyFile("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/index.php",Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/result/index.php", Folders.at(i).c_str()));

    hVector_h_cms_bx[i]->Write();

    if(save_picture){
      hVector_h_cms_bx[i]->Draw();
      c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/%s.png", Folders.at(i).c_str(), hVector_h_cms_bx[i]->GetName()));
      c1->Modified();
      c1->Update();
    }

    for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){
      hVector_h_arm_getT[i].at(arm_i)->Write();
      hVector_h_arm_getToT[i].at(arm_i)->Write();

      if(save_picture){
	hVector_h_arm_getT[i].at(arm_i)->Draw();
	c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getT/%s.png", Folders.at(i).c_str(), hVector_h_arm_getT[i].at(arm_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_arm_getToT[i].at(arm_i)->Draw();
	c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getToT/%s.png", Folders.at(i).c_str(), hVector_h_arm_getToT[i].at(arm_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_arm_check[i].at(arm_i)->Draw();
	c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/check/%s.png", Folders.at(i).c_str(), hVector_h_arm_check[i].at(arm_i)->GetName()));
	c1->Modified();
	c1->Update();

      }

      for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
	hVector_h_pl_getT[i].at(arm_i).at(pl_i)->Write();
	hVector_h_pl_getToT[i].at(arm_i).at(pl_i)->Write();

	if(save_picture){
	  hVector_h_pl_getT[i].at(arm_i).at(pl_i)->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getT/%s.png", Folders.at(i).c_str(),hVector_h_pl_getT[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();

	  hVector_h_pl_getToT[i].at(arm_i).at(pl_i)->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getToT/%s.png", Folders.at(i).c_str(),hVector_h_pl_getToT[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();

	  hVector_h_pl_check[i].at(arm_i).at(pl_i)->Draw();
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/check/%s.png", Folders.at(i).c_str(),hVector_h_pl_check[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();

	}

	for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){

	  hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->Write();
	  hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->Write();

	  if(hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){  
	    double max_x = hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.02*max_x,max_x+0.02*max_x);
	    int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	    if(migrad>1){
	      outstring << "\n\nGetT ---> Arm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "Mean: " <<  hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1)<< " ns" << endl;
	      outstring << "Sigma: " <<  hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2)<< " ns" << endl;
	      hVector_h_pl_result[i].at(arm_i).at(pl_i)->SetBinContent(ch_i+1,hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1));
	      hVector_h_pl_result[i].at(arm_i).at(pl_i)->SetBinError(ch_i+1,hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2));
	    }else{
	      outstring << "\n\nArm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "NOT CONVERGED FIT." << std::endl;
	    }
	  }

	  if(hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	    double max_x = hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.02*max_x,max_x+0.02*max_x);
	    int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	    if(migrad>1){
	      outstring << "\n\nGetToT ---> Arm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "Mean: " <<  hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1)<< " ns" << endl;
	      outstring << "Sigma: " <<  hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(2)<< " ns" << endl;
	    }else{
	      outstring << "\n\nArm: " << arm_i << " | Plane: " << pl_i << " | Ch: " << ch_i << std::endl;
	      outstring << "NOT CONVERGED FIT." << std::endl;
	    }
	  }

	  if(save_picture){
	    hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getT/%s.png",Folders.at(i).c_str(), hVector_h_ch_getT[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();

	    hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/getToT/%s.png",Folders.at(i).c_str(), hVector_h_ch_getToT[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();

	    hVector_h_ch_check[i].at(arm_i).at(pl_i).at(ch_i)->Draw();
	    c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/check/%s.png",Folders.at(i).c_str(), hVector_h_ch_check[i].at(arm_i).at(pl_i).at(ch_i)->GetName()));
	    c1->Modified();
	    c1->Update();

	  }

	}
	hVector_h_pl_result[i].at(arm_i).at(pl_i)->Write();
	if(save_picture){
	  hVector_h_pl_result[i].at(arm_i).at(pl_i)->SetStats(0);
	  hVector_h_pl_result[i].at(arm_i).at(pl_i)->Draw("EP");
	  c1->SaveAs(Form("/afs/cern.ch/work/d/dmf/public/html/TimingCalibration/Run296173/%s/result/%s.png",Folders.at(i).c_str(), hVector_h_pl_result[i].at(arm_i).at(pl_i)->GetName()));
	  c1->Modified();
	  c1->Update();
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
