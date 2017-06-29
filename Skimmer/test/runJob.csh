#!/bin/csh
setenv X509_USER_PROXY "/afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/TimingDataAnalysis/CMSSW_9_2_0_patch2/src/CTPPSDiamondAnalyzer/Skimmer/test/x509_proxy"
voms-proxy-init --voms cms --noregen

fusermount -u /afs/cern.ch/user/d/dmf/private/eos 
eosmount /afs/cern.ch/user/d/dmf/private/eos
cd /afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/TimingDataAnalysis/CMSSW_9_2_0_patch2/src/CTPPSDiamondAnalyzer/Skimmer/test
eval `scramv1 ru -csh`
cd -
env

cmsRun /afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/TimingDataAnalysis/CMSSW_9_2_0_patch2/src/CTPPSDiamondAnalyzer/Skimmer/test/RunSkimmer.py
cp output_all.root /afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/TimingDataAnalysis/CMSSW_9_2_0_patch2/src/CTPPSDiamondAnalyzer/Skimmer/test/output_all.root
