import FWCore.ParameterSet.Config as cms

Skimmer = cms.EDAnalyzer("Skimmer",
    tagStatus = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDigi = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDiamondRecHits = cms.InputTag("ctppsDiamondRecHits"),
    tagDiamondLocalTracks = cms.InputTag("ctppsDiamondLocalTracks"),
    tagLocalTrack = cms.InputTag("totemRPLocalTrackFitter"),
    
    excludeMultipleHits = cms.bool(True),
    minimumStripAngleForTomography = cms.double(0.1),
    centralOOT = cms.int32(2),
  
    verbosity = cms.untracked.uint32(10)
)
