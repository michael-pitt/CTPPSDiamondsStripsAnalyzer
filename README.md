<b>Instructions for running ntuple-maker with new diamond unpacking+mapping, and pixels reco:</b>

[forked from https://github.com/dfigueiredo/CTPPSDiamondAnalyzer - see there for instructions to run the 
other monitor/skimming code]

---------------------------------------------------------------
CMSSW_9_4_X with pixel tracks (9_4_0_pre2 or later)
---------------------------------------------------------------
cmsrel CMSSW_9_4_0_pre2

cd CMSSW_9_4_0_pre2

cmsenv

git cms-init

git clone https://github.com/jjhollar/CTPPSDiamondAnalyzer.git

scram b -j 8

cd CTPPSDiamondAnalyzer/LowPUTimingAnalysis/test

cmsRun zb10_ntuple_diamond_rereco_test_cfg.py

---------------------------------------------------------------
CMSSW_9_2_X
---------------------------------------------------------------
cmsrel CMSSW_9_2_8

cd CMSSW_9_2_8/src

cmsenv 

git cms-init

git cms-merge-topic 19991

cp ~fabferro/WORKSPACE/public/92x_201708012300.tgz .

tar -xzvf 92x_201708012300.tgz

git clone https://github.com/jjhollar/CTPPSDiamondAnalyzer.git

scram b -j 8

cd CTPPSDiamondAnalyzer/LowPUTimingAnalysis/test

cmsRun zb10_ntuple_diamond_rereco_test_cfg.py
