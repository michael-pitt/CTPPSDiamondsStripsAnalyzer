#define StripsEfficiency_cxx
#include "StripsEfficiency.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

#include <iostream>

using namespace std;


void StripsEfficiency::Loop(int runmin, int runmax, string era)
{
//   In a ROOT session, you can do:
//      root> .L StripsEfficiency.C
//      root> StripsEfficiency t
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

   Int_t nentries = fChain->GetEntries();

   TH1F *hdxi323 = new TH1F("hdxi323","hdxi323",500,-0.5,0.5);
   TH1F *hdxi103123 = new TH1F("hdxi103123","hdxi103123",500,-0.5,0.5);

   TH2F *hcorrxi323 = new TH2F("hcorrxi323","hcorrxi323",1000,0,0.3,1000,0,0.3);
   TH2F *hcorrxi103123 = new TH2F("hcorrxi103123","hcorrxi103123",1000,0,0.3,1000,0,0.3);

   TH1F *h1d3num = new TH1F("h1d3num","h1d3num",300,0,0.3);
   TH1F *h1d103num = new TH1F("h1d103num","h1d103num",300,0,0.3);
   TH1F *h1d3den = new TH1F("h1d3den","h1d3den",300,0,0.3);
   TH1F *h1d103den = new TH1F("h1d103den","h1d103den",300,0,0.3);

   TH2F *h2d3num = new TH2F("h2d3num","h2d3num",300,40,70,300,-15,15);
   TH2F *h2d103num = new TH2F("h2d103num","h2d103num",300,40,70,300,-15,15);
   TH2F *h2d3den = new TH2F("h2d3den","h2d3den",300,40,70,300,-15,15);
   TH2F *h2d103den = new TH2F("h2d103den","h2d103den",300,40,70,300,-15,15);

   TH1F *h1d3toofull = new TH1F("h1d3toofull","h1d3toofull",300,0,0.3);
   TH1F *h1d103toofull = new TH1F("h1d103toofull","h1d103toofull",300,0,0.3);
   TH1F *h1d3planes = new TH1F("h1d3planes","h1d3planes",300,0,0.3);
   TH1F *h1d103planes = new TH1F("h1d103planes","h1d103planes",300,0,0.3);
   TH1F *h1d3matchfail = new TH1F("h1d3matchfail","h1d3matchfail",300,0,0.3);
   TH1F *h1d103matchfail = new TH1F("h1d103matchfail","h1d103matchfail",300,0,0.3);

   TH2F *h2d23num = new TH2F("h2d23num","h2d23num",300,0,30,300,-15,15);
   TH2F *h2d123num = new TH2F("h2d123num","h2d123num",300,0,30,300,-15,15);
   TH2F *h2d23den = new TH2F("h2d23den","h2d23den",300,0,30,300,-15,15);
   TH2F *h2d123den = new TH2F("h2d123den","h2d123den",300,0,30,300,-15,15);

   TH1F *pixthx23pass = new TH1F("pixthx23pass","pixthx23pass",1000,-0.2,0.2);
   TH1F *pixthy23pass = new TH1F("pixthy23pass","pixthy23pass",1000,-0.2,0.2);
   TH1F *pixthx23fail = new TH1F("pixthx23fail","pixthx23fail",1000,-0.2,0.2);
   TH1F *pixthy23fail = new TH1F("pixthy23fail","pixthy23fail",1000,-0.2,0.2);
   TH1F *pixthx123pass = new TH1F("pixthx123pass","pixthx123pass",1000,-0.2,0.2);
   TH1F *pixthy123pass = new TH1F("pixthy123pass","pixthy123pass",1000,-0.2,0.2);
   TH1F *pixthx123fail = new TH1F("pixthx123fail","pixthx123fail",1000,-0.2,0.2);
   TH1F *pixthy123fail = new TH1F("pixthy123fail","pixthy123fail",1000,-0.2,0.2);


   TH1F *pix23chi2pass = new TH1F("pix23chi2pass","pix23chi2pass",500,0,50);
   TH1F *pix23chi2fail = new TH1F("pix23chi2fail","pix23chi2fail",500,0,50);
   TH1F *pix123chi2pass = new TH1F("pix123chi2pass","pix123chi2pass",500,0,50);
   TH1F *pix123chi2fail = new TH1F("pix123chi2fail","pix123chi2fail",500,0,50);

   TH2F *pixxy23pass = new TH2F ("pixxy23pass","pixxy23pass",200,40,70,200,-15,15);
   TH2F *pixxy23fail = new TH2F ("pixxy23fail","pixxy23fail",200,40,70,200,-15,15);
   TH2F *pixxy123pass = new TH2F ("pixxy123pass","pixxy123pass",200,40,70,200,-15,15);
   TH2F *pixxy123fail = new TH2F ("pixxy123fail","pixxy123fail",200,40,70,200,-15,15);

   TH2F *pixxy23faillargeth = new TH2F ("pixxy23faillargeth","pixxy23faillargeth",200,40,70,200,-15,15);
   TH2F *pixxy123faillargeth = new TH2F ("pixxy123faillargeth","pixxy123faillargeth",200,40,70,200,-15,15);

   TH1F *h1d3phys2017B = new TH1F("h1d3phys2017B","h1d3phys2017B",300,0,0.3);
   TH1F *h1d103phys2017B = new TH1F("h1d103phys2017B","h1d103phys2017B",300,0,0.3);

   TH2F *hdy2d45WeirdJan = new TH2F("hdy2d45WeirdJan","hdy2d45WeirdJan",500,-15,15,500,-15,15);
   TH1F *hdy1d45WeirdJan = new TH1F("hdy1d45WeirdJan","hdy1d45WeirdJan",500,-5,5);
   TH1F *htxpixWeirdJan = new TH1F("htxpixWeirdJan","htxpixWeirdJan",1000,-0.2,0.2);
   TH1F *htypixWeirdJan = new TH1F("htypixWeirdJan","htypixWeirdJan",1000,-0.2,0.2);
   TH1F *htxstrWeirdJan = new TH1F("htxstrWeirdJan","htxstrWeirdJan",1000,-0.2,0.2);
   TH1F *htystrWeirdJan = new TH1F("htystrWeirdJan","htystrWeirdJan",1000,-0.2,0.2);
   TH2F *h2dpixWeirdJan = new TH2F("h2dpixWeirdJan","h2dpixWeirdJan",300,40,70,300,-15,15);
   TH2F *h2dstrWeirdJan = new TH2F("h2dstrWeirdJan","h2dstrWeirdJan",300,0,30,300,-15,15);

   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if(jentry % 100000 == 0)
	cout << "Entry " << jentry << "/" << nentries << endl;

      if((Run < runmin) || (Run > runmax))
	continue;

      // For comparison w. strips method - early 2017B
      //      if(LumiSection < 928 || LumiSection > 1392)
      //	continue;
      //      if(Run != 297178)
      //	continue;

      // For comparison w. strips method - mid 2017B, L_int = 1.14fb-1
      //      if(LumiSection < 1129 || LumiSection > 2694)
      //	continue;
      //      if(Run != 297219)
      //	continue;

      // For comparison w. strips method - 2017C, before pixel ROC shift & MD, L_int = 3.40fb-1
	 //      if(LumiSection < 1 || LumiSection > 574)
	 //	continue;
	 //      if(Run != 299597)
	 //      	continue;

      // For comparison w. strips method - 2017D, just before TS2
      //      if(LumiSection < 303 || LumiSection > 1076)
      //	continue;
      //      if(Run != 302597)
      //	continue;

      // JH: Exclude runs without strips in for 2017B
      if(0)
	{
	  if(Run >= 299178 && Run <= 299185)
	    continue;
	  if(Run == 299096)
	    continue;
	  if(Run ==  299149)
	    continue;
	}
      // JH: Exclude runs without strips in for 2017E
      if(0)
	{
	  if(Run >= 303719 && Run < 304737)
	    continue;
	}
      // JH: Exclude runs with pixel ROC1 shifted for 2017C+D
      if(0)
	{
	  if(Run >= 300802 && Run <= 303337)
	    continue;
	}
      // JH: Exclude runs with either 45 or 56 pixel ROC1 shifted for 2017F
      if(0)
	{
	  if(Run > 305169)
	    continue;
	}

      Int_t thexingangle = 0;
      thexingangle = 150;

      Int_t npixt23 = 0;
      Int_t npixt123 = 0;
      Int_t npixloc23 = 0;
      Int_t npixloc123 = 0;

      Int_t nstrt3 = 0;
      Int_t nstrt103 = 0;

      Float_t xpixt23 = 0.0; Float_t ypixt23 = 0.0;
      Float_t xpixt123 = 0.0; Float_t ypixt123 = 0.0;
      Float_t chi2pix23 = 0.0; Float_t chi2pix123 = 0.0;
      Int_t ndfpix23 = 0; Int_t ndfpix123 = 0;
      Float_t chiprobpix23 = 0.0; Float_t chiprobpix123 = 0.0;
      Float_t txpix23 = 0.0; Float_t typix23 = 0.0; Float_t txpix123 = 0.0; Float_t typix123 = 0.0; 
      Float_t txstr3 = 0.0; Float_t tystr3 = 0.0;

      Float_t xstr3 = 0.0; Float_t ystr3 = 0.0;
      Float_t xstr103 = 0.0; Float_t ystr103 = 0.0;
      Float_t xstrloc3 = 0.0; Float_t ystrloc3 = 0.0;
      Float_t xstrloc103 = 0.0; Float_t ystrloc103 = 0.0;


      Float_t xistr3 = -999.0; Float_t xistr103 = -999.0;
      Float_t xipix23 = -999.0; Float_t xipix123 = -999.0;

      Int_t upatterns3 = 0; Int_t vpatterns3 = 0; Int_t upatterns103 = 0; Int_t vpatterns103 = 0;
      Int_t toofullu3 = 0; Int_t toofullv3 = 0; Int_t toofullu103 = 0; Int_t toofullv103 = 0;

      
      

      for(Int_t s = 0; s < nStripTracks; s++)
	{
	  if(StripTrackRPID[s] == 3)
	    {
	      toofullu3 = StripTrackTooFullU[s];
	      toofullv3 = StripTrackTooFullV[s];
	      upatterns3 = StripTrackUPatterns[s];
	      vpatterns3 = StripTrackVPatterns[s];
	      xstrloc3 = StripTrackX[s];
	      ystrloc3 = StripTrackY[s];
	      txstr3 = StripTrackThX[s];
              tystr3 = StripTrackThY[s];
	    }
	  if(StripTrackRPID[s] == 103)
	    {
              toofullu103 = StripTrackTooFullU[s];
              toofullv103 = StripTrackTooFullV[s];
              upatterns103 = StripTrackUPatterns[s];
              vpatterns103 = StripTrackVPatterns[s];
              xstrloc103 = StripTrackX[s];
              ystrloc103 = StripTrackY[s];
	    }
	}

      for(Int_t x = 0; x < nPixelTracks; x++)
	{
	  if(PixTrackZ[x] < 0)
	    {
	      xpixt23 = PixTrackX[x];
	      ypixt23 = PixTrackY[x];
	      txpix23 = PixTrackThX[x];
	      typix23 = PixTrackThY[x];
	      chi2pix23 = PixTrackChi2[x];
	      ndfpix23 = PixTrackNdof[x];
	      chiprobpix23 = TMath::Prob(chi2pix23,ndfpix23);
	      npixloc23++;
	    }
	  if(PixTrackZ[x] > 0)
	    {
	      xpixt123 = PixTrackX[x];
              ypixt123 = PixTrackY[x];
              txpix123 = PixTrackThX[x];
              typix123 = PixTrackThY[x];
              chi2pix123 = PixTrackChi2[x];
	      ndfpix123 = PixTrackNdof[x];
	      chiprobpix123 = TMath::Prob(chi2pix123,ndfpix123);
	      npixloc123++;
	    }
	}

      //      std::cout << "N(pixel tracks 45 = )" << npixloc23 << ", Strips (Upatterns, Vpatterns, Utoofull, Vtoofull) = " << upatterns3 << ", " << vpatterns3 
      //		<< ", " << toofullu3 << ", " << toofullv3 << std::endl;
      //      std::cout << "N(pixel tracks 56 = )" << npixloc123 << ", Strips (Upatterns, Vpatterns, Utoofull, Vtoofull) = " << upatterns103 << ", " << vpatterns103
      //                << ", " << toofullu103 << ", " << toofullv103 << std::endl;


      for(Int_t p = 0; p < nProtons; p++)
	{
	  if(ProtonIsMultiRP[p] == 0)
	    {
	      if(ProtonRPID[p] == 3)
		{
		  xistr3 = ProtonXi[p];
		  nstrt3++;
		}
	      if(ProtonRPID[p] == 23)
		{
		  xipix23 = ProtonXi[p];
		  npixt23++;
		}
	      if(ProtonRPID[p] == 103)
		{
		  xistr103 = ProtonXi[p];
		  nstrt103++;
		}
	      if(ProtonRPID[p] == 123)
		{
		  xipix123 = ProtonXi[p];
		  npixt123++;
		}
	    }
	}

      // Now strips efficiencies using single pixel track tags
      // JH - for Weird Jan test                                                                                                                                        
      //      if(npixt23 == 1 && ypixt23 >= 2 && ypixt23 <= 6)// && (ndfpix23 >= 4 && fabs(txpix23) < 0.02 && fabs(typix23) < 0.02 && chiprobpix23 > 0.1))
      if(npixt23 == 1)
      //      if(npixt23 == 1 && (ndfpix23 >= 4 && fabs(txpix23) < 0.02 && fabs(typix23) < 0.02 && chiprobpix23 > 0.1))
	{
	  //	  h1d3phys2017B->Fill(xistr3);

	  if(npixloc23 != 1)
	    std::cout << "WTF??? N(local pixel tracks 23) = " << npixloc23 << std::endl;

	  //                const bool si_pat_multr = (si.u_patterns > 1 || si.u_tooFull) || (si.v_patterns > 1 || si.v_tooFull);

	  if((toofullu3 < 3 && upatterns3 < 2) && (toofullv3 < 3 && vpatterns3 < 2)) // to exclude strip multi-track events
	    {
	      Float_t dxi = xipix23-xistr3;
	      hdxi323->Fill(dxi);
	      hcorrxi323->Fill(xipix23,xistr3);
	      h1d3den->Fill(xipix23);
	      h2d3den->Fill(xpixt23,ypixt23);
	      if(fabs(dxi) < 0.1)
		{
		  h1d3num->Fill(xipix23);
		  pixxy23pass->Fill(xpixt23,ypixt23);
		  h2d3num->Fill(xpixt23,ypixt23);
		  pixthx23pass->Fill(txpix23);
		  pixthy23pass->Fill(typix23);
		  pix23chi2pass->Fill(chi2pix23/ndfpix23);
		}
	      else
		{
                  pixxy23fail->Fill(xpixt23,ypixt23);
                  pixthx23fail->Fill(txpix23);
                  pixthy23fail->Fill(typix23);
                  pix23chi2fail->Fill(chi2pix23/ndfpix23);

		  if((txpix23 > (0.00542 + 3*0.00313)) || 
		     (txpix23 < (0.00542 - 3*0.00313)) || 
		     (typix23 > (0.001516 + 3*0.002465)) || 
		     (typix23 < (0.001516 - 3*0.002465)))
		    pixxy23faillargeth->Fill(xpixt23,ypixt23);
		}
	      if(fabs(dxi) >= 0.1 && fabs(dxi) < 99)
		h1d3matchfail->Fill(xipix23);

	      // Weird shifted events from Jan!!!
	      if(xipix23 > 0.16 && dxi > 0.02 && dxi < 0.04)
		{
		  hdy2d45WeirdJan->Fill(ypixt23,ystrloc3);
		  hdy1d45WeirdJan->Fill(ypixt23-ystrloc3);
		  htxpixWeirdJan->Fill(txpix23);
		  htypixWeirdJan->Fill(typix23);
		  htxstrWeirdJan->Fill(txstr3);
		  htystrWeirdJan->Fill(tystr3);
		  h2dpixWeirdJan->Fill(xpixt23,ypixt23);
		  h2dstrWeirdJan->Fill(xstrloc3,ystrloc3);
		}

	    }
	  else
	    {
	      if(toofullu3 >= 3 || toofullv3 >=3)
		h1d3toofull->Fill(xipix23);
	      if(upatterns3 > 1 || vpatterns3 > 1)
		h1d3planes->Fill(xipix23);
	    }
	}
      // JH - for Weird Jan test                                                                                                                                      
      //      if(npixt123 == 1 && ypixt123 >= 2 && ypixt123 <= 8)// && (ndfpix123 >= 4 && fabs(txpix123) < 0.02 && fabs(typix123) < 0.02 && chiprobpix123 > 0.1))
      if(npixt123 == 1)
      //      if(npixt123 == 1 && (ndfpix123 >= 4 && fabs(txpix123) < 0.02 && fabs(typix123) < 0.02 && chiprobpix123 > 0.1))
	{
	  //	  h1d103phys2017B->Fill(xistr103);

          if(npixloc123 != 1)
	    std::cout << "WTF??? N(local pixel tracks 123) = " << npixloc123 << std::endl;

	  //	  const bool si_pat_multr = (si.u_patterns > 1 || si.u_tooFull) || (si.v_patterns > 1 || si.v_tooFull);

	  if((toofullu103 < 3 && upatterns103 < 2) && (toofullv103 < 3 && vpatterns103 < 2)) // to exclude strip multi-track events
	  {
	    Float_t dxi = xipix123-xistr103;
	    hdxi103123->Fill(dxi);
	    hcorrxi103123->Fill(xipix123,xistr103);
	    h1d103den->Fill(xipix123);
	    h2d103den->Fill(xpixt123,ypixt123);

	    if(fabs(dxi) < 0.1)
	      {
		h1d103num->Fill(xipix123);
		h2d103num->Fill(xpixt123,ypixt123);
		pixxy123pass->Fill(xpixt123,ypixt123);
		pixthx123pass->Fill(txpix123);
		pixthy123pass->Fill(typix123);
		pix123chi2pass->Fill(chi2pix123/ndfpix123);
	      }
	    else
	      {
		pixxy123fail->Fill(xpixt123,ypixt123);
		pixthx123fail->Fill(txpix123);
		pixthy123fail->Fill(typix123);
		pix123chi2fail->Fill(chi2pix123/ndfpix123);

		if((txpix123 > (-0.0115 + 3*0.002881)) ||
		   (txpix123 < (-0.0115 - 3*0.002881)) ||
		   (typix123 > (0.0007735 + 3*0.002496)) ||
		   (typix123 < (0.0007735 - 3*0.002496)))
		  pixxy123faillargeth->Fill(xpixt123,ypixt123);

	      }
	    if(fabs(dxi) >= 0.1 && fabs(dxi) < 99)
	      h1d103matchfail->Fill(xipix123);
	  }
          else
            {
              if(toofullu103 >= 3 || toofullv103 >=3)
                h1d103toofull->Fill(xipix123);
              if(upatterns103 > 1 || vpatterns103 > 1)
                h1d103planes->Fill(xipix123);
            }

	}

      // Now pixels efficiencies using strip tracks
      if(nstrt3 == 1) // && (fabs(ystrloc3) < 8.5))
	{
	  Float_t dxi = xipix23-xistr3;
	  h2d23den->Fill(xstrloc3,ystrloc3);
	  if(fabs(dxi) < 0.1)
	    {
	      h2d23num->Fill(xstrloc3,ystrloc3);
	    }
	  else
	    {
	    }

	  // JH - for Weird Jan test
	  //	  if(ystrloc3 > 2 && ystrloc3 < 6)
	    h1d3phys2017B->Fill(xistr3);
	}
      if(nstrt103 == 1) // && (fabs(ystrloc103) < 8.5))
	{
	  Float_t dxi = xipix123-xistr103;
          h2d123den->Fill(xstrloc103,ystrloc103);
          if(fabs(dxi) < 0.1)
            {
	      h2d123num->Fill(xstrloc103,ystrloc103);
            }
	  else
	    {
	    }

	  //	  if(ystrloc103 > 2 && ystrloc103 < 8)
          // JH - for Weird Jan test                                                                                                                                    
	    h1d103phys2017B->Fill(xistr103);
	}
   }

   //   TString outfilename = "EfficiencyNumeratorAligned2017B_TestSingleEle_xangleall_withpixels_excludenostripruns_finebins_";
   //   TString outfilename = "EfficiencyDenominatorAlignedAlignment_";
   //   TString outfilename = "EffstudiesMatchingDebugTest_BTagCSV_";
   //   TString outfilename = "EffstudiesMatchingDebugTest_JetHT_";
   //   TString outfilename = "EffstudiesMatchingDebugTest_PixelQualityCuts_";
   TString outfilename = "EffstudiesMatchingDebugTestAllRunsAllTracksAllY_AllTrk_";
   outfilename += era;
   outfilename += "_";
   outfilename += runmin;
   outfilename += "_";
   outfilename += runmax;
   outfilename += ".root";
   TFile *fx = new TFile(outfilename,"RECREATE");

   hdxi323->Write();
   hdxi103123->Write();
   hcorrxi323->Write();
   hcorrxi103123->Write();

   h1d3num->Write();
   h1d3den->Write();
   h1d103num->Write();
   h1d103den->Write();

   pixxy23pass->Write();
   pixthx23pass->Write();
   pixthy23pass->Write();
   pix23chi2pass->Write();
   pixxy23fail->Write();
   pixthx23fail->Write();
   pixthy23fail->Write();
   pix23chi2fail->Write();

   pixxy123faillargeth->Write();
   pixxy23faillargeth->Write();
   
   pixxy123pass->Write();
   pixthx123pass->Write();
   pixthy123pass->Write();
   pix123chi2pass->Write();
   pixxy123fail->Write();
   pixthx123fail->Write();
   pixthy123fail->Write();
   pix123chi2fail->Write();

   h2d23den->Write();
   h2d23num->Write();
   h2d123den->Write();
   h2d123num->Write();

   h2d3den->Write();
   h2d3num->Write();
   h2d103den->Write();
   h2d103num->Write();

   h1d3toofull->Write();
   h1d3planes->Write();
   h1d103toofull->Write();
   h1d103planes->Write();
   h1d3matchfail->Write();
   h1d103matchfail->Write();

   h1d3phys2017B->Write();
   h1d103phys2017B->Write();

   hdy2d45WeirdJan->Write();
   hdy1d45WeirdJan->Write();
   htxpixWeirdJan->Write();
   htypixWeirdJan->Write();
   htxstrWeirdJan->Write();
   htystrWeirdJan->Write();
   h2dpixWeirdJan->Write();
   h2dstrWeirdJan->Write();

   fx->Close();
}
