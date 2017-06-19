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

#include <vector>
#include <stdlib.h>

using namespace std;

vector<TH1I*> hVector_h_cms_bx;
vector<TH1D*> hVector_h_plane;
vector<vector<TH1D*> > hVector_h_ch;
vector<string> Folders;

void CTPPSSkimmerAnalyzer::CreateHistos(){

  // vector<string> Folders;
  Folders.push_back("step0");
  Folders.push_back("step1");
  Folders.push_back("step2");

  for (std::vector<std::string>::size_type i=0; i<Folders.size(); i++){

    hVector_h_ch.push_back( std::vector<TH1D*>() );

    char name[300];
    sprintf(name,"CMSBX_%s",Folders.at(i).c_str());
    TH1I *histo_cms_bx = new TH1I(name,";CMS BX; N events",3600,0,3600);
    hVector_h_cms_bx.push_back(histo_cms_bx);

    sprintf(name,"arm0_pl0_%s", Folders.at(i).c_str());
    TH1D *histo_plane = new TH1D(name,";Time [ns]; N events",1250,0,125);
    hVector_h_plane.push_back(histo_plane);

    TString htitle;
    for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
      htitle = ";Time [ns]; N events";
      sprintf(name,"arm0_pl0_ch%i_%s", ch_i, Folders.at(i).c_str());
      TH1D *histo_ch = new TH1D(name,";Time [ns]; N events",1250,0,125);
      hVector_h_ch[i].push_back(histo_ch);
    }
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

  for (UInt_t j = 0; j < getT->size(); ++j) {
    if(arm->at(j) == 1 && plane->at(j) == 2){
      if(getOOTIndex->at(j)==0) {
	hVector_h_ch[i].at(channel->at(j))->Fill(getT->at(j));
	hVector_h_plane[i]->Fill(getT->at(j));
      }
      if(getOOTIndex->at(j)==1){
	hVector_h_ch[i].at(channel->at(j))->Fill(getT->at(j)+25);
	hVector_h_plane[i]->Fill(getT->at(j)+25);
      }
      if(getOOTIndex->at(j)==2){
	hVector_h_ch[i].at(channel->at(j))->Fill(getT->at(j)+50);
	hVector_h_plane[i]->Fill(getT->at(j)+50);
      }
      if(getOOTIndex->at(j)==3){
	hVector_h_ch[i].at(channel->at(j))->Fill(getT->at(j)+75);
	hVector_h_plane[i]->Fill(getT->at(j)+75);
      }
      if(getOOTIndex->at(j)==4){
	hVector_h_ch[i].at(channel->at(j))->Fill(getT->at(j)+100);
	hVector_h_plane[i]->Fill(getT->at(j)+100);
      }
    }
  }

}

void CTPPSSkimmerAnalyzer::WriteHistos(){

  TFile* f = new TFile("histo_run295977.root", "RECREATE");
  for (std::vector<std::string>::size_type i=0; i<Folders.size(); i++){
    hVector_h_cms_bx[i]->Write();
    hVector_h_plane[i]->Write();
    for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
      hVector_h_ch[i].at(ch_i)->Write();
    }
  }
  f->Close();

}

int run()
{

  CTPPSSkimmerAnalyzer m;
  m.Loop();

  return 0;
}
