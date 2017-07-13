<b>Installation:</b>

```
cmsrel CMSSW_9_2_6
cd CMSSW_9_2_6/src
cmsenv
git cms-init
git cms-merge-topic CTPPS:diamond_unpacker_afterTS1_2017
git cms-addpkg EventFilter/CTPPSRawToDigi
git cms-addpkg CondFormats/CTPPSReadoutObjects
cd CondFormats/CTPPSReadoutObjects/xml
wget https://raw.githubusercontent.com/CTPPS/cmssw/diamond_mapping2017_checkedTS1/CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond_2017.xml
git clone https://github.com/dfigueiredo/CTPPSDiamondAnalyzer.git
scram b -j 8
```

<b>Running Monitor Off-Line:</b>

```
cmsRun CTPPSDiamondAnalyzer/Skimmer/test/RunMonitor.py
```
