<b>Installation:</b>

While the new unpacker and the updated xml file is not included in the official CMSSW repo, use the instructions below. Please, check with CT-PPS software off-line experts.

```
cmsrel CMSSW_9_2_6
cd CMSSW_9_2_6/src
cmsenv
git cms-init
git cms-merge-topic CTPPS:diamond_unpacker_afterTS1_2017
git cms-addpkg EventFilter/CTPPSRawToDigi
git cms-addpkg CondFormats/CTPPSReadoutObjects
cd CondFormats/CTPPSReadoutObjects/xml
wget -q -N https://raw.githubusercontent.com/cms-sw/cmssw/master/CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond_2017.xml
cd $CMSSW_BASE/src
git clone https://github.com/dfigueiredo/CTPPSDiamondAnalyzer.git
scram b -j 8
```

If your are using the latest CMSSW release which include them:

```
cmsrel CMSSW_9_2_6
cd CMSSW_9_2_6/src
cmsenv
git cms-init
git cms-addpkg EventFilter/CTPPSRawToDigi
git cms-addpkg CondFormats/CTPPSReadoutObjects
cd $CMSSW_BASE/src
git clone https://github.com/dfigueiredo/CTPPSDiamondAnalyzer.git
scram b -j 8
```

<b>Running Monitor Off-Line:</b>

Edit and Run a python script to generate a list of files from a specific path.
i.e: /store/t0streamer/Data/PhysicsCommissioning/000/298/996/

```
cd $CMSSW_BASE/src/CTPPSDiamondAnalyzer/Skimmer/test
python generateFiles.py (it will generate the file python/AutoGenerate_cff.py that includes the list of files in t0streamer)
```

In case you would like to run a single file, edit RunMonitor.py. 

<b>Running Monitor Off-Line:</b>

```
cmsRun CTPPSDiamondAnalyzer/Skimmer/test/RunMonitor.py
```
In case you would like to submit jobs on lxbatch:

```
cd $CMSSW_BASE/src/CTPPSDiamondAnalyzer/Skimmer/test
python generateFiles.py (it will generate the file python/AutoGenerate_cff.py that includes the list of files in t0streamer)
./sendJob.sh (outputs will be saved on $CMSSW_BASE/src/CTPPSDiamondAnalyzer/Skimmer/test)
```

<b>Copy the Plots to your Private DNS folder in lxplus:</b>

```
cp -r RunXXXXX /afs/cern.ch/user/public/html/. (or www)
```

This is accessible from http://cmsdoc.cern.ch/~$USER
