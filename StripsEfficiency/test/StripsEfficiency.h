//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec 19 15:32:48 2018 by ROOT version 6.10/09
// from TTree ntp1/ntp1
// found on file: output_EfficiencyTestStrips_2017B_v3.root
//////////////////////////////////////////////////////////

#ifndef StripsEfficiency_h
#define StripsEfficiency_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class StripsEfficiency {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           Run;
   Int_t           LumiSection;
   Int_t           BX;
   Int_t           EventNum;
   Int_t           nLiteTracks;
   Float_t         TrackLiteX[42];   //[nLiteTracks]
   Float_t         TrackLiteY[42];   //[nLiteTracks]
   Int_t           TrackLiteRPID[42];   //[nLiteTracks]
   Int_t           nStripTracks;
   Int_t           StripTrackValid[2];   //[nStripTracks]
   Float_t         StripTrackX[2];   //[nStripTracks]
   Float_t         StripTrackY[2];   //[nStripTracks]
   Float_t         StripTrackThX[2];   //[nStripTracks]
   Float_t         StripTrackThY[2];   //[nStripTracks]
   Int_t           StripTrackRPID[2];   //[nStripTracks]
   Int_t           StripTrackTooFullU[2];   //[nStripTracks]
   Int_t           StripTrackTooFullV[2];   //[nStripTracks]
   Int_t           StripTrackUPatterns[2];   //[nStripTracks]
   Int_t           StripTrackVPatterns[2];   //[nStripTracks]
   Int_t           nPixelTracks;
   Double_t        PixTrackX[20];   //[nPixelTracks]
   Double_t        PixTrackY[20];   //[nPixelTracks]
   Double_t        PixTrackThX[20];   //[nPixelTracks]
   Double_t        PixTrackThY[20];   //[nPixelTracks]
   Double_t        PixTrackChi2[20];   //[nPixelTracks]
   Int_t           PixTrackNdof[20];  //[nPixelTracks]
   Double_t        PixTrackZ[20];   //[nPixelTracks]
   Int_t           PixTrackArm[20];   //[nPixelTracks]
   Int_t           nProtons;
   Float_t         ProtonXi[21];   //[nProtons]                               
   Float_t         ProtonThX[21];   //[nProtons]                              
   Float_t         ProtonThY[21];   //[nProtons]                              
   Float_t         Protont[21];   //[nProtons]                                
   Int_t           ProtonIsMultiRP[21];   //[nProtons]                        
   Int_t           ProtonRPID[21];   //[nProtons]                             
   Int_t           ProtonArm[21];   //[nProtons]                              

   // List of branches
   TBranch        *b_Run;   //!
   TBranch        *b_LumiSection;   //!
   TBranch        *b_BX;   //!
   TBranch        *b_EventNum;   //!
   TBranch        *b_nLiteTracks;   //!
   TBranch        *b_TrackLiteX;   //!
   TBranch        *b_TrackLiteY;   //!
   TBranch        *b_TrackLiteRPID;   //!
   TBranch        *b_nStripTracks;   //!
   TBranch        *b_StripTrackValid;   //!
   TBranch        *b_StripTrackX;   //!
   TBranch        *b_StripTrackY;   //!
   TBranch        *b_StripTrackThX;   //!
   TBranch        *b_StripTrackThY;   //!
   TBranch        *b_StripTrackRPID;   //!
   TBranch        *b_StripTrackTooFullU;   //!
   TBranch        *b_StripTrackTooFullV;   //!
   TBranch        *b_StripTrackUPatterns;   //!
   TBranch        *b_StripTrackVPatterns;   //!
   TBranch        *b_nPixelTracks;   //!
   TBranch        *b_PixTrackX;   //!
   TBranch        *b_PixTrackY;   //!
   TBranch        *b_PixTrackThX;   //!
   TBranch        *b_PixTrackThY;   //!
   TBranch        *b_PixTrackChi2;   //!
   TBranch        *b_PixTrackNdof;   //!
   TBranch        *b_PixTrackZ;   //!
   TBranch        *b_PixTrackArm;   //!
   TBranch        *b_nProtons;   //!                                                                                                                 
   TBranch        *b_ProtonXi;   //!                                                                                                                 
   TBranch        *b_ProtonThX;   //!                                                                                                                
   TBranch        *b_ProtonThY;   //!                                                                                                                
   TBranch        *b_Protont;   //!                                                                                                                  
   TBranch        *b_ProtonIsMultiRP;   //!                                                                                                          
   TBranch        *b_ProtonRPID;   //!                                                                                                               
   TBranch        *b_ProtonArm;   //!                                                                                                                

   StripsEfficiency(TTree *tree=0);
   virtual ~StripsEfficiency();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual Int_t    getXangle(int run,int lumi, const char* filename);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int runmin=0, int runmax=9999999, string era="2017B");
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef StripsEfficiency_cxx
StripsEfficiency::StripsEfficiency(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     //     TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/tmp/jjhollar/ctpps_2017E_efficiency_jetht_merge.root");
     //     TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/tmp/jjhollar/ctpps_2017E_efficiency_btagcsv_merge.root");
     TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/cms/store/group/dpg_ctpps/comm_ctpps/StripsEfficiencies2017/ctpps_2017B_efficiency_singleele_merge.root");

      if (!f || !f->IsOpen()) {
	//	f = new TFile("/tmp/jjhollar/ctpps_2017E_efficiency_jetht_merge.root");
	f = new TFile("/eos/cms/store/group/dpg_ctpps/comm_ctpps/StripsEfficiencies2017/ctpps_2017B_efficiency_singleele_merge.root");
	//	f = new TFile("/tmp/jjhollar/ctpps_2017E_efficiency_btagcsv_merge.root");
      }
      f->GetObject("ntp1",tree);

   }
   Init(tree);
}

