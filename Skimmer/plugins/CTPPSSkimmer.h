#ifndef CTPPSAnalyzer_CTPPSSkimmer_h
#define CTPPSAnalyzer_CTPPSSkimmer_h

// System Inputs
#include <fstream>
#include <memory>
#include <string>

// CMSSW Inputs
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// CMSSW Data Formats
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h"
#include "DataFormats/CTPPSDigi/interface/TotemFEDInfo.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TH1D.h>

// Constants
#define SEC_PER_LUMI_SECTION 23.31
#define CHANNEL_OF_VFAT_CLOCK 30
#define DISPLAY_RESOLUTION_FOR_HITS_MM 0.1
#define HPTDC_BIN_WIDTH_NS 25./1024
#define CTPPS_NUM_OF_ARMS 2
#define CTPPS_DIAMOND_STATION_ID 1
#define CTPPS_DIAMOND_RP_ID 6
#define CTPPS_NEAR_RP_ID 2
#define CTPPS_FAR_RP_ID 3
#define CTPPS_DIAMOND_NUM_OF_PLANES 4
#define CTPPS_DIAMOND_NUM_OF_CHANNELS 12
#define CTPPS_FED_ID_56 582
#define CTPPS_FED_ID_45 583

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class CTPPSSkimmer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
    explicit CTPPSSkimmer(const edm::ParameterSet&);
    ~CTPPSSkimmer();
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    edm::EDGetTokenT< edm::DetSetVector<TotemVFATStatus> > tokenStatus_;
    edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > tokenLocalTrack_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondDigi> > tokenDigi_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > tokenDiamondHit_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > tokenDiamondTrack_;
    edm::EDGetTokenT< std::vector<TotemFEDInfo> > tokenFEDInfo_;
    unsigned int verbosity_;
    bool valid;

    std::vector<int> arm_vec;
    std::vector<int> station_vec;
    std::vector<int> rp_vec;
    std::vector<int> plane_vec;
    std::vector<int> channel_vec;
    std::vector<int> multiplehits_vec;
    std::vector<double> getToT_vec;
    std::vector<int> getOOTIndex_vec;
    std::vector<double> getT_vec;
    std::vector<double> getX_vec;
    std::vector<double> getXWidth_vec;
    std::vector<double> getY_vec;
    std::vector<double> getYWidth_vec;
    std::vector<int> getFedId_vec;
    std::vector<int> getBX_vec;

};

TTree* tree_;

#endif
