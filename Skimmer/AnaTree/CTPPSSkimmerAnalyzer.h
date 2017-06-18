//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Dec  5 20:34:41 2016 by ROOT version 6.06/05
// from TTree ntp1/ntp1
// found on file: output_ee_data_merge_2016C_23Sep2016_partial1.root
//////////////////////////////////////////////////////////

#ifndef CTPPSSkimmerAnalyzer_h
#define CTPPSSkimmerAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "iostream"

// Constants
#define SEC_PER_LUMI_SECTION 23.31
#define CHANNEL_OF_VFAT_CLOCK 30
#define DISPLAY_RESOLUTION_FOR_HITS_MM 0.1
#define HPTDC_BIN_WIDTH_NS 25./1024
#define CTPPS_NUM_OF_ARMS 2
#define CTPPS_DIAMOND_STATION_ID 1
#define CTPPS_DIAMOND_RP_ID 6
#define CTPPS_NEAR_RP_ID 2
#define CTPPS_FAR_RP_ID 3
#define CTPPS_DIAMOND_NUM_OF_PLANES 4
#define CTPPS_DIAMOND_NUM_OF_CHANNELS 12
#define CTPPS_FED_ID_56 582
#define CTPPS_FED_ID_45 583

class CTPPSSkimmerAnalyzer {
  public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.
    Bool_t          valid;
    Int_t           getBxCMS;
    Int_t           getOrbitCMS;
    Int_t           getLsCMS;
    vector<int>     *HLT;
    vector<int>     *arm;
    vector<int>     *station;
    vector<int>     *rp;
    vector<int>     *plane;
    vector<int>     *channel;
    vector<int>     *multiplehits;
    vector<double>  *getToT;
    vector<int>     *getOOTIndex;
    vector<double>  *getT;
    vector<double>  *getX;
    vector<double>  *getXWidth;
    vector<double>  *getY;
    vector<double>  *getYWidth;
    vector<int>     *getFedId;
    vector<int>     *getBx;

    // List of branches
    TBranch        *b_valid;   //!
    TBranch        *b_bx_cms;   //!
    TBranch        *b_orbit;   //!
    TBranch        *b_lumi_section;   //!
    TBranch        *b_HLT;   //!
    TBranch        *b_arm;   //!
    TBranch        *b_station;   //!
    TBranch        *b_rp;   //!
    TBranch        *b_plane;   //!
    TBranch        *b_channel;   //!
    TBranch        *b_multiplehits;   //!
    TBranch        *b_getToT;   //!
    TBranch        *b_getOOTIndex;   //!
    TBranch        *b_getT;   //!
    TBranch        *b_getX;   //!
    TBranch        *b_getXWidth;   //!
    TBranch        *b_getY;   //!
    TBranch        *b_getYWidth;   //!
    TBranch        *b_getFedId;   //!
    TBranch        *b_getBx;   //!

    CTPPSSkimmerAnalyzer(TTree *tree=0);
    virtual ~CTPPSSkimmerAnalyzer();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);
    virtual void     Loop();
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
    virtual void     CreateHistos();
    virtual void     WriteHistos();
    virtual void     FillHistos(int i);

};

#endif

#ifdef CTPPSSkimmerAnalyzer_cxx
CTPPSSkimmerAnalyzer::CTPPSSkimmerAnalyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("run_295977.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("run_295977.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("run_295977.root:/Skimmer");
      dir->GetObject("CTPPSSkimmerDiamond",tree);

   }
   Init(tree);
}

CTPPSSkimmerAnalyzer::~CTPPSSkimmerAnalyzer()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t CTPPSSkimmerAnalyzer::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t CTPPSSkimmerAnalyzer::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void CTPPSSkimmerAnalyzer::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointer
  HLT = 0;
  arm = 0;
  station = 0;
  rp = 0;
  plane = 0;
  channel = 0;
  multiplehits = 0;
  getToT = 0;
  getOOTIndex = 0;
  getT = 0;
  getX = 0;
  getXWidth = 0;
  getY = 0;
  getYWidth = 0;
  getFedId = 0;
  getBx = 0;

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("valid", &valid, &b_valid);
  fChain->SetBranchAddress("getBxCMS", &getBxCMS, &b_bx_cms);
  fChain->SetBranchAddress("getOrbitCMS", &getOrbitCMS, &b_orbit);
  fChain->SetBranchAddress("getLsCMS", &getLsCMS, &b_lumi_section);
  fChain->SetBranchAddress("HLT", &HLT, &b_HLT);
  fChain->SetBranchAddress("arm", &arm, &b_arm);
  fChain->SetBranchAddress("station", &station, &b_station);
  fChain->SetBranchAddress("rp", &rp, &b_rp);
  fChain->SetBranchAddress("plane", &plane, &b_plane);
  fChain->SetBranchAddress("channel", &channel, &b_channel);
  fChain->SetBranchAddress("multiplehits", &multiplehits, &b_multiplehits);
  fChain->SetBranchAddress("getToT", &getToT, &b_getToT);
  fChain->SetBranchAddress("getOOTIndex", &getOOTIndex, &b_getOOTIndex);
  fChain->SetBranchAddress("getT", &getT, &b_getT);
  fChain->SetBranchAddress("getX", &getX, &b_getX);
  fChain->SetBranchAddress("getXWidth", &getXWidth, &b_getXWidth);
  fChain->SetBranchAddress("getY", &getY, &b_getY);
  fChain->SetBranchAddress("getYWidth", &getYWidth, &b_getYWidth);
  fChain->SetBranchAddress("getFedId", &getFedId, &b_getFedId);
  fChain->SetBranchAddress("getBx", &getBx, &b_getBx);

  Notify();
}

Bool_t CTPPSSkimmerAnalyzer::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void CTPPSSkimmerAnalyzer::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t CTPPSSkimmerAnalyzer::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  std::cout << "Entry: " << entry << std::endl;
  return 1;
}
#endif // #ifdef CTPPSSkimmerAnalyzer_cxx

