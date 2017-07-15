#!/bin/csh
setenv X509_USER_PROXY "/afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/TimingDataAnalysis/CMSSW_9_2_0_patch2/src/CTPPSDiamondAnalyzer/Skimmer/test/x509_proxy"
voms-proxy-init --voms cms --noregen

cd /afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/NewUnpacker/CMSSW_9_2_6/src/CTPPSDiamondAnalyzer/Skimmer/test
eval `scramv1 ru -csh`
cd -
env

cmsRun /afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/NewUnpacker/CMSSW_9_2_6/src/CTPPSDiamondAnalyzer/Skimmer/test/RunMonitor.py
cp -r Run*/ /afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/NewUnpacker/CMSSW_9_2_6/src/CTPPSDiamondAnalyzer/Skimmer/test/.
cp -r monitor*root /afs/cern.ch/user/d/dmf/private/work/private/PPSDAQ/NewUnpacker/CMSSW_9_2_6/src/CTPPSDiamondAnalyzer/Skimmer/test/.
