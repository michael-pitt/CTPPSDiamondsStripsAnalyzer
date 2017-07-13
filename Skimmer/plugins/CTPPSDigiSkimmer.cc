// -*- C++ -*-
//
// Package:    CTPPSDigiSkimmer
// Class:      CTPPSDigiSkimmer
// 

/**\class CTPPSDigiSkimmer CTPPSDigiSkimmer.cc CTPPSDigiSkimmer/plugins/CTPPSDigiSkimmer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/

//
// Original Author:  Diego Figueiredo
//         Created:  Tue, 06 Jun 2017 14:36:11 GMT
//
//

// system include files
#include "CTPPSDigiSkimmer.h"

//
// constructors and destructor
//
CTPPSDigiSkimmer::CTPPSDigiSkimmer(const edm::ParameterSet &iConfig):
  tokenStatus_      ( consumes< edm::DetSetVector<TotemVFATStatus> >       (iConfig.getParameter<edm::InputTag>( "tagStatus" ) ) ),
  tokenLocalTrack_  ( consumes< edm::DetSetVector<TotemRPLocalTrack> >     (iConfig.getParameter<edm::InputTag>( "tagLocalTrack" ) ) ),
  tokenDigi_        ( consumes< edm::DetSetVector<CTPPSDiamondDigi> >      (iConfig.getParameter<edm::InputTag>( "tagDigi" ) ) ),
  tokenDiamondHit_  ( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >    (iConfig.getParameter<edm::InputTag>( "tagDiamondRecHits" ) ) ),
  tokenDiamondTrack_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >(iConfig.getParameter<edm::InputTag>( "tagDiamondLocalTracks" ) ) ),
  tokenFEDInfo_     ( consumes< std::vector<TotemFEDInfo> >                (iConfig.getParameter<edm::InputTag>( "tagFEDInfo" ) ) ),
  verbosity_        (iConfig.getUntrackedParameter<unsigned int>( "verbosity", 0 ) ),
  hltMenuLabel_     (iConfig.getParameter<std::string>("HLTMenuLabel")),
  triggersList_     (iConfig.getParameter<std::vector<std::string> >("TriggersList")),
  triggerResultsToken_(consumes<edm::TriggerResults> (iConfig.getParameter<edm::InputTag>("TriggerResults")))
{

}

CTPPSDigiSkimmer::~CTPPSDigiSkimmer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

void CTPPSDigiSkimmer::fillTriggerInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  bool debug = true;

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByToken(triggerResultsToken_, triggerResults);

  if( triggerResults.isValid() ){

    int nSize = triggerResults->size();
    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults);

    size_t idxpath = 0;
    std::vector<std::string>::const_iterator hltpath = triggersList_.begin();
    std::vector<std::string>::const_iterator hltpaths_end = triggersList_.end();

    for(; hltpath != hltpaths_end; ++hltpath,++idxpath){
      std::string resolvedPathName;
      if( edm::is_glob( *hltpath ) ){
	std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(triggerNames.triggerNames(), *hltpath);
	if( matches.empty() ){
	  if (debug) edm::LogWarning("Configuration") << "Could not find trigger " << *hltpath << " in the path list.\n";
	}
	else if( matches.size() > 1)
	  throw cms::Exception("Configuration") << "HLT path type " << *hltpath << " not unique\n";
	else resolvedPathName = *(matches[0]);
      } else{
	resolvedPathName = *hltpath;
      }

      int idx_HLT = triggerNames.triggerIndex(resolvedPathName);

      if (idx_HLT >= 0 && idx_HLT < nSize){
	int accept_HLT = ( triggerResults->wasrun(idx_HLT) && triggerResults->accept(idx_HLT) ) ? 1 : 0;
	hltTrigResults_.push_back(accept_HLT);
	hltTriggerPassHisto_->Fill( (*hltpath).c_str(), accept_HLT );
      }else{
	hltTrigResults_.push_back(-1);
	hltTriggerPassHisto_->Fill( (*hltpath).c_str(), -1 );
      }

    }

  }else{
    if (debug) std::cout << "\n No valid trigger result.\n" <<std::endl;
  }

}

// ------------ method called for each event  ------------
  void
CTPPSDigiSkimmer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  arm_vec.clear();
  station_vec.clear();
  rp_vec.clear();
  plane_vec.clear();
  channel_vec.clear();
  multiplehits_vec.clear();
  getLeading_vec.clear();
  getTrailing_vec.clear();
  getFedId_vec.clear();
  getBX_vec.clear();
  getThresholdVoltage_vec.clear();
  hltTrigResults_.clear();
  bx_cms = -1;
  lumi_section = -1;
  orbit = -1;
  run_number = -1;

  //fillTriggerInfo(iEvent,iSetup);

  edm::Handle< edm::DetSetVector<TotemVFATStatus> > diamondVFATStatus;
  iEvent.getByToken( tokenStatus_, diamondVFATStatus );

  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripTracks;
  iEvent.getByToken( tokenLocalTrack_, stripTracks );

  edm::Handle< edm::DetSetVector<CTPPSDiamondDigi> > diamondDigis;
  iEvent.getByToken( tokenDigi_, diamondDigis );

  edm::Handle< std::vector<TotemFEDInfo> > fedInfo;
  iEvent.getByToken( tokenFEDInfo_, fedInfo );

  edm::Handle< edm::DetSetVector<CTPPSDiamondRecHit> > diamondRecHits;
  iEvent.getByToken( tokenDiamondHit_, diamondRecHits );

  edm::Handle< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamondLocalTracks;
  iEvent.getByToken( tokenDiamondTrack_, diamondLocalTracks );

  // check validity
  valid = true;
  valid &= diamondVFATStatus.isValid();
  valid &= diamondDigis.isValid();
  valid &= fedInfo.isValid();

  bx_cms = iEvent.bunchCrossing();
  lumi_section = iEvent.luminosityBlock();
  orbit = iEvent.orbitNumber();
  run_number = iEvent.id().run();

  for ( const auto& digis : *diamondDigis ) {
    const CTPPSDiamondDetId detId( digis.detId() );
    for ( const auto& digi : digis ) {

      if(digi.getLeadingEdge() == 0) continue;

      arm_vec.push_back(detId.arm());
      station_vec.push_back(detId.station());
      rp_vec.push_back(detId.rp());
      plane_vec.push_back(detId.plane());
      channel_vec.push_back(detId.channel());
      multiplehits_vec.push_back(digi.getMultipleHit());
      getLeading_vec.push_back(digi.getLeadingEdge());
      getTrailing_vec.push_back(digi.getTrailingEdge());
      getThresholdVoltage_vec.push_back(digi.getThresholdVoltage());
      getHPTDCError_vec.push_back(digi.getHPTDCErrorFlags().getErrorFlag());

      for ( const auto& fit : *fedInfo ) {
	getFedId_vec.push_back(fit.getFEDId());
	getBX_vec.push_back(fit.getBX());
      }

    }
  }

  if ( !valid ) {
    if ( verbosity_ ) {
      edm::LogProblem("CTPPSDigiSkimmer")
	<< "ERROR in TotemDQMModuleRP::analyze > some of the required inputs are not valid. Skipping this event.\n"
	<< "    diamondVFATStatus.isValid = " << diamondVFATStatus.isValid() << "\n"
	<< "    diamondDigis.isValid = " << diamondDigis.isValid() << "\n"
	<< "    fedInfo.isValid = " << fedInfo.isValid();
    }
    return;
  }

  tree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
  void 
CTPPSDigiSkimmer::beginJob()
{

  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("CTPPSSkimmerDiamond", "CTPPSSkimmerDiamond");

  std::ostringstream oss;
  TFileDirectory triggerDir = fs->mkdir("TriggerInfo");
  hltTriggerNamesHisto_ = triggerDir.make<TH1F>("HLTTriggerNames","HLTTriggerNames",1,0,1);
  for(unsigned k=0; k < triggersList_.size(); ++k){
    oss << "Using HLT reference trigger " << triggersList_[k] << std::endl;
    hltTriggerNamesHisto_->Fill(triggersList_[k].c_str(),1);
  }
  edm::LogVerbatim("Analysis") << oss.str();
  hltTriggerPassHisto_ = triggerDir.make<TH1F>("HLTTriggerPass","HLTTriggerPass",1,0,1);

  fs->cd();

  tree_->Branch("valid", &valid, "valid/B");
  tree_->Branch("run_number", &run_number, "run_number/I");
  tree_->Branch("getBxCMS", &bx_cms, "bx_cms/I");
  tree_->Branch("getOrbitCMS", &orbit, "orbit/I");
  tree_->Branch("getLsCMS", &lumi_section, "lumi_section/I");
  tree_->Branch("HLT", &hltTrigResults_);
  tree_->Branch("arm", &arm_vec);
  tree_->Branch("station", &station_vec);
  tree_->Branch("rp", &rp_vec);
  tree_->Branch("plane", &plane_vec);
  tree_->Branch("channel", &channel_vec);
  tree_->Branch("multiplehits", &multiplehits_vec);
  tree_->Branch("getLeading", &getLeading_vec);
  tree_->Branch("getTrailing", &getTrailing_vec);
  tree_->Branch("getThresholdVoltage", &getThresholdVoltage_vec);
  tree_->Branch("getHPTDCError", &getHPTDCError_vec);
  tree_->Branch("getFedId", &getFedId_vec);
  tree_->Branch("getBx", &getBX_vec);

}

// ------------ method called once each job just after ending the event loop  ------------
  void 
CTPPSDigiSkimmer::endJob() 
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CTPPSDigiSkimmer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSDigiSkimmer);
