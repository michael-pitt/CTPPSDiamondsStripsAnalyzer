#bin/tcsh
voms-proxy-destroy
voms-proxy-init --voms cms -valid 720:00 --out x509_proxy 
bsub -q 2nd -J job1 < runJob.csh
