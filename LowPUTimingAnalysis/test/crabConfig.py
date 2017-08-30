from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'ctpps_lowpileup300088_zerobias1'
config.General.workArea = '/tmp/jjhollar/ctpps_lowpileup300088_zerobias1'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'zb10_ntuple_diamond_rereco_test_cfg.py'
config.JobType.outputFiles = ['output_ZeroBias.root']

config.Data.inputDataset = '/ZeroBias1/Run2017C-PromptReco-v2/AOD'
config.Data.useParent = True
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 25
config.Data.runRange = '300088'

config.Data.outLFNDirBase = '/store/user/jjhollar/CTPPS_LowPileup300088ZB1/'
config.Data.publication = False
config.Data.outputDatasetTag = 'CTPPS_LowPileup300088ZB1'
config.Site.storageSite = 'T2_CH_CERN'
