
import FWCore.ParameterSet.Config as cms

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


#from CTPPSDiamondAnalyzer.LowPUTimingAnalysis.MCInputFiles_cff import readFiles
#from CTPPSDiamondAnalyzer.LowPUTimingAnalysis.ZeroBias_319488_cff import readFiles
#from CTPPSDiamondAnalyzer.LowPUTimingAnalysis.InputFiles_cff import readFiles

#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring(readFiles))
#process.source.secondaryFileNames = cms.untracked.vstring(secReadFiles)

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
#'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/B62FED11-B3D8-E711-AFAD-02163E013989.root',
#'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/CCFB8C4A-B2D8-E711-B26E-02163E019BA9.root',
#'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/1E5B8023-B7D8-E711-8BCD-02163E011F14.root',
#'/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/40003/A42F4354-B9D8-E711-9B00-02163E013981.root'

'/store/data/Run2017C/ZeroBias1/AOD/PromptReco-v2/000/300/088/00000/E4BF515D-1077-E711-BFA8-02163E014222.root'
        )
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
process.CondDBAlignment = CondDB.clone( connect = 'sqlite_file:/afs/cern.ch/user/c/cmora/public/CTPPSDB/AlignmentSQlite/CTPPSRPRealAlignment_table_v2\
6Apr.db' )
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


process.mydiamonds = cms.EDAnalyzer(
    'Diamonds',
#    tagStatus = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),                                                                  
#    tagDigi = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),                                                                    
#    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),                                                                 
    tagStatus = cms.InputTag("ctppsDiamondRawToDigi","TimingDiamond","RECO"),
    tagDigi = cms.InputTag("ctppsDiamondRawToDigi","TimingDiamond","RECO"),
    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi","TimingDiamond","RECO"),
    tagDiamondRecHits = cms.InputTag("ctppsDiamondRecHits"),
    tagDiamondLocalTracks = cms.InputTag("ctppsDiamondLocalTracks"),
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
    outfilename = cms.untracked.string( "output_ZeroBias.root" ),
#    outfilename = cms.untracked.string( "output_CD_MBR.root" ),
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


process.mydiamonds.isLowPU = cms.bool(True)



process.ALL = cms.Path(
# March 2019 re-reco from AOD
#    process.ctppsPixelClusters *
#    process.ctppsPixelRecHits *
#    process.ctppsPixelLocalTracks *
#    process.ctppsLocalTrackLiteProducer * 
#
#   for data:
    process.totemRPLocalReconstruction *
    process.ctppsDiamondRecHits *
    process.ctppsDiamondLocalTracks*
    process.ctppsPixelLocalReconstruction *
    process.ctppsLocalTrackLiteProducer *
    process.ctppsProtons *
    process.mydiamonds 
                       )

process.schedule = cms.Schedule(process.ALL)

