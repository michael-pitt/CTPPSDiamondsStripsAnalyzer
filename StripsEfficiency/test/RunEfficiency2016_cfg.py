
import FWCore.ParameterSet.Config as cms
import copy

from Configuration.StandardSequences.Eras import eras
process = cms.Process('ctppsRECO', eras.ctpps_2016)


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
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000 

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
'/store/data/Run2016C/SingleElectron/AOD/07Aug17-v1/50000/C0A3CB65-A586-E711-AEDD-842B2B6F5D95.root'



        ))

process.myefficiency = cms.EDAnalyzer(
    'StripsEfficiency',
    verticesTag = cms.InputTag('offlinePrimaryVertices'),
    tagTrackLites = cms.InputTag( "ctppsLocalTrackLiteProducer" ,"", "ctppsRECO"),
    ppsRecoProtonSingleRPTag = cms.InputTag("ctppsProtons", "singleRP"),
    ppsRecoProtonMultiRPTag = cms.InputTag("ctppsProtons", "multiRP"),
    includeTimingTracks = cms.bool(False),
    selectSinglePixelTrackEvents = cms.bool(False),
    is2016data = cms.bool(True),
    outfilename = cms.untracked.string( "output_EfficiencyTest.root" )
)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, "106X_dataRun2_v26")

#JH - for tests of updated 2017 alignment+optics candidate w. vertical dispersion updates                                                                                                    
# this is an example how to load optical functions from a DB tag (and override those from GT)                                                                                                
#from CondCore.CondDB.CondDB_cfi import *
#process.CondDBOptics = CondDB.clone( connect = 'frontier://FrontierProd/CMS_CONDITIONS' )
#process.PoolDBESSourceOptics = cms.ESSource("PoolDBESSource",
#                                            process.CondDBOptics,
#                                            DumpStat = cms.untracked.bool(False),
#                                            toGet = cms.VPSet(
#                                                cms.PSet(
#                                                    record = cms.string('CTPPSOpticsRcd'),
#                                                    tag = cms.string("PPSOpticalFunctions_offline_v3")
#                                                ),
#                                                cms.PSet(
#                                                    record = cms.string('RPRealAlignmentRecord'),
#                                                    tag = cms.string("CTPPSRPAlignment_real_offline_v2")
#                                                )
#                                            )
#                                        )
#
#process.esPreferDBFileOptics = cms.ESPrefer("PoolDBESSource", "PoolDBESSourceOptics")


# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

#process.ctppsPixelRecHits.RPixClusterTag = cms.InputTag("ctppsPixelClusters","","ctppsRECO")
#process.ctppsPixelLocalTracks.label = cms.string("ctppsPixelRecHits::ctppsRECO")
process.ctppsLocalTrackLiteProducer.tagPixelTrack = cms.InputTag("ctppsPixelLocalTracks","","ctppsRECO")
process.ctppsProtons.tagLocalTrackLite = cms.InputTag("ctppsLocalTrackLiteProducer","","ctppsRECO")
process.ctppsLocalTrackLiteProducer.includePixels = cms.bool(False)
process.ctppsLocalTrackLiteProducer.includeStrips = cms.bool(True)
process.ctppsProtons.doSingleRPReconstruction = cms.bool(True)
process.ctppsProtons.doMultiRPReconstruction = cms.bool(True)

# Trigger                                                                                                                                                   
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
#process.hltFilter.HLTPaths = ['HLT_Ele32_WPTight_Gsf_L1DoubleEG_v*','HLT_Ele35_WPTight_Gsf_v*']
#process.hltFilter.HLTPaths = ['HLT_IsoMu27_*']                                                                                                             
process.hltFilter.HLTPaths = ['HLT_Ele27_WPTight_Gsf_v*']
  
process.hltFilter.throw = cms.bool(False)

process.ALL = cms.Path(
#    process.hltFilter *
#    process.recoCTPPS *
    process.totemRPUVPatternFinder *
    process.totemRPLocalTrackFitter *
#    process.ctppsDiamondRecHits *
#    process.ctppsDiamondLocalTracks *
#    process.ctppsPixelLocalTracks *
    process.ctppsLocalTrackLiteProducer *
    process.ctppsProtons *
    process.myefficiency
                       )

process.schedule = cms.Schedule(process.ALL)

print process.dumpPython()

