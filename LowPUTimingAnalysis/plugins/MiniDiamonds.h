#ifndef CTPPSDiamondsStripsAnalyzer_MiniDiamonds_h
#define CTPPSDiamondsStripsAnalyzer_MiniDiamonds_h

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
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"


// RP 
#include "DataFormats/Common/interface/DetSetVector.h" 
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h" 
#include "DataFormats/CTPPSDigi/interface/TotemRPDigi.h" 
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

// Timing
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h"
#include "DataFormats/CTPPSDigi/interface/TotemFEDInfo.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

// Pixels 
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
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
#define MAX_GENPRO 10 // Maximum number of generated protons
#define MAX_LOCALPCAND 10 // Maximum number of reconstructed local tracks in RPs

#define MASS_MU 0.1057
#define MASS_E  0.000511
#define MASS_P  0.938272029
#define pi 3.14159265359

//
// class declaration
//

class MiniDiamonds : public edm::EDAnalyzer {
   public:
      explicit MiniDiamonds(const edm::ParameterSet&);
      ~MiniDiamonds();

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

      edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > tokenDiamondTrack_;
      edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > tokenLocalTrack_;
      edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelLocalTrack> > tokenPixelLocalTrack_;
      edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite> > pps_tracklite_token_;

      edm::EDGetTokenT< edm::View<reco::Vertex> > verticesToken_;
      edm::EDGetTokenT<pat::PackedCandidateCollection>  tracksToken_;

      edm::EDGetTokenT<reco::GenParticleCollection> tokenGen_;

      edm::EDGetTokenT<std::vector<reco::ForwardProton> > recoProtonsSingleRPToken_;
      edm::EDGetTokenT<std::vector<reco::ForwardProton> > recoProtonsMultiRPToken_;

      ////// Tree contents //////


      
      // Run/event quantities
      Int_t BX, Run, LumiSection, EventNum;
      Float_t CrossingAngle;
      
      //Double_t AvgInstDelLumi, BunchInstLumi[3]; 
      Int_t nVertices;

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
TFile* minifile;
TTree* minitree;

#endif
