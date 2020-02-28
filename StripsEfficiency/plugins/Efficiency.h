#ifndef CTPPSDiamondsStripsAnalyzer_StripsEfficiency_h
#define CTPPSDiamondsStripsAnalyzer_StripsEfficiency_h

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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"

// HLT information
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

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

// Vertices
#include "DataFormats/VertexReco/interface/Vertex.h"


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

class StripsEfficiency : public edm::EDAnalyzer {
   public:
      explicit StripsEfficiency(const edm::ParameterSet&);
      ~StripsEfficiency();

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
      edm::EDGetTokenT< std::vector<TotemFEDInfo> > tokenFEDInfo_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelDigi> > tokenPixelDigi_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelCluster> > tokenPixelCluster_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelRecHit> > tokenPixelRecHit_;
      edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite> > pps_tracklite_token_;

      edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack >> tokenLocalTrack_;
      edm::EDGetTokenT< edm::DetSetVector<TotemRPUVPattern >> tokenUVPattern_;
      edm::EDGetTokenT< edm::DetSetVector<TotemRPRecHit >> tokenRecHit_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelLocalTrack> > tokenPixelLocalTrack_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > tokenDiamondTrack_;
      edm::EDGetTokenT<std::vector<reco::ForwardProton> > recoProtonsSingleRPToken_;
      edm::EDGetTokenT<std::vector<reco::ForwardProton> > recoProtonsMultiRPToken_;

      edm::EDGetTokenT< edm::View<reco::Vertex> > verticesToken_;

      ////// Tree contents //////
      bool addTimingTracks_;
      bool onlySinglePixelTrackEvents_;
      bool is2016data_;

      
      // Run/event quantities
      Int_t BX, Run, LumiSection, EventNum;
      Float_t CrossingAngle;
      
      Int_t nLiteTracks;
      Int_t nStripTracks;
      Float_t TrackLiteX[1000], TrackLiteY[1000];
      Float_t StripTrackX[4], StripTrackY[4], StripTrackThX[4], StripTrackThY[4];
      Int_t TrackLiteRPID[1000];
      Int_t StripTrackValid[4], StripTrackRPID[4], StripTrackTooFullU[4], StripTrackTooFullV[4], StripTrackUPatterns[4], StripTrackVPatterns[4];
      Int_t nPixelTracks;
      Double_t PixTrackX[1000], PixTrackY[1000], PixTrackThX[1000], PixTrackThY[1000], PixTrackChi2[1000], PixTrackZ[1000];
      Int_t PixTrackNdof[1000], PixTrackShift[1000];
      Int_t PixTrackArm[1000];
      Int_t nTimingTracks;
      Float_t TimingTrackX[100], TimingTrackY[100], TimingTrackZ[100];
      Int_t TimingTrackArm[100];

      Int_t nPixelTracks45, nPixelTracks56;

      Int_t nProtons;
      Float_t ProtonXi[100];
      Float_t ProtonThY[100];
      Float_t ProtonThX[100];
      Float_t Protont[100];
      Int_t ProtonIsMultiRP[100];
      Int_t ProtonRPID[100];
      Int_t ProtonArm[100];

      Int_t nVertices;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
TFile* efffile;
TTree* efftree;

#endif
