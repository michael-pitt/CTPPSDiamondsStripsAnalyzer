import FWCore.ParameterSet.Config as cms
import string

process = cms.Process('RECODQM')

process.maxEvents = cms.untracked.PSet(
input = cms.untracked.int32(100)
)


#process.verbosity = cms.untracked.PSet(
#    input = cms.untracked.int32(-1)
#)




# diamonds mapping
process.totemDAQMappingESSourceXML_TimingDiamond = cms.ESSource("TotemDAQMappingESSourceXML",
  verbosity = cms.untracked.uint32(0),
  subSystem = cms.untracked.string("TimingDiamond"),
  configuration = cms.VPSet(
    # before diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("1:min - 283819:max"),
      mappingFileNames = cms.vstring(),
      maskFileNames = cms.vstring()
    ),
    # after diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("283820:min - 999999999:max"),
      mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond.xml"),
      maskFileNames = cms.vstring()
    )
  )
)


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# minimum of logs
#process.MessageLogger = cms.Service("MessageLogger",
    #statistics = cms.untracked.vstring(),
    #destinations = cms.untracked.vstring('cerr'),
    #cerr = cms.untracked.PSet(
    #    threshold = cms.untracked.string('WARNING')
    #)
#)

# load DQM framework
#process.load("DQM.Integration.config.environment_cfi")
#process.dqmEnv.subSystemFolder = "CTPPS"
#process.dqmEnv.eventInfoFolder = "EventInfo"
#process.dqmSaver.path = ""
#process.dqmSaver.tag = "CTPPS"


# raw data source
process.source = cms.Source("NewEventStreamFileReader",
  fileNames = cms.untracked.vstring(
        '/store/t0streamer/Minidaq/A/000/295/626/run295626_ls0001_streamA_StorageManager.dat',
  )
)


# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# rechits production
process.load('Geometry.VeryForwardGeometry.geometryRP_cfi')
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi')

# local tracks fitter
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi')


# CTPPS DQM modules
#process.load("DQM.Skimmer.ctppsDQM_cff")
#from DQM.Skimmer.Skimmer_cfi import *
#process.ctppsDiamondDQMSource.excludeMultipleHits = cms.bool(True);

process.Skimmer = cms.EDAnalyzer("CTPPSSkimmer",
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



#TFile Service
process.TFileService = cms.Service("TFileService",
      fileName = cms.string("output.root"),
      closeFileFast = cms.untracked.bool(True)
  )


process.path = cms.Path(
  process.ctppsRawToDigi *
  process.recoCTPPS *
  process.ctppsDiamondRawToDigi *
  process.ctppsDiamondRecHits *
  process.ctppsDiamondLocalTracks *
  process.Skimmer
)

#process.end_path = cms.EndPath(
#  process.dqmEnv +
#  process.dqmSaver
#)

#process.schedule = cms.Schedule(
#  process.path
#  process.end_path
#)
