// -*- C++ -*-
//
// Package:    CTPPSSkimmer
// Class:      CTPPSSkimmer
// 

/**\class CTPPSSkimmer CTPPSSkimmer.cc CTPPSSkimmer/plugins/CTPPSSkimmer.cc

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
#include "CTPPSSkimmer.h"

//
// constructors and destructor
//
CTPPSSkimmer::CTPPSSkimmer(const edm::ParameterSet &iConfig):
  tokenStatus_      ( consumes< edm::DetSetVector<TotemVFATStatus> >       (iConfig.getParameter<edm::InputTag>( "tagStatus" ) ) ),
  tokenLocalTrack_  ( consumes< edm::DetSetVector<TotemRPLocalTrack> >     (iConfig.getParameter<edm::InputTag>( "tagLocalTrack" ) ) ),
  tokenDigi_        ( consumes< edm::DetSetVector<CTPPSDiamondDigi> >      (iConfig.getParameter<edm::InputTag>( "tagDigi" ) ) ),
  tokenDiamondHit_  ( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >    (iConfig.getParameter<edm::InputTag>( "tagDiamondRecHits" ) ) ),
  tokenDiamondTrack_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >(iConfig.getParameter<edm::InputTag>( "tagDiamondLocalTracks" ) ) ),
  tokenFEDInfo_     ( consumes< std::vector<TotemFEDInfo> >                (iConfig.getParameter<edm::InputTag>( "tagFEDInfo" ) ) ),
  verbosity_        (iConfig.getUntrackedParameter<unsigned int>( "verbosity", 0 ) )
{
  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("CTPPSSkimmerDiamond", "CTPPSSkimmerDiamond");
}


CTPPSSkimmer::~CTPPSSkimmer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
  void
CTPPSSkimmer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  arm_vec.clear();
  station_vec.clear();
  rp_vec.clear();
  plane_vec.clear();
  channel_vec.clear();
  multiplehits_vec.clear();
  getToT_vec.clear();
  getOOTIndex_vec.clear();
  getT_vec.clear();
  getX_vec.clear();
  getXWidth_vec.clear();
  getY_vec.clear();
  getYWidth_vec.clear();
  getFedId_vec.clear();
  getBX_vec.clear();

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

  for ( const auto& rechits : *diamondRecHits ) {
    const CTPPSDiamondDetId detId( rechits.detId() );

    for ( const auto& rechit : rechits ) {

      arm_vec.push_back(detId.arm());
      station_vec.push_back(detId.station());
      rp_vec.push_back(detId.rp());
      plane_vec.push_back(detId.plane());
      channel_vec.push_back(detId.channel());
      multiplehits_vec.push_back(rechit.getMultipleHits());
      getToT_vec.push_back(rechit.getToT());
      getOOTIndex_vec.push_back(rechit.getOOTIndex());
      getT_vec.push_back(rechit.getT());
      getX_vec.push_back(rechit.getX());
      getXWidth_vec.push_back(rechit.getXWidth());
      getY_vec.push_back(rechit.getY());
      getYWidth_vec.push_back(rechit.getYWidth());

      for ( const auto& fit : *fedInfo ) {
	getFedId_vec.push_back(fit.getFEDId());
	getBX_vec.push_back(fit.getBX());
      }

    }
  }

  if ( !valid ) {
    if ( verbosity_ ) {
      edm::LogProblem("CTPPSSkimmer")
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
CTPPSSkimmer::beginJob()
{

  tree_->Branch("arm", &arm_vec);
  tree_->Branch("station", &station_vec);
  tree_->Branch("rp", &rp_vec);
  tree_->Branch("plane", &plane_vec);
  tree_->Branch("channel", &channel_vec);
  tree_->Branch("multiplehits", &multiplehits_vec);
  tree_->Branch("getToT", &getToT_vec);
  tree_->Branch("getOOTIndex", &getOOTIndex_vec);
  tree_->Branch("getT", &getT_vec);
  tree_->Branch("getX", &getX_vec);
  tree_->Branch("getXWidth", &getXWidth_vec);
  tree_->Branch("getY", &getY_vec);
  tree_->Branch("getYWidth", &getYWidth_vec);
  tree_->Branch("getFedId", &getFedId_vec);
  tree_->Branch("getBX", &getBX_vec);

}

// ------------ method called once each job just after ending the event loop  ------------
  void 
CTPPSSkimmer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CTPPSSkimmer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSSkimmer);
