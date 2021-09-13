// -*- C++ -*-
//
// Package:    MiniDiamonds
// Class:      MiniDiamonds
// 
/**\class MiniDiamonds MiniDiamonds.cc DiffractiveForwardAnalysis/GammaGammaLeptonLepton/src/MiniDiamonds.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme,40 4-B20,+41227671567,
//         Created:  Thu Sep 13 15:17:14 CET 2012
// $Id: MiniDiamonds.cc,v 1.3 2013/04/28 08:40:45 lforthom Exp $
//
//

#include "MiniDiamonds.h"
//
// constructors and destructor
//
MiniDiamonds::MiniDiamonds(const edm::ParameterSet& iConfig) : 
  pps_tracklite_token_ ( consumes<std::vector<CTPPSLocalTrackLite>>(iConfig.getParameter<edm::InputTag>("tagTrackLites") ) ),
  verticesToken_    ( consumes< edm::View<reco::Vertex> >( iConfig.getParameter<edm::InputTag>( "verticesTag" ) ) ),
  tracksToken_    ( consumes< pat::PackedCandidateCollection >( iConfig.getParameter<edm::InputTag>( "tracksTag" ) ) ),
  tokenGen_         ( consumes<reco::GenParticleCollection>(edm::InputTag("genParticles")) ),
  recoProtonsSingleRPToken_   ( consumes<std::vector<reco::ForwardProton> >      ( iConfig.getParameter<edm::InputTag>( "ppsRecoProtonSingleRPTag" ) ) ),
  recoProtonsMultiRPToken_   ( consumes<std::vector<reco::ForwardProton> >      ( iConfig.getParameter<edm::InputTag>( "ppsRecoProtonMultiRPTag" ) ) ),
  CrossingAngle(-999.0)
{
  

  //now do what ever initialization is needed
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outfilename", "output.root");
  
  isMC = iConfig.getParameter<bool>("isMC");
	
  minifile = new TFile(outputFile_.c_str(), "recreate");
  minifile->cd();
  // tree definition
  minitree = new TTree("ntp1", "ntp1");

}


MiniDiamonds::~MiniDiamonds()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

  minifile->Write();
  minifile->Close();

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MiniDiamonds::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //std::cout << "Beginning First init" << std::endl;

  // get event data                                                                                                                                           
                                     
  edm::Handle< edm::View<reco::Vertex> > vertices;
  iEvent.getByToken( verticesToken_, vertices );

  edm::Handle<pat::PackedCandidateCollection> tks;
  iEvent.getByToken(tracksToken_, tks);

  // check validity                                                                                                                                                                                
  // First initialization of the variables
  
  // Run and BX information
  BX = iEvent.bunchCrossing();
  Run = iEvent.id().run();
  LumiSection = iEvent.luminosityBlock();
  EventNum = iEvent.id().event();

  // Crossing angle                                                                                                                                           
  edm::ESHandle<LHCInfo> hLHCInfo;
  iSetup.get<LHCInfoRcd>().get("", hLHCInfo);
  if (hLHCInfo->crossingAngle() != CrossingAngle)
    {
      CrossingAngle = hLHCInfo->crossingAngle();
    }

  nGenProtons = 0;
  nProtons = 0;

  nLiteTracks = 0;

  nVertices = 0;
  nTracksNoVertex = 0;
  nTracksOneVertex = 0;

  for(int i = 0; i < 100; i++)
    {
      PrimVertexZ[i] = -999.;
      PrimVertexX[i] = -999.;
      PrimVertexY[i] = -999.;
      PrimVertexIsBS[i] = -999;

      ProtonXi[i] = -999;
      ProtonThY[i] = -999;
      ProtonThX[i] = -999;
      Protont[i] = -999;
      ProtonIsMultiRP[i] = -999;
      ProtonRPID[i] = -999;
      ProtonArm[i] = -999;
      ProtonTime[i] = -999.;
    }

  for(int i = 0; i < 1000; i++)
    {
      TrackZNoVertex[i] = 0;
      TrackPtOneVertex[i] = 0;
      TrackEtaOneVertex[i] = 0;
      TrackLiteX[i] = 0;
      TrackLiteY[i] = 0;
      TrackLiteTime[i] = 0;
      TrackLiteRPID[i] = 0;
    }

  for(int i = 0; i < 10; i++)
    {
      GenProtXi[i] = 0;
      GenProtPz[i] = 0;
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
      
      //      std::cout << "\tLite track with x, y, ID = " << trklite.getX() << ", " << trklite.getY() << ", " << raw_id << std::endl;
      TrackLiteX[nLiteTracks] = trklite.getX();
      TrackLiteY[nLiteTracks] = trklite.getY();
      TrackLiteTime[nLiteTracks] = trklite.getTime();
      TrackLiteRPID[nLiteTracks] = raw_id;
      //      std::cout << "\t\tFilled arrays with x, y, ID = " << TrackLiteX[nLiteTracks] << ", " << TrackLiteY[nLiteTracks] << ", " 
      //		<< TrackLiteRPID[nLiteTracks] << std::endl;
      nLiteTracks++;
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
  float xi = -999.;
  float protontime = -999.;
  
  // Single-RP algorithm                                                                                                                             
  for (const auto & proton : *recoSingleRPProtons)
    {
      //      std::cout << "Single-RP proton with xi = " << proton.xi() << std::endl;
      if (proton.validFit())
	{
	  th_y = proton.thetaY();
	  th_x = proton.thetaX();
	  xi = proton.xi();
	  //	  t = proton.t();
	  protontime = proton.time();

	  CTPPSDetId rpId((*proton.contributingLocalTracks().begin())->getRPId());
	  decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
	  
	  ismultirp = 0;
	  
	  ProtonXi[nProtons] = xi;
	  ProtonThX[nProtons] = th_x;
	  ProtonThY[nProtons] = th_y;
	  Protont[nProtons] = t;
	  ProtonIsMultiRP[nProtons] = ismultirp;
	  ProtonRPID[nProtons] = decRPId;
	  ProtonArm[nProtons] = armId;
	  ProtonTime[nProtons] = protontime;
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
	  //	  t = proton.t();
          protontime = proton.time();
	  
          CTPPSDetId rpId((*proton.contributingLocalTracks().begin())->getRPId());
          armId = rpId.arm();
	  
	  ismultirp = 1;
	  
	  ProtonXi[nProtons] = xi;
          ProtonThX[nProtons] = th_x;
          ProtonThY[nProtons] = th_y;
          Protont[nProtons] = t;
          ProtonIsMultiRP[nProtons] = ismultirp;
	  ProtonRPID[nProtons] = decRPId;
	  ProtonArm[nProtons] = armId;
          ProtonTime[nProtons] = protontime;
	  nProtons++;
	}
    }
  
  /* Primary vertices */
  for ( const auto& vtx : *vertices ) {                                                                           
    PrimVertexZ[nVertices] = vtx.z();
    PrimVertexX[nVertices] = vtx.x();
    PrimVertexY[nVertices] = vtx.y();

    if(vtx.isFake() == 1)
      PrimVertexIsBS[nVertices] = 1;
    else
      PrimVertexIsBS[nVertices] = 0;
    nVertices++;
  }

  /* Tracks if no real vertex found */
  if(nVertices <= 1)
    {
      if(PrimVertexIsBS[0] == 1)
	{
	  for( const auto& tk : *tks )
	    {
	      if(fabs(tk.pdgId()) == 211)
		{
		  TrackZNoVertex[nTracksNoVertex] = tk.vertex().z();
		  nTracksNoVertex++;
		}
	    }
	}
      if(PrimVertexIsBS[0] == 0)
	{
	  for( const auto& tk : *tks )
            {
              if(fabs(tk.pdgId()) == 211)
		{
		  TrackPtOneVertex[nTracksOneVertex] = tk.pt();
		  TrackEtaOneVertex[nTracksOneVertex] = tk.eta();
		  nTracksOneVertex++;
		}
            }
	}
    }


    if(isMC == true)
      {
	edm::Handle<reco::GenParticleCollection> genP;
	iEvent.getByLabel("genParticles",genP);
	
	for (reco::GenParticleCollection::const_iterator mcIter=genP->begin(); mcIter != genP->end(); mcIter++ ) 
	  {
	    if(mcIter->status() == 1)
	      {
		if((mcIter->pdgId() == 2212) && (fabs(mcIter->pz()) > 3000))
		  {
		    GenProtXi[nGenProtons]=((6500.0-mcIter->energy())/6500.0);
		    GenProtPz[nGenProtons]=mcIter->pz();
		    nGenProtons++;
		  }
	      }
	  }
      }
    
  minitree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
