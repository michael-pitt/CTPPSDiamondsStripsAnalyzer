#define NewTimingMacro_cxx
#include "NewTimingMacro.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void NewTimingMacro::Loop()
{
//   In a ROOT session, you can do:
//      root> .L NewTimingMacro.C
//      root> NewTimingMacro t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TH1F *h1 = new TH1F("h1","h1",200,-20,20);
   TH1F *h7 = new TH1F("h7","h7",200,-20,20);


   TH2F *h2 = new TH2F("h2","h2",100,-20,20,50,-3,3);
   //   TH1F *h1 = new TH1F("h1","h1",100,-2,2);
   TH1F *h3 = new TH1F("h3","h3",50,0,0.3);
   TH1F *h4 = new TH1F("h4","h4",50,0,0.3);
   TH1F *h5 = new TH1F("h5","h5",500,-1,1);
   TH2F *h6 = new TH2F("h6","h6",100,-2,2,250,0,1);
   //   TH1F *h7 = new TH1F("h7","h7",100,-2,2);
   TH1F *h8 = new TH1F("h8","h8",200,0,200);

   Int_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);

      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   
      nbytes += nb;

      // if (Cut(ientry) < 0) continue;

      if(jentry % 10000 == 0)
	std::cout << "Entry " << jentry << "/" << nentries << std::endl;

	Float_t p1time = 0.0;
	Float_t p2time = 0.0;
	Float_t p1timeerr = 0.0;
	Float_t p2timeerr = 0.0;
	Int_t np1 = 0;
	Int_t np2 = 0;
	Int_t nGoodVertex = 0;
      
	nGoodVertex = nVertices - PrimVertexIsBS[0];
	
	if(nTracksTiming == 2 && nGoodVertex == 1)
	  {
	    for(Int_t p = 0; p < nProtons; p++)
	      {
		if(ProtonIsMultiRP[p] == 1)
		  {
		    if(ProtonArm[p] == 0)
		      {
			if(ProtonTime[p] != 0)
			  {
			    p1time = ProtonTime[p];
			  }
			np1++;
		      }
		    if(ProtonArm[p] == 1)
		      {
			if(ProtonTime[p] != 0)
			  {
			    p2time = ProtonTime[p];
			  }
			np2++;
		      }
		  }
	      }
	    
	    if(np2 == 1 && np1 == 1)
	      std::cout << "N(protons) = " << np1 << ", " << np2 << ", t(p1) = " << p1time << ", t(p2) = " << p2time << std::endl;
	    
	    if((np1==1) && (np2==1) && (p1time != 0) && (p2time != 0) && (nTracksOneVertex<= 10))
	      {
		h2->Fill(PrimVertexZ[0],p2time-p1time);
		Float_t cmtons = 1.0/15.0;
		//		h1->Fill(PrimVertexZ[0]*cmtons - (p2time-p1time));
		h1->Fill(PrimVertexZ[0] - (p2time-p1time)/cmtons);

		h5->Fill((p2time+p1time)/2.0);
		h6->Fill(PrimVertexZ[0]*cmtons - (p2time-p1time), (p2time+p1time)/2.0);

		h8->Fill(nTracksFromVertex);
		
		if(TimingTrackTErr[0] < 0.1 && TimingTrackTErr[1] < 0.1)
		  {
		    //		    h7->Fill(PrimVertexZ[0]*cmtons - (p2time-p1time));
		    h7->Fill(PrimVertexZ[0] - (p2time-p1time)/cmtons);
		  }
		if(TimingTrackArm[0] == 0 && TimingTrackArm[1] == 1)
		  {
		    h3->Fill(TimingTrackTErr[0]); h4->Fill(TimingTrackTErr[1]);
		  }
		if(TimingTrackArm[0] == 1 && TimingTrackArm[1] == 0)
		  {
		    h4->Fill(TimingTrackTErr[0]); h3->Fill(TimingTrackTErr[1]);
		  }
	      }
	  }
   }

   
   TCanvas *c1 = new TCanvas("c1","c1");
   c1->cd(1);
   h2->Draw("colz");
   
   TCanvas *c2 = new TCanvas("c2","c2");
   c2->cd(1);
   h1->SetLineWidth(3); h1->SetLineColor(1); h1->SetMarkerStyle(20);
   h1->SetBinErrorOption(TH1::kPoisson);
   h1->Draw("E0");
   //   h1->Rebin(2);

   /*
   TF1 *fit1 = new TF1("fit1", "gaus(0)+gaus(3)",-1.5,1.5);
   fit1->SetParameter(0,500);
   fit1->SetParLimits(0,1,20000);
   fit1->SetParameter(1,0);
   fit1->SetParLimits(1,-0.1,0.1);
   fit1->SetParameter(2,0.2);
   fit1->SetParLimits(2,0,0.5);
   fit1->SetParameter(3,500);
   fit1->SetParLimits(3,1,100);
   fit1->SetParameter(4,0);
   fit1->SetParLimits(4,-0.1,0.1);
   fit1->SetParameter(5,0.4);
   fit1->SetParLimits(5,0.02,1.5);

   TFitResultPtr fitr = h1->Fit("fit1","LEMVS","",-1.5,1.5);
   TF1 *fit1bkg = new TF1("fit1bkg","gaus(0)",-1.5,1.5);
   fit1bkg->SetParameter(0,fit1->GetParameter(3));
   fit1bkg->SetParameter(1,fit1->GetParameter(4));
   fit1bkg->SetParameter(2,fit1->GetParameter(5));
   fit1bkg->SetLineColor(4); fit1bkg->SetLineWidth(3); fit1bkg->SetLineStyle(2); fit1bkg->Draw("same"); fit1->Draw("same");

   TF1 *fit1sig = new TF1("fit1sig","gaus(0)",-1.5,1.5);
   fit1sig->SetParameter(0,fit1->GetParameter(0));
   fit1sig->SetParameter(1,fit1->GetParameter(1));
   fit1sig->SetParameter(2,fit1->GetParameter(2));
   fit1sig->SetLineColor(2); fit1sig->SetLineWidth(3); fit1sig->SetLineStyle(2); fit1sig->Draw("same"); //fit1->Draw("same");
   */

   TCanvas *c3 = new TCanvas("c3","c3");
   c3->Divide(2,1);
   c3->cd(1);
   h3->Draw("hist");
   c3->cd(2);
   h4->Draw("hist");


   TFile *fx = new TFile("OuptutHistsTrackMultCut10_300088.root","RECREATE");
   h1->Write();
   h2->Write();
   h3->Write();
   h4->Write();
   h5->Write();
   h6->Write();
   h7->Write();
   h8->Write();
   fx->Close();

}
