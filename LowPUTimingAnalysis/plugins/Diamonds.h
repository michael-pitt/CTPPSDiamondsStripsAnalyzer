#ifndef CTPPSDiamondsStripsAnalyzer_Diamonds_h
#define CTPPSDiamondsStripsAnalyzer_Diamonds_h

// system include files
#include <memory>
#include <map>

#include <vector>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"

#include "FWCore/Framework/interface/EventSetup.h"

// HLT information
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

// Central
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"


// RP 
#include "DataFormats/Common/interface/DetSetVector.h" 
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h" 
#include "DataFormats/CTPPSDigi/interface/TotemRPDigi.h" 
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h" 
#include "DataFormats/CTPPSReco/interface/TotemRPCluster.h" 
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h" 
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h" 
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h" 
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

// Timing
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h"
#include "DataFormats/CTPPSDigi/interface/TotemFEDInfo.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"

#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

// Pixels 
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigiCollection.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"  
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"

// Protons
#include "DataFormats/ProtonReco/interface/ForwardProton.h"
#include "DataFormats/ProtonReco/interface/ForwardProtonFwd.h"

#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>

#define MAX_HLT    10 // Maximum number of HLT to check
#define MAX_LL     50 // Maximum number of leptons per event
#define MAX_MUONS  25 // Maximum number of muons per event
#define MAX_ELE    25 // Maximum number of electrons per event
#define MAX_PHO    50 // Maximum number of photons per event
#define MAX_PAIRS  1 // Maximum number of leptons pairs per event
#define MAX_VTX    100 // Maximum number of primary vertices per event
#define MAX_ET     5000// Maximum number of extra tracks per event
#define MAX_GENMU  25 // Maximum number of generator level muons per event
#define MAX_GENELE 25 // Maximum number of generator level electrons per event
#define MAX_GENPHO 10 // Maximum number of generator level photons per event
#define MAX_JETS   30 // Maximum nulber of jets per event
#define MAX_GENPRO 10 // Maximum number of generated protons
#define MAX_LOCALPCAND 10 // Maximum number of reconstructed local tracks in RPs

#define MASS_MU 0.1057
#define MASS_E  0.000511
#define MASS_P  0.938272029
#define pi 3.14159265359

//
// class declaration
//

class Diamonds : public edm::EDAnalyzer {
   public:
      explicit Diamonds(const edm::ParameterSet&);
      ~Diamonds();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      
      // Input tags                                                                                                                                                                               
      std::string outputFile_;

      edm::EDGetTokenT< edm::DetSetVector<TotemVFATStatus> > tokenStatus_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondDigi> > tokenDigi_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > tokenDiamondHit_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > tokenDiamondTrack_;
      edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > tokenLocalTrack_;
      edm::EDGetTokenT< std::vector<TotemFEDInfo> > tokenFEDInfo_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelDigi> > tokenPixelDigi_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelCluster> > tokenPixelCluster_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelRecHit> > tokenPixelRecHit_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelLocalTrack> > tokenPixelLocalTrack_;
      edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite> > pps_tracklite_token_;

      edm::EDGetTokenT< edm::View<reco::Vertex> > verticesToken_;
      edm::EDGetTokenT<reco::PFJetCollection>  jetsToken_;
      edm::EDGetTokenT<reco::TrackCollection>  tracksToken_;
      edm::EDGetTokenT<reco::PFCandidateCollection> pflowToken_;

      edm::EDGetTokenT<reco::GenParticleCollection> tokenGen_;

      edm::EDGetTokenT<std::vector<reco::ForwardProton> > recoProtonsSingleRPToken_;
      edm::EDGetTokenT<std::vector<reco::ForwardProton> > recoProtonsMultiRPToken_;

      ////// Tree contents //////


      
      // Run/event quantities
      Int_t BX, Run, LumiSection, EventNum;
      Float_t CrossingAngle;

