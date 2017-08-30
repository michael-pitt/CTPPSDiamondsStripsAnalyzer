// -*- C++ -*-
//
// Package:    Diamonds
// Class:      Diamonds
// 
/**\class Diamonds Diamonds.cc DiffractiveForwardAnalysis/GammaGammaLeptonLepton/src/Diamonds.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme,40 4-B20,+41227671567,
//         Created:  Thu Sep 13 15:17:14 CET 2012
// $Id: Diamonds.cc,v 1.3 2013/04/28 08:40:45 lforthom Exp $
//
//

#include "Diamonds.h"
//
// constructors and destructor
//
Diamonds::Diamonds(const edm::ParameterSet& iConfig) : 
  tokenStatus_      ( consumes< edm::DetSetVector<TotemVFATStatus> >       ( iConfig.getParameter<edm::InputTag>( "tagStatus" ) ) ),
  tokenDigi_        ( consumes< edm::DetSetVector<CTPPSDiamondDigi> >      ( iConfig.getParameter<edm::InputTag>( "tagDigi" ) ) ),
  tokenDiamondHit_  ( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >    ( iConfig.getParameter<edm::InputTag>( "tagDiamondRecHits" ) ) ),
  tokenDiamondTrack_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >( iConfig.getParameter<edm::InputTag>( "tagDiamondLocalTracks" ) ) ),
  tokenLocalTrack_  ( consumes< edm::DetSetVector<TotemRPLocalTrack> >     ( iConfig.getParameter<edm::InputTag>( "tagLocalTrack" ) ) ),
  tokenFEDInfo_     ( consumes< std::vector<TotemFEDInfo> >                ( iConfig.getParameter<edm::InputTag>( "tagFEDInfo" ) ) ),
  tokenPixelDigi_   ( consumes<edm::DetSetVector<CTPPSPixelDigi> >(iConfig.getParameter<edm::InputTag>("tagRPixDigi") ) ),
  tokenPixelCluster_ ( consumes<edm::DetSetVector<CTPPSPixelCluster> >(iConfig.getParameter<edm::InputTag>("tagRPixCluster") ) ),
  tokenPixelRecHit_  ( consumes<edm::DetSetVector<CTPPSPixelRecHit> >(iConfig.getParameter<edm::InputTag>("tagRPixRecHit") ) ),
  verticesToken_    ( consumes< edm::View<reco::Vertex> >( iConfig.getParameter<edm::InputTag>( "verticesTag" ) ) ),
  jetsToken_        ( consumes< reco::PFJetCollection >(iConfig.getParameter<edm::InputTag>( "jetsTag" ) ) ),
  pflowToken_       ( consumes< reco::PFCandidateCollection >( iConfig.getParameter<edm::InputTag>("tagParticleFlow") ) )
{
  

  //now do what ever initialization is needed
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outfilename", "output.root");
  
	
  file = new TFile(outputFile_.c_str(), "recreate");
  file->cd();
  // tree definition
  tree = new TTree("ntp1", "ntp1");

}


Diamonds::~Diamonds()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

  file->Write();
  file->Close();

}


//
// member functions
//

// ------------ method called for each event  ------------
void
Diamonds::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //std::cout << "Beginning First init" << std::endl;

  // get event data                                                                                                                                                                                
  edm::Handle< edm::DetSetVector<TotemVFATStatus> > diamondVFATStatus;
  iEvent.getByToken( tokenStatus_, diamondVFATStatus );

  edm::Handle< edm::DetSetVector<CTPPSDiamondDigi> > diamondDigis;
  iEvent.getByToken( tokenDigi_, diamondDigis );

  edm::Handle< std::vector<TotemFEDInfo> > fedInfo;
  iEvent.getByToken( tokenFEDInfo_, fedInfo );

  edm::Handle< edm::DetSetVector<CTPPSDiamondRecHit> > diamondRecHits;
  iEvent.getByToken( tokenDiamondHit_, diamondRecHits );

  edm::Handle< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamondLocalTracks;
  iEvent.getByToken( tokenDiamondTrack_, diamondLocalTracks );

  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripTracks;
  iEvent.getByToken( tokenLocalTrack_, stripTracks );

  edm::Handle< edm::DetSetVector<CTPPSPixelDigi> > pixDigi;
  iEvent.getByToken(tokenPixelDigi_, pixDigi);

  edm::Handle< edm::DetSetVector<CTPPSPixelCluster> > pixClusters;
  iEvent.getByToken(tokenPixelCluster_, pixClusters);

  edm::Handle< edm::DetSetVector<CTPPSPixelRecHit> > pixRecHits;
  iEvent.getByToken(tokenPixelRecHit_, pixRecHits);

  edm::Handle< edm::View<reco::Vertex> > vertices;
  iEvent.getByToken( verticesToken_, vertices );

  edm::Handle<reco::PFJetCollection> pfjets;
  iEvent.getByToken( jetsToken_, pfjets );

  edm::Handle<reco::PFCandidateCollection> pflowColl;
  iEvent.getByToken(pflowToken_, pflowColl);
  const reco::PFCandidateCollection pfcand = *(pflowColl.product());

  // check validity                                                                                                                                                                                
  bool valid = true;
  valid &= diamondVFATStatus.isValid();
  valid &= diamondDigis.isValid();
  valid &= fedInfo.isValid();


  // First initialization of the variables
  
  // Run and BX information
  BX = iEvent.bunchCrossing();
  Run = iEvent.id().run();
  LumiSection = iEvent.luminosityBlock();
  EventNum = iEvent.id().event();

  nArmsTiming = 0;
  nHitsTiming = 0;
  nTracksTiming = 0;
  nRecHitsTiming = 0;
  nLayersArm1Timing = 0;
  nLayersArm2Timing = 0;
  nArmsStrips = 0;
  nTracksStrips = 0;
  nPixelRecHits = 0;
  nArmsPixelDigis = 0;
  nLayersArm1PixelDigis = 0;
  nLayersArm2PixelDigis = 0;
  nArmsPixRecHits = 0;
  nLayersArm1PixRecHits = 0;
  nLayersArm2PixRecHits = 0;
  nVertices = 0;
  nJets = 0;
  nPFCand = 0;
  PFCentralY = 0;
  PFCentralMass = 0;
  DijetMass = 0.0;
  DijetY = 0.0;
  nHFplus = 0;
  HFplusE = 0.0;
  nHFminus = 0;
  HFminusE = 0.0;


  int is45 = 0;
  int is56 = 0;
  int is45strips = 0;
  int is56strips = 0;

  int layer1arm1 = 0; int layer2arm1 = 0; int layer3arm1 = 0; int layer4arm1 = 0;
  int layer1arm2 = 0; int layer2arm2 = 0; int layer3arm2 = 0; int layer4arm2 = 0;
  int nlayersarm1 = 0;
  int nlayersarm2 = 0;

  unsigned int Channel = -1;
  unsigned int Arm = -1;
  double LE = 0.0; double TE = 0.0;
  unsigned int MH = -1;

  for(int i = 0; i < 100; i++)
    {
      PrimVertexZ[i] = -999.;
      PrimVertexIsBS[i] = -999;
      ArmTiming[i] = -1;
      LeadingEdge[i] = 0;
      TrailingEdge[i] = 0;
      ToT[i] = 0;
      MultiHit[i] = 0;
      OOTIndex[i] = 0;
      ChannelTiming[i] = -999;
      PlaneTiming[i] = -999;
      XTiming[i] = -999;
      YTiming[i] = -999;
      TimingTrackT[i] = 0;
      TimingTrackTErr[i] = 0;
      TimingTrackX[i] = -999; 
      TimingTrackY[i] = -999;
      TimingTrackZ[i] = -999;
      TimingTrackOOTIndex[i] = -999;
      TimingTrackMultiHit[i] = -999;
      TimingTrackChi2[i] = -999;
      TimingRecHitT[i] = -999;
      TimingRecHitX[i] = -999;
      TimingRecHitY[i] = -999;
      TimingRecHitOOTIndex[i] = -999; 
      TimingRecHitMultiHit[i] = -999;
      TimingRecHitToT[i] = 0;
      TimingRecHitChannel[i] = -999;
      TimingRecHitArm[i] = -999;
      TimingRecHitPlane[i] = -999;
      ArmStrips[i] = -1;
      StripTrackX[i] = 0; 
      StripTrackY[i] = 0;
      StripTrackTx[i] = 0;
      StripTrackTy[i] = 0;
    }

  for(int i = 0; i < 1000; i++)
    {
      PixRecHitX[i] = 0;
      PixRecHitY[i] = 0;
      PixRecHitZ[i] = 0;
      PixRecHitArm[i] = -1;
      PixRecHitPlane[i] = -1;
    }

  for(int i = 0; i < 100; i++)
    {
      JetCandEt[i] = 0;
      JetCandEta[i] = 0;
      JetCandPhi[i] = 0;
      JetCandE[i] = 0;
    }

  //  for(int i = 0; i < 1000; i++)
  //    {
  //      PFCandID[i] = 0;
  //      PFCandE[i] = 0;
  //      PFCandEta[i] = 0;
  //      PFCandPhi[i] = 0;
  //    }

  /* RecHits - timing */
  for ( const auto& rechits_ds : *diamondRecHits ) {
    const CTPPSDiamondDetId detidforrh( rechits_ds.detId() );
    for ( const auto& rechit : rechits_ds ) {

      TimingRecHitArm[nRecHitsTiming] = detidforrh.arm();
      TimingRecHitChannel[nRecHitsTiming] = detidforrh.channel();      
      TimingRecHitPlane[nRecHitsTiming] = detidforrh.plane();
      TimingRecHitT[nRecHitsTiming] = rechit.getT();
      TimingRecHitX[nRecHitsTiming] = rechit.getX();                                                                                                            TimingRecHitY[nRecHitsTiming] = rechit.getY();                                                                                                            TimingRecHitOOTIndex[nRecHitsTiming] = rechit.getOOTIndex();                                                                                              TimingRecHitMultiHit[nRecHitsTiming] = rechit.getMultipleHits();
      TimingRecHitToT[nRecHitsTiming] = rechit.getToT();
      nRecHitsTiming++;
    }
  }


  /* Diamond tracks */
  for ( const auto& ds2 : *diamondLocalTracks ) 
    {
      for ( const auto& tr2 : ds2 ) 
	{
	  if ( ! tr2.isValid() ) continue;
	  
	  CTPPSDetId diamId2( ds2.detId() );
	  //	  unsigned int arm2 = diamId2.arm();
	  TimingTrackT[nTracksTiming] = tr2.getT();
	  TimingTrackTErr[nTracksTiming] = tr2.getTSigma();
	  TimingTrackX[nTracksTiming] = tr2.getX0();
	  TimingTrackY[nTracksTiming] = tr2.getY0();
	  TimingTrackZ[nTracksTiming] = tr2.getZ0();
	  TimingTrackOOTIndex[nTracksTiming] = tr2.getOOTIndex();
	  TimingTrackMultiHit[nTracksTiming] = tr2.getMultipleHits();
	  TimingTrackChi2[nTracksTiming] = tr2.getChiSquared();
	  nTracksTiming++;
	}
    }

  /* Digis - timing */
  Channel = -1;                                                                                                                                    
  Arm = -1;                                                                                                                                        
  LE = 0;                                                                                                                                          
  TE = 0;                                                                                                                                          
  MH = 0;                                                                                                                                          

  for ( const auto& digis : *diamondDigis ) {                                                                                                      
    const CTPPSDiamondDetId detId( digis.detId() );                                                                                                
    CTPPSDiamondDetId detId_pot( digis.detId() );                                                                                                  
    for ( const auto& digi : digis ) {                                                                                                             
      detId_pot.setPlane( 0 );                                                                                                                     
      detId_pot.setChannel( 0 );                                                                                                                   

      LE = digi.getLeadingEdge() * 0.025;                                                                                                          
      TE = digi.getTrailingEdge() * 0.025;                                                                                                         

      Channel = detId.channel();                                                                                                                   
      MH = digi.getMultipleHit();                                                                                                                  
      Arm = detId.arm();                                                                                                                           

      // Clock
      if(Channel != 30)
	{
	  ArmTiming[nHitsTiming] = Arm;                                                                                                            
	  if(ArmTiming[nHitsTiming] == 0 && LE > 0)                                                                                                
	    {                                                                                                                                       
	      is45=1;                                                                                                                              
	      if(detId.plane() == 0) layer1arm1 = 1;                                                                                               
	      if(detId.plane() == 1) layer2arm1 = 1;                                                                                               
	      if(detId.plane() == 2) layer3arm1 = 1;                                                                                               
	      if(detId.plane() == 3) layer4arm1 = 1;                                                                                               
	    }                                                                                                                                      
 	  
	  if(ArmTiming[nHitsTiming] == 1 && LE > 0)                                                                                                
	    {                                                                                                                                      
	      is56=1;                                                                                                                              
	      if(detId.plane() == 0) layer1arm2 = 1;                                                                                               
	      if(detId.plane() == 1) layer2arm2 = 1;                                                                                               
	      if(detId.plane() == 2) layer3arm2 = 1;                                                                                               
	      if(detId.plane() == 3) layer4arm2 = 1;                                                                                               
	    }                                                                                                                                      
	  
	  ChannelTiming[nHitsTiming] = Channel;                                                                                                    
	  PlaneTiming[nHitsTiming] = detId.plane();                                                                                                
	  LeadingEdge[nHitsTiming] = LE;                                                                                                           
             
	  TrailingEdge[nHitsTiming] = TE;
	  ToT[nHitsTiming] = TE - LE;
	  MultiHit[nHitsTiming] = MH;
	  //      OOTIndex[nHitsTiming] = rechit.getOOTIndex();                                                                                 

	  nHitsTiming++;
	}         
    }
  }                                                                                                                                                
  
  nArmsTiming = is45+is56;
  nlayersarm1 = layer1arm1 + layer2arm1 + layer3arm1 + layer4arm1;
  nlayersarm2 = layer1arm2 + layer2arm2 + layer3arm2 + layer4arm2;
  nLayersArm1Timing = nlayersarm1;
  nLayersArm2Timing = nlayersarm2;

  /* Strips */
  for ( const auto& ds1 : *stripTracks ) {
    for ( const auto& tr1 : ds1 ) {
      if ( ! tr1.isValid() )  continue;

      CTPPSDetId rpId1( ds1.detId() );
      unsigned int arm1 = rpId1.arm();
      unsigned int stNum1 = rpId1.station();
      unsigned int rpNum1 = rpId1.rp();
      if (stNum1 != 0 || ( rpNum1 != 2 && rpNum1 != 3 ) )  continue;

      StripTrackX[nTracksStrips] = tr1.getX0();
      StripTrackY[nTracksStrips] = tr1.getY0();
      StripTrackTx[nTracksStrips] = tr1.getTx();
      StripTrackTy[nTracksStrips] = tr1.getTy();
      ArmStrips[nTracksStrips] = arm1;

      if(arm1 == 0)
	is45strips = 1;
      if(arm1 == 1)
	is56strips = 1;

      nTracksStrips++;
    }
  }

  nArmsStrips = is45strips+is56strips;

  /* Pixel digis */
  int pixelsarm1 = 0;
  int pixelsarm2 = 0;
  int planespixelsarm1 = 0;
  int planespixelsarm2 = 0;

  int is45pixels = 0;
  int is56pixels = 0;
  int prevplane1 = -1;
  int prevplane2 = -1;

  if(pixDigi.isValid())
    {
      for(const auto &ds_digi : *pixDigi)
        {
          int idet = (ds_digi.id>>DetId::kDetOffset)&0xF;
          if(idet != DetId::VeryForward) {
            continue;
          }

          int plane = ((ds_digi.id>>16)&0x7);

          CTPPSDetId theId(ds_digi.id);
          int pixelsarm = theId.arm()&0x1;

          if(pixelsarm == 0)
            {
              if(plane != prevplane1)
                {
                  planespixelsarm1++;
                  prevplane1 = plane;
                }
              pixelsarm1++;
	      is45pixels = 1;
            }
          if(pixelsarm == 1)
            {
              if(plane != prevplane2)
                {
                  planespixelsarm2++;
                  prevplane2 = plane;
                }
              pixelsarm2++;
	      is56pixels = 1;
            }
	  //          int station = theId.station()&0x3;
	  //          int rpot = theId.rp()&0x7;
        }
    }

  nArmsPixelDigis = is45pixels + is56pixels; 
  nLayersArm1PixelDigis = planespixelsarm1;
  nLayersArm2PixelDigis = planespixelsarm2;

  /* Pixel RecHits */
  int pixelsrharm1 = 0;
  int pixelsrharm2 = 0;
  int planespixelsrharm1 = 0;
  int planespixelsrharm2 = 0;

  int is45pixelsrh = 0;
  int is56pixelsrh = 0;
  unsigned int pixelsrhprevplane1 = -1;
  unsigned int pixelsrhprevplane2 = -1;

  if(pixRecHits.isValid())
    {
      for ( const auto& rechits_px : *pixRecHits ) 
	{
	  const CTPPSPixelDetId pxDetid( rechits_px.detId() );
	  for ( const auto& rechitpx : rechits_px ) 
	    {
	      PixRecHitX[nPixelRecHits] = rechitpx.getPoint().x();
	      PixRecHitY[nPixelRecHits] = rechitpx.getPoint().y();
	      PixRecHitZ[nPixelRecHits] = rechitpx.getPoint().z();
	      PixRecHitArm[nPixelRecHits] = pxDetid.arm();
              PixRecHitPlane[nPixelRecHits] = pxDetid.plane();
	      nPixelRecHits++;

	      if(pxDetid.arm() == 0)
		{
		  if(pxDetid.plane() != pixelsrhprevplane1)
		    {
		      planespixelsrharm1++;
		      pixelsrhprevplane1 = pxDetid.plane();
		    }
		  pixelsrharm1++;
		  is45pixelsrh = 1;
		}
	      if(pxDetid.arm() == 1)
		{
		  if(pxDetid.plane() != pixelsrhprevplane2)
		    {
		      planespixelsrharm2++;
		      pixelsrhprevplane2 = pxDetid.plane();
		    }
		  pixelsrharm2++;
		  is56pixelsrh = 1;
		}
	    }
	}
    }

  nArmsPixRecHits = is45pixelsrh + is56pixelsrh;
  nLayersArm1PixRecHits = planespixelsrharm1;
  nLayersArm2PixRecHits = planespixelsrharm2;


  /* Primary vertices */
  for ( const auto& vtx : *vertices ) {                                                                                                                                        
    PrimVertexZ[nVertices] = vtx.z();
    if(vtx.isFake() == 1)
      PrimVertexIsBS[nVertices] = 1;
    else
      PrimVertexIsBS[nVertices] = 0;
    nVertices++;
  }


  /* Jets */
  for(reco::PFJetCollection::const_iterator it=pfjets->begin(); it!=pfjets->end(); ++it) {
    const reco::PFJet* pfjet=&(*it);
    JetCandEt[nJets] = pfjet->et();                                                                                                                                      
    JetCandEta[nJets] = pfjet->eta();                                                                                                                                    
    JetCandPhi[nJets] = pfjet->phi();                                                                                                                                    
    JetCandE[nJets] = pfjet->energy();
    nJets++;                                                                                                                                                             
  }

  TLorentzVector jet1, jet2, dijet;
  if(nJets >= 2)
    {
      jet1.SetPtEtaPhiE(JetCandEt[0],JetCandEta[0],JetCandPhi[0],JetCandE[0]);
      jet2.SetPtEtaPhiE(JetCandEt[1],JetCandEta[1],JetCandPhi[1],JetCandE[1]);
      dijet = jet1 + jet2;
      DijetMass = dijet.M();
      DijetY = dijet.Rapidity();
    }

  /* Do PF sums */
  TLorentzVector particle, central;
  central.SetPxPyPzE(0,0,0,0);

  for(reco::PFCandidateCollection::const_iterator pflow=pfcand.begin(); pflow!=pfcand.end(); pflow++) {
    int pid = pflow->particleId();
    float pfe = pflow->energy();

    if(pid == 6 && pfe < 4.0)
      continue;
    if(pid == 7 && pfe < 3.5)
      continue;
    if(pid == 4 && pfe < 2.5 && fabs(pflow->eta() >= 1.48))
      continue;
    if(pid == 4 && pfe < 0.9 && fabs(pflow->eta() < 1.48))
      continue;
    if(pid == 5 && pfe < 1.4 && fabs(pflow->eta() < 1.48)) 
      continue;
    if(pid == 5 && pfe < 2.7 && fabs(pflow->eta() > 1.48) && fabs(pflow->eta() < 1.74))
      continue;
    if(pid == 5 && pfe < 3.8 && fabs(pflow->eta() >= 1.74))
      continue;

    if((pid == 6) && (pflow->eta() > 0))
      {
	nHFplus++;
	HFplusE += pfe;
      }
    if((pid == 6) && (pflow->eta() < 0))
      {
        nHFminus++;
        HFminusE += pfe;
      }


    //    PFCandID[nPFCand] = pid;
    //    PFCandE[nPFCand] = pflow->energy();
    //    PFCandEta[nPFCand] = pflow->eta();
    //    PFCandPhi[nPFCand] = pflow->phi();

    particle.SetPxPyPzE(pflow->px(),pflow->py(),pflow->pz(),pflow->energy());
    central = central + particle;
    nPFCand++;
  }

  PFCentralMass = central.M();
  PFCentralY = central.Rapidity();

  tree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
