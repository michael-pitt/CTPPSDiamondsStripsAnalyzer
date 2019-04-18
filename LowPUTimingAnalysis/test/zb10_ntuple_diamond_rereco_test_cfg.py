
import FWCore.ParameterSet.Config as cms

#from Configuration.StandardSequences.Eras import eras

##process = cms.Process('CTPPS2',eras.Run2_25ns)
#from Configuration.StandardSequences.Eras import eras

#process = cms.Process("CTPPS2", eras.ctpps_2016)
process = cms.Process("CTPPS2")


# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

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
'/store/data/Run2017C/ZeroBias1/AOD/PromptReco-v2/000/300/088/00000/E4BF515D-1077-E711-BFA8-02163E014222.root'
#'file:/tmp/jjhollar/ctppsSim_MB_QGSJet_xangle150_divergence30.root'
        )
                             )

process.load("RecoCTPPS.ProtonReconstruction.year_2017_OF.ctppsProtonReconstructionOF_cfi")
#process.load("RecoCTPPS.ProtonReconstruction.year_2018_OFDB.ctppsProtonReconstructionOFDB_cfi")                                                                                  
# conditions DB for 2018                                                                                                                                                         
#from CondCore.CondDB.CondDB_cfi import *                                                                                                                                         
#CondDB.connect = 'frontier://FrontierProd/CMS_CONDITIONS'                                                                                                                        
#process.PoolDBESSource2 = cms.ESSource("PoolDBESSource",                                                                                                                         
#                                       CondDB,                                                                                                                                   
#                                       DumpStat = cms.untracked.bool(False),                                                                                                     
#                                       toGet = cms.VPSet(cms.PSet(                                                                                                               
#            record = cms.string('LHCInfoRcd'),                                                                                                                                   
#            #tag = cms.string("LHCInfoTest_prompt_v3")                                                                                                                           
#            tag = cms.string("LHCInfoEndFill_prompt_v1")                                                                                                                         
#            )),                                                                                                                                                                  
#                                       )                                                                                                                                         


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
    tagRecoProtons = cms.InputTag( "ctppsProtonReconstructionOFDB"),
    outfilename = cms.untracked.string( "output_ZeroBias.root" ),
#    outfilename = cms.untracked.string( "output_CD_MBR.root" ),
    isMC = cms.bool(False)
)

process.mydiamonds.isLowPU = cms.bool(True)


# raw-to-digi conversion
#process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")
# Local RECO
#process.load("RecoCTPPS.Configuration.recoCTPPS_cff")
# Local pixel RECO
process.load("RecoCTPPS.Configuration.recoCTPPS_sequences_cff")
from Geometry.VeryForwardGeometry.geometryRPFromDB_cfi import *
process.load("Geometry.VeryForwardGeometry.geometryRPFromDB_cfi")
process.load("RecoCTPPS.PixelLocal.ctppsPixelLocalReconstruction_cff")
process.ctppsLocalTrackLiteProducer.tagPixelTrack = cms.InputTag("ctppsPixelLocalTracks","","CTPPS2")
process.ctppsLocalTrackLiteProducer.doNothing = cms.bool(False)
# March 2019 re-reco from AOD
process.ctppsProtonReconstructionOFDB.tagLocalTrackLite = cms.InputTag("ctppsLocalTrackLiteProducer","","CTPPS2")

############
#process.o1 = cms.OutputModule("PoolOutputModule",
##        outputCommands = cms.untracked.vstring('drop *',
##                                               'keep *_totemRP*_*_*',
##                                               'keep *_ctpps*_*_*'
##    
#                              outputCommands = cms.untracked.vstring('keep *_*_*_*'),
#        fileName = cms.untracked.string('/tmp/jjhollar/CTPPS_rereco_300088_ZB.root')
#        )


process.ALL = cms.Path(
#    process.ctppsDiamondRawToDigi * 
#    process.ctppsRawToDigi * 
#    process.recoCTPPS * 
#
# March 2019 re-reco from AOD
    process.ctppsPixelClusters *
    process.ctppsPixelRecHits *
    process.ctppsPixelLocalTracks *
    process.ctppsLocalTrackLiteProducer * 
#
#   for data:
    process.ctppsProtonReconstructionOFDB *
    process.mydiamonds 
                       )

process.schedule = cms.Schedule(process.ALL)

#process.outpath = cms.EndPath(process.o1)

#process.schedule = cms.Schedule(process.ALL,process.outpath)

# filter all path with the production filter sequence
#for path in process.paths:
#    getattr(process,path)._seq =  getattr(process,path)._seq


#print process.dumpPython()

