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
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

#########################
#    RAW-DIGI-RECO      #
#########################
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

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# rechits production
process.load('Geometry.VeryForwardGeometry.geometryRP_cfi')
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi')

# local tracks fitter
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi')

#########################
#      Input files      #
#########################
process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(
        '/store/t0streamer/Data/Physics/000/298/597/run298597_ls0260_streamPhysics_StorageManager.dat',
    )
)

######################
#      Analyzer      #
######################
process.Monitor = cms.EDAnalyzer("CTPPSMonitor",
    tagStatus = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDigi = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDiamondRecHits = cms.InputTag("ctppsDiamondRecHits"),
    tagDiamondLocalTracks = cms.InputTag("ctppsDiamondLocalTracks"),
    tagLocalTrack = cms.InputTag("totemRPLocalTrackFitter"),
    verbosity = cms.untracked.uint32(0),
    path = cms.untracked.string("Run298597")
)

######################
#      TFile         #
######################
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('monitor_histograms.root'),
    closeFileFast = cms.untracked.bool(True)
)

process.p = cms.Path(
    process.ctppsRawToDigi *
    process.recoCTPPS *
    process.ctppsDiamondRawToDigi *
    process.ctppsDiamondRecHits *
    process.ctppsDiamondLocalTracks *
    process.Monitor
    )
