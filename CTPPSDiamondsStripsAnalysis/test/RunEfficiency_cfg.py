
import FWCore.ParameterSet.Config as cms

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
#'/store/data/Run2017B/SingleMuon/AOD/17Nov2017-v1/60000/CC68436B-7ED7-E711-B900-02163E01A211.root'

'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/B62FED11-B3D8-E711-AFAD-02163E013989.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/CCFB8C4A-B2D8-E711-B26E-02163E019BA9.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/1E5B8023-B7D8-E711-8BCD-02163E011F14.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/A42F4354-B9D8-E711-9B00-02163E013981.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/D683C4C3-B0D8-E711-9102-02163E0120EC.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/EC7CE753-B2D8-E711-82CA-02163E0142F9.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/9A6D1465-B2D8-E711-9366-02163E01A4DE.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/D64221AD-B5D8-E711-A905-02163E0142A7.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/60002/02AE8057-28D9-E711-A0B7-02163E0136DA.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/60002/40380B6E-28D9-E711-A154-02163E019D26.root',
'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/60002/B4AB5602-D9D9-E711-9C24-02163E019C3C.root'



        ))

process.myefficiency = cms.EDAnalyzer(
    'StripsEfficiency',
    verticesTag = cms.InputTag('offlinePrimaryVertices'),
    tagTrackLites = cms.InputTag( "ctppsLocalTrackLiteProducer" ,"", "ctppsRECO"),
    ppsRecoProtonSingleRPTag = cms.InputTag("ctppsProtons", "singleRP"),
    ppsRecoProtonMultiRPTag = cms.InputTag("ctppsProtons", "multiRP"),
    includeTimingTracks = cms.bool(False),
    selectSinglePixelTrackEvents = cms.bool(True),
    outfilename = cms.untracked.string( "output_EfficiencyTest.root" )
)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, "106X_dataRun2_v10")

# get optics from a DB tag
from CondCore.CondDB.CondDB_cfi import *
process.CondDBOptics = CondDB.clone( connect = 'frontier://FrontierProd/CMS_CONDITIONS' )
process.PoolDBESSourceOptics = cms.ESSource("PoolDBESSource",
    process.CondDBOptics,
    DumpStat = cms.untracked.bool(False),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSOpticsRcd'),
        tag = cms.string("PPSOpticalFunctions_offline_v1")
    )),
)

process.esPreferDBFileOptics = cms.ESPrefer("PoolDBESSource", "PoolDBESSourceOptics")

# get alignment from a SQLite file
from CondCore.CondDB.CondDB_cfi import *
process.CondDBAlignment = CondDB.clone( connect = 'sqlite_file:/afs/cern.ch/user/c/cmora/public/CTPPSDB/AlignmentSQlite/CTPPSRPRealAlignment_table_v26Apr.db' )
process.PoolDBESSourceAlignment = cms.ESSource("PoolDBESSource",
    process.CondDBAlignment,
    #timetype = cms.untracked.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('RPRealAlignmentRecord'),
        tag = cms.string('CTPPSRPAlignment_real_table_v26A19')
    ))
)

process.esPreferDBFileAlignment = cms.ESPrefer("PoolDBESSource", "PoolDBESSourceAlignment")

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

process.ctppsPixelRecHits.RPixClusterTag = cms.InputTag("ctppsPixelClusters","","ctppsRECO")
process.ctppsPixelLocalTracks.label = cms.string("ctppsPixelRecHits::ctppsRECO")
process.ctppsLocalTrackLiteProducer.tagPixelTrack = cms.InputTag("ctppsPixelLocalTracks","","ctppsRECO")
process.ctppsProtons.tagLocalTrackLite = cms.InputTag("ctppsLocalTrackLiteProducer","","ctppsRECO")
process.ctppsLocalTrackLiteProducer.includePixels = cms.bool(True)
process.ctppsLocalTrackLiteProducer.includeStrips = cms.bool(True)
process.ctppsProtons.doSingleRPReconstruction = cms.bool(True)
process.ctppsProtons.doMultiRPReconstruction = cms.bool(True)


process.ALL = cms.Path(
#    process.recoCTPPS *
    process.totemRPLocalReconstruction *
    process.ctppsDiamondRecHits *
    process.ctppsDiamondLocalTracks*
    process.ctppsPixelLocalReconstruction *
    process.ctppsLocalTrackLiteProducer *
    process.ctppsProtons *
    process.myefficiency
                       )

process.schedule = cms.Schedule(process.ALL)

print process.dumpPython()