Diamonds::beginJob()
{
  // Booking the ntuple

  tree->Branch("Run", &Run, "Run/I");
  tree->Branch("LumiSection", &LumiSection, "LumiSection/I");
  tree->Branch("BX", &BX, "BX/I");
  tree->Branch("EventNum", &EventNum, "EventNum/I");

  tree->Branch("nHitsTiming", &nHitsTiming, "nHitsTiming/I");
  tree->Branch("LeadingEdge", &LeadingEdge, "LeadingEdge[nHitsTiming]/D");
  tree->Branch("TrailingEdge", &TrailingEdge, "TrailingEdge[nHitsTiming]/D");
  tree->Branch("ToT", &ToT, "ToT[nHitsTiming]/D");
  tree->Branch("ChannelTiming", &ChannelTiming, "ChannelTiming[nHitsTiming]/I");
  tree->Branch("PlaneTiming", &PlaneTiming, "PlaneTiming[nHitsTiming]/I");
  tree->Branch("ArmTiming", &ArmTiming, "ArmTiming[nHitsTiming]/I");
  tree->Branch("XTiming", &XTiming, "XTiming[nHitsTiming]/D");
  tree->Branch("YTiming", &YTiming, "YTiming[nHitsTiming]/D");

  tree->Branch("nRecHitsTiming", &nRecHitsTiming, "nRecHitsTiming/I");
  tree->Branch("TimingRecHitArm",&TimingRecHitArm,"TimingRecHitArm[nRecHitsTiming]/I");
  tree->Branch("TimingRecHitChannel",&TimingRecHitChannel,"TimingRecHitChannel[nRecHitsTiming]/I");
  tree->Branch("TimingRecHitPlane",&TimingRecHitPlane,"TimingRecHitPlane[nRecHitsTiming]/I");
  tree->Branch("TimingRecHitT",&TimingRecHitT,"TimingRecHitT[nRecHitsTiming]/D");
  tree->Branch("TimingRecHitX",&TimingRecHitX,"TimingRecHitX[nRecHitsTiming]/D");
  tree->Branch("TimingRecHitY",&TimingRecHitY,"TimingRecHitY[nRecHitsTiming]/D");
  tree->Branch("TimingRecHitToT",&TimingRecHitToT,"TimingRecHitToT[nRecHitsTiming]/D");
  tree->Branch("TimingRecHitOOTIndex",&TimingRecHitOOTIndex,"TimingRecHitOOTIndex[nRecHitsTiming]/I");
  tree->Branch("TimingRecHitMultiHit",&TimingRecHitMultiHit,"TimingRecHitMultiHit[nRecHitsTiming]/I");

  tree->Branch("nTracksTiming", &nTracksTiming, "nTracksTiming/I");
  tree->Branch("TimingTrackT", &TimingTrackT, "TimingTrackT[nTracksTiming]/D");
  tree->Branch("TimingTrackTErr", &TimingTrackTErr, "TimingTrackTErr[nTracksTiming]/D");
  tree->Branch("TimingTrackX", &TimingTrackX, "TimingTrackX[nTracksTiming]/D");
  tree->Branch("TimingTrackY", &TimingTrackY, "TimingTrackY[nTracksTiming]/D");
  tree->Branch("TimingTrackZ", &TimingTrackZ, "TimingTrackZ[nTracksTiming]/D");
  tree->Branch("TimingTrackOOTIndex", &TimingTrackOOTIndex, "TimingTrackOOTIndex[nTracksTiming]/I");
  tree->Branch("TimingTrackMultiHit", &TimingTrackMultiHit, "TimingTrackMultiHit[nTracksTiming]/I");
  tree->Branch("TimingTrackChi2", &TimingTrackChi2, "TimingTrackChi2[nTracksTiming]/D");

  //  tree->Branch("IsClock", &IsClock, "IsClock[nHitsTiming]/I");
  tree->Branch("nVertices", &nVertices, "nVertices/I");
  tree->Branch("nArmsTiming", &nArmsTiming, "nArmsTiming/I");
  tree->Branch("nLayersArm1Timing", &nLayersArm1Timing, "nLayersArm1Timing/I");
  tree->Branch("nLayersArm2Timing", &nLayersArm2Timing, "nLayersArm2Timing/I");

  tree->Branch("nTracksStrips", &nTracksStrips, "nTracksStrips/I");
  tree->Branch("StripTrackX", &StripTrackX, "StripTrackX[nTracksStrips]/D");
  tree->Branch("StripTrackY", &StripTrackY, "StripTrackY[nTracksStrips]/D");
  tree->Branch("StripTrackTx", &StripTrackTx, "StripTrackTx[nTracksStrips]/D");
  tree->Branch("StripTrackTy", &StripTrackTy, "StripTrackTy[nTracksStrips]/D");
  tree->Branch("ArmStrips", &ArmStrips, "ArmStrips[nTracksStrips]/I");
  tree->Branch("nArmsStrips", &nArmsStrips, "nArmsStrips/I");

  tree->Branch("nArmsPixelDigis", &nArmsPixelDigis, "nArmsPixelDigis/I");
  tree->Branch("nLayersArm1PixelDigis", &nLayersArm1PixelDigis, "nLayersArm1PixelDigis/I");
  tree->Branch("nLayersArm2PixelDigis", &nLayersArm2PixelDigis, "nLayersArm2PixelDigis/I");
  tree->Branch("nPixelRecHits", &nPixelRecHits, "nPixelRecHits/I");
  tree->Branch("PixRecHitX", &PixRecHitX, "PixRecHitX[nPixelRecHits]/D");
  tree->Branch("PixRecHitY", &PixRecHitY, "PixRecHitY[nPixelRecHits]/D");
  tree->Branch("PixRecHitZ", &PixRecHitZ, "PixRecHitZ[nPixelRecHits]/D");
  tree->Branch("PixRecHitArm", &PixRecHitArm, "PixRecHitArm[nPixelRecHits]/I");
  tree->Branch("PixRecHitPlane", &PixRecHitPlane, "PixRecHitPlane[nPixelRecHits]/I");
  tree->Branch("nArmsPixRecHits", &nArmsPixRecHits, "nArmsPixRecHits/I");
  tree->Branch("nLayersArm1PixRecHits", &nLayersArm1PixRecHits, "nLayersArm1PixRecHits/I");
  tree->Branch("nLayersArm2PixRecHits", &nLayersArm2PixRecHits, "nLayersArm2PixRecHits/I");

  tree->Branch("PrimVertexZ", &PrimVertexZ, "PrimVertexZ[nVertices]/D");
  tree->Branch("PrimVertexIsBS", &PrimVertexIsBS, "PrimVertexIsBS[nVertices]/I");

  tree->Branch("nJets", &nJets, "nJets/I");
  tree->Branch("JetCandEt", &JetCandEt, "JetCandEt[nJets]/D");
  tree->Branch("JetCandEta", &JetCandEta, "JetCandEta[nJets]/D");
  tree->Branch("JetCandPhi", &JetCandPhi, "JetCandPhi[nJets]/D");
  tree->Branch("JetCandE", &JetCandE, "JetCandE[nJets]/D");
  tree->Branch("DijetMass", &DijetMass, "DijetMass/D");
  tree->Branch("DijetY", &DijetY, "DijetY/D");

  //  tree->Branch("nPFCand", &nPFCand, "nPFCand/I");
  //  tree->Branch("PFCandID", &PFCandID, "PFCandID[nPFCand]/I");
  //  tree->Branch("PFCandE", &PFCandE, "PFCandE[nPFCand]/D");
  //  tree->Branch("PFCandEta", &PFCandEta, "PFCandEta[nPFCand]/D");
  //  tree->Branch("PFCandPhi", &PFCandPhi, "PFCandPhi[nPFCand]/D");
  tree->Branch("nHFplus", &nHFplus, "nHFplus/I");
  tree->Branch("nHFminus", &nHFminus, "nHFminus/I");
  tree->Branch("HFplusE", &HFplusE, "HFplusE/D");
  tree->Branch("HFminusE", &HFminusE, "HFminusE/D");

  tree->Branch("PFCentralMass", &PFCentralMass, "PFCentralMass/D");
  tree->Branch("PFCentralY", &PFCentralY, "PFCentralY/D");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Diamonds::endJob() 
{
  //  std::cout << "==> Number of candidates in the dataset : " << nCandidates << std::endl;
}

// ------------ method called when starting to processes a run  ------------
void 
Diamonds::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
Diamonds::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
Diamonds::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
Diamonds::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Diamonds::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

/// Class PrimaryVertex

//define this as a plug-in
DEFINE_FWK_MODULE(Diamonds);
