
import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('CTPPS2')


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


process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
                                 #                                 '/store/data/Run2017C/ZeroBias7/AOD/09Aug2019_UL2017_LowPU-v1/230000/9C1D5DEC-FF09-5C45-AAAF-10D5FC5A9217.root'
                                 #'/store/data/Run2018C/SingleMuon/AOD/12Nov2019_UL2018-v2/2810000/CFB1C304-132A-984D-BB52-9C6AC337304E.root'
                                 '/store/data/Run2017H/ZeroBias/MINIAOD/09Aug2019_UL2017_LowPU-v1/240000/B5B386B1-FD6B-2E4D-9C64-AD70F17EA663.root'
        )
                             )

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
# 2017+2018 - all eff+timing studies
process.GlobalTag = GlobalTag(process.GlobalTag, "106X_dataRun2_v24") 

# local RP reconstruction chain with standard settings                                                                                              
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

process.mydiamonds = cms.EDAnalyzer(
    'MiniDiamonds',
    verticesTag = cms.InputTag('offlineSlimmedPrimaryVertices'),
    tracksTag = cms.InputTag('packedPFCandidates'),
    tagTrackLites = cms.InputTag( "ctppsLocalTrackLiteProducer"),
    ppsRecoProtonSingleRPTag = cms.InputTag("ctppsProtons", "singleRP"),
    ppsRecoProtonMultiRPTag = cms.InputTag("ctppsProtons", "multiRP"),
    outfilename = cms.untracked.string( "output_ZeroBias.root" ),
    isMC = cms.bool(False)
)

# Trigger                                                                                                                  
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltFilter.HLTPaths = ['HLT_ZeroBias_*']

process.mydiamonds.isLowPU = cms.bool(True)

process.ALL = cms.Path(
#   for data:
    process.hltFilter * 
    process.mydiamonds 
                       )

process.schedule = cms.Schedule(process.ALL)

