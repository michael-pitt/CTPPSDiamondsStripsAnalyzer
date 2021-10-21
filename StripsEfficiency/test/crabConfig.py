from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = "output_Efficiency_v1"
config.General.workArea = "grid"
config.General.transferOutputs=True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = "StripEfficiency"
config.JobType.psetName = "/afs/cern.ch/work/m/mpitt/PROPOG/Efficiency/CMSSW_10_6_24/src/CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/RunEfficiency_cfg.py"
config.JobType.outputFiles = ['output_EfficiencyTest.root']

config.section_("Data")
config.Data.inputDataset = '/SingleMuon/Run2017H-09Aug2019_UL2017_LowPU-v1/AOD'
config.Data.ignoreLocality = False
config.Data.inputDBS = "global"
config.Data.lumiMask = '/afs/cern.ch/work/m/mpitt/PROPOG/Efficiency/CMSSW_10_6_24/src/CTPPSDiamondsStripsAnalyzer/StripsEfficiency/data/combined_RPIN_CMS_LOWMU.json'
config.Data.splitting = "LumiBased"
config.Data.unitsPerJob = 30
config.Data.publication = False
config.Data.outLFNDirBase = "/store/group/cmst3/user/mpitt/StripEfficiencies/SingleMuon/"

config.section_("Site")
config.Site.storageSite = "T2_CH_CERN"