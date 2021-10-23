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


## Analysis 

### Creating ntuples for strips radiation damage studies

Local tests:
```
voms-proxy-init --voms cms
cmsRun $CMSSW_BASE/src/CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/RunEfficiency_cfg.py
```
Submision to grid:
```
voms-proxy-init --voms cms

source /cvmfs/cms.cern.ch/crab3/crab.sh
crab submit -c $CMSSW_BASE/src/CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/crabConfig.py
```

Check the AOD files you are using and the trigger in `RunEfficiency_cfg.py` files

To test the progress, run:
```
crab status -d grid/crab_output_Efficiency_v1
```

It is possible that not all blocks are on DISK, then crab will request a disk replica via [rucio](https://twiki.cern.ch/twiki/bin/viewauth/CMS/Rucio). Initialize rucio in a new shell
 with:
```
source /cvmfs/cms.cern.ch/rucio/setup-py3.sh
export RUCIO_ACCOUNT=`whoami`
```
 

### Processing the ntuples
   
   ```
.L CMSSW_10_6_24/src/CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/StripsEfficiency.C+
StripsEfficiency t;
t.Loop()
   ```  
### Making plots
   
   ```
.L CMSSW_10_6_24/src/CTPPSDiamondsStripsAnalyzer/StripsEfficiency/test/EffCurves.C
TString filename="EffstudiesMatchingDebugTestAllRunsAllTracksAllY_Run2_2017_2017H_0_9999999.root";
EffCurves(filename);
Eff2D(filename);
EffCategories(filename);
   ```
   