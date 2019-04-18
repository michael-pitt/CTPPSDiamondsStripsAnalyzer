<b>Instructions for running ntuple-makers with new diamond unpacking+mapping, and pixels reco:</b>

Analyzers for studies of strips radiation damage efficiency, diamonds timing in Low-PU data, y* reconstruction/optics in Low-PU data

[forked from https://github.com/dfigueiredo/CTPPSDiamondAnalyzer - see there for instructions to run the 
original monitor/skimming code]

---------------------------------------------------------------
CMSSW_10_2_X 
---------------------------------------------------------------
cmsrel CMSSW_10_2_13

cd CMSSW_10_2_13

cmsenv

-> Follow the recipe to setup the standard proton reconstruction for 10_2_X https://twiki.cern.ch/twiki/bin/viewauth/CMS/CTPPSStandardProtonReconstruction
   (this is necessary until the reco is integrated in 10_6_X for the legacy re-RECO)

git cms-init

git clone https://github.com/jjhollar/CTPPSDiamondsStripsAnalyzer.git

scram b -j 8

      *** To make ntuples on low-PU data with information for timing studies, y* reconstruction studies, etc.

            cd CTPPSDiamondsStripsAnalyzer/LowPUTimingAnalysis/test/

      	    cmsRun zb10_ntuple_diamond_rereco_test_cfg.py

      *** To make ntuples for strips radiation damage studies

      	    cd CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/

	    cmsRun RunEfficiency_cfg.py
