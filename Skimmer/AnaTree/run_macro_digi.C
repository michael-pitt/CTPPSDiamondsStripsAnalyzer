void run_macro_digi(){
  gROOT->Reset();
  gROOT->ProcessLine(".L CTPPSSkimmerDigiAnalyzer.C++");
  gROOT->ProcessLine("run()");
}
