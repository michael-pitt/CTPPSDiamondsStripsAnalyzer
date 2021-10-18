<b>Instructions for running ntuple-makers:</b>

Analyzers for studies of strips radiation damage efficiency, diamonds timing in Low-PU data, y* reconstruction/optics in Low-PU data

[forked from https://github.com/jjhollar/CTPPSDiamondAnalyzer - see there for instructions to run the 
original monitor/skimming code]
[which forked from https://github.com/dfigueiredo/CTPPSDiamondAnalyzer - see there for instructions to run the 
original monitor/skimming code]

---------------------------------------------------------------
CMSSW_10_6_X 
---------------------------------------------------------------
cmsrel CMSSW_10_6_17

cd CMSSW_10_6_17

cmsenv

git cms-init

git clone https://github.com/michael-pitt/CTPPSDiamondsStripsAnalyzer.git

scram b -j 8

      *** To make ntuples on low-PU Ultra-Legacy data with information for timing studies, y* reconstruction studies, etc.

            cd CTPPSDiamondsStripsAnalyzer/LowPUTimingAnalysis/test/

      	    cmsRun RunTimingAnalysis_cfg.py

      *** To make ntuples for strips radiation damage studies

      	    cd CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/

	    cmsRun RunEfficiency_cfg.py
