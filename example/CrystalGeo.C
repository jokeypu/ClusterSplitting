int CrystalGeo(int id = 125160007){

   PndEmcMapper::Init(1);
   TString RunDir      = gSystem->Getenv("MY_RUN_DIR");
   TFile *parfile = new TFile(RunDir+"/simparams.root");
   parfile->Get("FairGeoParSet");
   PndEmcMapper *emcMap=PndEmcMapper::Instance();
   PndEmcTwoCoordIndex* tci=emcMap->GetTCI(id);
   PndEmcTciXtalMap const &tciXtalMap=PndEmcStructure::Instance()->GetTciXtalMap();
   PndEmcXtal* xtal = tciXtalMap.find(tci)->second;
   TGeoTrap trap = xtal->geometry();

   cout << "Out---------------------------" << endl;
   cout << "Alpha1:" << trap.GetAlpha1 () << endl;
   cout << "Alpha2:" << trap.GetAlpha2 () << endl;
   cout << "Bl1:" << trap.GetBl1 () << endl;
   cout << "Bl2:" << trap.GetBl2 () << endl;
   cout << "H1:" << trap.GetH1 () << endl;
   cout << "H2:" << trap.GetH2 () << endl;
   cout << "Phi:" << trap.GetPhi () << endl;
   cout << "Theta:" << trap.GetTheta () << endl;
   cout << "Tl1:" << trap.GetTl1 () << endl;
   cout << "Tl2:" << trap.GetTl2 () << endl;

   return 0;
}
