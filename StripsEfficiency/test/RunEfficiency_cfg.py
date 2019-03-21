
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
#'/store/data/Run2017A/ZeroBias5/MINIAOD/PromptReco-v1/000/294/737/00000/38D4C37A-0940-E711-8F9C-02163E0142CE.root',
#'/store/data/Run2017A/ZeroBias5/MINIAOD/PromptReco-v1/000/294/737/00000/72BC20B9-0A40-E711-924C-02163E014375.root',
#'/store/data/Run2017A/ZeroBias5/MINIAOD/PromptReco-v1/000/294/737/00000/3EC8DC04-0840-E711-AB6B-02163E01A548.root',
#'/store/data/Run2017A/ZeroBias5/MINIAOD/PromptReco-v1/000/294/737/00000/7A837FC9-0840-E711-BB23-02163E01203D.root',
#'/store/data/Run2017A/ZeroBias5/MINIAOD/PromptReco-v1/000/294/737/00000/AA1C2154-1340-E711-922D-02163E019E3C.root',
#'/store/data/Run2017A/ZeroBias5/MINIAOD/PromptReco-v1/000/294/737/00000/567B73D3-9F40-E711-B188-02163E01A62D.root'
#'file:/tmp/jjhollar/38D4C37A-0940-E711-8F9C-02163E0142CE.root'
#'/store/data/Run2017B/SingleMuon/AOD/17Nov2017-v1/60000/CC68436B-7ED7-E711-B900-02163E01A211.root'
#'file:/tmp/jjhollar/72713F8B-16DE-E711-8528-02163E0145C9.root'
#'file:/tmp/jjhollar/4E012B5C-C6E2-E711-B52F-02163E014520.root'
'file:/tmp/jjhollar/4AAE90A8-1EE4-E711-A0DA-02163E01A1D8.root'

        ))

process.load("CTPPSDiamondAnalyzer.LowPUTimingAnalysis.HLTFilter_cfi")
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")

#process.load("RecoCTPPS.ProtonReconstruction.year_2018_OFDB.ctppsProtonReconstructionOFDB_cfi")
process.load("RecoCTPPS.ProtonReconstruction.year_2017_OF.ctppsProtonReconstructionOF_cfi")

process.myefficiency = cms.EDAnalyzer(
    'Efficiency',
    verticesTag = cms.InputTag('offlinePrimaryVertices'),
    tagTrackLites = cms.InputTag( "ctppsLocalTrackLiteProducer" ),
    outfilename = cms.untracked.string( "output_EfficiencyTest.root" )
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')


#process.load("RecoCTPPS.Configuration.recoCTPPS_cff")


process.ALL = cms.Path(
    process.ctppsProtonReconstructionOFDB *
    process.myefficiency
                       )

process.schedule = cms.Schedule(process.ALL)

#print process.dumpPython()

