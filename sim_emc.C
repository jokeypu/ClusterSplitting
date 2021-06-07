// Macro created 20/09/2006 by S.Spataro
// It creates a geant simulation file for emc

int sim_emc(
        Int_t nEvents = 10,
	Int_t pdg = 22, Int_t nParticle = 1,
	Float_t mom_min = 1., Float_t mom_max = 1., 
	Float_t theta_min = 90., Float_t theta_max = 90.,
        Float_t phi_min = 0., Float_t phi_max = 360.
){
  // flag which define if the full PANDA geomerty is used or only EMC
  Bool_t useFullPANDAGeometry=false;
  TStopwatch timer;
  timer.Start();
  gDebug=0;

  //TString digiFile = "emc.par";
  TString emcOutputFile = gSystem->Getenv("MY_OUTPUT_FILE");
  TString emcParFile = gSystem->Getenv("MY_PAR_FILE");
  TString emcAsciiFile = gSystem->Getenv("MY_ASCII_FILE");
  
  FairRunSim *fRun = new FairRunSim();

  // set the MC version used
  // ------------------------
  fRun->SetName("TGeant4");
  fRun->SetOutputFile(emcOutputFile);

  // Set the parameters
  //-------------------------------
  //TString emcDigiFile = gSystem->Getenv("VMCWORKDIR");
  //emcDigiFile += "/macro/params/";
  //emcDigiFile += digiFile;
 
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open(emcAsciiFile.Data(),"in");
  rtdb->setFirstInput(parIo1);        
  Bool_t kParameterMerged=kTRUE;
	
  FairParRootFileIo* output=new FairParRootFileIo(kParameterMerged);
  output->open(emcParFile);
  rtdb->setOutput(output);
  
  // Set Material file Name
  //-----------------------
  fRun->SetMaterials("media_pnd.geo");

  // Create and add detectors
  //-------------------------
  FairModule *Cave= new PndCave("CAVE");
  Cave->SetGeometryFileName("pndcave.geo");
  fRun->AddModule(Cave); 
  
  PndEmc *Emc = new PndEmc("EMC",kTRUE);
  Emc->SetGeometryVersion(1);
  // See PndEmc::SetGeometryVersion() for available geometries and add there new one if necessary
  //Emc->SetStorageOfData(kTRUE);
  Emc->SetStorageOfData(kFALSE);
  fRun->AddModule(Emc);

  if (useFullPANDAGeometry)
  {
	FairModule *Magnet= new PndMagnet("MAGNET");
	//Magnet->SetGeometryFileName("FullSolenoid_V842.root");
	Magnet->SetGeometryFileName("FullSuperconductingSolenoid_v831.root");
	fRun->AddModule(Magnet);

	FairModule *Dipole= new PndMagnet("MAGNET");
	Dipole->SetGeometryFileName("dipole.geo");
	fRun->AddModule(Dipole);

	FairModule *Pipe= new PndPipe("PIPE");
	fRun->AddModule(Pipe);

	FairDetector *Stt= new PndStt("STT", kTRUE);
	Stt->SetGeometryFileName("straws_skewed_blocks_35cm_pipe.geo");
	fRun->AddModule(Stt);

	FairDetector *Mvd = new PndMvdDetector("MVD", kTRUE);
	Mvd->SetGeometryFileName("Mvd-2.1_FullVersion.root");
	fRun->AddModule(Mvd);

        PndMdt *Muo = new PndMdt("MDT",kTRUE);
        Muo->SetMdtMagnet(kTRUE);
        //Muo->SetMdtMFIron(kFALSE);
        Muo->SetMdtCoil(kTRUE);
        Muo->SetBarrel("muon_TS_barrel_strip_v1_noGeo.root");
        Muo->SetEndcap("muon_TS_endcap_strip_v1_noGeo.root");
        Muo->SetForward("muon_Forward_strip_v1_noGeo.root");
        Muo->SetMuonFilter("muon_MuonFilter_strip_v1_noGeo.root");
        fRun->AddModule(Muo);

	FairDetector *Gem = new PndGemDetector("GEM", kTRUE);
	Gem->SetGeometryFileName("gem_3Stations.root");
	fRun->AddModule(Gem);

	PndDsk* Dsk = new PndDsk("DSK", kTRUE);
	Dsk->SetGeometryFileName("dsk.root");
	Dsk->SetStoreCerenkovs(kFALSE);
	Dsk->SetStoreTrackPoints(kFALSE);
	fRun->AddModule(Dsk);

  PndDrc *Drc = new PndDrc("DIRC", kTRUE);
  Drc->SetGeometryFileName("dirc_l0_p0.root");
  Drc->SetRunCherenkov(kFALSE); // for fast sim Cherenkov -> kFALSE
  fRun->AddModule(Drc);
	
	FairDetector *Fts= new PndFts("FTS", kTRUE);
	Fts->SetGeometryFileName("fts.geo");
	fRun->AddModule(Fts);
  }
  // Create and Set Event Generator
  //-------------------------------

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  fRun->SetGenerator(primGen);

  // Box Generator: 
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdg, nParticle); // 13 = muon; 1 = multipl. // 211 = pi+
  // first number: PDG particle code: 2nd number: particle multiplicity per event

  boxGen->SetPRange(mom_min,mom_max); // GeV/c
  boxGen->SetPhiRange(phi_min, phi_max); // Azimuth angle range [degree]
  boxGen->SetThetaRange(theta_min, theta_max); // Polar angle in lab system range [degree]
  boxGen->SetXYZ(0., 0., 0.); // vertex coordinates [mm]
  primGen->AddGenerator(boxGen);  

  
  // Create and Set Magnetic Field
  //-------------------------------
  fRun->SetBeamMom(15);
  //---------------------Create and Set the Field(s)---------- 
  PndMultiField *fField= new PndMultiField("FULL");
  fRun->SetField(fField);


  // EMC Hit producer
  //-------------------------------
  PndEmcHitProducer* emcHitProd = new PndEmcHitProducer();
  // Linear non-uniformity -1.5%
  //TString nonuniformityFile=gSystem->Getenv("VMCWORKDIR");
  //nonuniformityFile+="/macro/params/EmcDigiNoniformityPars2.root";
  //emcHitProd->SetNonuniformityFile(nonuniformityFile.Data());
  fRun->AddTask(emcHitProd);
  
  /**Initialize the session*/
  fRun->Init();
  
  PndMultiFieldPar* Par = (PndMultiFieldPar*) rtdb->getContainer("PndMultiFieldPar");
  if (fField) {  Par->SetParameters(fField); }
  Par->setInputVersion(fRun->GetRunId(),1);
  Par->setChanged();

  rtdb->setOutput(output);
  rtdb->saveOutput();
  rtdb->print();

  // Transport nEvents
  // -----------------
  fRun->Run(nEvents);

  timer.Stop();

  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
return 0;
}
