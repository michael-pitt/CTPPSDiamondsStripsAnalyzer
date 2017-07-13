from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'ctpps_alignment_runs'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'RunSkimmerCrab.py'

config.Data.inputDataset = '/ExpressPhysics/Run2017B-Express-v1/FEVT'
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 20
config.Data.runRange = '298593,298594,298596,298597,298598'

config.Data.outLFNDirBase = '/store/group/phys_pps/AlignmentRun2017B'
config.Data.publication = False
config.Data.outputDatasetTag = 'CTPPS-AlignmentRun'

config.Site.storageSite = 'T2_CH_CERN'
