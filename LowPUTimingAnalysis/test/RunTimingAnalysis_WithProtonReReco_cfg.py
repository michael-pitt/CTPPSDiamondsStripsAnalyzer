
import FWCore.ParameterSet.Config as cms
import copy

from Configuration.StandardSequences.Eras import eras
process = cms.Process('CTPPS2', eras.ctpps_2016)


# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.verbosity = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
#    statistics = cms.untracked.vstring(),
#    destinations = cms.untracked.vstring('cerr'),
#    cerr = cms.untracked.PSet(
#        threshold = cms.untracked.string('WARNING')
#    )
)


process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

# Track memory leaks
process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )


#from CTPPSDiamondAnalyzer.LowPUTimingAnalysis.InputFiles_cff import readFiles


process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/58C8FB5E-9A87-E811-AD9B-FA163E9943A6.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/8AC0601A-9687-E811-A8E4-FA163E910130.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/CE42555B-9A87-E811-9E57-FA163E36A073.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/1243B79C-9987-E811-A5E0-FA163E1B69FC.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/34470DD6-9887-E811-82EF-FA163E82EDA3.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/6EFC02C6-A087-E811-9E2F-FA163EE09AAE.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/18263696-A187-E811-A6B8-FA163EDE9AD6.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/0C5C6A76-9A87-E811-B47F-FA163E3B297F.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/5228046C-9E87-E811-A258-FA163EC1DE16.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/9E9B7C66-9E87-E811-BE40-FA163E7B3281.root',
'/store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/488/00000/6863662E-A687-E811-B117-FA163E7C864F.root'
        )
                             )

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
# 2017+2018 - all eff+timing studies
process.GlobalTag = GlobalTag(process.GlobalTag, "106X_dataRun2_v24") 

# local RP reconstruction chain with standard settings                                                                                              
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

process.mydiamonds = cms.EDAnalyzer(
    'Diamonds',
#    tagStatus = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),                                                                  
#    tagDigi = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),                                                                    
#    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),                                                                 
    tagStatus = cms.InputTag("ctppsDiamondRawToDigi","TimingDiamond","RECO"),
    tagDigi = cms.InputTag("ctppsDiamondRawToDigi","TimingDiamond","RECO"),
    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi","TimingDiamond","RECO"),
    tagDiamondRecHits = cms.InputTag("ctppsDiamondRecHits","","CTPPS2"),
    tagDiamondLocalTracks = cms.InputTag("ctppsDiamondLocalTracks","","CTPPS2"),
    tagLocalTrack = cms.InputTag("totemRPLocalTrackFitter"),
    verticesTag = cms.InputTag('offlinePrimaryVertices'),
    tracksTag = cms.InputTag('generalTracks'),
    jetsTag = cms.InputTag('ak4PFJets'),
    tagParticleFlow = cms.InputTag('particleFlow'),
    tagRPixDigi = cms.InputTag( "ctppsPixelDigis" ),
    tagRPixCluster = cms.InputTag( "ctppsPixelClusters" ),
    tagRPixRecHit = cms.InputTag( "ctppsPixelRecHits" ),
    tagRPixLocalTrack = cms.InputTag( "ctppsPixelLocalTracks", "", "CTPPS2" ),
    tagTrackLites = cms.InputTag( "ctppsLocalTrackLiteProducer", "", "CTPPS2"),
#    tagTrackLites = cms.InputTag( "ctppsLocalTrackLiteProducer"),
    ppsRecoProtonSingleRPTag = cms.InputTag("ctppsProtons", "singleRP"),
    ppsRecoProtonMultiRPTag = cms.InputTag("ctppsProtons", "multiRP"),
    outfilename = cms.untracked.string( "output_ZeroBias_Run319488_ULtest.root" ),
#    outfilename = cms.untracked.string( "output_CD_MBR.root" ),
#    outfilename = cms.untracked.string( "output_ZeroBias.root" ),
    isMC = cms.bool(False)
)

process.ctppsPixelRecHits.RPixClusterTag = cms.InputTag("ctppsPixelClusters","","CTPPS2")
process.ctppsPixelLocalTracks.label = cms.string("ctppsPixelRecHits::CTPPS2")
process.ctppsLocalTrackLiteProducer.tagPixelTrack = cms.InputTag("ctppsPixelLocalTracks","","CTPPS2")

process.ctppsProtons.tagLocalTrackLite = cms.InputTag("ctppsLocalTrackLiteProducer","","CTPPS2")
process.ctppsLocalTrackLiteProducer.includePixels = cms.bool(True)
process.ctppsLocalTrackLiteProducer.includeStrips = cms.bool(True)
process.ctppsLocalTrackLiteProducer.includeDiamonds = cms.bool(True)
process.ctppsProtons.doSingleRPReconstruction = cms.bool(True)
process.ctppsProtons.doMultiRPReconstruction = cms.bool(True)

# Trigger                                                                                                                  
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltFilter.HLTPaths = ['HLT_ZeroBias_v*']

process.mydiamonds.isLowPU = cms.bool(True)



process.ALL = cms.Path(
#   for data:
    process.hltFilter * 
    process.totemRPLocalReconstruction *
    process.ctppsDiamondRecHits *
    process.ctppsDiamondLocalTracks*
    process.ctppsPixelLocalReconstruction *
    process.ctppsLocalTrackLiteProducer *
    process.ctppsProtons *
    process.mydiamonds 
                       )

process.schedule = cms.Schedule(process.ALL)