      Double_t LeadingEdge[100], TrailingEdge[100], ToT[100], XTiming[100], YTiming[100]; 
      Double_t TimingTrackT[100], TimingTrackTErr[100], TimingTrackX[100], TimingTrackY[100], TimingTrackZ[100], TimingTrackChi2[100];
      Double_t TimingRecHitT[100], TimingRecHitX[100], TimingRecHitY[100], TimingRecHitToT[100];
      Int_t TimingTrackOOTIndex[100],TimingRecHitOOTIndex[100], TimingTrackMultiHit[100], TimingRecHitMultiHit[100], TimingTrackArm[100];
      unsigned int ChannelTiming[100], MultiHit[100], ArmTiming[100], OOTIndex[100], PlaneTiming[100];
      unsigned int TimingRecHitChannel[100], TimingRecHitArm[100], TimingRecHitPlane[100];
      
      //Double_t AvgInstDelLumi, BunchInstLumi[3]; 
      Int_t nVertices, nArmsTiming, nJets, nHitsTiming, nLayersArm1Timing, nLayersArm2Timing, nTracksTiming, nRecHitsTiming;
      Int_t nlayersarm1, nlayersarm2;

      Int_t nTracksStrips, nArmsStrips;
      Int_t ArmStrips[100];
      Double_t StripTrackX[100], StripTrackY[100], StripTrackTx[100], StripTrackTy[100];

      Int_t nArmsPixelDigis, nLayersArm1PixelDigis, nLayersArm2PixelDigis; 
      Int_t nPixelRecHits, nArmsPixRecHits, nLayersArm1PixRecHits, nLayersArm2PixRecHits;
      Double_t PixRecHitX[1000], PixRecHitY[1000], PixRecHitZ[1000];
      Int_t PixRecHitArm[1000], PixRecHitPlane[1000];

      Int_t nPixelTracks, nArmsPixelTracks, nPixelTracksArm1, nPixelTracksArm2;
      Double_t PixTrackX[1000], PixTrackY[1000], PixTrackTx[1000], PixTrackTy[1000], PixTrackChi2[1000], PixTrackZ[1000];
      Int_t PixTrackArm[1000];

      Int_t nLiteTracks;
      Float_t TrackLiteX[1000], TrackLiteY[1000], TrackLiteTime[1000];
      Int_t TrackLiteRPID[1000];

      Double_t PrimVertexZ[100], PrimVertexY[100], PrimVertexX[100];
      Int_t PrimVertexIsBS[100];

      Int_t nTracksNoVertex;
      Int_t nTracksOneVertex;
      Double_t TrackZNoVertex[1000];
      Double_t TrackPtOneVertex[1000];
      Double_t TrackEtaOneVertex[1000];

      Double_t JetCandEt[1000];
      Double_t JetCandEta[1000];
      Double_t JetCandPhi[1000];
      Double_t JetCandE[1000];
      Int_t nPFCand;
      Int_t PFCandID[1000];
      Double_t PFCandE[1000], PFCandEta[1000], PFCandPhi[1000];
      Double_t PFCentralMass, PFCentralY, DijetMass, DijetY, PFCentralPx, PFCentralPy, PFCentralPz, PFCentralE;
      Double_t PFCentralMass_NoThresh, PFCentralY_NoThresh, PFCentralPx_NoThresh, PFCentralPy_NoThresh, PFCentralPz_NoThresh, PFCentralE_NoThresh;
      Int_t   nHFplus, nHFminus;
      Double_t HFplusE, HFminusE;

      Int_t nGenProtons;
      Float_t GenProtXi[100], GenProtPz[100];

      Int_t nProtons;
      Float_t ProtonXi[100];
      Float_t ProtonThY[100];
      Float_t ProtonThX[100];
      Float_t Protont[100];
      Int_t ProtonIsMultiRP[100];
      Int_t ProtonRPID[100];
      Int_t ProtonArm[100];
      Float_t ProtonTime[100];

      bool isMC;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
TFile* file;
TTree* tree;

#endif
