<b>Instructions for running ntuple-makers:</b>

Analyzers for studies of strips radiation damage efficiency, diamonds timing in Low-PU data, y* reconstruction/optics in Low-PU data

   * Forked from https://github.com/jjhollar/CTPPSDiamondAnalyzer - see there for instructions to run the 
original monitor/skimming code
   * Which was forked from https://github.com/dfigueiredo/CTPPSDiamondAnalyzer - see there for instructions to run the 
original monitor/skimming code

---------------------------------------------------------------
CMSSW_10_6_X 
---------------------------------------------------------------
cmsrel CMSSW_10_6_24

cd CMSSW_10_6_24

cmsenv

git clone git@github.com:michael-pitt/CTPPSDiamondsStripsAnalyzer.git

scram b -j 8


      *** To make ntuples for strips radiation damage studies

      	    cd CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/

	    cmsRun RunEfficiency_cfg.py
