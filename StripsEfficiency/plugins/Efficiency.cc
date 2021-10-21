// -*- C++ -*-
//
// Package:    StripsEfficiency
// Class:      StripsEfficiency
// 
/**\class StripsEfficiency StripsEfficiency.cc DiffractiveForwardAnalysis/GammaGammaLeptonLepton/src/StripsEfficiency.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme,40 4-B20,+41227671567,
//         Created:  Thu Sep 13 15:17:14 CET 2012
// $Id: StripsEfficiency.cc,v 1.3 2013/04/28 08:40:45 lforthom Exp $
//
//

#include "Efficiency.h"
//
// constructors and destructor
//
StripsEfficiency::StripsEfficiency(const edm::ParameterSet& iConfig) : 
  pps_tracklite_token_ ( consumes<std::vector<CTPPSLocalTrackLite>>(iConfig.getParameter<edm::InputTag>("tagTrackLites") ) ),
  //  tokenLocalTrack_  ( consumes< edm::DetSetVector<TotemRPLocalTrack> >     ( iConfig.getParameter<edm::InputTag>( "tagLocalTrack" ) ) ),
  tokenLocalTrack_  ( consumes< edm::DetSetVector<TotemRPLocalTrack> >     ( edm::InputTag("totemRPLocalTrackFitter" ) ) ),                             
  tokenUVPattern_ (consumes< edm::DetSetVector<TotemRPUVPattern> > (edm::InputTag("totemRPUVPatternFinder"))),
  tokenRecHit_ (consumes< edm::DetSetVector<TotemRPRecHit> > (edm::InputTag("totemRPRecHitProducer"))),
  tokenPixelLocalTrack_  ( consumes<edm::DetSetVector<CTPPSPixelLocalTrack> >(edm::InputTag("ctppsPixelLocalTracks") ) ),
  tokenDiamondTrack_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >( edm::InputTag( "ctppsDiamondLocalTracks" ) ) ),
  recoProtonsSingleRPToken_   ( consumes<std::vector<reco::ForwardProton> >      ( iConfig.getParameter<edm::InputTag>( "ppsRecoProtonSingleRPTag" ) ) ),
  recoProtonsMultiRPToken_   ( consumes<std::vector<reco::ForwardProton> >      ( iConfig.getParameter<edm::InputTag>( "ppsRecoProtonMultiRPTag" ) ) ),
  verticesToken_    ( consumes< edm::View<reco::Vertex> >( iConfig.getParameter<edm::InputTag>( "verticesTag" ) ) ),
  addTimingTracks_ ( iConfig.getParameter<bool>("includeTimingTracks") ),
  onlySinglePixelTrackEvents_ ( iConfig.getParameter<bool>("selectSinglePixelTrackEvents") ),
  is2016data_ ( iConfig.getParameter<bool>("is2016data") ),
  CrossingAngle(-999.0)

{
  

  //now do what ever initialization is needed
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outfilename", "output.root");
  
	
  efffile = new TFile(outputFile_.c_str(), "recreate");
  efffile->cd();
  // tree definition
  efftree = new TTree("ntp1", "ntp1");

}


StripsEfficiency::~StripsEfficiency()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

  efffile->Write();
  efffile->Close();

}


//
// member functions
//

// ------------ method called for each event  ------------
void
StripsEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //std::cout << "Beginning First init" << std::endl;

  // get event data                                                                                                                                                                                
  // First initialization of the variables
  
  // Run and BX information
  BX = iEvent.bunchCrossing();
  Run = iEvent.id().run();
  LumiSection = iEvent.luminosityBlock();
  EventNum = iEvent.id().event();


  nLiteTracks = 0;
  nStripTracks = 0;
  nPixelTracks = 0;
  nPixelTracks45 = 0;
  nPixelTracks56 = 0;
  nProtons = 0;
  nTimingTracks = 0;
  nVertices = 0;

  for(int i = 0; i < 1000; i++)
    {
      TrackLiteX[i] = 0;
      TrackLiteY[i] = 0;
      TrackLiteRPID[i] = 0;

      PixTrackX[i] = 0;
      PixTrackY[i] = 0;
      PixTrackThX[i] = 0;
      PixTrackThY[i] = 0;
      PixTrackChi2[i] = 0;
      PixTrackNdof[i] = 0;
      PixTrackZ[i] = 0;
      PixTrackArm[i] = -1;
      PixTrackShift[i] = -1;
    }
  for(int j = 0; j < 4; j++)
    {
      StripTrackValid[j] = -1;
      StripTrackX[j] = 0;
      StripTrackY[j] = 0;
      StripTrackThX[j] = 0;
      StripTrackThY[j] = 0;
      StripTrackRPID[j] = 0;
      StripTrackTooFullU[j] = 0;
      StripTrackTooFullV[j] = 0;
      StripTrackUPatterns[j] = 0;
      StripTrackVPatterns[j] = 0;
    }

  for(int k = 0; k < 100; k++)
    {
      ProtonXi[k] = -999;
      ProtonThY[k] = -999;
      ProtonThX[k] = -999;
      Protont[k] = -999;
      ProtonIsMultiRP[k] = -999;
      ProtonRPID[k] = -999;
      ProtonArm[k] = -999;

      TimingTrackX[k] = -999.;
      TimingTrackY[k] = -999.; 
      TimingTrackZ[k] = -999.;
      TimingTrackArm[k] = -999;
    }

  // Crossing angle
  edm::ESHandle<LHCInfo> hLHCInfo;
  iSetup.get<LHCInfoRcd>().get("", hLHCInfo);
  if (hLHCInfo->crossingAngle() != CrossingAngle)
    {
      CrossingAngle = hLHCInfo->crossingAngle();
    }

  /* Lite tracks */
  // Proton lite tracks                                        
  edm::Handle<std::vector<CTPPSLocalTrackLite> > ppsTracksLite;
  iEvent.getByToken( pps_tracklite_token_, ppsTracksLite );

  for ( const auto& trklite : *ppsTracksLite )
    {
      const CTPPSDetId detid( trklite.getRPId() );

      // transform the raw, 32-bit unsigned integer detId into the TOTEM "decimal" notation                                                              
      const unsigned short raw_id = 100*detid.arm()+10*detid.station()+detid.rp();
      
      TrackLiteX[nLiteTracks] = trklite.getX();
      TrackLiteY[nLiteTracks] = trklite.getY();
      TrackLiteRPID[nLiteTracks] = raw_id;

      nLiteTracks++;
    }

  // For strips efficiency studies
  edm::Handle< DetSetVector<TotemRPLocalTrack> > stripTracks;
  iEvent.getByToken(tokenLocalTrack_, stripTracks);
  edm::Handle< DetSetVector<TotemRPUVPattern> > stripPatterns;
  iEvent.getByToken(tokenUVPattern_, stripPatterns);
  edm::Handle< DetSetVector<TotemRPRecHit> > stripRecHits;
  iEvent.getByToken(tokenRecHit_, stripRecHits);

  // No pixels or diamonds in 2016
  if(is2016data_ == false)
    {
      // Pixels                                                                                                                                                                    
      edm::Handle< edm::DetSetVector<CTPPSPixelLocalTrack> > pixLocalTracks;
      iEvent.getByToken(tokenPixelLocalTrack_, pixLocalTracks);
      
      for ( const auto& dspxtr1 : *pixLocalTracks ) {
	for ( const auto& pxtr1 : dspxtr1 ) {
	  if ( ! pxtr1.isValid() )  continue;
	  
	  CTPPSDetId pxrpId1( dspxtr1.detId() );
	  unsigned int pxarm1 = pxrpId1.arm();
	  int pixshift1 = -1;
	  
	  // Check if the pixel track comes from shifted plane and skip if true:
	  
	  CTPPSpixelLocalTrackReconstructionInfo pixtrackinfo1 = pxtr1.getRecoInfo();
	  if(pixtrackinfo1 == CTPPSpixelLocalTrackReconstructionInfo::notShiftedRun || pixtrackinfo1 == CTPPSpixelLocalTrackReconstructionInfo::noShiftedPlanes ||
	     pixtrackinfo1 == CTPPSpixelLocalTrackReconstructionInfo::invalid)
	    pixshift1 = 0;
	  else
	    pixshift1 = 1;
	
	  if(pixshift1) continue; // MP: We agree with Jonathan that there will be no use of these tracks so we can skip it
	  
	  PixTrackShift[nPixelTracks] = pixshift1;   
	  PixTrackX[nPixelTracks] = pxtr1.getX0();
	  PixTrackY[nPixelTracks] = pxtr1.getY0();
	  PixTrackThX[nPixelTracks] = pxtr1.getTx();
	  PixTrackThY[nPixelTracks] = pxtr1.getTy();
	  PixTrackChi2[nPixelTracks] = pxtr1.getChiSquared();
	  PixTrackNdof[nPixelTracks] = pxtr1.getNDF();
	  PixTrackZ[nPixelTracks] = pxtr1.getZ0();
	  PixTrackArm[nPixelTracks] = pxarm1;
	  
	  
	  if(pxarm1 == 0)
	    nPixelTracks45++;
	  if(pxarm1 == 1)
	    nPixelTracks56++;
	  
	  nPixelTracks++;
	}
      }
      
      /* Diamond tracks */
      edm::Handle< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamondLocalTracks;
      iEvent.getByToken( tokenDiamondTrack_, diamondLocalTracks );
      
      if(addTimingTracks_ == true)
	{
	  for ( const auto& ds2 : *diamondLocalTracks )
	    {
	      for ( const auto& tr2 : ds2 )
		{
		  if ( ! tr2.isValid() ) continue;
		  
		  CTPPSDetId diamId2( ds2.detId() );
		  unsigned int arm1 = diamId2.arm();
		  
		  TimingTrackX[nTimingTracks] = tr2.getX0();
		  TimingTrackY[nTimingTracks] = tr2.getY0();
		  TimingTrackZ[nTimingTracks] = tr2.getZ0();
		  TimingTrackArm[nTimingTracks] = arm1;
		  
		  nTimingTracks++;
		}
	    }
	}
    }

  // Jan...
  struct StripInfo
  {
    std::set<unsigned int> u_planes;
    std::set<unsigned int> v_planes;

    std::map<unsigned int, unsigned int> clustersPerPlane;

    bool u_tooFull = false;
    bool v_tooFull = false;

    unsigned int u_patterns = 0;
    unsigned int v_patterns = 0;

    int hasValidTrack = -1;
    float x = 0;
    float y = 0;
    float thx = 0;
    float thy = 0;
    int rpid = 0;
  };

  std::map<unsigned int, StripInfo> stripInfo;

  for (const auto &dsRecHits : *stripRecHits)
    {
      TotemRPDetId planeId(dsRecHits.detId());
      unsigned int rpDecId = planeId.arm()*100 + planeId.station()*10 + planeId.rp();
      unsigned int planeIdx = planeId.plane();

      stripInfo[rpDecId].clustersPerPlane[planeIdx] = dsRecHits.size();

      if (dsRecHits.size() == 0)
	continue;

      if ((planeIdx % 2) == 0)
	{
	  stripInfo[rpDecId].rpid = rpDecId;
	  stripInfo[rpDecId].v_planes.insert(planeIdx);
	}
      else
	{
	  stripInfo[rpDecId].rpid = rpDecId;
	  stripInfo[rpDecId].u_planes.insert(planeIdx);
	}
    }

  for (const auto &dsPatterns : *stripPatterns)
    {
      TotemRPDetId rpId(dsPatterns.detId());
      unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

      for (const auto &pat : dsPatterns)
	{
	  if (! pat.getFittable())
	    continue;

	  if (pat.getProjection() == TotemRPUVPattern::projU)
	    {
	      stripInfo[rpDecId].rpid = rpDecId;
	      stripInfo[rpDecId].u_patterns++;
	    }
	  if (pat.getProjection() == TotemRPUVPattern::projV)
	    {
              stripInfo[rpDecId].rpid = rpDecId;
	      stripInfo[rpDecId].v_patterns++;
	    }
	}
    }

  for (const auto &dsTracks : *stripTracks)
    {
      TotemRPDetId rpId(dsTracks.detId());
      unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

      for (const auto &tr : dsTracks)
	{
	  if (tr.isValid())
	    {
	      stripInfo[rpDecId].hasValidTrack = 1;
	      stripInfo[rpDecId].x = tr.getX0();
	      stripInfo[rpDecId].y = tr.getY0();
	      stripInfo[rpDecId].thx = tr.getTx();
	      stripInfo[rpDecId].thy = tr.getTy();
	      stripInfo[rpDecId].rpid = rpDecId;
	    }
	  else
	    {
              stripInfo[rpDecId].hasValidTrack = 0;
              stripInfo[rpDecId].x = tr.getX0();
              stripInfo[rpDecId].y = tr.getY0();
              stripInfo[rpDecId].thx = tr.getTx();
              stripInfo[rpDecId].thy = tr.getTy();
              stripInfo[rpDecId].rpid = rpDecId;
	    }
	}
    }

  for (auto &p : stripInfo)
    {
      auto &rpInfo = p.second;

      unsigned int n_too_full_u = 0, n_too_full_v = 0;


      for (const auto &clP : rpInfo.clustersPerPlane)
	{
	  if (clP.second <= 5)
	    continue;

	  if ((clP.first % 2) == 1)
	    n_too_full_u++;
	  else
	    n_too_full_v++;
	}

      rpInfo.u_tooFull = (n_too_full_u >= 3);
      rpInfo.v_tooFull = (n_too_full_v >= 3);

      StripTrackValid[nStripTracks] = rpInfo.hasValidTrack;
      StripTrackX[nStripTracks] = rpInfo.x;
      StripTrackY[nStripTracks] = rpInfo.y;
      StripTrackThX[nStripTracks] = rpInfo.thx;
      StripTrackThY[nStripTracks] = rpInfo.thy;
      StripTrackRPID[nStripTracks] = rpInfo.rpid;
      StripTrackTooFullU[nStripTracks] = n_too_full_u;
      StripTrackTooFullV[nStripTracks] = n_too_full_v;
      StripTrackUPatterns[nStripTracks] = rpInfo.u_patterns;
      StripTrackVPatterns[nStripTracks] = rpInfo.v_patterns;
      nStripTracks++;
    }

  /* Full Reco protons */
  edm::Handle<std::vector<reco::ForwardProton>> recoMultiRPProtons;
  iEvent.getByToken(recoProtonsMultiRPToken_, recoMultiRPProtons);
  edm::Handle<std::vector<reco::ForwardProton>> recoSingleRPProtons;
  iEvent.getByToken(recoProtonsSingleRPToken_, recoSingleRPProtons);


  // make single-RP-reco plots                                                                                                                     
  int ismultirp = -999;
  unsigned int decRPId = -999;
  unsigned int armId = -999;
  float th_y = -999;
  float th_x = -999;
  float t = -999;
  float xi = -999;
  
  // Single-RP algorithm                                                                                                                                            
  for (const auto & proton : *recoSingleRPProtons)
    {
      if (proton.validFit())
        {
          th_y = proton.thetaY();
          th_x = proton.thetaX();
          xi = proton.xi();
          //      t = proton.t();                                                                                                                                   

          CTPPSDetId rpId((*proton.contributingLocalTracks().begin())->getRPId());
          decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

          ismultirp = 0;

          ProtonXi[nProtons] = xi;
          ProtonThX[nProtons] = th_x;
          ProtonThY[nProtons] = th_y;
          Protont[nProtons] = t;
          ProtonIsMultiRP[nProtons] = ismultirp;
          ProtonRPID[nProtons] = decRPId;
	  //          ProtonArm[nProtons] = armId;
          nProtons++;
        }
    }

  for (const auto & proton : *recoMultiRPProtons)
    {
      if (proton.validFit())
        {
          th_y = proton.thetaY();
          th_x = proton.thetaX();
          xi = proton.xi();
          //      t = proton.t();                                                                                                                                   

          CTPPSDetId rpId((*proton.contributingLocalTracks().begin())->getRPId());
          armId = rpId.arm();

          ismultirp = 1;

          ProtonXi[nProtons] = xi;
          ProtonThX[nProtons] = th_x;
          ProtonThY[nProtons] = th_y;
          Protont[nProtons] = t;
          ProtonIsMultiRP[nProtons] = ismultirp;
	  //          ProtonRPID[nProtons] = decRPId;
          ProtonArm[nProtons] = armId;
          nProtons++;
        }
    }

  // Pedro...
  edm::Handle< edm::View<reco::Vertex> > vertices;
  iEvent.getByToken( verticesToken_, vertices );

  /* Primary vertices */
  for ( const auto& vtx : *vertices ) {
    if(vtx.isFake() != 1)
      nVertices++;
  }

  if((onlySinglePixelTrackEvents_ == true && (nPixelTracks45==1 || nPixelTracks56==1)) || (onlySinglePixelTrackEvents_ == false))
    efftree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
