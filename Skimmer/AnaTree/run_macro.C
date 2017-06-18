void run_macro(){
  gROOT->Reset();
  gROOT->ProcessLine(".L CTPPSSkimmerAnalyzer.C++");
  gROOT->ProcessLine("run()");
}