StripsEfficiency::~StripsEfficiency()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t StripsEfficiency::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t StripsEfficiency::LoadTree(Long64_t entry)
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

void StripsEfficiency::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Run", &Run, &b_Run);
   fChain->SetBranchAddress("LumiSection", &LumiSection, &b_LumiSection);
   fChain->SetBranchAddress("BX", &BX, &b_BX);
   fChain->SetBranchAddress("EventNum", &EventNum, &b_EventNum);
   fChain->SetBranchAddress("nLiteTracks", &nLiteTracks, &b_nLiteTracks);
   fChain->SetBranchAddress("TrackLiteX", TrackLiteX, &b_TrackLiteX);
   fChain->SetBranchAddress("TrackLiteY", TrackLiteY, &b_TrackLiteY);
   fChain->SetBranchAddress("TrackLiteRPID", TrackLiteRPID, &b_TrackLiteRPID);
   fChain->SetBranchAddress("nStripTracks", &nStripTracks, &b_nStripTracks);
   fChain->SetBranchAddress("StripTrackValid", StripTrackValid, &b_StripTrackValid);
   fChain->SetBranchAddress("StripTrackX", StripTrackX, &b_StripTrackX);
   fChain->SetBranchAddress("StripTrackY", StripTrackY, &b_StripTrackY);
   fChain->SetBranchAddress("StripTrackThX", StripTrackThX, &b_StripTrackThX);
   fChain->SetBranchAddress("StripTrackThY", StripTrackThY, &b_StripTrackThY);
   fChain->SetBranchAddress("StripTrackRPID", StripTrackRPID, &b_StripTrackRPID);
   fChain->SetBranchAddress("StripTrackTooFullU", StripTrackTooFullU, &b_StripTrackTooFullU);
   fChain->SetBranchAddress("StripTrackTooFullV", StripTrackTooFullV, &b_StripTrackTooFullV);
   fChain->SetBranchAddress("StripTrackUPatterns", StripTrackUPatterns, &b_StripTrackUPatterns);
   fChain->SetBranchAddress("StripTrackVPatterns", StripTrackVPatterns, &b_StripTrackVPatterns);
   fChain->SetBranchAddress("nPixelTracks", &nPixelTracks, &b_nPixelTracks);
   fChain->SetBranchAddress("PixTrackX", PixTrackX, &b_PixTrackX);
   fChain->SetBranchAddress("PixTrackY", PixTrackY, &b_PixTrackY);
   fChain->SetBranchAddress("PixTrackThX", PixTrackThX, &b_PixTrackThX);
   fChain->SetBranchAddress("PixTrackThY", PixTrackThY, &b_PixTrackThY);
   fChain->SetBranchAddress("PixTrackChi2", PixTrackChi2, &b_PixTrackChi2);
   fChain->SetBranchAddress("PixTrackNdof", PixTrackNdof, &b_PixTrackNdof);
   fChain->SetBranchAddress("PixTrackZ", PixTrackZ, &b_PixTrackZ);
   fChain->SetBranchAddress("PixTrackArm", PixTrackArm, &b_PixTrackArm);
   fChain->SetBranchAddress("nProtons", &nProtons, &b_nProtons);
   fChain->SetBranchAddress("ProtonXi", ProtonXi, &b_ProtonXi);
   fChain->SetBranchAddress("ProtonThX", ProtonThX, &b_ProtonThX);
   fChain->SetBranchAddress("ProtonThY", ProtonThY, &b_ProtonThY);
   fChain->SetBranchAddress("Protont", Protont, &b_Protont);
   fChain->SetBranchAddress("ProtonIsMultiRP", ProtonIsMultiRP, &b_ProtonIsMultiRP);
   fChain->SetBranchAddress("ProtonRPID", ProtonRPID, &b_ProtonRPID);
   fChain->SetBranchAddress("ProtonArm", ProtonArm, &b_ProtonArm);
   Notify();
}

Bool_t StripsEfficiency::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void StripsEfficiency::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t StripsEfficiency::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef StripsEfficiency_cxx
