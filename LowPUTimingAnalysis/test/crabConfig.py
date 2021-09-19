from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = "testTiming_v1"
config.General.workArea = "grid"
config.General.transferOutputs=True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "RunTimingAnalysisMINIAOD_cfg.py"
config.JobType.outputFiles = ['output_ZeroBias.root']

config.section_("Data")
config.Data.inputDataset = "/ZeroBias/Run2017H-09Aug2019_UL2017_LowPU-v1/MINIAOD"
config.Data.ignoreLocality = False
config.Data.inputDBS = "global"
config.Data.lumiMask = '/eos/project/c/ctpps/Operations/DataExternalConditions/2017/combined_RPIN_CMS_2017H.json'
config.Data.splitting = "LumiBased"
config.Data.unitsPerJob = 30
config.Data.publication = False
config.Data.outLFNDirBase = "/store/group/cmst3/user/mpitt/test/"

config.section_("Site")
config.Site.storageSite = "T2_CH_CERN"
