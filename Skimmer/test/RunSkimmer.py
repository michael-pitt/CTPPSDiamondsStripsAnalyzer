import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("SkimmerCTPPS")
useAOD = True

#########################
#    General options    #
#########################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options   = cms.untracked.PSet(
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

#########################
#      Input files      #
#########################
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/Run2017A/ZeroBias/AOD/PromptReco-v1/000/295/977/00000/1476F58C-F249-E711-B12B-02163E01473C.root',
        '/store/data/Run2017A/ZeroBias/AOD/PromptReco-v1/000/295/977/00000/921E437D-FC49-E711-9CB5-02163E01A549.root',
        '/store/data/Run2017A/ZeroBias/AOD/PromptReco-v1/000/295/977/00000/FC1DC0D5-EF49-E711-8CFB-02163E01A2E7.root'
    ),
)

#########################
#        Triggers       #
#########################
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltFilter.HLTPaths = ['HLT_ZeroBias_IsolatedBunches_v*','HLT_ZeroBias_part*_v*']
process.hltFilter.throw = cms.bool(False)

#########################
#      Preskimming      #
#########################
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '92X_dataRun2_HLT_v2','')

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

######################
#      Analyzer      #
######################
process.Skimmer = cms.EDAnalyzer("CTPPSSkimmer",
    tagStatus = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDigi = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDiamondRecHits = cms.InputTag("ctppsDiamondRecHits"),
    tagDiamondLocalTracks = cms.InputTag("ctppsDiamondLocalTracks"),
    tagLocalTrack = cms.InputTag("totemRPLocalTrackFitter"),
    verbosity = cms.untracked.uint32(10)
)

######################
#      TFile         #
######################
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('output.root'),
    closeFileFast = cms.untracked.bool(True)
)

process.p = cms.Path(
    process.hltFilter*
    process.Skimmer 
    )