MiniDiamonds::beginJob()
{
  // Booking the ntuple

  minitree->Branch("Run", &Run, "Run/I");
  minitree->Branch("LumiSection", &LumiSection, "LumiSection/I");
  minitree->Branch("BX", &BX, "BX/I");
  minitree->Branch("EventNum", &EventNum, "EventNum/I");
  minitree->Branch("CrossingAngle", &CrossingAngle, "CrossingAngle/F");

  minitree->Branch("nVertices", &nVertices, "nVertices/I");

  minitree->Branch("nLiteTracks", &nLiteTracks, "nLiteTracks/I");
  minitree->Branch("TrackLiteX", &TrackLiteX, "TrackLiteX[nLiteTracks]/F");
  minitree->Branch("TrackLiteY", &TrackLiteY, "TrackLiteY[nLiteTracks]/F");
  minitree->Branch("TrackLiteTime", &TrackLiteTime, "TrackLiteTime[nLiteTracks]/F");
  minitree->Branch("TrackLiteRPID", &TrackLiteRPID, "TrackLiteRPID[nLiteTracks]/I");

  minitree->Branch("PrimVertexZ", &PrimVertexZ, "PrimVertexZ[nVertices]/D");
  minitree->Branch("PrimVertexX", &PrimVertexX, "PrimVertexX[nVertices]/D");
  minitree->Branch("PrimVertexY", &PrimVertexY, "PrimVertexY[nVertices]/D");
  minitree->Branch("PrimVertexIsBS", &PrimVertexIsBS, "PrimVertexIsBS[nVertices]/I");

  minitree->Branch("nTracksNoVertex", &nTracksNoVertex, "nTracksNoVertex/I");
  minitree->Branch("TrackZNoVertex", &TrackZNoVertex, "TrackZNoVertex[nTracksNoVertex]/D");
  minitree->Branch("nTracksOneVertex", &nTracksOneVertex, "nTracksOneVertex/I");
  minitree->Branch("TrackPtOneVertex", &TrackPtOneVertex, "TrackPtOneVertex[nTracksOneVertex]/D");
  minitree->Branch("TrackEtaOneVertex", &TrackEtaOneVertex, "TrackEtaOneVertex[nTracksOneVertex]/D");

  minitree->Branch("nGenProtons", &nGenProtons, "nGenProtons/I");
  minitree->Branch("GenProtXi", &GenProtXi, "GenProtXi[nGenProtons]/F");
  minitree->Branch("GenProtPz", &GenProtPz, "GenProtPz[nGenProtons]/F");

  minitree->Branch("nProtons", &nProtons, "nProtons/I");
  minitree->Branch("ProtonXi", &ProtonXi, "ProtonXi[nProtons]/F");
  minitree->Branch("ProtonThX", &ProtonThX, "ProtonThX[nProtons]/F");
  minitree->Branch("ProtonThY", &ProtonThY, "ProtonThY[nProtons]/F");
  minitree->Branch("Protont", &Protont, "Protont[nProtons]/F");
  minitree->Branch("ProtonIsMultiRP", &ProtonIsMultiRP, "ProtonIsMultiRP[nProtons]/I");
  minitree->Branch("ProtonRPID", &ProtonRPID, "ProtonRPID[nProtons]/I");
  minitree->Branch("ProtonArm", &ProtonArm, "ProtonArm[nProtons]/I");
  minitree->Branch("ProtonTime", &ProtonTime, "ProtonTime[nProtons]/F");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniDiamonds::endJob() 
{
  //  std::cout << "==> Number of candidates in the dataset : " << nCandidates << std::endl;
}

// ------------ method called when starting to processes a run  ------------
void 
MiniDiamonds::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MiniDiamonds::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MiniDiamonds::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MiniDiamonds::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MiniDiamonds::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

/// Class PrimaryVertex

//define this as a plug-in
DEFINE_FWK_MODULE(MiniDiamonds);
