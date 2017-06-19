import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import copy

process = cms.Process("SkimmerCTPPS")

#########################
#    General options    #
#########################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options   = cms.untracked.PSet(
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#########################
#      Input files      #
#########################
from CTPPSDiamondAnalyzer.Skimmer.RunsLowPU_cff import fileNames
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(fileNames))

#########################
#        Triggers       #
#########################
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltFilter.HLTPaths = ['HLT_ZeroBias_IsolatedBunches_v*','HLT_ZeroBias_v*','HLT_ZeroBias_part0_v*','HLT_ZeroBias_part1_v*','HLT_ZeroBias_part2_v*','HLT_ZeroBias_part3_v*','HLT_ZeroBias_part4_v*','HLT_ZeroBias_part5_v*','HLT_ZeroBias_part6_v*','HLT_ZeroBias_part7_v*','HLT_ZeroBias_part8_v*','HLT_ZeroBias_part9_v*']
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
    verbosity = cms.untracked.uint32(10),
    HLTMenuLabel = cms.string('HLT'),
    TriggersList = process.hltFilter.HLTPaths,
    TriggerResults = cms.InputTag('TriggerResults', '', 'HLT'),
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
