import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CTPPS2',eras.Run2_25ns)

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


from CTPPSDiamondAnalyzer.LowPUTimingAnalysis.ZeroBias10_300088_cff import readFiles
from CTPPSDiamondAnalyzer.LowPUTimingAnalysis.ZeroBias10_300088_RAW_cff import secReadFiles

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring(readFiles))
process.source.secondaryFileNames = cms.untracked.vstring(secReadFiles)

process.mydiamonds = cms.EDAnalyzer(
    'Diamonds',
    tagStatus = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),
    tagDigi = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),
    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond", "CTPPS2"),
    tagDiamondRecHits = cms.InputTag("ctppsDiamondRecHits"),
    tagDiamondLocalTracks = cms.InputTag("ctppsDiamondLocalTracks"),
    tagLocalTrack = cms.InputTag("totemRPLocalTrackFitter"),
    verticesTag = cms.InputTag('offlinePrimaryVertices'),
    jetsTag = cms.InputTag('ak4PFJets'),
    tagParticleFlow = cms.InputTag('particleFlow'),
    tagRPixDigi = cms.InputTag( "ctppsPixelDigis" ),
    tagRPixCluster = cms.InputTag( "ctppsPixelClusters" ),
    tagRPixRecHit = cms.InputTag( "ctppsPixelRecHits" ),
    outfilename = cms.untracked.string( "output_ZeroBias.root" )
)


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')


# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

############
#process.o1 = cms.OutputModule("PoolOutputModule",
##        outputCommands = cms.untracked.vstring('drop *',
##                                               'keep *_totemRP*_*_*',
##                                               'keep *_ctpps*_*_*'
##    
#                              outputCommands = cms.untracked.vstring('keep *_*_*_*'
#),
#        fileName = cms.untracked.string('/tmp/jjhollar/CTPPS_redigi_rereco_300088_HINCaloJets.root')
#        )

process.load("RecoCTPPS.Configuration.recoCTPPS_cff")


process.ALL = cms.Path(
    process.ctppsDiamondRawToDigi * 
    process.recoCTPPS * 
    process.mydiamonds
                       )

process.schedule = cms.Schedule(process.ALL)

#process.outpath = cms.EndPath(process.o1)

#process.schedule = cms.Schedule(process.ALL,process.outpath)

# filter all path with the production filter sequence
#for path in process.paths:
#    getattr(process,path)._seq =  getattr(process,path)._seq


print process.dumpPython()