StripsEfficiency::beginJob()
{
  // Booking the ntuple

  efftree->Branch("Run", &Run, "Run/I");
  efftree->Branch("LumiSection", &LumiSection, "LumiSection/I");
  efftree->Branch("BX", &BX, "BX/I");
  efftree->Branch("EventNum", &EventNum, "EventNum/I");
  efftree->Branch("CrossingAngle", &CrossingAngle, "CrossingAngle/F");

  efftree->Branch("nLiteTracks", &nLiteTracks, "nLiteTracks/I");
  efftree->Branch("TrackLiteX", &TrackLiteX, "TrackLiteX[nLiteTracks]/F");
  efftree->Branch("TrackLiteY", &TrackLiteY, "TrackLiteY[nLiteTracks]/F");
  efftree->Branch("TrackLiteRPID", &TrackLiteRPID, "TrackLiteRPID[nLiteTracks]/I");

  efftree->Branch("nStripTracks", &nStripTracks, "nStripTracks/I");
  efftree->Branch("StripTrackValid", &StripTrackValid, "StripTrackValid[nStripTracks]/I");
  efftree->Branch("StripTrackX", &StripTrackX, "TrackLiteX[nStripTracks]/F");
  efftree->Branch("StripTrackY", &StripTrackY, "TrackLiteY[nStripTracks]/F");
  efftree->Branch("StripTrackThX", &StripTrackThX, "TrackLiteThX[nStripTracks]/F");
  efftree->Branch("StripTrackThY", &StripTrackThY, "TrackLiteThY[nStripTracks]/F");
  efftree->Branch("StripTrackRPID", &StripTrackRPID, "TrackLiteRPID[nStripTracks]/I");
  efftree->Branch("StripTrackTooFullU", &StripTrackTooFullU, "StripTrackTooFullU[nStripTracks]/I");
  efftree->Branch("StripTrackTooFullV", &StripTrackTooFullV, "StripTrackTooFullV[nStripTracks]/I");
  efftree->Branch("StripTrackUPatterns", &StripTrackUPatterns, "StripTrackUPatterns[nStripTracks]/I");
  efftree->Branch("StripTrackVPatterns", &StripTrackVPatterns, "StripTrackVPatterns[nStripTracks]/I");

  efftree->Branch("nPixelTracks", &nPixelTracks, "nPixelTracks/I");
  efftree->Branch("PixTrackX", &PixTrackX, "PixTrackX[nPixelTracks]/D");
  efftree->Branch("PixTrackY", &PixTrackY, "PixTrackY[nPixelTracks]/D");
  efftree->Branch("PixTrackThX", &PixTrackThX, "PixTrackThX[nPixelTracks]/D");
  efftree->Branch("PixTrackThY", &PixTrackThY, "PixTrackThY[nPixelTracks]/D");
  efftree->Branch("PixTrackChi2", &PixTrackChi2, "PixTrackChi2[nPixelTracks]/D");
  efftree->Branch("PixTrackNdof", &PixTrackNdof, "PixTrackNdof[nPixelTracks]/I");
  efftree->Branch("PixTrackZ", &PixTrackZ, "PixTrackZ[nPixelTracks]/D");
  efftree->Branch("PixTrackArm", &PixTrackArm, "PixTrackArm[nPixelTracks]/I");
  efftree->Branch("PixTrackShift", &PixTrackShift, "PixTrackShift[nPixelTracks]/I");

  efftree->Branch("nTimingTracks", &nTimingTracks, "nTimingTracks/I");
  efftree->Branch("TimingTrackX", &TimingTrackX, "TimingTrackX[nTimingTracks]/D");
  efftree->Branch("TimingTrackY", &TimingTrackY, "TimingTrackY[nTimingTracks]/D");
  efftree->Branch("TimingTrackZ", &TimingTrackZ, "TimingTrackZ[nTimingTracks]/D");
  efftree->Branch("TimingTrackArm", &TimingTrackArm, "TimingTrackArm[nTimingTracks]/I");

  efftree->Branch("nProtons", &nProtons, "nProtons/I");
  efftree->Branch("ProtonXi", &ProtonXi, "ProtonXi[nProtons]/F");
  efftree->Branch("ProtonThX", &ProtonThX, "ProtonThX[nProtons]/F");
  efftree->Branch("ProtonThY", &ProtonThY, "ProtonThY[nProtons]/F");
  efftree->Branch("Protont", &Protont, "Protont[nProtons]/F");
  efftree->Branch("ProtonIsMultiRP", &ProtonIsMultiRP, "ProtonIsMultiRP[nProtons]/I");
  efftree->Branch("ProtonRPID", &ProtonRPID, "ProtonRPID[nProtons]/I");
  efftree->Branch("ProtonArm", &ProtonArm, "ProtonArm[nProtons]/I");

  efftree->Branch("nVertices", &nVertices, "nVertices/I");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
StripsEfficiency::endJob() 
{
  //  std::cout << "==> Number of candidates in the dataset : " << nCandidates << std::endl;
}

// ------------ method called when starting to processes a run  ------------
void 
StripsEfficiency::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
StripsEfficiency::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
StripsEfficiency::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
StripsEfficiency::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
StripsEfficiency::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(StripsEfficiency);
